#pragma once
#include "sprites.h"

const uint32_t one = 7;
const uint32_t zro = 21;
const uint32_t stop = 14;
const uint32_t off = 0;
const uint32_t b[2] = { zro, one };

volatile uint16_t buffer[36];
const uint32_t buffer_size = 36;
const uint32_t buffer_delay = 1520;
volatile uint32_t frame_count = 0;

struct instructions {
	uint32_t data;
	uint32_t cycles;
};

/*
	For simplicity, the entire label is always drawn
	If the label is shorter than the maximum length, the rest of the letters are blanks
*/
struct area {
	const uint32_t instruction_count;
	const instructions* inst;
	const uint32_t* label;
};

const uint32_t boot_count = 1;
const instructions boot_area[boot_count] = { { 0x0,120} };
const uint32_t boot_label[] = { (uint32_t)l_B,(uint32_t)l_O,(uint32_t)l_O ,(uint32_t)l_T,(uint32_t)l_hyphen,(uint32_t)l_U,(uint32_t)l_P,0 };
const area bootup = { boot_count, boot_area, boot_label };

const uint32_t file_sel_count = 4;
const instructions file_sel_inst[file_sel_count] = { { 0x8, 5 }, { 0x0, 30},{ 0x45000000, 4 }, {0x1,5} };
const uint32_t file_sel_label[] = { (uint32_t)l_F, (uint32_t)l_I, (uint32_t)l_L, (uint32_t)l_E, (uint32_t)l_space,
										   (uint32_t)l_S, (uint32_t)l_E, (uint32_t)l_L, (uint32_t)l_E, (uint32_t)l_C, (uint32_t)l_T, 0 };
const area file_sel = { file_sel_count, file_sel_inst, file_sel_label };

const uint32_t pp_speech_count = 6;
const instructions pp_speech_inst[pp_speech_count] = { {0x0,255}, {0x0,255}, {0x0,255}, {0x0,255}, {0x0,255},{0x0,220} };
const uint32_t pp_speech_label[] = { (uint32_t)l_E,(uint32_t)l_X,(uint32_t)l_C,(uint32_t)l_I,(uint32_t)l_T,(uint32_t)l_I,(uint32_t)l_N,(uint32_t)l_G,(uint32_t)l_space,
									 (uint32_t)l_G, (uint32_t)l_A, (uint32_t)l_M, (uint32_t)l_E, (uint32_t)l_P, (uint32_t)l_L, (uint32_t)l_A, (uint32_t)l_Y,(uint32_t)l_ellipsis, 0 };
const area pp_speech = { pp_speech_count,pp_speech_inst,pp_speech_label };

const uint32_t outside_count = 14;
const instructions outside_area[outside_count] = { { 0x2, 2 },{0x0,10},{ 0x2, 2 },{0x0,37}, { 0xba080000, 30 },{ 0x7a100000, 40 },{ 0x7a000000, 79 }, { 0x7acf0000, 6 }, { 0x7a000004, 1 },{0x7a000005,1},{0x450004,6} ,{0x7a000004,19},{0x7a000005,3},{ 0x525c0000, 45 } };
const uint32_t outside_label[] = { (uint32_t)l_O,(uint32_t)l_U ,(uint32_t)l_T ,(uint32_t)l_S ,(uint32_t)l_I ,(uint32_t)l_D ,(uint32_t)l_E,(uint32_t)l_space,
										   (uint32_t)l_T,(uint32_t)l_H,(uint32_t)l_E,(uint32_t)l_space,
										   (uint32_t)l_C,(uint32_t)l_A,(uint32_t)l_S,(uint32_t)l_T,(uint32_t)l_L,(uint32_t)l_E,0 };
const area outside = { outside_count, outside_area, outside_label };

const uint32_t to_BoB1_count = 22;
const instructions inside_area[to_BoB1_count] = { {0x0, 100}, {0x02,1}, {0x0, 45}, {0x8b4a0000, 28},{0x8a0c0000, 8},{0x8a0c0002, 1}, {0x8a0c0000,10}, {0x8a0c0001, 1},{0x1a440000, 26},{0x146a0002, 1},{0xd4280000, 10} ,{0x6a640001, 1}, {0xc2bd0000, 67},
/******************************In the door*/      {0x7a000000,4}, {0x7a000004, 1},{0x7a000005, 1},{0x7aaf0000, 10}, {0x7aaf0000, 22}, { 0x7aaf0004,1},{0x7aaf0005,1}, {0x7aaf0001, 30}, {0x0,85} };
const uint32_t to_BoB1_label[] = { (uint32_t)l_G,(uint32_t)l_O,(uint32_t)l_I,(uint32_t)l_N,(uint32_t)l_G,(uint32_t)l_space,
									(uint32_t)l_T,(uint32_t)l_O,(uint32_t)l_space,(uint32_t)l_B,(uint32_t)l_O,(uint32_t)l_B,0 };
const area to_BoB1 = { to_BoB1_count, inside_area, to_BoB1_label };

const uint32_t BoB1_count = 56;	//****Starts at the A press to select the star					This pause is long because of a stupid fucking goomba ->{0x0,190}<-
const instructions BoB1_inst[BoB1_count] = { {0x1,1}, {0x0,20}, {0x2000,1}, {0x0, 5}, {0x4000,1},  {0x0,60},{0x2,1},{0x0,15},{0x2,1},{0x0,15}, {0x2,1}, {0x0,190}, { 0x7a1f0000, 6 }, { 0x7a1f0004,1 }, { 0x7a1f0005,1 }, { 0x7a1f0000, 35 }, { 0x7a1f0001,1 }, { 0x7a1f0002,1 }, { 0x7a1f0000, 26 },
/************************jump dive rollout*/ {0x7a1f0001,1}, {0x7a000000,16}, {0x7a000001,1},{0x7a000002,1},{0x7a000000,25}, {0x7a000001,1}, {0x7a000000,15},{0x7a000001,1},{0x7a000002,1},{0x7a000000,21},{0x7a000001,1},
/**************************top of the ramp*/ {0x7a000000,15}, {0xf2b30000,3},{0xf2b30004,3},{0xf2b30001,3},{0xf2b30000,34},{0x2,1},{0x0,16}, {0x450000,4},{0x450001,5},{0x450000,14},{0x18a50001,1},{0x18a50003,1},{0x18a50001,6},
/*******************turn to catch the bomb*/ {0x18a50000,11}, {0x0,5}, { 0xab240000, 4},{0x2,1},{0x0,25},{0x8000,1},{0x0,3}, {0x8b000000,4},{0x0,41},{0x2,1},{0x0,25},{0x4,4}, {0x1,20} };
const uint32_t BoB1_label[] = {(uint32_t)l_B,(uint32_t)l_E,(uint32_t)l_H,(uint32_t)l_I,(uint32_t)l_N,(uint32_t)l_D,(uint32_t)l_space,
								(uint32_t)l_C,(uint32_t)l_H,(uint32_t)l_A,(uint32_t)l_I,(uint32_t)l_N,(uint32_t)l_space,(uint32_t)l_C,(uint32_t)l_H,(uint32_t)l_O,(uint32_t)l_M,(uint32_t)l_P,(uint32_t)l_S,
								(uint32_t)l_space,(uint32_t)l_G,(uint32_t)l_A,(uint32_t)l_T,(uint32_t)l_E,0 };
const area BoB1 = { BoB1_count, BoB1_inst, BoB1_label };

const uint32_t crap_count = 2;
const instructions crap_inst[crap_count] = { {0xFFFFFFFF,1},{0,1} };
const uint32_t crap_label[] = { (uint32_t)l_A,(uint32_t)l_L,(uint32_t)l_L,(uint32_t)l_space,(uint32_t)l_D,(uint32_t)l_O,(uint32_t)l_N,(uint32_t)l_E,
								(uint32_t)l_hyphen,(uint32_t)l_S,(uint32_t)l_E,(uint32_t)l_I,(uint32_t)l_Z,(uint32_t)l_U,(uint32_t)l_R,(uint32_t)l_E,
								(uint32_t)l_space, (uint32_t)l_T, (uint32_t)l_I,(uint32_t)l_M, (uint32_t)l_E, 0 };
const area crap = { crap_count, crap_inst, crap_label };

//const uint32_t area_count = 2;
//const area areas[area_count] = { crap, all };
const uint32_t area_count = 7;
const area areas[area_count] = { bootup, file_sel,pp_speech, outside,to_BoB1,BoB1, crap };


const area* volatile current_area = areas;
volatile uint32_t areas_remaining = area_count;
const instructions* volatile current_inst = nullptr;
volatile uint32_t current_data = 0;
volatile uint32_t inst_remaining = 0;
volatile uint8_t cycles_remaining = 0;
const uint32_t* volatile current_label;
volatile uint8_t out_of_inst = 0;

inline void load_inst(const instructions* const inst) {
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
inline void load_area(const area* const area) {
	inst_remaining = area->instruction_count;
	current_inst = area->inst;
	load_inst(current_inst);
	current_label = area->label;
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