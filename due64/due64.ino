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

//for the USART when getting controller data from the PC
const uint8_t usart0_rx_buffer_len = 4;
volatile uint8_t usart0_rx_buffer[usart0_rx_buffer_len];

//when controller data comes in, signal for an update instead of updating during the interrupt
//a race condition on this scale means an input might be messed up for one cycle
volatile bool update_ready = false;

//"one", "zro" and "stop" are values for the PWM duty cycle
const uint32_t one = 7;
const uint32_t zro = 21;
const uint32_t stop = 14; //duty cycle for the stop bit
const uint32_t off = 0;
const uint32_t b[2] = { zro, one };
volatile uint16_t buffer[36];
const uint32_t buffer_size = 36;
const uint32_t buffer_delay = 1520;
volatile uint8_t rx_read[8];
const uint32_t rx_read_count = 8;

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

/*
	The values for the buttons should be either 0 or 1
*/
void update_buttons() {
	/*
	Definition from the JS script:
		let buttons = ["A", "B", "Z", "L", "R", "START", "CU", "CD", "CL", "CR", "DU", "DD", "DL", "DR"];
		
	Order that the buttons come in:
		bit number		7	6	5	4	3	2	1	0
		Byte 0			CD	CU	ST	R	L	Z	B	A
		Byte 1			0	0	DR	DL	DD	DU	CR	CL
		Byte 2			X-axis
		Byte 3			Y-axis

	*/
#define B(bytenum, pos) b[((usart0_rx_buffer[(bytenum)] >> (pos)) & 1)]

	buffer[bA] = B(0, 0);
	buffer[bB] = B(0, 1);
	buffer[bZ] = B(0, 2);
	buffer[bSTART] = B(0, 5);
	buffer[bDU] = B(1, 2);
	buffer[bDD] = B(1, 3);
	buffer[bDL] = B(1, 4);
	buffer[bDR] = B(1, 5);
	buffer[bL] = B(0, 3);
	buffer[bR] = B(0, 4);
	buffer[bCU] = B(0, 6);
	buffer[bCD] = B(0, 7);
	buffer[bCL] = B(1, 0);
	buffer[bCR] = B(1, 1);
	
	buffer[sX + 0] = B(2, 7); //stick x
	buffer[sX + 1] = B(2, 6);
	buffer[sX + 2] = B(2, 5);
	buffer[sX + 3] = B(2, 4);
	buffer[sX + 4] = B(2, 3);
	buffer[sX + 5] = B(2, 2);
	buffer[sX + 6] = B(2, 1);
	buffer[sX + 7] = B(2, 0);

	buffer[sY + 0] = B(3, 7); //stick y
	buffer[sY + 1] = B(3, 6);
	buffer[sY + 2] = B(3, 5);
	buffer[sY + 3] = B(3, 4);
	buffer[sY + 4] = B(3, 3);
	buffer[sY + 5] = B(3, 2);
	buffer[sY + 6] = B(3, 1);
	buffer[sY + 7] = B(3, 0);
	
#undef B
}

void init_pins() {
	/*
	Pins for interfacing with the N64:
	Pin 0 - UART RX - BROWN WIRE of the adapter
	Pin 1 - UART TX (unused right now)
	Pin 2 - TIOA output
	Pin 18 - USART0 TX (echoes the RX input)
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
	*/
	REG_USART0_MR = (3 << 6) | (4 << 9);


	/*
		USART0 Baud rate generator register
		I want 230,400 BAUD
		Fractional part is disabled
		BAUD = Selected clock/(oversample * CD)
		0 < CD <= 65535 (0 is disabled)
		An error higher than 5% is "not recommended"
	*/
	REG_USART0_BRGR = 23;


	/*
		USART Interrupt enable register
		ENDRX interrupt (bit 3) - End of transfer signal from PDC active
	*/
	REG_USART0_IER = (1 << 3);
	NVIC_SetPriority(USART0_IRQn, 1);
	volatile uint32_t dummy = REG_UART_RHR;
	NVIC_ClearPendingIRQ(USART0_IRQn);
	NVIC_EnableIRQ(USART0_IRQn);

	/*
		USART0 PDC transfer control register
		Receive pointer register
		Receive count register
		RX  and TX transfer enable
	*/
	REG_USART0_RPR = (uint32_t)usart0_rx_buffer;
	REG_USART0_RCR = usart0_rx_buffer_len;
	REG_USART0_TPR = (uint32_t)usart0_rx_buffer;
	//don't set the TCR here or the transfer will start
	REG_USART0_PTCR = 1 | (1 << 8);

	/*
		USART0 Control register
		reset receiver and transmitter
		enable receiver and transmitter
	*/
	REG_USART0_CR = (1 << 4) | (1 << 6);

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

void init_tc0() {
	pmc_enable_periph_clk(ID_TC0);
	/*
	Clock selected: MCK/8
	Counter is stopped when counter hits RC
	WAVSEL (waveform selection): UP mode with auto trigger on RC compare
	WAVE (waveform mode) is enabled
	ACPA: RA compare effect on TIOA is SET
	ACPC: RC compare effect on TIOA is TOGGLE
	*/
	REG_TC0_CMR0 = 1 | (1 << 6) | (1 << 14) | (1 << 15) | (1 << 16) | (0b11 << 18);	//Using TIOA on TC0

	/*
		The timer in normal operation should only last as long as the UART RX phase (8 bytes)
		In testing mode, the timer restarts the PWM DMA so it needs to be long enough for the PWM to finish
	*/
	REG_TC0_RC0 = 21;

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
	REG_PWM_CDTY0 = off;

	//PWM - peripheral DMA transmitter transfer enable
	REG_PWM_PTCR = (1 << 8);
	REG_PWM_ENA |= 1;
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

	buffer[0] = off;
	buffer[1] = off;
	buffer[34] = stop;
	buffer[35] = off;
	for (uint8_t i = 2; i < 34; ++i) {
		buffer[i] = zro;
	}

	init_pins();
	delayMicros(50000); //Let things stabilize - 50ms

	init_tc0();
	init_pwm();
	init_uart();
	init_usart0();
	//init_dmac();

	delayMicros(10000); //10ms


	REG_UART_RPR = (uint32_t)rx_read;
	REG_UART_RCR = rx_read_count;

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

	REG_UART_RPR = (uint32_t)rx_read; //set UART DMA

	//setting the RCR to not zero starts the transfer
	REG_UART_RCR = rx_read_count;
	//UART end of receive transfer interrupt enable
	REG_UART_IER = (1 << 3);

}


void UART_Handler() {
	volatile uint32_t dummy = REG_UART_SR;
	REG_UART_IDR = ~0;

	REG_PWM_TPR = (uint32_t)buffer;
	//setting the TCR to not zero starts the transfer
	REG_PWM_TCR = buffer_size;

	REG_TC0_RC0 = buffer_delay;
	//software trigger: counter is reset and the clock is started
	REG_TC0_CCR0 = (1 << 2);

}

/*
	This is the order the buttons are sent in (14 buttons)
	let buttons = ["A", "B", "Z", "L", "R", "START", "CU", "CD", "CL", "CR", "DU", "DD", "DL", "DR"];
	Then the X-axis, then the Y-axis

	This fires every time 4 bytes are received
*/
void USART0_Handler() {
	volatile uint32_t dummy = REG_USART0_CSR;

	//echo controller data for testing
	REG_USART0_TPR = (uint32_t)usart0_rx_buffer;
	REG_USART0_TCR = usart0_rx_buffer_len;

	update_ready = true;
	//reset the interrupt
	REG_USART0_RPR = (uint32_t)usart0_rx_buffer;
	REG_USART0_RCR = usart0_rx_buffer_len;
}

void loop() {
	if (update_ready) {
		update_buttons();
		update_ready = false;
	}
}

