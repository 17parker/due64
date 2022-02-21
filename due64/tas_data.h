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
const instructions boot_area[boot_count] = { { 0x0,120}, { 0x8, 5 }, { 0x0, 30},{ 0x45000000, 4 }, {0x1,5}, {0x0,255}, {0x0,255}, {0x0,255}, {0x0,255}, {0x0,255},{0x0,220} };
const char* boot_label = "boot-up";
const area bootup = { boot_count, boot_area, boot_label };

const uint32_t outside_count = 14;
const instructions outside_area[outside_count] = { { 0x2, 2 },{0x0,10},{ 0x2, 2 },{0x0,37}, { 0xba080000, 30 },{ 0x7a100000, 40 },{ 0x7a000000, 79 }, { 0x7acf0000, 6 }, { 0x7a000004, 1 },{0x7a000005,1},{0x450004,6} ,{0x7a000004,19},{0x7a000005,3},{ 0x525c0000, 45 } };
const char* outside_label = "outside";
const area outside = { outside_count, outside_area, outside_label };

const uint32_t to_BoB1_count = 22;
const instructions inside_area[to_BoB1_count] = { {0x0, 100}, {0x02,1}, {0x0, 45}, {0x8b4a0000, 28},{0x8a0c0000, 8},{0x8a0c0002, 1}, {0x8a0c0000,10}, {0x8a0c0001, 1},{0x1a440000, 26},{0x146a0002, 1},{0xd4280000, 10} ,{0x6a640001, 1}, {0xc2bd0000, 67},
/******************************In the door*/      {0x7a000000,4}, {0x7a000004, 1},{0x7a000005, 1},{0x7aaf0000, 10}, {0x7aaf0000, 22}, { 0x7aaf0004,1},{0x7aaf0005,1}, {0x7aaf0001, 30}, {0x0,85} };
const char* inside_label = "To BoB1";
const area to_BoB1 = { to_BoB1_count, inside_area, inside_label };

const uint32_t BoB1_count = 56;	//****Starts at the A press to select the star					This pause is long because of a stupid fucking goomba ->{0x0,190}<-
const instructions BoB1_inst[BoB1_count] = { {0x1,1}, {0x0,20}, {0x2000,1}, {0x0, 5}, {0x4000,1},  {0x0,60},{0x2,1},{0x0,15},{0x2,1},{0x0,15}, {0x2,1}, {0x0,190}, { 0x7a1f0000, 6 }, { 0x7a1f0004,1 }, { 0x7a1f0005,1 }, { 0x7a1f0000, 35 }, { 0x7a1f0001,1 }, { 0x7a1f0002,1 }, { 0x7a1f0000, 26 },
/************************jump dive rollout*/ {0x7a1f0001,1}, {0x7a000000,16}, {0x7a000001,1},{0x7a000002,1},{0x7a000000,25}, {0x7a000001,1}, {0x7a000000,15},{0x7a000001,1},{0x7a000002,1},{0x7a000000,21},{0x7a000001,1},
/**************************top of the ramp*/ {0x7a000000,15}, {0xf2b30000,3},{0xf2b30004,3},{0xf2b30001,3},{0xf2b30000,34},{0x2,1},{0x0,16}, {0x450000,4},{0x450001,5},{0x450000,14},{0x18a50001,1},{0x18a50003,1},{0x18a50001,6},
/*******************turn to catch the bomb*/ {0x18a50000,11}, {0x0,5}, { 0xab240000, 4},{0x2,1},{0x0,25},{0x8000,1},{0x0,3}, {0x8b000000,4},{0x0,41},{0x2,1},{0x0,25},{0x4,4}, {0x1,20} };
const char* BoB1_label = "  BoB 1";
const area BoB1 = { BoB1_count, BoB1_inst, BoB1_label };

const uint32_t jump_count = 3;
const instructions jump_kick[jump_count] = { { 0x1, 2}, {0x4, 45}, {0x0, 45} };
const char* jump_label = "jumping";
const area jumps = { jump_count, jump_kick, jump_label };

const uint32_t end_count = 2;
const instructions end_stand[end_count] = { {0x0,15 },{0x0,15 } };
const char* end_label = "    fin";
const area end = { end_count, end_stand,end_label };

//This just loads the savestate
const uint32_t d_up_count = 2;
const instructions d_up_area[d_up_count] = { {0x10,3},{0x0,20} };
const char* d_up_label = "   d_up";
const area d_up = { d_up_count, d_up_area,d_up_label };

const uint32_t L_R_count = 1;
const instructions L_R_inst[L_R_count] = { {0xc00,4} };
const char* L_R_label = "    L_R";
const area L_R = { L_R_count, L_R_inst,L_R_label };

const uint32_t area_count = 5;
const area areas[area_count] = { bootup, outside, to_BoB1, BoB1, end };

const area* volatile current_area = areas;
volatile uint32_t areas_remaining = area_count;
const instructions* volatile current_inst = nullptr;
volatile uint32_t current_data = 0;
volatile uint32_t inst_remaining = 0;
volatile uint8_t cycles_remaining = 0;
const char* volatile current_label;
volatile uint8_t out_of_inst = 0;

inline void load_inst(const instructions* inst) {
	current_data = inst->data;
	cycles_remaining = inst->cycles;
	buffer[2] = b[current_data & 1];
	buffer[3] = b[(current_data >> 1) & 1];
	buffer[4] = b[(current_data >> 2) & 1];
	buffer[5] = b[(current_data >> 3) & 1];
	buffer[6] = b[(current_data >> 4) & 1];
	buffer[7] = b[(current_data >> 5) & 1];
	buffer[8] = b[(current_data >> 6) & 1];
	buffer[9] = b[(current_data >> 7) & 1];
	buffer[10] = b[(current_data >> 8) & 1];
	buffer[11] = b[(current_data >> 9) & 1];
	buffer[12] = b[(current_data >> 10) & 1];
	buffer[13] = b[(current_data >> 11) & 1];
	buffer[14] = b[(current_data >> 12) & 1];
	buffer[15] = b[(current_data >> 13) & 1];
	buffer[16] = b[(current_data >> 14) & 1];
	buffer[17] = b[(current_data >> 15) & 1];
	buffer[18] = b[(current_data >> 16) & 1];
	buffer[19] = b[(current_data >> 17) & 1];
	buffer[20] = b[(current_data >> 18) & 1];
	buffer[21] = b[(current_data >> 19) & 1];
	buffer[22] = b[(current_data >> 20) & 1];
	buffer[23] = b[(current_data >> 21) & 1];
	buffer[24] = b[(current_data >> 22) & 1];
	buffer[25] = b[(current_data >> 23) & 1];
	buffer[26] = b[(current_data >> 24) & 1];
	buffer[27] = b[(current_data >> 25) & 1];
	buffer[28] = b[(current_data >> 26) & 1];
	buffer[29] = b[(current_data >> 27) & 1];
	buffer[30] = b[(current_data >> 28) & 1];
	buffer[31] = b[(current_data >> 29) & 1];
	buffer[32] = b[(current_data >> 30) & 1];
	buffer[33] = b[(current_data >> 31) & 1];
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
void init_controller_buffer() {
	buffer[0] = off;
	buffer[1] = off;
	buffer[34] = stop;
	buffer[35] = off;
}