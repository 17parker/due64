//Emulate N64 controller to interface with an actual N64
//Using UART at a specific baud rate...
//The N64 data is MSB first, but UART is LSB first. This means the UART TX data is "mirrored"


//I know you're not good at it, but please look at the commit notes to see where you left off

//Use UART to receive N64 command, then use PWM with variable duty cycle to transmit data

#include "SAM3XDUE.h"


volatile uint8_t uart_data[5];				//volatile because might update controller data with interrupts
volatile uint8_t* volatile uart_data_ptr;	//volatile volatile, volatile
volatile uint8_t total_tx_count = 0;	//how many bytes to tx


uint8_t status_response[3] = { 237, 0x00, 0x00 };
uint8_t status_counter = 3;

uint8_t pwm_dtys[5] = { 100, 200, 255, 100, 50 };

uint16_t dtys[2] = { 1000, 20000 };

void pwm_update_unlock() { REG_PWM_SCUC = 1; }


void setup() {
	//Pin setup
	pio_disable_pullup(PIOA, UTXD | URXD);
	pio_enable_output(PIOA, UTXD);
	pio_disable_output(PIOA, URXD);
	pio_set_periph_mode_A(PIOA, UTXD | URXD);
	pio_disable_pio(PIOA, URXD | URXD);

	//PWM setup - PDF page 991 goes over how to setup DMA w/ PWM
	pio_disable_pullup(PIOB, PWML0 | PIN_20B);
	pio_enable_output(PIOB, PWML0 | PIN_20B);
	pio_disable_pio(PIOB, PWML0 | PIN_20B);
	pio_set_periph_mode_B(PIOB, PWML0 | PIN_20B);
	pmc_enable_periph_clk(ID_PWM);
	
	//REG_PWM_CLK = 1 | (1 << 16)|(0b1010 << 8) | 255;
	REG_PWM_CMR0 |= 0b0101;
	REG_PWM_SCM |= 1;
	REG_PWM_CPRD0 = 50000;
	REG_PWM_CDTY0 = 25000;
	REG_PWM_ENA |= 1;

	REG_PWM_IER1 |= 1;
	NVIC_ClearPendingIRQ(PWM_IRQn);
	NVIC_SetPriority(PWM_IRQn, 0);
	NVIC_EnableIRQ(PWM_IRQn);
	while (1) {}


	//UART setup
	pmc_enable_periph_clk(ID_UART);
	/*
	Baud rate = MCK / (16 * CD)
	N64 controller BAUD rate = 250kHz
	250,000 = 84,000,000/(16 * CD) --> CD = 21
	*/
	uart_set_clk_div(21);
	uart_set_parity_ch_mode();
	//Interrupts and PDC setup
	REG_UART_TNCR = 0;	//tx next counter = 0
	REG_UART_RNCR = 0;	//rx next counter = 0;
	REG_UART_IDR = ~0;
	REG_UART_IER = 1;	//enable rxrdy interrupt (data received has not been read yet)
	NVIC_ClearPendingIRQ(UART_IRQn);
	NVIC_SetPriority(UART_IRQn, 0);
	NVIC_EnableIRQ(UART_IRQn);
	uart_enable_rx_tx();
}

volatile uint8_t crnt_dty = 0;
void PWM_Handler() {
	if (crnt_dty == 0)
		crnt_dty = 1;
	else
		crnt_dty = 0;
	REG_PWM_CDTY0 = dtys[crnt_dty];
	pwm_update_unlock();
}

void UART_Handler() {
	switch (REG_UART_RHR) {
	case 0x0:	//status - controller sends 3 bytes: 0x05, 0x00, 0x00 (bitflags)

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
