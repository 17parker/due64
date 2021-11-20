//Emulate N64 controller to interface with an actual N64
//Using UART at a specific baud rate...
//The N64 data is MSB first, but UART is LSB first. This means the UART TX data is "mirrored"


//I know you're not good at it, but please look at the commit notes to see where you left off

#include "SAM3XDUE.h"


volatile uint8_t uart_data[5];				//volatile because might update controller data with interrupts
volatile uint8_t* volatile uart_data_ptr;	//volatile volatile, volatile
volatile uint8_t tx_counter = 0;		//how many bytes have been tx'd so far
volatile uint8_t total_tx_count = 0;	//how many bytes to tx
void reset_tx_counter() { tx_counter = 0; }

void enable_rx_rdy_interrupt() { REG_UART_IER |= 1; }
void disable_rx_rdy_interrupt() { REG_UART_IDR |= 1; }
void enable_tx_rdy_interrupt() { REG_UART_IER |= (1 << 1); }
void disable_tx_rdy_interrupt() { REG_UART_IDR |= (1 << 1); }


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
	/*
	UART interrupts used :
	RX_RDY - RX ready - used to start tx by writing to TX holding register
	TX_RDY - TX ready - used to continue TX by writing to TX register
		Note: Count the number of tx, then compare tx_count to desired number of tx
	*/
	REG_UART_IDR = ~0;
	enable_rx_rdy_interrupt();
	NVIC_ClearPendingIRQ(UART_IRQn);
	NVIC_SetPriority(UART_IRQn, 0);
	NVIC_EnableIRQ(UART_IRQn);
	uart_enable_rx_tx();
}


void UART_Handler() {
	if (REG_UART_SR & (1 << 1)) {
		//TX_RDY interrupt
		if (tx_counter < total_tx_count) {
			disable_tx_rdy_interrupt();
			reset_tx_counter();
			enable_rx_rdy_interrupt();
			return;
		}
		else {
			uart_write_tx(0);
			++tx_counter;
		}
	}
	else {
		//RX_RDY interrupt
		/*
		Note: N64 is MSB first, UART is LSB first
		That means the received UART data is "mirrored"
		*/
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
		disable_rx_rdy_interrupt();
		uart_write_tx(0);
		enable_tx_rdy_interrupt();
	}

}

void loop() {



	while (1) {
		//I have bad feelings about loop()...
	}
}
