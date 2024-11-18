/*
controller joystick x / y is capable of values 0x80 - 0x7F (-128 to 127)
In reality, it is only capable of about +/- 72 (one resource says -81 to 81)
*/

#include "SAM3XDUE.h"
#include "tas_data.h"
#include "tasmc.h"


#define CONTROLLER_TX_TEST

struct Controller_Buttons {
	uint8_t A, B, Z, START, DU, DD, DL, DR, RST, L, R, CU, CD, CL, CR, STICK_X, STICK_Y;

	constexpr Controller_Buttons(uint8_t a, uint8_t b, uint8_t z, uint8_t start, uint8_t du, uint8_t dd,
		uint8_t  dl, uint8_t  dr, uint8_t  rst, uint8_t  l, uint8_t  r, uint8_t cu, uint8_t  cd, uint8_t cl, uint8_t cr,
		int8_t stick_x, int8_t stick_y) : A(a), B(b), Z(z), START(start), DU(du), DD(dd), DL(dl), DR(dr), RST(rst), L(l),
		R(r), CU(cu), CD(cd), CL(cl), CR(cr), STICK_X(stick_x), STICK_Y(stick_y) {

	}
};

struct Controller {
	uint16_t buffer[36];
	static const uint32_t buffer_size = 36;
	static const uint32_t buffer_delay = 1520;
	uint8_t rx_read[8];
	static const uint32_t rx_read_count = 8;
	const uint32_t one = 7;
	const uint32_t zro = 21;
	const uint32_t stop = 14;
	const uint32_t off = 0;
	const uint32_t b[2] = { zro, one };

	enum button_offsets {
		bA = 2,
		bB,
		bZ,
		bSTART,
		bDU,
		bDD,
		bDL,
		bDR,
		bRST,
		bUNUSED,
		bL,
		bR,
		bCU,
		bCD,
		bCL,
		bCR,
		sX = 18,
		sY = 26
	};

	Controller() {
		this->buffer[0] = off;
		this->buffer[1] = off;
		this->buffer[34] = stop;
		this->buffer[35] = off;
		this->buffer[bUNUSED] = zro;
	}

	void update_buttons(const Controller_Buttons& buttons) {
		buffer[bA] = b[buttons.A];
		buffer[bB] = b[buttons.B];
		buffer[bZ] = b[buttons.Z];
		buffer[bSTART] = b[buttons.START];
		buffer[bDU] = b[buttons.DU];
		buffer[bDD] = b[buttons.DD];
		buffer[bDL] = b[buttons.DL];
		buffer[bDR] = b[buttons.DR];
		buffer[bRST] = b[buttons.RST];
		//buffer[bUNUSED] = zro; //unused?
		buffer[bL] = b[buttons.L];
		buffer[bR] = b[buttons.R];
		buffer[bCU] = b[buttons.CU];
		buffer[bCD] = b[buttons.CD];
		buffer[bCL] = b[buttons.CL];
		buffer[bCR] = b[buttons.CR];
		uint8_t sx = (uint8_t)buttons.STICK_X;
		buffer[sX + 0] = b[(sx >> 0) & 1]; //stick x
		buffer[sX + 1] = b[(sx >> 1) & 1];
		buffer[sX + 2] = b[(sx >> 2) & 1];
		buffer[sX + 3] = b[(sx >> 3) & 1];
		buffer[sX + 4] = b[(sx >> 4) & 1];
		buffer[sX + 5] = b[(sx >> 5) & 1];
		buffer[sX + 6] = b[(sx >> 6) & 1];
		buffer[sX + 7] = b[(sx >> 7) & 1];
		uint8_t sy = (uint8_t)buttons.STICK_Y;
		buffer[sY + 0] = b[(sy >> 0) & 1]; //stick y
		buffer[sY + 1] = b[(sy >> 1) & 1];
		buffer[sY + 2] = b[(sy >> 2) & 1];
		buffer[sY + 3] = b[(sy >> 3) & 1];
		buffer[sY + 4] = b[(sy >> 4) & 1];
		buffer[sY + 5] = b[(sy >> 5) & 1];
		buffer[sY + 6] = b[(sy >> 6) & 1];
		buffer[sY + 7] = b[(sy >> 7) & 1];
	}

	void read_buttons() {

	}

};

volatile Controller controller;

/*
	TO DO:
		- Remove all unnecessary config (like the pins and stuff)
		- Finish implementing all the controller functionality
		- instead of having one block of code and sectioning out parts of it with macros for testing,
			separate the different modes into functions and wrap the function calls (this is for clarity)
*/


/*

	I don't think the DMAC is used, that was only for the TFT display and the 8x8 LED matrix
	
	How data transfers work:
		1. The UART RX interrupt is enabled and receives 8 bytes from the console
			- when 8 bytes have been read from the console, it is time for a data TX
		2. When the UART RX interrupt is triggered:
			- the PWM transfer is started to respond to the console
			- the UART RX interrupt is disabled
			- a timer is started, and its interrupt is enabled
		3. When the timer interrupt is triggered:
			- the PWM has finished transmitting
			- the UART RX interrupt is re-enabled
			- because the RX and TX lines are connected, the PWM will trigger the UART RX interrupt if there is no delay
			- the timer interrupt is disabled
*/

void update_buttons(Controller& controller) {
	Controller_Buttons zero_buttons{ .A = 0, .B = 0, .Z = 0, .START = 0,
												.DD = 0, .DL = 0, .DR = 0, .DU = 0, .L = 0, .R = 0,
												.CR = 0, .CL = 0, .CU = 0, .CD = 0, .RST = 0,
												.STICK_X = 0, .STICK_Y = 0 };
	Controller_Buttons test_buttons{ .A = 1, .B = 1, .Z = 0, .START = 0,
											.DD = 0, .DL = 0, .DR = 1, .DU = 0, .L = 0, .R = 0,
											.CR = 1, .CL = 0, .CU = 0, .CD = 0, .RST = 0,
											.STICK_X = 10, .STICK_Y = -35 };

	controller.update_buttons(test_buttons);
}


void setup() {
	/*
	Pins for interfacing with the N64:
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
	//pmc_enable_periph_clk(ID_SMC);
	pmc_enable_periph_clk(ID_DMAC);
	//pmc_enable_periph_clk(ID_SPI0);

	delayMicros(50000); //Let things stabilize - 50ms


	/*
		Clock selected: MCK/8
		Counter is stopped when counter hits RC
		WAVSEL (waveform selection): UP mode with auto trigger on RC compare
		WAVE (waveform mode) is enabled
		ACPA: RA compare effect on TIOA is SET (is this a mistake?)
		ACPC: RC compare effect on TIOA is TOGGLE
	*/
	REG_TC0_CMR0 = 1 | (1 << 6) | (1 << 14) | (1 << 15) | (1 << 16) | (0b11 << 18);	//Using TIOA on TC0
	
#ifndef CONTROLLER_TX_TEST
	REG_TC0_RC0 = 21;
#else
	REG_TC0_RC0 = 210;
#endif

	REG_TC0_IER0 = (1 << 4);
	NVIC_ClearPendingIRQ(TC0_IRQn);
	NVIC_SetPriority(TC0_IRQn, 1);
	NVIC_EnableIRQ(TC0_IRQn);
	REG_TC0_CCR0 = 1 | (1 << 2); //CLK enable and software trigger

	REG_PWM_CLK = 3 | (1 << 9);
	REG_PWM_CMR0 |= 0b1011;	//CLKA
	REG_PWM_SCM |= 1 | (1 << 17);
	REG_PWM_CPRD0 = 28;
	REG_PWM_CDTY0 = off;

	//PWM - peripheral DMA transmitter transfer enable
	REG_PWM_PTCR = (1 << 8); 

	uart_set_clk_div(2);
	uart_set_parity_ch_mode();
	REG_UART_IDR = ~0;

	//UART end of receive transfer interrupt enable
	REG_UART_IER = (1 << 3);

	//UART - peripheral DMA receiver transfer enable
	REG_UART_PTCR = 1;
	NVIC_SetPriority(UART_IRQn, 0);
	uart_enable_rx();

	//******TFT DISPLAY
	//init_tft();
	//draw_frame_count_label();
	//init_smc_dma();
	//init_controller_buffer();

	//Disable all DMAC interrupts
	REG_DMAC_EBCIDR = ~0;
	NVIC_ClearPendingIRQ(DMAC_IRQn);
	NVIC_SetPriority(DMAC_IRQn, 5);
	NVIC_EnableIRQ(DMAC_IRQn);
	REG_PWM_ENA |= 1;
	REG_DMAC_EN = 1;
	/*
	tene0 = 0;
	tene1 = 0;
	tene2 = 0;
	tene3 = 0;
	tene4 = 0;
	tene5 = 0;
	tene6 = 0;
	lli_digit_e6.dscr = (uint32_t)&lli_l1_start;
	load_area(current_area);
	update_lli_l();
	lli_update_frame_numbers();
	update_lli_buttons();
	lli_start_number_draw();
	dmac_wait_for_done();
	*/

	delayMicros(10000);
	volatile uint32_t dummy = REG_UART_RHR;
	NVIC_ClearPendingIRQ(UART_IRQn);
	NVIC_EnableIRQ(UART_IRQn);
	//lli_digit_e6.dscr = 0;

#ifndef CONTROLLER_TX_TEST
	REG_UART_RPR = (uint32_t)controller.rx_read;
	REG_UART_RCR = controller.rx_read_count;
#else
	REG_UART_IDR = ~0;
	REG_PWM_TPR = (uint32_t)controller.buffer;
	//setting the TCR to not zero starts the transfer
	REG_PWM_TCR = controller.buffer_size;

	REG_TC0_RC0 = controller.buffer_delay;
	//software trigger: counter is reset and the clock is started
	REG_TC0_CCR0 = (1 << 2);
#endif

}

void DMAC_Handler() {
	volatile uint32_t dummy = REG_DMAC_EBCISR;

	//if DMAC channel 0 or 1 is active, return
	if (REG_DMAC_CHSR & 0b11)
		return;
	//lli_digit_e6.dscr = 0;

	//disable all DMAC channel interrupts
	REG_DMAC_EBCIDR = ~0;
}

void TC0_Handler() {
	volatile uint32_t dummy = REG_TC0_SR0;

#ifndef CONTROLLER_TX_TEST
	REG_UART_RPR = (uint32_t)controller.rx_read; //set UART DMA

	//setting the RCR to not zero starts the transfer
	REG_UART_RCR = controller.rx_read_count;
#else
	REG_PWM_TPR = (uint32_t)controller.buffer;
	//setting the TCR to not zero starts the transfer
	REG_PWM_TCR = controller.buffer_size;

	REG_TC0_RC0 = controller.buffer_delay;
	//software trigger: counter is reset and the clock is started
	REG_TC0_CCR0 = (1 << 2);
#endif
	/*
	if (!--cycles_remaining) {
		if (!--inst_remaining) {
			if (!--areas_remaining) {
				load_area(current_area);
				areas_remaining = 1;
			}
			else {
				load_area(++current_area);
				update_lli_l();
				lli_digit_e6.dscr = (uint32_t)&lli_l1_start;

				//set the DMAC chained buffer interrupt
				REG_DMAC_EBCIER = 0b11 << 8;
			}
		}
		else
			load_inst(++current_inst);
	}
	*/

#ifndef CONTROLLER_TX_TEST
	//UART end of receive transfer interrupt enable
	REG_UART_IER = (1 << 3);
#else
	REG_UART_IDR = ~0;
#endif

}


void UART_Handler() {
	REG_UART_IDR = ~0;

	REG_PWM_TPR = (uint32_t)controller.buffer;
	//setting the TCR to not zero starts the transfer
	REG_PWM_TCR = controller.buffer_size;

	REG_TC0_RC0 = controller.buffer_delay;
	//software trigger: counter is reset and the clock is started
	REG_TC0_CCR0 = (1 << 2);
	/*
	scroll_dot();
	led_matrix_start_dma();
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
	*/
}

void loop() {
	/*
	if (update_buttons_flag) {
		update_lli_buttons();
		lli_start_frame_draw();
		update_buttons_flag = 0;
	}
	*/
}

