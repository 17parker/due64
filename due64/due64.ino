//I know you're not good at it, but please look at the commit notes to see where you left off
/*
controller joystick x / y is capable of values 0x80 - 0x7F (-128 to 127)
In reality, it is only capable of about +/- 72 (one resource says -81 to 81)
*/

#include "SAM3XDUE.h"
#include "tas_data.h"
#include "tasmc.h"


const uint16_t status_response[28] = { off, off, b[0], b[0], b[0], b[0], b[0], b[1], b[0], b[1],	//0x05
								 b[0], b[0], b[0], b[0], b[0], b[0], b[0], b[0],	//0x00
								 b[0], b[0], b[0], b[0], b[0], b[0], b[0], b[0],	//0x00
								 stop, off };
const uint16_t status_count = 28;
const uint32_t status_delay = 1350;

volatile uint8_t rx_read[8];
const uint16_t rx_count = 8;

void setup() {
	/*Pins for interfacing with the N64
	Pin 0 - UART RX - BROWN WIRE
	Pin 1 - UART TX (unused right now)
	Pin 2 - TIOA output
	Pin 20 - PWMH0 output - PURPLE WIRE
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
	pmc_enable_periph_clk(ID_SMC);
	pmc_enable_periph_clk(ID_DMAC);

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
	REG_PWM_PTCR = (1 << 8); //PWM - DMA TXEN

	uart_set_clk_div(2);
	uart_set_parity_ch_mode();
	REG_UART_IDR = ~0;
	REG_UART_IER = (1 << 3);
	REG_UART_PTCR = 1;
	NVIC_ClearPendingIRQ(UART_IRQn);
	NVIC_SetPriority(UART_IRQn, 0);
	NVIC_EnableIRQ(UART_IRQn);
	uart_enable_rx();

	//******TFT DISPLAY
	init_tft();
	draw_frame_count_label();
	init_smc_dma();
	init_controller_buffer();
	/*
	REG_DMAC_EBCIER = 1;
	NVIC_ClearPendingIRQ(DMAC_IRQn);
	NVIC_SetPriority(DMAC_IRQn, 5);
	NVIC_EnableIRQ(DMAC_IRQn);
	*/
	REG_PWM_ENA |= 1;
	REG_DMAC_EN = 0b11;
	tene0 = 0;
	tene1 = 0;
	tene2 = 0;
	tene3 = 0;
	tene4 = 0;
	tene5 = 0;
	tene6 = 0;
	lli_update_frame_numbers();
	lli_start_number_draw();
	load_area(current_area);
	update_buttons_lli();
	REG_UART_RPR = (uint32_t)rx_read;
	REG_UART_RCR = rx_count;
	dmac_wait_for_done();
}

void DMAC_Handler() {
	volatile uint32_t dummy = REG_DMAC_EBCISR;
	while (REG_DMAC_CHSR & 1) {}
	//lli_start_frame_draw();
}

void TC0_Handler() {
	volatile uint32_t dummy = REG_TC0_SR0;
	REG_UART_RPR = (uint32_t)rx_read;	//set UART DMA
	REG_UART_RCR = rx_count;
	if (!--cycles_remaining) {
		if (!--inst_remaining) {
			if (!--areas_remaining) {
				load_area(current_area);
				areas_remaining = 1;
			}
			else
				load_area(++current_area);
		}
		else
			load_inst(++current_inst);
	}
	REG_UART_IER = (1 << 3);
	return;
}

void UART_Handler() {
	REG_UART_IDR = ~0;
	REG_PWM_TPR = (uint32_t)buffer;
	REG_PWM_TCR = buffer_size;
	REG_TC0_RC0 = buffer_delay;
	REG_TC0_CCR0 = (1 << 2);
	if (++tene0 == 10) {
		tene0 = 0;
		if (++tene1 == 10) {
			tene1 = 0;
			if (++tene2 == 10) {
				tene2 = 0;
				if (++tene3 == 10) {
					tene3 = 0;
					if (++tene4 == 10) {
						tene4 = 0;
						if (++tene5 == 10) {
							tene5 = 0;
							++tene6;
						}
					}
				}
			}
		}
	}
	update_buttons_flag = 1;
	lli_update_frame_numbers();
}

void loop() {
	if (update_buttons_flag) {
		update_buttons_lli();
		lli_start_frame_draw();
		update_buttons_flag = 0;
	}
}