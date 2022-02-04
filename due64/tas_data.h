#pragma once

const uint16_t one = 7;
const uint16_t zro = 21;
const uint16_t stop = 14;
const uint16_t off = 0;
const uint16_t b[2] = { zro, one };

volatile uint16_t buffer[36];
const uint16_t buffer_size = 36;
const uint32_t buffer_delay = 1520;
volatile uint32_t frame_count = 0;

const uint32_t stand = 0;

struct instructions {
	uint32_t data;
	uint8_t cycles;
};
struct area {
	const uint32_t instruction_count;
	const instructions* inst;
	const char* area_label;	//7 chars long (8 including '\n')
};

const uint32_t boot_count = 11;
const instructions boot_area[boot_count] = { { stand,120}, { 0x8, 5 }, { stand, 30},{ 0x45000000, 4 }, {0x1,5}, {stand,255}, {stand,255}, {stand,255}, {stand,255}, {stand,255},{stand,220} };
const char* boot_label = "boot-up";
const area bootup = { boot_count, boot_area, boot_label };

const uint32_t outside_count = 14;
const instructions outside_area[outside_count] = { { 0x2, 2 },{stand,10},{ 0x2, 2 },{stand,20}, { 0xba080000, 30 },{ 0x7a100000, 40 },{ 0x7a000000, 79 }, { 0x7acf0000, 6 }, { 0x7a000004, 1 },{0x7a000005,1},{0x450004,6} ,{0x7a000004,19},{0x7a000005,3},{ 0x525c0000, 45 } };
const char* outside_label = "outside";
const area outside = { outside_count, outside_area, outside_label };

const uint32_t to_BoB1_count = 24;
const instructions inside_area[to_BoB1_count] = { {stand, 100}, {0x02,1}, {stand, 45}, {0x8b4a0000, 28},{0x8a0c0000, 8},{0x8a0c0002, 1}, {0x8a0c0000,10}, {0x8a0c0001, 1},{0x1a440000, 26},{0x146a0002, 1},{0xd4280000, 10} ,{0x6a640001, 1}, {0xc2bd0000, 67},
/******************************In the door*/      {0x7a000000,4}, {0x7a000004, 1},{0x7a000005, 1},{0x7aaf0000, 10}, {0x7aaf0000, 22}, { 0x7aaf0004,1},{0x7aaf0005,1}, {0x7aaf0001, 30}, {0x0,85}, {0x1,1}, {stand,100} };
const char* inside_label = "To BoB1";
const area to_BoB1 = { to_BoB1_count, inside_area, inside_label };

const uint32_t BoB1_count = 1;
const instructions BoB1_inst[BoB1_count] = { {0x0,1} };
const char* BoB1_label = "  BoB 1";
const area BoB1 = { BoB1_count, BoB1_inst, BoB1_label };

const uint32_t jump_count = 3;
const instructions jump_kick[jump_count] = { { 0x1, 2}, {0x4, 45}, {stand, 45} };
const char* jump_label = "jumping";
const area jumps = { jump_count, jump_kick, jump_label };

const uint32_t end_count = 2;
const instructions end_stand[end_count] = { {stand,255 },{stand,255 } };
const char* end_label = "    fin";
const area end = { end_count, end_stand,end_label };

//This just loads the savestate
const uint32_t d_up_count = 2;
const instructions d_up_area[d_up_count] = { {0x10,3},{stand,20} };
const char* d_up_label = "   d_up";
const area d_up = { d_up_count, d_up_area,d_up_label };

const uint32_t L_R_count = 1;
const instructions L_R_inst[L_R_count] = { {0xc00,4}};
const char* L_R_label = "    L_R";
const area L_R = { L_R_count, L_R_inst,L_R_label };

const uint32_t area_count = 3;
const area areas[area_count] = { L_R, BoB1, end };

const area* volatile current_area = areas;
volatile uint32_t areas_remaining = area_count;
const instructions* volatile current_inst = nullptr;
volatile uint32_t inst_remaining = 0;
volatile uint8_t cycles_remaining = 0;
const char* volatile current_label;

inline void load_inst(const instructions* inst) {
	cycles_remaining = inst->cycles;
	buffer[2] = b[inst->data & 1];
	buffer[3] = b[(inst->data >> 1) & 1];
	buffer[4] = b[(inst->data >> 2) & 1];
	buffer[5] = b[(inst->data >> 3) & 1];
	buffer[6] = b[(inst->data >> 4) & 1];
	buffer[7] = b[(inst->data >> 5) & 1];
	buffer[8] = b[(inst->data >> 6) & 1];
	buffer[9] = b[(inst->data >> 7) & 1];
	buffer[10] = b[(inst->data >> 8) & 1];
	buffer[11] = b[(inst->data >> 9) & 1];
	buffer[12] = b[(inst->data >> 10) & 1];
	buffer[13] = b[(inst->data >> 11) & 1];
	buffer[14] = b[(inst->data >> 12) & 1];
	buffer[15] = b[(inst->data >> 13) & 1];
	buffer[16] = b[(inst->data >> 14) & 1];
	buffer[17] = b[(inst->data >> 15) & 1];
	buffer[18] = b[(inst->data >> 16) & 1];
	buffer[19] = b[(inst->data >> 17) & 1];
	buffer[20] = b[(inst->data >> 18) & 1];
	buffer[21] = b[(inst->data >> 19) & 1];
	buffer[22] = b[(inst->data >> 20) & 1];
	buffer[23] = b[(inst->data >> 21) & 1];
	buffer[24] = b[(inst->data >> 22) & 1];
	buffer[25] = b[(inst->data >> 23) & 1];
	buffer[26] = b[(inst->data >> 24) & 1];
	buffer[27] = b[(inst->data >> 25) & 1];
	buffer[28] = b[(inst->data >> 26) & 1];
	buffer[29] = b[(inst->data >> 27) & 1];
	buffer[30] = b[(inst->data >> 28) & 1];
	buffer[31] = b[(inst->data >> 29) & 1];
	buffer[32] = b[(inst->data >> 30) & 1];
	buffer[33] = b[(inst->data >> 31) & 1];
}
inline void load_area(const area* area) {
	inst_remaining = area->instruction_count;
	current_inst = area->inst;
	load_inst(current_inst);
	current_label = area->area_label;
}

/*
Each packet of transmitted data is formatted like this:
[off, off, ...(n bits to send)..., stop bit, off) = (n bits) + 4
So (2 off bits) + (32 bits when controller responds) + (stop and off) = 36
The delay timings are mostly arbitrary, but ~45*(n bits)
*/
void init_buffer() {
	buffer[0] = off;
	buffer[1] = off;
	buffer[34] = stop;
	buffer[35] = off;
}