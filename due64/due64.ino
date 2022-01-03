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

const uint16_t one = 7;
const uint16_t zro = 21;
const uint16_t stop = 14;
const uint16_t off = 0;
const uint16_t b[2] = { zro, one };

uint16_t status_response[26] = { b[0], b[0], b[0], b[0], b[0], b[1], b[0], b[1],	//0x05
								 b[0], b[0], b[0], b[0], b[0], b[0], b[0], b[0],	//0x00
								 b[0], b[0], b[0], b[0], b[0], b[0], b[0], b[0],	//0x00
								 stop, off };
uint16_t status_count = 26;

uint16_t rx_fail[10] = { stop, stop, stop, stop, stop, stop, stop, stop,
						 stop, off };
uint16_t rx_fail_count = 10;

uint16_t dummy_stop[50] = { 0 };
uint16_t dummy_count = 50;


volatile uint8_t rx_read[8];
volatile uint16_t rx_count = 8;
volatile uint16_t test[10];
volatile uint16_t test_count = 10;

volatile uint16_t controller_data[34];
volatile uint16_t controller_count = 34;

void diasble_uart_interrupt() { REG_UART_IDR = ~0; }
void enable_uart_interrupt() { REG_UART_IER = (1 << 3); }
void set_uart_dma() {
	REG_UART_RPR = (uint32_t)rx_read;
	REG_UART_RCR = rx_count;
}

void setup() {
	for (uint8_t i = 0; i < 32; i++) {
		controller_data[i] = zro;
	}
	controller_data[0] = one;
	controller_data[32] = stop;
	controller_data[33] = off;

	pio_disable_pullup(PIOA, PIN_17A);
	pio_enable_output(PIOA, PIN_17A);
	pio_enable_pio(PIOA, PIN_17A);
	pio_output_write_HIGH(PIOA, PIN_17A);

	pio_disable_pullup(PIOA, UTXD | URXD);
	pio_enable_output(PIOA, UTXD);
	pio_disable_output(PIOA, URXD);
	pio_set_periph_mode_A(PIOA, UTXD | URXD);
	pio_disable_pio(PIOA, URXD | URXD);
	pio_disable_pullup(PIOB, PWML0 | PIN_20B);
	pio_enable_output(PIOB, PWML0 | PIN_20B);
	pio_disable_pio(PIOB, PWML0 | PIN_20B);
	pio_set_periph_mode_B(PIOB, PWML0 | PIN_20B);
	pmc_enable_periph_clk(ID_PWM);
	pmc_enable_periph_clk(ID_UART);

	REG_PWM_CLK = 3 | (1 << 9);
	REG_PWM_CMR0 |= 0b1011;	//CLKA
	REG_PWM_SCM |= 1 | (1 << 17);
	REG_PWM_CPRD0 = 28;
	REG_PWM_CDTY0 = off;
	//REG_PWM_TPR = (uint32_t)status_response;
	//REG_PWM_TCR = status_count;
	REG_PWM_PTCR = (1 << 8);
	//REG_PWM_IER2 |= (1 << 2);
	//NVIC_ClearPendingIRQ(PWM_IRQn);
	//NVIC_SetPriority(PWM_IRQn, 1);

	/*
	Baud rate = MCK / (16 * CD)
	N64 controller BAUD rate = 250kHz
	250,000 = 84,000,000/(16 * CD) --> CD = 21
	21 + 1 = 22 because the n64 is closer to 240kHz
	*/
	uart_set_clk_div(2);
	uart_set_parity_ch_mode();
	REG_UART_IDR = ~0;
	REG_UART_IER = (1 << 3);
	REG_UART_PTCR = 1;
	NVIC_ClearPendingIRQ(UART_IRQn);
	NVIC_SetPriority(UART_IRQn, 0);
	NVIC_EnableIRQ(UART_IRQn);
	uart_enable_rx();
	//NVIC_EnableIRQ(PWM_IRQn);
	REG_PWM_ENA |= 1;
	set_uart_dma();
}

void PWM_Handler() {
	volatile uint32_t dummy = REG_PWM_ISR2;
	enable_uart_interrupt();
	pio_output_write_HIGH(PIOA, PIN_17A);
	//read controller data for next frame
}

//Set the baud rate really high and use a byte or two for each bit??
void UART_Handler() {
	diasble_uart_interrupt();

	*(uint32_t*)(rx_read) = ((*(uint32_t*)(rx_read)) >> 6) & 1;
	*(uint32_t*)(rx_read + 4) = ((*(uint32_t*)(rx_read + 4)) >> 6) & 1;
	test[0] = b[rx_read[0]];
	test[1] = b[rx_read[1]];
	test[2] = b[rx_read[2]];
	test[3] = b[rx_read[3]];
	test[4] = b[rx_read[4]];
	test[5] = b[rx_read[5]];
	test[6] = b[rx_read[6]];
	test[7] = b[rx_read[7]];
	test[8] = stop;
	test[9] = off;


	REG_PWM_TPR = (uint32_t)test;
	REG_PWM_TCR = test_count;

	//Sometimes the pwm dma would send an extra bit if this for loop was disabled
	//check to see if it does that when the loop is enabled
	for (volatile uint32_t dummy = 0; dummy < 4000; dummy++) {}
	enable_uart_interrupt();
	set_uart_dma();
	return;
	volatile uint32_t uart_read = REG_UART_RHR;
	switch (uart_read) {
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
	for (volatile uint32_t dummy = 0; dummy < 4000; dummy++) {}
	uart_read = REG_UART_RHR;
	enable_uart_interrupt();
}

void loop() {


	while (1) {
		//I have bad feelings about loop()...
	}
}
