/*
	controller joystick x / y is capable of values 0x80 - 0x7F (-128 to 127)
	In reality, it is only capable of about +/- 72 (one resource says -81 to 81)


	DMAC was only for the TFT display and the 8x8 LED matrix

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

	I only have one UART but I can run the USART as UART and that also means they have separate interrupt handlers
*/

#include "SAM3XDUE.h"

#define CONTROLLER_TX_TEST

struct Controller_Buttons {
	const uint8_t A, B, Z, START, DU, DD, DL, DR, RST, L, R, CU, CD, CL, CR, STICK_X, STICK_Y;

	/*
	Controller_Buttons(uint8_t a, uint8_t b, uint8_t z, uint8_t start, uint8_t du, uint8_t dd,
		uint8_t  dl, uint8_t  dr, uint8_t  rst, uint8_t  l, uint8_t  r, uint8_t cu, uint8_t  cd, uint8_t cl, uint8_t cr,
		int8_t stick_x, int8_t stick_y) : A(a), B(b), Z(z), START(start), DU(du), DD(dd), DL(dl), DR(dr), RST(rst), L(l),
		R(r), CU(cu), CD(cd), CL(cl), CR(cr), STICK_X(stick_x), STICK_Y(stick_y) {
	}
	*/
};

/*
	I was thinking that I could theoretically extend this to have more than one controller
	Then I'd have to have different PWMs, timers and UARTs for each one
	I am not ready for that yet
*/
struct Controller {
	uint16_t buffer[36];
	static const uint32_t buffer_size = 36;
	static const uint32_t buffer_delay = 1520;
	uint8_t rx_read[8];
	static const uint32_t rx_read_count = 8;

	//"one", "zro" and "stop" are values for the PWM duty cycle
	static const uint32_t one = 7;
	static const uint32_t zro = 21;
	static const uint32_t stop = 14; //duty cycle for the stop bit
	static const uint32_t off = 0;
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

	/*
		The values for the buttons should be either 0 or 1

		This can be optimized in a nasty way. The assembly generated is:
			ld r3, <val>
			uxth r3
			str buffer[i], <val>

		If the controller buttons are written to the buffer on construction, there isn't a need to copy at all
		This gets a lot nastier if the Controller_Buttons object is volatile. Each button turns into:
			2 ldr, 1 add, 1 uxth, 1 strh
		Each of the stick axes gets turned into 1 ldr, 1 add, 2 extensions and one store (~one less cycle)

		It might be better to have a volatile bool signal to the program that the buttons need to be updated, then update the buttons outside of an interrupt
		That way the Controller_Buttons object does not need to be volatile

		I think these optimizations are overkill for this and it wouldn't be easy if I ever implement multiple controllers

	*/

	void update_buttons(volatile Controller_Buttons& buttons) volatile {
		buffer[bA] = b[buttons.A];
		buffer[bB] = b[buttons.B];
		buffer[bZ] = b[buttons.Z];
		buffer[bSTART] = b[buttons.START];
		buffer[bDU] = b[buttons.DU];
		buffer[bDD] = b[buttons.DD];
		buffer[bDL] = b[buttons.DL];
		buffer[bDR] = b[buttons.DR];
		buffer[bRST] = b[buttons.RST];
		//buffer[bUNUSED] = zro; // this is set in the Controller constructor
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
		// I'm not sure if I'm going to implement this here
	}

};

volatile Controller controller;

/*
	TO DO:
		- Finish implementing all the controller functionality
*/

#ifdef CONTROLLER_TX_TEST
volatile bool update_ready = false;
volatile bool update_mode = true;
#endif

void update_buttons(volatile Controller& controller, volatile bool mode = true) {

	volatile Controller_Buttons zero_buttons{ .A = 0, .B = 0, .Z = 0, .START = 0,
												.DU = 0, .DD = 0, .DL = 0, .DR = 0, .RST = 0, .L = 0,
												.R = 0, .CU = 0, .CD = 0, .CL = 0, .CR = 0,
												.STICK_X = 0, .STICK_Y = 0 };
	volatile Controller_Buttons test_buttons{ .A = 0, .B = 0, .Z = 0, .START = 1,
												.DU = 0, .DD = 0, .DL = 1, .DR = 1, .RST = 0, .L = 0,
												.R = 1, .CU = 1, .CD = 1, .CL = 0, .CR = 0,
												.STICK_X = 42, .STICK_Y = -35 };

	if (mode)
		controller.update_buttons(zero_buttons);
	else
		controller.update_buttons(test_buttons);
}


void update_buttons(volatile Controller& controller) {

}

void init_pins() {
	/*
	Pins for interfacing with the N64:
	Pin 0 - UART RX - BROWN WIRE of the adapter
	Pin 1 - UART TX (unused right now)
	Pin 2 - TIOA output
	Pin 18 - USART0 TX (unused right now)
	Pin 19 - USART0 RX
	Pin 20 - PWMH0 output - PURPLE WIRE
	*/
	pio_disable_pullup(PIOA, UTXD | URXD | TX0 | RX0);
	pio_enable_output(PIOA, UTXD | TX0);
	pio_disable_output(PIOA, URXD | RX0);
	pio_set_periph_mode_A(PIOA, UTXD | URXD | TX0 | RX0);
	pio_disable_pio(PIOA, URXD | URXD | TX0 | RX0);
	pio_disable_pullup(PIOB, PIN_2B | PIN_20B);
	pio_enable_output(PIOB, PIN_2B | PIN_20B);
	pio_disable_pio(PIOB, PIN_2B | PIN_20B);
	pio_set_periph_mode_B(PIOB, PIN_2B | PIN_20B);

	//I just like having these around for some reason
	//pmc_enable_periph_clk(ID_SMC);
	//pmc_enable_periph_clk(ID_DMAC);
	//pmc_enable_periph_clk(ID_SPI0);
}


/*
	USART will be used to get button data from the PC (web server)
*/
const uint8_t usart0_rx_buffer_len = 4;
uint8_t usart0_rx_buffer[usart0_rx_buffer_len];
void init_usart0() {
	pmc_enable_periph_clk(ID_USART0);

	/*
		USART0 Mode register
		Normal mode
		MCK is selected
		character length is 8 bits
		Asynchronous mode
		No parity
		1 stop bit
		normal channel mode
		LSB sent first (bit 16 = 0, set bit 16 = 1 for MSB first)
		16x oversampling mode (bit 19, set bit 19 to 1 for 8x)
		NACK not generated (bit 20)
	*/
	REG_USART0_MR = (3 << 6) | (4 << 9) | (1 << 20);


	/*
		USART0 Baud rate generator register
		I want 230,400 BAUD
		Fractional part is disabled
		BAUD = Selected clock/(oversample * CD)
		0 < CD <= 65535 (0 is disabled)
		CD = 365 --> BAUD = 230,136 (about .11% error)
		CD = 364 --> BAUD = 230,769 (about .16% error)
		An error higher than 5% is "not recommended"
	*/
	REG_USART0_BRGR = 365;


	/*
		USART Interrupt enable and mask registers
		ENDRX interrupt (bit 3) - End of transfer signal from PDC active
	*/
	REG_USART0_IMR = (1 << 3);
	REG_USART0_IER = (1 << 3);


	/*
		USART0 PDC transfer control register
		Receive pointer register
		Receive count register
		RX transfer enable
	*/
	REG_USART0_RPR = (uint32_t)usart0_rx_buffer;
	REG_USART0_RCR = usart0_rx_buffer_len;
	REG_USART0_PTCR = 1;

	/*
		USART0 Control register
		reset receiver and transmitter
		enable receiver and transmitter
	*/
	REG_USART0_CR = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 6);

}

void init_tc0() {
	pmc_enable_periph_clk(ID_TC0);
	/*
	Clock selected: MCK/8
	Counter is stopped when counter hits RC
	WAVSEL (waveform selection): UP mode with auto trigger on RC compare
	WAVE (waveform mode) is enabled
	ACPA: RA compare effect on TIOA is SET (is this a mistake?)
	ACPC: RC compare effect on TIOA is TOGGLE
	*/
	REG_TC0_CMR0 = 1 | (1 << 6) | (1 << 14) | (1 << 15) | (1 << 16) | (0b11 << 18);	//Using TIOA on TC0

	/*
		The timer in normal operation should only last as long as the UART RX phase (8 bytes)
		In testing mode, the timer restarts the PWM DMA so it needs to be long enough for the PWM to finish
	*/
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
}

void init_pwm() {
	pmc_enable_periph_clk(ID_PWM);
	//Base CLK source = MCK/4, DIV = 3 (~250kHz)
	REG_PWM_CLK = 3 | (0b10 << 8);

	//PWM clock source = CLKA
	REG_PWM_CMR0 |= 0b1011;	//CLKA

	//PWM is a synchronous channel, WRDY flag in PWM interrupt is set to 1 as soon as update period has elapsed
	//This is necessary for the PWM peripheral DMA
	REG_PWM_SCM |= 1 | (1 << 17);

	//Upper value for the duty cycle (what it "counts" to)
	REG_PWM_CPRD0 = 28;
	//current duty cycle
	REG_PWM_CDTY0 = Controller::off;

	//PWM - peripheral DMA transmitter transfer enable
	REG_PWM_PTCR = (1 << 8);
	REG_PWM_ENA |= 1;
}

void init_uart() {
	pmc_enable_periph_clk(ID_UART);
	uart_set_clk_div(2);
	uart_set_parity_ch_mode();
	//UART end of receive transfer interrupt enable
	REG_UART_IER = (1 << 3);
	//UART - peripheral DMA receiver transfer enable
	REG_UART_PTCR = 1;
	NVIC_SetPriority(UART_IRQn, 0);
	uart_enable_rx();
	volatile uint32_t dummy = REG_UART_RHR;
	NVIC_ClearPendingIRQ(UART_IRQn);
	NVIC_EnableIRQ(UART_IRQn);

}

void init_dmac() {
	//Disable all DMAC interrupts
	REG_DMAC_EBCIDR = ~0;
	NVIC_ClearPendingIRQ(DMAC_IRQn);
	NVIC_SetPriority(DMAC_IRQn, 5);
	NVIC_EnableIRQ(DMAC_IRQn);
	REG_DMAC_EN = 1;
}

void setup() {

	init_pins();
	delayMicros(50000); //Let things stabilize - 50ms

	init_tc0();
	init_pwm();
#ifndef CONTROLLER_TX_TEST
	init_uart();
#endif
	//init_dmac();
	init_usart0();

	delayMicros(10000);


	update_buttons(controller, update_mode);

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

	//disable all DMAC channel interrupts
	REG_DMAC_EBCIDR = ~0;
}

void TC0_Handler() {
	volatile uint32_t dummy = REG_TC0_SR0;

#ifndef CONTROLLER_TX_TEST
	REG_UART_RPR = (uint32_t)controller.rx_read; //set UART DMA

	//setting the RCR to not zero starts the transfer
	REG_UART_RCR = controller.rx_read_count;
	//UART end of receive transfer interrupt enable
	REG_UART_IER = (1 << 3);
#else
	REG_PWM_TPR = (uint32_t)controller.buffer;
	//setting the TCR to not zero starts the transfer
	REG_PWM_TCR = controller.buffer_size;

	REG_TC0_RC0 = controller.buffer_delay;
	//software trigger: counter is reset and the clock is started
	REG_TC0_CCR0 = (1 << 2);
	update_ready = true;
	REG_UART_IDR = ~0;
#endif

}


void UART_Handler() {
	volatile uint32_t dummy = REG_UART_SR;
	REG_UART_IDR = ~0;

	REG_PWM_TPR = (uint32_t)controller.buffer;
	//setting the TCR to not zero starts the transfer
	REG_PWM_TCR = controller.buffer_size;

	REG_TC0_RC0 = controller.buffer_delay;
	//software trigger: counter is reset and the clock is started
	REG_TC0_CCR0 = (1 << 2);

}

void USART0_Handler() {
	volatile uint32_t dummy = REG_USART0_CSR;

	//reset the interrupt
	REG_USART0_RPR = (uint32_t)usart0_rx_buffer;
	REG_USART0_RCR = usart0_rx_buffer_len;
}

void loop() {
#ifndef CONTROLLER_TX_TEST
	//put the real code here
#else
	if (update_ready) {
		update_buttons(controller, update_mode);
		update_mode = !update_mode;
		update_ready = false;
	}
#endif
}

