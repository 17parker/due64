//I know you're not good at it, but please look at the commit notes to see where you left off
/*
controller joystick x / y is capable of values 0x80 - 0x7F (-128 to 127)
In reality, it is only capable of about +/- 72 (one resource says -81 to 81)
*/
#include "SAM3XDUE.h"

inline void init_player_buffers();
inline void p1_read_buttons();
inline void p2_read_buttons();
inline void p1_init_buttons();
inline void p2_init_buttons();

const uint32_t one = 7;
const uint32_t zro = 21;
const uint32_t stop = 14;
const uint32_t off = 0;
const uint32_t b[2] = { zro, one };

enum button_offsets {
	A = 2,
	B,
	Z,
	START,
	DU,
	DD,
	DL,
	DR,
	RST,
	UNUSED,
	L,
	R,
	CU,
	CD,
	CL,
	CR,
	X = 18,
	Y = 26
};

volatile uint16_t p1_buffer[36];
volatile uint16_t p2_buffer[36];
const uint32_t buffer_size = 36;
const uint32_t buffer_delay = 1520;

volatile uint32_t p1_button_read = 0;

inline void init_player_buffers() {
	p1_buffer[0] = off;
	p1_buffer[1] = off;
	p1_buffer[34] = stop;
	p1_buffer[35] = off;
	p2_buffer[0] = off;
	p2_buffer[1] = off;
	p2_buffer[34] = stop;
	p2_buffer[35] = off;
	for (uint32_t i = 2; i < 34; ++i) {
		p1_buffer[i] = zro;
		p2_buffer[i] = zro;
	}
}

const uint16_t status_response[28] = { off, off, b[0], b[0], b[0], b[0], b[0], b[1], b[0], b[1],	//0x05
								 b[0], b[0], b[0], b[0], b[0], b[0], b[0], b[0],	//0x00
								 b[0], b[0], b[0], b[0], b[0], b[0], b[0], b[0],	//0x00
								 stop, off };
const uint16_t status_count = 28;
const uint32_t status_delay = 1350;

volatile uint8_t rx_read[8];
const uint16_t rx_count = 8;

/*PINS USED
	PIOA:
		PA8 - URXD - RX console command
		PA9 - UTXD (unused at the moment)

	PIOB:
		PB25 - TIOA output - starts on PWM TX, URXD is disabled so it does not "receive" PWM output
						   - this can be disabled, used for oscilloscope syncing
		PB12 - PWMH0 output - send data to console

	PIOC:
		PC10-PC17, PC22-PC28

*/



void setup() {
	Serial.begin(115200);
	//I'm just going to leave the periph clocks enabled
	pmc_enable_periph_clk(ID_PWM);
	pmc_enable_periph_clk(ID_UART);
	pmc_enable_periph_clk(ID_TC0);
	pmc_enable_periph_clk(ID_SMC);
	pmc_enable_periph_clk(ID_DMAC);
	pmc_enable_periph_clk(ID_SPI0);
	pmc_enable_periph_clk(ID_PIOC);

	init_player_buffers();
	p1_init_buttons();

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

	delayMicros(500000); //Let things stabilize - 500ms

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
	NVIC_SetPriority(UART_IRQn, 0);
	uart_enable_rx();

	REG_UART_RPR = (uint32_t)rx_read;
	REG_UART_RCR = rx_count;
	delayMicros(10000);
	volatile uint32_t dummy = REG_UART_RHR;
	NVIC_ClearPendingIRQ(UART_IRQn);
	NVIC_EnableIRQ(UART_IRQn);

	REG_PWM_TPR = (uint32_t)p1_buffer;
	REG_PWM_TCR = buffer_size;
}


void DMAC_Handler() {
	volatile uint32_t dummy = REG_DMAC_EBCISR;
	if (REG_DMAC_CHSR & 0b11)
		return;

	REG_DMAC_EBCIDR = ~0;
}

void TC0_Handler() {
	volatile uint32_t dummy = REG_TC0_SR0;
	REG_UART_RPR = (uint32_t)rx_read;	//set UART DMA
	REG_UART_RCR = rx_count;
	REG_UART_IER = (1 << 3);
	p1_read_buttons();
}

void UART_Handler() {
	REG_UART_IDR = ~0;
	REG_PWM_TPR = (uint32_t)p1_buffer;
	REG_PWM_TCR = buffer_size;
	REG_TC0_RC0 = buffer_delay;
	REG_TC0_CCR0 = (1 << 2);
}

void loop() {

}


inline void p1_read_buttons() {
	uint32_t pins = REG_PIOC_PDSR;
	/*
	These pins are pulled up, so they're low when they're pressed
	PC10 - A
	PC11 - B
	PC12 - CU
	PC13 - CD
	PC14 - CL
	PC15 - CR
	PC16 - DU
	PC17 - DD
	PC22 - DL
	PC23 - DR
	PC24 - START
	PC25 - L
	PC26 - R
	PC27 - Z
*/
	if ((pins >> 10) & 1)
		p1_buffer[A] = zro;
	else
		p1_buffer[A] = one;
	if ((pins >> 11) & 1)
		p1_buffer[B] = zro;
	else
		p1_buffer[B] = one;


}


inline void p2_read_buttons() {

}


inline void p1_init_buttons() {
	/*
		PC10 - A
		PC11 - B
		PC12 - CU
		PC13 - CD
		PC14 - CL
		PC15 - CR
		PC16 - DU
		PC17 - DD
		PC22 - DL
		PC23 - DR
		PC24 - START
		PC25 - L
		PC26 - R
		PC27 - Z
	*/
	pio_enable_pio(PIOC, 0xFC3FC00); //trust me with this hex number
	pio_disable_output(PIOC, 0xFC3FC00);
	pio_enable_pullup(PIOC, 0xFC3FC00);
}

inline void p2_init_buttons() {

}