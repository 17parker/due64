//Emulate N64 controller to interface with an actual N64
//Using UART at a specific baud rate...
//The N64 data is MSB first, but UART is LSB first. This means the UART TX data is "mirrored"


//I know you're not good at it, but please look at the commit notes to see where you left off

//Use UART to receive N64 command, then use PWM with variable duty cycle to transmit data

/*
controller joystick x / y is capable of values 0x80 - 0x7F (-128 to 127)
In reality, it is only capable of about +/- 72 (one resource says -81 to 81)
*/

#include "SAM3XDUE.h"

void run_test();
void run_controller();
void dumbo_wait(uint32_t count) {
	for (volatile uint32_t i = 0; i < count; ++i) {}
}

const uint16_t one = 7;
const uint16_t zro = 21;
const uint16_t stop = 14;
const uint16_t off = 0;
const uint16_t b[2] = { zro, one };


uint16_t status_response[28] = { off, off, b[0], b[0], b[0], b[0], b[0], b[1], b[0], b[1],	//0x05
								 b[0], b[0], b[0], b[0], b[0], b[0], b[0], b[0],	//0x00
								 b[0], b[0], b[0], b[0], b[0], b[0], b[0], b[0],	//0x00
								 stop, off };
uint16_t status_count = 28;
uint32_t status_delay = 1350;

volatile uint8_t rx_read[8];
volatile uint16_t rx_count = 8;
volatile uint16_t complete_rx[66];
volatile uint16_t complete_count = 66;
volatile uint32_t complete_delay = 3200;
volatile uint16_t test[10];
volatile uint16_t test_count = 10;
volatile uint32_t test_delay = 400;

volatile uint16_t controller_data[36];
volatile uint16_t controller_count = 36;
volatile uint32_t controller_delay = 1520;

volatile uint32_t frame_count = 0;
volatile uint8_t a_press = 0;

void set_uart_dma() {
	REG_UART_RPR = (uint32_t)rx_read;
	REG_UART_RCR = rx_count;
}

void setup() {
	for (uint8_t i = 0; i < 34; i++)
		controller_data[i] = zro;
	controller_data[0] = off;
	controller_data[1] = off;
	controller_data[34] = stop;
	controller_data[35] = off;

	/*
	Pin 0 - UART RX
	Pin 1 - UART TX
	Pin 2 - TIOA output
	Pin 20 - PWMH0 output
	*/
	pio_disable_pullup(PIOA, UTXD | URXD);
	pio_enable_output(PIOA, UTXD);
	pio_disable_output(PIOA, URXD);
	pio_set_periph_mode_A(PIOA, UTXD | URXD);
	pio_disable_pio(PIOA, URXD | URXD);
	pio_disable_pullup(PIOB, PIN_2B | PIN_20B);
	pio_enable_output(PIOB, PIN_2B | PIN_20B);
	pio_disable_pio(PIOB, PIN_2B | PIN_20B);
	pio_set_periph_mode_B(PIOB, PIN_2B | PIN_20B);
	pmc_enable_periph_clk(ID_PWM);
	pmc_enable_periph_clk(ID_UART);
	pmc_enable_periph_clk(ID_TC0);

	REG_TC0_CMR0 = 1 | (1 << 6) | (1 << 14) | (1 << 15) | (1 << 16) | (0b11 << 18);	//Using TIOA on TC0
	REG_TC0_RC0 = 21;
	REG_TC0_IER0 = (1 << 4);
	NVIC_ClearPendingIRQ(TC0_IRQn);
	NVIC_SetPriority(TC0_IRQn, 1);
	NVIC_EnableIRQ(TC0_IRQn);
	REG_TC0_CCR0 = 1 | (1 << 2); //Enables the clock

	REG_PWM_CLK = 3 | (1 << 9);
	REG_PWM_CMR0 |= 0b1011;	//CLKA
	REG_PWM_SCM |= 1 | (1 << 17);
	REG_PWM_CPRD0 = 28;
	REG_PWM_CDTY0 = off;
	REG_PWM_PTCR = (1 << 8);

	uart_set_clk_div(2);	//One n64 bit is one byte received
	uart_set_parity_ch_mode();
	REG_UART_IDR = ~0;
	REG_UART_IER = (1 << 3);
	REG_UART_PTCR = 1;
	NVIC_ClearPendingIRQ(UART_IRQn);
	NVIC_SetPriority(UART_IRQn, 0);
	NVIC_EnableIRQ(UART_IRQn);
	uart_enable_rx();

	REG_PWM_ENA |= 1;
	REG_UART_RPR = (uint32_t)rx_read;
	REG_UART_RCR = rx_count;
}

void TC0_Handler() {
	volatile uint32_t dummy = REG_TC0_SR0;
	REG_UART_RPR = (uint32_t)rx_read;
	REG_UART_RCR = rx_count;
	REG_UART_IER = (1 << 3);
	if (frame_count >= 7) {
		controller_data[2] = b[0];
		controller_data[3] = b[1];
		frame_count = 0;
	}
	else if (frame_count <= 4) {
		controller_data[2] = b[1];
		controller_data[3] = b[0];
		++frame_count;
	}
	else {
		controller_data[2] = b[0];
		controller_data[3] = b[1];
		++frame_count;
	}
}

void UART_Handler() {
	REG_UART_IDR = ~0;
	REG_PWM_TPR = (uint32_t)controller_data;
	REG_PWM_TCR = controller_count;
	REG_TC0_RC0 = controller_delay;
	REG_TC0_CCR0 = (1 << 2);
	//run_test();
	//run_controller();
	return;


	switch (test[0]) {
	case 0x00:	//status - controller sends 3 bytes: 0x05, 0x00, 0x00 (bitflags)
		REG_PWM_TPR = (uint32_t)status_response;
		REG_PWM_TCR = status_count;
		break;

	case 0x80:	//poll - controller sends 4 bytes
		REG_PWM_TPR = (uint32_t)controller_data;
		REG_PWM_TCR = controller_count;
		break;

	case 0x40:	//read - controller returns 33 bytes of 0x00
		break;

	case 0xC0:	//write - please don't make me do it
		break;

	case 0xFF:	//reset - controller responds identically to command 0x00
		break;
	default:
		REG_PWM_TPR = (uint32_t)controller_data;
		REG_PWM_TCR = controller_count;
		/*default:
			rx_fail[0] = b[uart_read & 1];
			uart_read = uart_read >> 1;
			rx_fail[1] = b[uart_read & 1];
			uart_read = uart_read >> 1;
			rx_fail[2] = b[uart_read & 1];
			uart_read = uart_read >> 1;
			rx_fail[3] = b[uart_read & 1];
			uart_read = uart_read >> 1;
			rx_fail[4] = b[uart_read & 1];
			uart_read = uart_read >> 1;
			rx_fail[5] = b[uart_read & 1];
			uart_read = uart_read >> 1;
			rx_fail[6] = b[uart_read & 1];
			uart_read = uart_read >> 1;
			rx_fail[7] = b[uart_read & 1];
			REG_PWM_TPR = (uint32_t)rx_fail;
			REG_PWM_TCR = rx_fail_count;
			*/
	}
	REG_UART_IER = (1 << 3);
}

void loop() {
}

void run_controller() {
	REG_PWM_TPR = (uint32_t)controller_data;
	REG_PWM_TCR = controller_count;
	REG_TC0_RC0 = controller_delay;
	REG_TC0_CCR0 = (1 << 2);
}

void run_test() {
	test[0] = b[(rx_read[0] >> 6) & 1];
	test[1] = b[(rx_read[1] >> 6) & 1];
	test[2] = b[(rx_read[2] >> 6) & 1];
	test[3] = b[(rx_read[3] >> 6) & 1];
	test[4] = b[(rx_read[4] >> 6) & 1];
	test[5] = b[(rx_read[5] >> 6) & 1];
	test[6] = b[(rx_read[6] >> 6) & 1];
	test[7] = b[(rx_read[7] >> 6) & 1];
	test[8] = stop;
	test[9] = off;
	REG_PWM_TPR = (uint32_t)test;
	REG_PWM_TCR = test_count;
	REG_TC0_RC0 = test_delay;
	REG_TC0_CCR0 = (1 << 2);
}