//Emulate N64 controller to interface with an actual N64
//Using UART at a specific baud rate...
//The N64 data is MSB first, but UART is LSB first. This means the UART TX data is "mirrored"


//I know you're not good at it, but please look at the commit notes to see where you left off

//Use UART to receive N64 command, then use PWM with variable duty cycle to transmit data

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

/*
controller joystick x / y is capable of values 0x80 - 0x7F (-128 to 127)
In reality, it is only capable of about +/- 72 (one resource says -81 to 81)
*/

volatile uint16_t controller_data[34];
volatile uint16_t controller_count = 34;

void setup() {
	for (uint8_t i = 0; i < 32; i++) {
		controller_data[i] = zro;
	}
	controller_data[0] = one;
	controller_data[32] = stop;
	controller_data[33] = off;
	//Pin setup
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
	REG_PWM_TPR = (uint32_t)status_response;
	REG_PWM_TCR = status_count;
	REG_PWM_PTCR = (1 << 8);
	REG_PWM_IER2 |= (1 << 2);
	NVIC_ClearPendingIRQ(PWM_IRQn);
	NVIC_SetPriority(PWM_IRQn, 1);

	/*
	Baud rate = MCK / (16 * CD)
	N64 controller BAUD rate = 250kHz
	250,000 = 84,000,000/(16 * CD) --> CD = 21
	21 + 1 = 22 because the n64 is closer to 240kHz
	*/
	uart_set_clk_div(22);
	uart_set_parity_ch_mode();
	REG_UART_IDR = ~0;
	REG_UART_IER = 1;
	NVIC_ClearPendingIRQ(UART_IRQn);
	NVIC_SetPriority(UART_IRQn, 0);
	NVIC_EnableIRQ(UART_IRQn);
	uart_enable_rx();
	NVIC_EnableIRQ(PWM_IRQn);
	REG_PWM_ENA |= 1;
}


void PWM_Handler() {
	volatile uint32_t dummy = REG_PWM_ISR2;
	uart_enable_rx();
	//read controller data for next frame
}

void UART_Handler() {
	uart_disable_rx();
	volatile uint32_t uart_read = REG_UART_RHR;
	uart_read = (uart_read << 1) & 0b11111111;
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
}

void loop() {


	while (1) {
		//I have bad feelings about loop()...
	}
}
