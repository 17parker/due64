#pragma once

const uint16_t one = 7;
const uint16_t zro = 21;
const uint16_t stop = 14;
const uint16_t off = 0;
const uint16_t b[2] = { zro, one };

volatile uint32_t frame_count = 0;

const uint32_t stand = 0;
const uint32_t dpad_u = (1 << 4);
const uint32_t n = 0b10001010 << 24;
const uint32_t ne = (0b10001010 << 16) | (0b10001010 << 24);
const uint32_t nw = (0b11110101 << 16) | (0b10001010 << 24);
const uint32_t e = 0b11110101 << 16;
const uint32_t s = 0b11110101 << 24;
const uint32_t sw =(0b11110101 << 16) | (0b11110101 << 24);
const uint32_t se = (0b10001010 << 16) | (0b11110101 << 24);
const uint32_t w = (0b10001010 << 16);
const uint32_t inst_size = 7;
const uint32_t instructions[inst_size] = { dpad_u, stand, n,ne,nw,n,stand };
const uint8_t cycles[inst_size] = {2,15, 70,30,30,200,100 };
volatile uint32_t current_inst = 0;
enum button_offsets {
	A,
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
	X = 16,
	Y = 24
};

volatile uint16_t buffer[36];
const uint16_t buffer_size = 36;
const uint32_t buffer_delay = 1520;
volatile uint8_t buffer_count = 0;

void init_buffers() {
	buffer[0] = off;
	buffer[1] = off;
	buffer[34] = stop;
	buffer[35] = off;
}
inline void set_buffer_count(uint8_t count) { buffer_count = count; }
inline void load_buffer(uint32_t inst) {
	buffer[2] = b[inst & 1];
	buffer[3] = b[(inst >> 1) & 1];
	buffer[4] = b[(inst >> 2) & 1];
	buffer[5] = b[(inst >> 3) & 1];
	buffer[6] = b[(inst >> 4) & 1];
	buffer[7] = b[(inst >> 5) & 1];
	buffer[8] = b[(inst >> 6) & 1];
	buffer[9] = b[(inst >> 7) & 1];
	buffer[10] = b[(inst >> 8) & 1];
	buffer[11] = b[(inst >> 9) & 1];
	buffer[12] = b[(inst >> 10) & 1];
	buffer[13] = b[(inst >> 11) & 1];
	buffer[14] = b[(inst >> 12) & 1];
	buffer[15] = b[(inst >> 13) & 1];
	buffer[16] = b[(inst >> 14) & 1];
	buffer[17] = b[(inst >> 15) & 1];
	buffer[18] = b[(inst >> 16) & 1];
	buffer[19] = b[(inst >> 17) & 1];
	buffer[20] = b[(inst >> 18) & 1];
	buffer[21] = b[(inst >> 19) & 1];
	buffer[22] = b[(inst >> 20) & 1];
	buffer[23] = b[(inst >> 21) & 1];
	buffer[24] = b[(inst >> 22) & 1];
	buffer[25] = b[(inst >> 23) & 1];
	buffer[26] = b[(inst >> 24) & 1];
	buffer[27] = b[(inst >> 25) & 1];
	buffer[28] = b[(inst >> 26) & 1];
	buffer[29] = b[(inst >> 27) & 1];
	buffer[30] = b[(inst >> 28) & 1];
	buffer[31] = b[(inst >> 29) & 1];
	buffer[32] = b[(inst >> 30) & 1];
	buffer[33] = b[(inst >> 31) & 1];
}