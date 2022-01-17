#pragma once

const uint16_t one = 7;
const uint16_t zro = 21;
const uint16_t stop = 14;
const uint16_t off = 0;
const uint16_t b[2] = { zro, one };
const uint16_t start_bits[2] = { off, off };
const uint16_t stop_bits[2] = { stop, off };
const uint16_t start_count = 2;

volatile uint32_t frame_count = 0;
volatile uint32_t inst_num = 0;
volatile uint8_t inst_countdown = 0;

struct controller_instruction {
	uint8_t count;
	uint32_t data;
};
const uint32_t inst_count = 1;
const controller_instruction instructions[inst_count] = { 0 };
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
	X,
	Y = 24
};

volatile uint16_t buffer_A[32];
volatile uint16_t buffer_B[32];
volatile uint8_t buffer_A_count = 0;
volatile uint8_t buffer_B_count = 0;

inline void load_buffer_A(uint32_t inst) {
	buffer_A[0] = inst & 1;
	buffer_A[1] = (inst >> 1) & 1;
	buffer_A[2] = (inst >> 2) & 1;
	buffer_A[3] = (inst >> 3) & 1;
	buffer_A[4] = (inst >> 4) & 1;
	buffer_A[5] = (inst >> 5) & 1;
	buffer_A[6] = (inst >> 6) & 1;
	buffer_A[7] = (inst >> 7) & 1;
	buffer_A[8] = (inst >> 8) & 1;
	buffer_A[9] = (inst >> 9) & 1;
	buffer_A[10] = (inst >> 10) & 1;
	buffer_A[11] = (inst >> 11) & 1;
	buffer_A[12] = (inst >> 12) & 1;
	buffer_A[13] = (inst >> 13) & 1;
	buffer_A[14] = (inst >> 14) & 1;
	buffer_A[15] = (inst >> 15) & 1;
	buffer_A[16] = (inst >> 16) & 1;
	buffer_A[17] = (inst >> 17) & 1;
	buffer_A[18] = (inst >> 18) & 1;
	buffer_A[19] = (inst >> 19) & 1;
	buffer_A[20] = (inst >> 20) & 1;
	buffer_A[21] = (inst >> 21) & 1;
	buffer_A[22] = (inst >> 22) & 1;
	buffer_A[23] = (inst >> 23) & 1;
	buffer_A[24] = (inst >> 24) & 1;
	buffer_A[25] = (inst >> 25) & 1;
	buffer_A[26] = (inst >> 26) & 1;
	buffer_A[27] = (inst >> 27) & 1;
	buffer_A[28] = (inst >> 28) & 1;
	buffer_A[29] = (inst >> 29) & 1;
	buffer_A[30] = (inst >> 30) & 1;
	buffer_A[31] = (inst >> 31) & 1;
}

inline void load_buffer_B(uint32_t inst) {
	buffer_B[0] = inst & 1;
	buffer_B[1] = (inst >> 1) & 1;
	buffer_B[2] = (inst >> 2) & 1;
	buffer_B[3] = (inst >> 3) & 1;
	buffer_B[4] = (inst >> 4) & 1;
	buffer_B[5] = (inst >> 5) & 1;
	buffer_B[6] = (inst >> 6) & 1;
	buffer_B[7] = (inst >> 7) & 1;
	buffer_B[8] = (inst >> 8) & 1;
	buffer_B[9] = (inst >> 9) & 1;
	buffer_B[10] = (inst >> 10) & 1;
	buffer_B[11] = (inst >> 11) & 1;
	buffer_B[12] = (inst >> 12) & 1;
	buffer_B[13] = (inst >> 13) & 1;
	buffer_B[14] = (inst >> 14) & 1;
	buffer_B[15] = (inst >> 15) & 1;
	buffer_B[16] = (inst >> 16) & 1;
	buffer_B[17] = (inst >> 17) & 1;
	buffer_B[18] = (inst >> 18) & 1;
	buffer_B[19] = (inst >> 19) & 1;
	buffer_B[20] = (inst >> 20) & 1;
	buffer_B[21] = (inst >> 21) & 1;
	buffer_B[22] = (inst >> 22) & 1;
	buffer_B[23] = (inst >> 23) & 1;
	buffer_B[24] = (inst >> 24) & 1;
	buffer_B[25] = (inst >> 25) & 1;
	buffer_B[26] = (inst >> 26) & 1;
	buffer_B[27] = (inst >> 27) & 1;
	buffer_B[28] = (inst >> 28) & 1;
	buffer_B[29] = (inst >> 29) & 1;
	buffer_B[30] = (inst >> 30) & 1;
	buffer_B[31] = (inst >> 31) & 1;
}