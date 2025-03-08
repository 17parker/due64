#pragma once
#include "tas_data.h"

extern volatile uint32_t current_data;
volatile uint8_t update_buttons_flag = 0;

inline void init_tft();
inline void lcd_set_pages(uint16_t start, uint16_t end);
inline void lcd_set_columns(uint8_t start, uint8_t end);
inline void draw_frame_count_label();
inline void lli_start_frame_draw();
inline void lli_update_frame_numbers();
inline void lli_start_number_draw();
inline void update_lli_buttons();
inline void update_lli_l();


volatile const uint8_t* volatile const numbers[10] = { number_zero, number_one, number_two, number_three, number_four, number_five, number_six, number_seven, number_eight, number_nine };
volatile uint32_t tene0;
volatile uint32_t tene1;
volatile uint32_t tene2;
volatile uint32_t tene3;
volatile uint32_t tene4;
volatile uint32_t tene5;
volatile uint32_t tene6;
volatile uint32_t num_buff[7] = { 0,0,0,0,0,0,0 };

//Spells "FRAME:", 32px wide by 8px tall. 0 means black, 1 means white
const uint32_t frame_line1 = 0b11111011110000100010001011111000;
const uint32_t frame_line2 = 0b10000010001001010011011010000011;
const uint32_t frame_line3 = 0b10000010001010001010101010000011;
const uint32_t frame_line4 = 0b11110011110010001010001011110000;
const uint32_t frame_line5 = 0b10000011000011111010001010000000;
const uint32_t frame_line6 = 0b10000010100010001010001010000011;
const uint32_t frame_line7 = 0b10000010010010001010001010000011;
const uint32_t frame_line8 = 0b10000010001010001010001011111000;
const uint32_t f[8] = { frame_line1,frame_line2,frame_line3,frame_line4,frame_line5,frame_line6,frame_line7,frame_line8 };

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

//"linked list item"
struct lli {
	uint32_t saddr; //points to source of data
	uint32_t daddr; //CONSTANT - points to address of destination
	uint32_t ctrla; //CONSTANT - config
	uint32_t ctrlb; //CONSTANT - config
	uint32_t dscr;	//points to address of next lli
};
//saddr is the address where the command/data is held
//daddr is either 0x60x for commands or 0x61x for data
//for commands, ctrlA = size; ctrlB = (0b10 << 28);
//for data, ctrlA = size; ctrlB = (0b10 << 28);
//dscr is the address of the next lli

volatile const uint8_t cu_pages[4] = { 0,60,0,75 };
volatile const uint8_t cu_cols[4] = { 0,64,0,79 };
volatile const uint8_t r_cols[4] = { 0,104,0,119 };
volatile const uint8_t l_cols[4] = { 0,128,0,143 };
volatile const uint8_t cr_pages[4] = { 0,72,0,87 };
volatile const uint8_t cr_cols[4] = { 0,48,0,63 };
volatile const uint8_t cl_cols[4] = { 0,80,0,95 };
volatile const uint8_t du_cols[4] = { 0,160,0,175 };
volatile const uint8_t start_pages[4] = { 0,80,0,95 };
volatile const uint8_t start_cols[4] = { 0,116,0,131 };
volatile const uint8_t cd_pages[4] = { 0,84,0,99 };
volatile const uint8_t cd_cols[4] = { 0,64,0,79 };
volatile const uint8_t dr_cols[4] = { 0,144,0,159 };
volatile const uint8_t dl_cols[4] = { 0,176,0,191 };
volatile const uint8_t b_pages[4] = { 0,92,0,107 };
volatile const uint8_t b_cols[4] = { 0,92,0,107 };
volatile const uint8_t dd_pages[4] = { 0,96,0,111 };
volatile const uint8_t dd_cols[4] = { 0,160,0,175 };
volatile const uint8_t a_pages[4] = { 0,108,0,123 };
volatile const uint8_t a_cols[4] = { 0,80,0,95 };
volatile const uint8_t z_cols[4] = { 0,116,0,131 };
volatile const uint8_t digit_pages[4] = { 302 >> 8, 302 & 0b11111111,316 >> 8, 316 & 0b11111111 };
volatile const uint8_t digit_e0_cols[4] = { 0,146,0,153 };
volatile const uint8_t digit_e1_cols[4] = { 0,154,0,161 };
volatile const uint8_t digit_e2_cols[4] = { 0,162,0,169 };
volatile const uint8_t digit_e3_cols[4] = { 0,170,0,177 };
volatile const uint8_t digit_e4_cols[4] = { 0,178,0,185 };
volatile const uint8_t digit_e5_cols[4] = { 0,186,0,193 };
volatile const uint8_t digit_e6_cols[4] = { 0,194,0,201 };
volatile const uint8_t letter_pages[4] = { 0,4,0,15 };
volatile const uint8_t l26_cols[4] = { 0,4,0,11 };
volatile const uint8_t l25_cols[4] = { 0,13,0,20 };
volatile const uint8_t l24_cols[4] = { 0,22,0,29 };
volatile const uint8_t l23_cols[4] = { 0,31,0,38 };
volatile const uint8_t l22_cols[4] = { 0,40,0,47 };
volatile const uint8_t l21_cols[4] = { 0,49,0,56 };
volatile const uint8_t l20_cols[4] = { 0,58,0,65 };
volatile const uint8_t l19_cols[4] = { 0,67,0,74 };
volatile const uint8_t l18_cols[4] = { 0,76,0,83 };
volatile const uint8_t l17_cols[4] = { 0,85,0,92 };
volatile const uint8_t l16_cols[4] = { 0,94,0,101 };
volatile const uint8_t l15_cols[4] = { 0,103,0,110 };
volatile const uint8_t l14_cols[4] = { 0,112,0,119 };
volatile const uint8_t l13_cols[4] = { 0,121,0,128 };
volatile const uint8_t l12_cols[4] = { 0,130,0,137 };
volatile const uint8_t l11_cols[4] = { 0,139,0,146 };
volatile const uint8_t l10_cols[4] = { 0,148,0,155 };
volatile const uint8_t l9_cols[4] = { 0,157,0,164 };
volatile const uint8_t l8_cols[4] = { 0,166,0,173 };
volatile const uint8_t l7_cols[4] = { 0,175,0,182 };
volatile const uint8_t l6_cols[4] = { 0,184,0,191 };
volatile const uint8_t l5_cols[4] = { 0,193,0,200 };
volatile const uint8_t l4_cols[4] = { 0,202,0,209 };
volatile const uint8_t l3_cols[4] = { 0,211,0,218 };
volatile const uint8_t l2_cols[4] = { 0,220,0,227 };
volatile const uint8_t l1_cols[4] = { 0,229,0,236 };

volatile lli lli_l26 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,0 };
volatile const lli lli_l26_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l26 };
volatile const lli lli_set_l26_cols_data = { (uint32_t)l26_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l26_mem_write };
volatile const lli lli_l26_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l26_cols_data };
volatile lli lli_l25 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l26_start };
volatile const lli lli_l25_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l25 };
volatile const lli lli_set_l25_cols_data = { (uint32_t)l25_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l25_mem_write };
volatile const lli lli_l25_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l25_cols_data };
volatile lli lli_l24 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l25_start };
volatile const lli lli_l24_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l24 };
volatile const lli lli_set_l24_cols_data = { (uint32_t)l24_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l24_mem_write };
volatile const lli lli_l24_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l24_cols_data };
volatile lli lli_l23 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l24_start };
volatile const lli lli_l23_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l23 };
volatile const lli lli_set_l23_cols_data = { (uint32_t)l23_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l23_mem_write };
volatile const lli lli_l23_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l23_cols_data };
volatile lli lli_l22 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l23_start };
volatile const lli lli_l22_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l22 };
volatile const lli lli_set_l22_cols_data = { (uint32_t)l22_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l22_mem_write };
volatile const lli lli_l22_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l22_cols_data };
volatile lli lli_l21 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l22_start };
volatile const lli lli_l21_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l21 };
volatile const lli lli_set_l21_cols_data = { (uint32_t)l21_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l21_mem_write };
volatile const lli lli_l21_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l21_cols_data };
volatile lli lli_l20 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l21_start };
volatile const lli lli_l20_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l20 };
volatile const lli lli_set_l20_cols_data = { (uint32_t)l20_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l20_mem_write };
volatile const lli lli_l20_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l20_cols_data };
volatile lli lli_l19 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l20_start };
volatile const lli lli_l19_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l19 };
volatile const lli lli_set_l19_cols_data = { (uint32_t)l19_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l19_mem_write };
volatile const lli lli_l19_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l19_cols_data };
volatile lli lli_l18 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l19_start };
volatile const lli lli_l18_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l18 };
volatile const lli lli_set_l18_cols_data = { (uint32_t)l18_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l18_mem_write };
volatile const lli lli_l18_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l18_cols_data };
volatile lli lli_l17 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l18_start };
volatile const lli lli_l17_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l17 };
volatile const lli lli_set_l17_cols_data = { (uint32_t)l17_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l17_mem_write };
volatile const lli lli_l17_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l17_cols_data };
volatile lli lli_l16 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l17_start };
volatile const lli lli_l16_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l16 };
volatile const lli lli_set_l16_cols_data = { (uint32_t)l16_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l16_mem_write };
volatile const lli lli_l16_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l16_cols_data };
volatile lli lli_l15 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l16_start };
volatile const lli lli_l15_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l15 };
volatile const lli lli_set_l15_cols_data = { (uint32_t)l15_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l15_mem_write };
volatile const lli lli_l15_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l15_cols_data };
volatile lli lli_l14 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l15_start };
volatile const lli lli_l14_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l14 };
volatile const lli lli_set_l14_cols_data = { (uint32_t)l14_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l14_mem_write };
volatile const lli lli_l14_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l14_cols_data };
volatile lli lli_l13 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l14_start };
volatile const lli lli_l13_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l13 };
volatile const lli lli_set_l13_cols_data = { (uint32_t)l13_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l13_mem_write };
volatile const lli lli_l13_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l13_cols_data };
volatile lli lli_l12 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l13_start };
volatile const lli lli_l12_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l12 };
volatile const lli lli_set_l12_cols_data = { (uint32_t)l12_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l12_mem_write };
volatile const lli lli_l12_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l12_cols_data };
volatile lli lli_l11 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l12_start };
volatile const lli lli_l11_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l11 };
volatile const lli lli_set_l11_cols_data = { (uint32_t)l11_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l11_mem_write };
volatile const lli lli_l11_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l11_cols_data };
volatile lli lli_l10 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l11_start };
volatile const lli lli_l10_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l10 };
volatile const lli lli_set_l10_cols_data = { (uint32_t)l10_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l10_mem_write };
volatile const lli lli_l10_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l10_cols_data };
volatile lli lli_l9 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l10_start };
volatile const lli lli_l9_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l9 };
volatile const lli lli_set_l9_cols_data = { (uint32_t)l9_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l9_mem_write };
volatile const lli lli_l9_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l9_cols_data };
volatile lli lli_l8 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l9_start };
volatile const lli lli_l8_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l8 };
volatile const lli lli_set_l8_cols_data = { (uint32_t)l8_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l8_mem_write };
volatile const lli lli_l8_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l8_cols_data };
volatile lli lli_l7 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l8_start };
volatile const lli lli_l7_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l7 };
volatile const lli lli_set_l7_cols_data = { (uint32_t)l7_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l7_mem_write };
volatile const lli lli_l7_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l7_cols_data };
volatile lli lli_l6 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l7_start };
volatile const lli lli_l6_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l6 };
volatile const lli lli_set_l6_cols_data = { (uint32_t)l6_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l6_mem_write };
volatile const lli lli_l6_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l6_cols_data };
volatile lli lli_l5 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l6_start };
volatile const lli lli_l5_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l5 };
volatile const lli lli_set_l5_cols_data = { (uint32_t)l5_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l5_mem_write };
volatile const lli lli_l5_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l5_cols_data };
volatile lli lli_l4 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l5_start };
volatile const lli lli_l4_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l4 };
volatile const lli lli_set_l4_cols_data = { (uint32_t)l4_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l4_mem_write };
volatile const lli lli_l4_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l4_cols_data };
volatile lli lli_l3 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l4_start };
volatile const lli lli_l3_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l3 };
volatile const lli lli_set_l3_cols_data = { (uint32_t)l3_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l3_mem_write };
volatile const lli lli_l3_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l3_cols_data };
volatile lli lli_l2 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l3_start };
volatile const lli lli_l2_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l2 };
volatile const lli lli_set_l2_cols_data = { (uint32_t)l2_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l2_mem_write };
volatile const lli lli_l2_start = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l2_cols_data };
volatile lli lli_l1 = { (uint32_t)l_space,0x61000000,288,0b10 << 28,(uint32_t)&lli_l2_start };
volatile const lli lli_l1_mem_write = { (uint32_t)&mem_write,0x60000000,1,0b10 << 28,(uint32_t)&lli_l1 };
volatile const lli lli_set_l1_cols_data = { (uint32_t)l1_cols,0x61000000,4,0b10 << 28,(uint32_t)&lli_l1_mem_write };
volatile const lli lli_set_l1_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l1_cols_data };
volatile const lli lli_set_l_pages_data = { (uint32_t)letter_pages,0x61000000,4,0b10 << 28,(uint32_t)&lli_set_l1_cols_cmd };
volatile const lli lli_l1_start = { (uint32_t)&page_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_l_pages_data };
//Frame number
volatile lli lli_digit_e6 = { (uint32_t)number_zero,0x61000000,360,0b10 << 28,0 };
volatile const lli lli_digit_e6_mem_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_digit_e6 };
volatile const lli lli_set_digit_e6_cols_data = { (uint32_t)digit_e6_cols,0x61000000,4,0b10 << 28, (uint32_t)&lli_digit_e6_mem_write };
volatile const lli lli_set_digit_e6_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_e6_cols_data };
volatile lli lli_digit_e5 = { (uint32_t)number_zero,0x61000000,360,0b10 << 28,(uint32_t)&lli_set_digit_e6_cols_cmd };
volatile const lli lli_digit_e5_mem_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_digit_e5 };
volatile const lli lli_set_digit_e5_cols_data = { (uint32_t)digit_e5_cols,0x61000000,4,0b10 << 28, (uint32_t)&lli_digit_e5_mem_write };
volatile const lli lli_set_digit_e5_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_e5_cols_data };
volatile lli lli_digit_e4 = { (uint32_t)number_zero,0x61000000,360,0b10 << 28,(uint32_t)&lli_set_digit_e5_cols_cmd };
volatile const lli lli_digit_e4_mem_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_digit_e4 };
volatile const lli lli_set_digit_e4_cols_data = { (uint32_t)digit_e4_cols,0x61000000,4,0b10 << 28, (uint32_t)&lli_digit_e4_mem_write };
volatile const lli lli_set_digit_e4_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_e4_cols_data };
volatile lli lli_digit_e3 = { (uint32_t)number_zero,0x61000000,360,0b10 << 28,(uint32_t)&lli_set_digit_e4_cols_cmd };
volatile const lli lli_digit_e3_mem_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_digit_e3 };
volatile const lli lli_set_digit_e3_cols_data = { (uint32_t)digit_e3_cols,0x61000000,4,0b10 << 28, (uint32_t)&lli_digit_e3_mem_write };
volatile const lli lli_set_digit_e3_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_e3_cols_data };
volatile lli lli_digit_e2 = { (uint32_t)number_zero,0x61000000,360,0b10 << 28,(uint32_t)&lli_set_digit_e3_cols_cmd };
volatile const lli lli_digit_e2_mem_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_digit_e2 };
volatile const lli lli_set_digit_e2_cols_data = { (uint32_t)digit_e2_cols,0x61000000,4,0b10 << 28, (uint32_t)&lli_digit_e2_mem_write };
volatile const lli lli_set_digit_e2_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_e2_cols_data };
volatile lli lli_digit_e1 = { (uint32_t)number_zero,0x61000000,360,0b10 << 28,(uint32_t)&lli_set_digit_e2_cols_cmd };
volatile const lli lli_digit_e1_mem_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_digit_e1 };
volatile const lli lli_set_digit_e1_cols_data = { (uint32_t)digit_e1_cols,0x61000000,4,0b10 << 28, (uint32_t)&lli_digit_e1_mem_write };
volatile const lli lli_set_digit_e1_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_e1_cols_data };
volatile lli lli_digit_e0 = { (uint32_t)number_zero,0x61000000,360,0b10 << 28,(uint32_t)&lli_set_digit_e1_cols_cmd };
volatile const lli lli_digit_e0_mem_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_digit_e0 };
volatile const lli lli_set_digit_e0_cols_data = { (uint32_t)digit_e0_cols,0x61000000,4,0b10 << 28, (uint32_t)&lli_digit_e0_mem_write };
volatile const lli lli_set_digit_e0_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_e0_cols_data };
volatile const lli lli_set_digit_pages_data = { (uint32_t)digit_pages,0x61000000,4,0b10 << 28,(uint32_t)&lli_set_digit_e0_cols_cmd };
volatile const lli lli_set_digit_page_cmd = { (uint32_t)&page_addr_set, 0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_pages_data };
//Buttons
volatile lli lli_button_Z = { (uint32_t)button_Z, 0x61000000, 768, 0b10 << 28, (uint32_t)&lli_set_digit_page_cmd };
volatile const lli lli_z_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_button_Z };
volatile const lli lli_set_z_cols_data = { (uint32_t)z_cols, 0x61000000,4,0b10 << 28,(uint32_t)&lli_z_write };
volatile const lli lli_set_z_cols_cmd = { (uint32_t)&col_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_z_cols_data };
volatile lli lli_button_A = { (uint32_t)button_A, 0x61000000, 768, 0b10 << 28, (uint32_t)&lli_set_z_cols_cmd };
volatile const lli lli_a_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_button_A };
volatile const lli lli_set_a_cols_data = { (uint32_t)a_cols, 0x61000000,4,0b10 << 28,(uint32_t)&lli_a_write };
volatile const lli lli_set_a_cols_cmd = { (uint32_t)&col_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_a_cols_data };
volatile const lli lli_set_a_pages_data = { (uint32_t)a_pages, 0x61000000,4,0b10 << 28,(uint32_t)&lli_set_a_cols_cmd };
volatile const lli lli_set_a_pages_cmd = { (uint32_t)&page_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_a_pages_data };
volatile lli lli_button_DD = { (uint32_t)button_DD, 0x61000000, 768, 0b10 << 28, (uint32_t)&lli_set_a_pages_cmd };
volatile const lli lli_dd_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_button_DD };
volatile const lli lli_set_dd_cols_data = { (uint32_t)dd_cols, 0x61000000,4,0b10 << 28,(uint32_t)&lli_dd_write };
volatile const lli lli_set_dd_cols_cmd = { (uint32_t)&col_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_dd_cols_data };
volatile const lli lli_set_dd_pages_data = { (uint32_t)dd_pages, 0x61000000,4,0b10 << 28,(uint32_t)&lli_set_dd_cols_cmd };
volatile const lli lli_set_dd_pages_cmd = { (uint32_t)&page_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_dd_pages_data };
volatile lli lli_button_B = { (uint32_t)button_B, 0x61000000, 768, 0b10 << 28, (uint32_t)&lli_set_dd_pages_cmd };
volatile const lli lli_b_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_button_B };
volatile const lli lli_set_b_cols_data = { (uint32_t)b_cols, 0x61000000,4,0b10 << 28,(uint32_t)&lli_b_write };
volatile const lli lli_set_b_cols_cmd = { (uint32_t)&col_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_b_cols_data };
volatile const lli lli_set_b_pages_data = { (uint32_t)b_pages, 0x61000000,4,0b10 << 28,(uint32_t)&lli_set_b_cols_cmd };
volatile const lli lli_set_b_pages_cmd = { (uint32_t)&page_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_b_pages_data };
volatile lli lli_button_START = { (uint32_t)button_START, 0x61000000, 768, 0b10 << 28, (uint32_t)&lli_set_b_pages_cmd };
volatile const lli lli_start_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_button_START };
volatile const lli lli_set_start_cols_data = { (uint32_t)start_cols, 0x61000000,4,0b10 << 28,(uint32_t)&lli_start_write };
volatile const lli lli_set_start_cols_cmd = { (uint32_t)&col_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_start_cols_data };
volatile const lli lli_set_start_pages_data = { (uint32_t)start_pages, 0x61000000,4,0b10 << 28,(uint32_t)&lli_set_start_cols_cmd };
volatile const lli lli_set_start_pages_cmd = { (uint32_t)&page_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_start_pages_data };
volatile lli lli_button_DL = { (uint32_t)button_DL, 0x61000000, 768, 0b10 << 28, (uint32_t)&lli_set_start_pages_cmd };
volatile const lli lli_dl_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_button_DL };
volatile const lli lli_set_dl_cols_data = { (uint32_t)dl_cols, 0x61000000,4,0b10 << 28,(uint32_t)&lli_dl_write };
volatile const lli lli_set_dl_cols_cmd = { (uint32_t)&col_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_dl_cols_data };
volatile lli lli_button_DR = { (uint32_t)button_DR, 0x61000000, 768, 0b10 << 28, (uint32_t)&lli_set_dl_cols_cmd };
volatile const lli lli_dr_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_button_DR };
volatile const lli lli_set_dr_cols_data = { (uint32_t)dr_cols, 0x61000000,4,0b10 << 28,(uint32_t)&lli_dr_write };
volatile const lli lli_set_dr_cols_cmd = { (uint32_t)&col_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_dr_cols_data };
volatile lli lli_button_CD = { (uint32_t)button_CD, 0x61000000, 768, 0b10 << 28, (uint32_t)&lli_set_dr_cols_cmd };
volatile const lli lli_cd_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_button_CD };
volatile const lli lli_set_cd_cols_data = { (uint32_t)cd_cols, 0x61000000,4,0b10 << 28,(uint32_t)&lli_cd_write };
volatile const lli lli_set_cd_cols_cmd = { (uint32_t)&col_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_cd_cols_data };
volatile const lli lli_set_cd_pages_data = { (uint32_t)cd_pages, 0x61000000,4,0b10 << 28,(uint32_t)&lli_set_cd_cols_cmd };
volatile const lli lli_set_cd_pages_cmd = { (uint32_t)&page_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_cd_pages_data };
volatile lli lli_button_DU = { (uint32_t)button_DU, 0x61000000, 768, 0b10 << 28, (uint32_t)&lli_set_cd_pages_cmd };
volatile const lli lli_du_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_button_DU };
volatile const lli lli_set_du_cols_data = { (uint32_t)du_cols, 0x61000000,4,0b10 << 28,(uint32_t)&lli_du_write };
volatile const lli lli_set_du_cols_cmd = { (uint32_t)&col_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_du_cols_data };
volatile lli lli_button_CL = { (uint32_t)button_CL, 0x61000000, 768, 0b10 << 28, (uint32_t)&lli_set_du_cols_cmd };
volatile const lli lli_cl_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_button_CL };
volatile const lli lli_set_cl_cols_data = { (uint32_t)cl_cols, 0x61000000,4,0b10 << 28,(uint32_t)&lli_cl_write };
volatile const lli lli_set_cl_cols_cmd = { (uint32_t)&col_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_cl_cols_data };
volatile lli lli_button_CR = { (uint32_t)button_CR, 0x61000000, 768, 0b10 << 28, (uint32_t)&lli_set_cl_cols_cmd };
volatile const lli lli_cr_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_button_CR };
volatile const lli lli_set_cr_cols_data = { (uint32_t)cr_cols, 0x61000000,4,0b10 << 28,(uint32_t)&lli_cr_write };
volatile const lli lli_set_cr_cols_cmd = { (uint32_t)&col_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_cr_cols_data };
volatile const lli lli_set_cr_pages_data = { (uint32_t)cr_pages, 0x61000000,4,0b10 << 28,(uint32_t)&lli_set_cr_cols_cmd };
volatile const lli lli_set_cr_pages_cmd = { (uint32_t)&page_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_cr_pages_data };
volatile lli lli_button_L = { (uint32_t)button_L, 0x61000000, 768, 0b10 << 28, (uint32_t)&lli_set_cr_pages_cmd };
volatile const lli lli_l_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_button_L };
volatile const lli lli_set_l_cols_data = { (uint32_t)l_cols, 0x61000000,4,0b10 << 28,(uint32_t)&lli_l_write };
volatile const lli lli_set_l_cols_cmd = { (uint32_t)&col_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_l_cols_data };
volatile lli lli_button_R = { (uint32_t)button_R, 0x61000000, 768, 0b10 << 28, (uint32_t)&lli_set_l_cols_cmd };
volatile const lli lli_r_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_button_R };
volatile const lli lli_set_r_cols_data = { (uint32_t)r_cols, 0x61000000,4,0b10 << 28,(uint32_t)&lli_r_write };
volatile const lli lli_set_r_cols_cmd = { (uint32_t)&col_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_r_cols_data };
volatile lli lli_button_CU = { (uint32_t)button_CU, 0x61000000, 768, 0b10 << 28, (uint32_t)&lli_set_r_cols_cmd };
volatile const lli lli_cu_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_button_CU };
volatile const lli lli_set_cu_cols_data = { (uint32_t)cu_cols, 0x61000000,4,0b10 << 28,(uint32_t)&lli_cu_write };
volatile const lli lli_set_cu_cols_cmd = { (uint32_t)&col_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_cu_cols_data };
volatile const lli lli_set_cu_pages_data = { (uint32_t)cu_pages, 0x61000000,4,0b10 << 28,(uint32_t)&lli_set_cu_cols_cmd };
volatile const lli lli_set_cu_pages_cmd = { (uint32_t)&page_addr_set, 0x60000000,1,0b10 << 28, (uint32_t)&lli_set_cu_pages_data };

inline void lli_start_frame_draw() {
	REG_DMAC_CTRLB0 = (0b10 << 28);
	REG_DMAC_DSCR0 = (uint32_t)&lli_set_cu_pages_cmd;
	REG_DMAC_CHER |= 1;
}

inline void lli_update_frame_numbers() {
	lli_digit_e0.saddr = (uint32_t)(numbers[tene0]);
	lli_digit_e1.saddr = (uint32_t)(numbers[tene1]);
	lli_digit_e2.saddr = (uint32_t)(numbers[tene2]);
	lli_digit_e3.saddr = (uint32_t)(numbers[tene3]);
	lli_digit_e4.saddr = (uint32_t)(numbers[tene4]);
	lli_digit_e5.saddr = (uint32_t)(numbers[tene5]);
	lli_digit_e6.saddr = (uint32_t)(numbers[tene6]);
}

inline void lli_start_number_draw() {
	REG_DMAC_CTRLB0 = (0b10 << 28);
	REG_DMAC_DSCR0 = (uint32_t)&lli_set_digit_page_cmd;
	REG_DMAC_CHER |= 1;
}

inline void draw_frame_count_label() {
	lcd_set_columns(206, 237);
	lcd_set_pages(310, 317);
	*command = mem_write;
	const uint32_t* volatile line_ptr = f;
	for (uint8_t i = 0; i < 8; ++i) {
		for (uint8_t j = 0; j < 32; ++j) {
			if (((*line_ptr) >> j) & 1) {
				*data = 0xff;
				*data = 0xff;
				*data = 0xff;
			}
			else {
				*data = 0x00;
				*data = 0x00;
				*data = 0x00;
			}
		}
		++line_ptr;
	}
}

inline void update_lli_buttons() {
	if (current_data & 1)   //A
		lli_button_A.saddr = (uint32_t)button_A;
	else
		lli_button_A.saddr = (uint32_t)ublack;
	if ((current_data >> 1) & 1)   //B
		lli_button_B.saddr = (uint32_t)button_B;
	else
		lli_button_B.saddr = (uint32_t)ublack;
	if ((current_data >> 2) & 1)   //Z
		lli_button_Z.saddr = (uint32_t)button_Z;
	else
		lli_button_Z.saddr = (uint32_t)ublack;
	if ((current_data >> 3) & 1)   //START
		lli_button_START.saddr = (uint32_t)button_START;
	else
		lli_button_START.saddr = (uint32_t)ublack;
	if ((current_data >> 4) & 1)   //DU
		lli_button_DU.saddr = (uint32_t)button_DU;
	else
		lli_button_DU.saddr = (uint32_t)ublack;
	if ((current_data >> 5) & 1)   //DD
		lli_button_DD.saddr = (uint32_t)button_DD;
	else
		lli_button_DD.saddr = (uint32_t)ublack;
	if ((current_data >> 6) & 1)   //DL
		lli_button_DL.saddr = (uint32_t)button_DL;
	else
		lli_button_DL.saddr = (uint32_t)ublack;
	if ((current_data >> 7) & 1)   //DR
		lli_button_DR.saddr = (uint32_t)button_DR;
	else
		lli_button_DR.saddr = (uint32_t)ublack;
	if ((current_data >> 10) & 1)   //L trigger
		lli_button_L.saddr = (uint32_t)button_L;
	else
		lli_button_L.saddr = (uint32_t)ublack;
	if ((current_data >> 11) & 1)   //R trigger
		lli_button_R.saddr = (uint32_t)button_R;
	else
		lli_button_R.saddr = (uint32_t)ublack;
	if ((current_data >> 12) & 1)   //CU
		lli_button_CU.saddr = (uint32_t)button_CU;
	else
		lli_button_CU.saddr = (uint32_t)ublack;
	if ((current_data >> 13) & 1)   //CD
		lli_button_CD.saddr = (uint32_t)button_CD;
	else
		lli_button_CD.saddr = (uint32_t)ublack;
	if ((current_data >> 14) & 1)   //CL
		lli_button_CL.saddr = (uint32_t)button_CL;
	else
		lli_button_CL.saddr = (uint32_t)ublack;
	if ((current_data >> 15) & 1)   //CR
		lli_button_CR.saddr = (uint32_t)button_CR;
	else
		lli_button_CR.saddr = (uint32_t)ublack;
}

inline void update_lli_l() {
	lli_l1.saddr = *current_label++;
	if (*current_label)
		lli_l2.saddr = *current_label++;
	else
		lli_l2.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l3.saddr = *current_label++;
	else
		lli_l3.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l4.saddr = *current_label++;
	else
		lli_l4.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l5.saddr = *current_label++;
	else
		lli_l5.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l6.saddr = *current_label++;
	else
		lli_l6.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l7.saddr = *current_label++;
	else
		lli_l7.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l8.saddr = *current_label++;
	else
		lli_l8.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l9.saddr = *current_label++;
	else
		lli_l9.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l10.saddr = *current_label++;
	else
		lli_l10.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l11.saddr = *current_label++;
	else
		lli_l11.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l12.saddr = *current_label++;
	else
		lli_l12.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l13.saddr = *current_label++;
	else
		lli_l13.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l14.saddr = *current_label++;
	else
		lli_l14.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l15.saddr = *current_label++;
	else
		lli_l15.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l16.saddr = *current_label++;
	else
		lli_l16.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l17.saddr = *current_label++;
	else
		lli_l17.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l18.saddr = *current_label++;
	else
		lli_l18.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l19.saddr = *current_label++;
	else
		lli_l19.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l20.saddr = *current_label++;
	else
		lli_l20.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l21.saddr = *current_label++;
	else
		lli_l21.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l22.saddr = *current_label++;
	else
		lli_l22.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l23.saddr = *current_label++;
	else
		lli_l23.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l24.saddr = *current_label++;
	else
		lli_l24.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l25.saddr = *current_label++;
	else
		lli_l25.saddr = (uint32_t)l_space;
	if (*current_label)
		lli_l26.saddr = *current_label++;
	else
		lli_l26.saddr = (uint32_t)l_space;
}


