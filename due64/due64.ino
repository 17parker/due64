//Emulate N64 controller to interface with an actual N64
//Using UART at a specific baud rate...
//The N64 data is MSB first, but UART is LSB first. This means the UART TX data is "mirrored"


//I know you're not good at it, but please look at the commit notes to see where you left off

#include "SAM3XDUE.h"


volatile uint8_t uart_data[5];				//volatile because might update controller data with interrupts
volatile uint8_t* volatile uart_data_ptr;	//volatile volatile, volatile
volatile uint8_t total_tx_count = 0;	//how many bytes to tx


uint8_t status_response[3] = { 237, 0x00, 0x00 };
uint8_t status_counter = 3;

void setup() {
	//Pin setup
	pio_disable_pullup(PIOA, UTXD | URXD);
	pio_enable_output(PIOA, UTXD);
	pio_disable_output(PIOA, URXD);
	pio_set_periph_mode_A(PIOA, UTXD | URXD);
	pio_disable_pio(PIOA, URXD | URXD);

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


volatile uint8_t rx_read;
void UART_Handler() {
	rx_read = REG_UART_RHR;
	switch (REG_UART_SR) {
	case 1:		//RXRDY bit
		switch (rx_read){
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
	case (1 << 11):	//TXBUFE bit
		REG_UART_IDR = (1 << 11);	//disable TXBUFE interrupt
		REG_UART_IER = 1;			//Enable RXRDY interrupt
		break;
	default:
		REG_UART_TPR = (uint32_t)status_response;
		REG_UART_TCR = 3;
		REG_UART_IER = (1 << 11);	//Enable TXBUFE interrupt
		REG_UART_THR = *status_response;
		break;
	}
}

void loop() {



	while (1) {
		//I have bad feelings about loop()...
	}
}
