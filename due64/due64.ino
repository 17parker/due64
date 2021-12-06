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

uint16_t status_response[25] = { b[0], b[0], b[0], b[0], b[0], b[1], b[0], b[1],	//0x05
								 b[0], b[0], b[0], b[0], b[0], b[0], b[0], b[0],	//0x00
								 b[0], b[0], b[0], b[0], b[0], b[0], b[0], b[0],	//0x00
								 stop };
uint16_t status_count = 25;


/*
controller joystick x / y is capable of values 0x80 - 0x7F (-128 to 127)
In reality, it is only capable of about +/- 72 (one resource says -81 to 81)
*/

volatile uint16_t controller_data[33];

void setup() {
	for (uint8_t i = 0; i < 32; i++) {
		controller_data[i] = zro;
	}
	controller_data[0] = one;
	controller_data[32] = stop;
	//Pin setup
	pio_disable_pullup(PIOA, UTXD | URXD);
	pio_enable_output(PIOA, UTXD);
	pio_disable_output(PIOA, URXD);
	pio_set_periph_mode_A(PIOA, UTXD | URXD);
	pio_disable_pio(PIOA, URXD | URXD);
	pio_disable_pullup(PIOB, PWML0);
	pio_enable_output(PIOB, PWML0);
	pio_disable_pio(PIOB, PWML0);
	pio_set_periph_mode_B(PIOB, PWML0);
	pmc_enable_periph_clk(ID_PWM);
	pmc_enable_periph_clk(ID_UART);

	REG_PWM_CLK = 3 | (1 << 9);
	REG_PWM_CMR0 |= 0b1011;	//CLKA
	REG_PWM_SCM |= 1 | (1 << 17);
	REG_PWM_CPRD0 = 28;
	REG_PWM_CDTY0 = off;
	REG_PWM_TPR = (uint32_t)status_response;
	REG_PWM_TCR = 25;
	REG_PWM_PTCR = (1 << 8);
	REG_PWM_IER2 |= (1 << 2);
	NVIC_ClearPendingIRQ(PWM_IRQn);
	NVIC_SetPriority(PWM_IRQn, 1);
	NVIC_EnableIRQ(PWM_IRQn);
	REG_PWM_ENA |= 1;
	/*
	Baud rate = MCK / (16 * CD)
	N64 controller BAUD rate = 250kHz
	250,000 = 84,000,000/(16 * CD) --> CD = 21
	*/
	uart_set_clk_div(21);
	uart_set_parity_ch_mode();
	REG_UART_TNCR = 0;
	REG_UART_RNCR = 0;
	REG_UART_IDR = ~0;
	REG_UART_IER = 1;	//enable rxrdy interrupt (data received has not been read yet)
	NVIC_ClearPendingIRQ(UART_IRQn);
	NVIC_SetPriority(UART_IRQn, 0);
	NVIC_EnableIRQ(UART_IRQn);
	uart_enable_rx();
}


void PWM_Handler() {
	volatile uint32_t dummy = REG_PWM_ISR2;
	REG_PWM_CDTY0 = off;
	//read controller data for next frame
}

void UART_Handler() {
	switch (REG_UART_RHR) {
	case 0x0:	//status - controller sends 3 bytes: 0x05, 0x00, 0x00 (bitflags)
		REG_PWM_TPR = (uint32_t)status_response;
		REG_PWM_TCR = 25;
		break;

	case 0xFE:	//poll - controller sends 4 bytes
		break;

	case 0xFD:	//read - controller returns 33 bytes of 0x00
		break;

	case 0xFC:	//write - please don't make me do it
		break;

	case 0xFF:	//reset - controller responds identically to command 0x00
		break;
	}
}

void loop() {


	while (1) {
		//I have bad feelings about loop()...
	}
}
