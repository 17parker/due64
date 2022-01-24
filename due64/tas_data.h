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

const uint32_t stand = 0;
const uint32_t n = 0b10001010 << Y;
const uint32_t ne = (0b10001010 << X) | (0b10001010 << Y);
const uint32_t nw = (0b11110101 << X) | (0b10001010 << Y);
const uint32_t e = 0b11110101 << X;
const uint32_t s = 0b11110101 << Y;
const uint32_t sw = (0b11110101 << X) | (0b11110101 << Y);
const uint32_t se = (0b10001010 << X) | (0b11110101 << Y);
const uint32_t w = (0b10001010 << X);

struct instructions {
	uint32_t data;
	uint8_t cycles;
};
struct area {
	const uint32_t instruction_count;
	const instructions* inst;
	const char* area_label;	//7 chars long (8 including '\n')
};

const uint32_t boot_count = 5;
const instructions boot_area[boot_count] = { { stand,120}, { 1 << START, 5 }, { stand, 30}, {1 << A,5}, {stand,60} };
const char* boot_label = "boot-up";
const area bootup = { boot_count, boot_area, boot_label };

const uint32_t outside_count = 9;
const instructions outside_area[outside_count] = { {n, 90},{ne,20},{nw,15},{ne,20},{nw,14} ,{ne,15},{nw,27}, {n,100}, {stand, 100} };
const char* outside_label = "outside";
const area outside = { outside_count, outside_area, outside_label };

const uint32_t inside_count = 12;
const instructions inside_area[inside_count] = { {se,25},{ne,15},{n,60},{ne,30},{n,75},{(1 << A) | n,3},{nw, 10},{n,25},{(1 << A) | n,3}, {stand, 80}, {1 << A, 5},{stand,70} };
const char* inside_label = " inside";
const area inside = { inside_count, inside_area, inside_label };

const uint32_t d_up_count = 2;
const instructions d_up_area[d_up_count] = { {1 << DU,3},{stand,20} };
const char* d_up_label = "   d_up";
const area d_up = { d_up_count, d_up_area,d_up_label };

const uint32_t jump_count = 3;
const instructions jump_kick[jump_count] = { { 1 << A, 5}, {1 << Z, 45}, {stand, 45} };
const char* jump_label = "jumping";
const area jumps = { jump_count, jump_kick, jump_label };

const uint32_t area_count = 4;
const area areas[area_count] = { bootup,outside, inside, jumps };

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

void init_buffer() {
	buffer[0] = off;
	buffer[1] = off;
	buffer[34] = stop;
	buffer[35] = off;
}