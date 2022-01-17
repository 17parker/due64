//I know you're not good at it, but please look at the commit notes to see where you left off
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

const uint16_t status_response[28] = { off, off, b[0], b[0], b[0], b[0], b[0], b[1], b[0], b[1],	//0x05
								 b[0], b[0], b[0], b[0], b[0], b[0], b[0], b[0],	//0x00
								 b[0], b[0], b[0], b[0], b[0], b[0], b[0], b[0],	//0x00
								 stop, off };
const uint16_t status_count = 28;
const uint32_t status_delay = 1350;

volatile uint8_t rx_read[8];
const uint16_t rx_count = 8;
/*
Each packet of transmitted data is formatted like this:
[off, off, ...(n bits to send)..., stop bit, off) = (n bits) + 4
So (2 off bits) + (32 bits when controller responds) + (stop and off) = 36
The delay timings are mostly arbitrary, but ~45*(n bits)
*/
volatile uint16_t controller_data[36];
const uint16_t controller_count = 36;
const uint32_t controller_delay = 1520;
volatile uint16_t complete_rx[68] = {off, off};	//all 8 bytes of rx data
const uint16_t complete_count = 68;
const uint32_t complete_delay = 3400;
volatile uint16_t test[12] = {off, off};			//one bit from each byte of rx data
const uint16_t test_count = 12;
const uint32_t test_delay = 500;

volatile uint32_t frame_count = 0;
volatile uint8_t a_press = 0;

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
	REG_TC0_CCR0 = 1 | (1 << 2); //CLK EN

	REG_PWM_CLK = 3 | (1 << 9);
	REG_PWM_CMR0 |= 0b1011;	//CLKA
	REG_PWM_SCM |= 1 | (1 << 17);
	REG_PWM_CPRD0 = 28;
	REG_PWM_CDTY0 = off;
	REG_PWM_PTCR = (1 << 8); //DMA TXEN

	uart_set_clk_div(2);
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
	REG_UART_RPR = (uint32_t)rx_read;	//set UART DMA
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
}

void loop() {
	//Nothing here...
}

void run_controller() {
	REG_PWM_TPR = (uint32_t)controller_data;
	REG_PWM_TCR = controller_count;
	REG_TC0_RC0 = controller_delay;
	REG_TC0_CCR0 = (1 << 2);
}

void run_test() {
	test[2] = b[(rx_read[0] >> 6) & 1];
	test[3] = b[(rx_read[1] >> 6) & 1];
	test[4] = b[(rx_read[2] >> 6) & 1];
	test[5] = b[(rx_read[3] >> 6) & 1];
	test[6] = b[(rx_read[4] >> 6) & 1];
	test[7] = b[(rx_read[5] >> 6) & 1];
	test[8] = b[(rx_read[6] >> 6) & 1];
	test[9] = b[(rx_read[7] >> 6) & 1];
	test[10] = stop;
	test[11] = off;
	REG_PWM_TPR = (uint32_t)test;
	REG_PWM_TCR = test_count;
	REG_TC0_RC0 = test_delay;
	REG_TC0_CCR0 = (1 << 2);
}