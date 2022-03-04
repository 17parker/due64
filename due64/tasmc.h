#pragma once
#include "sprites.h"

#define LCD_WR PIN_45C	//active HIGH
#define LCD_RS PIN_47C	//active HIGH
//Commands for the TFT
volatile const uint32_t col_addr_set = 0x2A;
volatile const uint32_t page_addr_set = 0x2B;
volatile const uint32_t mem_write = 0x2C;

volatile uint8_t frame_buffer[320][240] = { 0 };
extern volatile uint32_t current_data;
volatile uint8_t update_buttons_flag = 0;

//Set the RS (D/CX) pin LOW to send command
//Set the RS (D/CX) pin HIGH to send data
inline void lcd_set_columns(uint8_t start, uint8_t end);
inline void lcd_set_pages(uint16_t start, uint16_t end);
inline void lcd_clear();
inline void init_tft();
inline void draw_frame_count_label();
inline void draw_frame_num();

const uint32_t num_blank = 0;
const uint32_t num_0 = 0b00000110100110011001100110010110;
const uint32_t num_1 = 0b00000100010001000100010001000100;
const uint32_t num_2 = 0b00001111010000100001000100011110;
const uint32_t num_3 = 0b00001110000100010010000100011110;
const uint32_t num_4 = 0b00000001000100011111100110011001;
const uint32_t num_5 = 0b00001110000100010001111010001111;
const uint32_t num_6 = 0b00000110100110011111100010000111;
const uint32_t num_7 = 0b00001000010000100010000100011111;
const uint32_t num_8 = 0b00000110100110010110100110010110;
const uint32_t num_9 = 0b00000001000100010111100110010111;
const uint32_t num[10] = { num_0, num_1,num_2,num_3,num_4,num_5,num_6,num_7,num_8,num_9 };
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

struct lli {
	uint32_t saddr;
	uint32_t daddr;
	uint32_t ctrla;
	uint32_t ctrlb;
	uint32_t dscr;
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
volatile const uint8_t digit_pages[4] = { 302 >> 8, 302 & 0b11111111,317 >> 8, 317 & 0b11111111 };
volatile const uint8_t digit_e0_cols[4] = { 0,146,0,153 };
volatile const uint8_t digit_e1_cols[4] = { 0,154,0,161 };
volatile const uint8_t digit_e2_cols[4] = { 0,162,0,169 };
volatile const uint8_t digit_e3_cols[4] = { 0,170,0,177 };
volatile const uint8_t digit_e4_cols[4] = { 0,178,0,185 };
volatile const uint8_t digit_e5_cols[4] = { 0,186,0,193 };
volatile const uint8_t digit_e6_cols[4] = { 0,194,0,201 };


volatile lli lli_digit_e6 = { (uint32_t)number_zero,0x61000000,384,0b10 << 28,0 };
volatile const lli lli_digit_e6_blank = { (uint32_t)number_blank,0x61000000,384,0b10 << 28,(uint32_t)&lli_digit_e6 };
volatile const lli lli_digit_e6_mem_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_digit_e6_blank };
volatile const lli lli_set_digit_e6_cols_data = { (uint32_t)digit_e6_cols,0x61000000,4,0b10 << 28, (uint32_t)&lli_digit_e6_mem_write };
volatile const lli lli_set_digit_e6_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_e6_cols_data };
volatile lli lli_digit_e5 = { (uint32_t)number_zero,0x61000000,384,0b10 << 28,(uint32_t)&lli_set_digit_e6_cols_cmd };
volatile const lli lli_digit_e5_blank = { (uint32_t)number_blank,0x61000000,384,0b10 << 28,(uint32_t)&lli_digit_e5 };
volatile const lli lli_digit_e5_mem_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_digit_e5_blank };
volatile const lli lli_set_digit_e5_cols_data = { (uint32_t)digit_e5_cols,0x61000000,4,0b10 << 28, (uint32_t)&lli_digit_e5_mem_write };
volatile const lli lli_set_digit_e5_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_e5_cols_data };
volatile lli lli_digit_e4 = { (uint32_t)number_zero,0x61000000,384,0b10 << 28,(uint32_t)&lli_set_digit_e5_cols_cmd };
volatile const lli lli_digit_e4_blank = { (uint32_t)number_blank,0x61000000,384,0b10 << 28,(uint32_t)&lli_digit_e4 };
volatile const lli lli_digit_e4_mem_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_digit_e4_blank };
volatile const lli lli_set_digit_e4_cols_data = { (uint32_t)digit_e4_cols,0x61000000,4,0b10 << 28, (uint32_t)&lli_digit_e4_mem_write };
volatile const lli lli_set_digit_e4_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_e4_cols_data };
volatile lli lli_digit_e3 = { (uint32_t)number_zero,0x61000000,384,0b10 << 28,(uint32_t)&lli_set_digit_e4_cols_cmd };
volatile const lli lli_digit_e3_blank = { (uint32_t)number_blank,0x61000000,384,0b10 << 28,(uint32_t)&lli_digit_e3 };
volatile const lli lli_digit_e3_mem_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_digit_e3_blank };
volatile const lli lli_set_digit_e3_cols_data = { (uint32_t)digit_e3_cols,0x61000000,4,0b10 << 28, (uint32_t)&lli_digit_e3_mem_write };
volatile const lli lli_set_digit_e3_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_e3_cols_data };
volatile lli lli_digit_e2 = { (uint32_t)number_zero,0x61000000,384,0b10 << 28,(uint32_t)&lli_set_digit_e3_cols_cmd };
volatile const lli lli_digit_e2_blank = { (uint32_t)number_blank,0x61000000,384,0b10 << 28,(uint32_t)&lli_digit_e2 };
volatile const lli lli_digit_e2_mem_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_digit_e2_blank };
volatile const lli lli_set_digit_e2_cols_data = { (uint32_t)digit_e2_cols,0x61000000,4,0b10 << 28, (uint32_t)&lli_digit_e2_mem_write };
volatile const lli lli_set_digit_e2_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_e2_cols_data };
volatile lli lli_digit_e1 = { (uint32_t)number_zero,0x61000000,384,0b10 << 28,(uint32_t)&lli_set_digit_e2_cols_cmd };
volatile const lli lli_digit_e1_blank = { (uint32_t)number_blank,0x61000000,384,0b10 << 28,(uint32_t)&lli_digit_e1 };
volatile const lli lli_digit_e1_mem_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_digit_e1_blank };
volatile const lli lli_set_digit_e1_cols_data = { (uint32_t)digit_e1_cols,0x61000000,4,0b10 << 28, (uint32_t)&lli_digit_e1_mem_write };
volatile const lli lli_set_digit_e1_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_e1_cols_data };
volatile lli lli_digit_e0 = { (uint32_t)number_zero,0x61000000,384,0b10 << 28,(uint32_t)&lli_set_digit_e1_cols_cmd };
volatile const lli lli_digit_e0_blank = { (uint32_t)number_blank,0x61000000,384,0b10 << 28,(uint32_t)&lli_digit_e0 };
volatile const lli lli_digit_e0_mem_write = { (uint32_t)&mem_write, 0x60000000, 1, 0b10 << 28, (uint32_t)&lli_digit_e0_blank };
volatile const lli lli_set_digit_e0_cols_data = { (uint32_t)digit_e0_cols,0x61000000,4,0b10 << 28, (uint32_t)&lli_digit_e0_mem_write };
volatile const lli lli_set_digit_e0_cols_cmd = { (uint32_t)&col_addr_set,0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_e0_cols_data };
volatile const lli lli_set_digit_pages_data = { (uint32_t)digit_pages,0x61000000,4,0b10 << 28,(uint32_t)&lli_set_digit_e0_cols_cmd };
volatile const lli lli_set_digit_page_cmd = { (uint32_t)&page_addr_set, 0x60000000,1,0b10 << 28,(uint32_t)&lli_set_digit_pages_data };

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
	lli_digit_e0.saddr = (uint32_t)numbers[tene0];
	lli_digit_e1.saddr = (uint32_t)numbers[tene1];
	lli_digit_e2.saddr = (uint32_t)numbers[tene2];
	lli_digit_e3.saddr = (uint32_t)numbers[tene3];
	lli_digit_e4.saddr = (uint32_t)numbers[tene4];
	lli_digit_e5.saddr = (uint32_t)numbers[tene5];
	lli_digit_e6.saddr = (uint32_t)numbers[tene6];
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

inline void update_buttons_lli() {
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

inline void draw_frame_num() {
	lcd_set_pages(310, 317);
	lcd_set_columns(168, 171);
	*command = mem_write;
	for (uint8_t i = 0; i <= 31; ++i) {
		*data = 0x00;
		*data = 0x00;
		*data = 0x00;
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[0] >> i) & 1) {
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
	lcd_set_columns(173, 176);
	*command = mem_write;
	for (uint8_t i = 0; i <= 31; ++i) {
		*data = 0x00;
		*data = 0x00;
		*data = 0x00;
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[1] >> i) & 1) {
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
	lcd_set_columns(178, 181);
	*command = mem_write;
	for (uint8_t i = 0; i <= 31; ++i) {
		*data = 0x00;
		*data = 0x00;
		*data = 0x00;
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[2] >> i) & 1) {
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
	lcd_set_columns(183, 186);
	*command = mem_write;
	for (uint8_t i = 0; i <= 31; ++i) {
		*data = 0x00;
		*data = 0x00;
		*data = 0x00;
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[3] >> i) & 1) {
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
	lcd_set_columns(188, 191);
	*command = mem_write;
	for (uint8_t i = 0; i <= 31; ++i) {
		*data = 0x00;
		*data = 0x00;
		*data = 0x00;
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[4] >> i) & 1) {
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
	lcd_set_columns(193, 196);
	*command = mem_write;
	for (uint8_t i = 0; i <= 31; ++i) {
		*data = 0x00;
		*data = 0x00;
		*data = 0x00;
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[5] >> i) & 1) {
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
	lcd_set_columns(198, 201);
	*command = mem_write;
	for (uint8_t i = 0; i <= 31; ++i) {
		*data = 0x00;
		*data = 0x00;
		*data = 0x00;
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[6] >> i) & 1) {
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
}

inline void lcd_set_columns(uint8_t start, uint8_t end) {
	//This sends 1 byte to specify command and 4 bytes of data (2 for start, 2 for end)
	*command = col_addr_set;
	*data = 0x00;
	*data = start;
	*data = 0x00;
	*data = end;
}

//The values for pages goes from 0-319, and anything over 255 is over 1 byte, so this needs 2 bytes
inline void lcd_set_pages(uint16_t start, uint16_t end) {
	*command = page_addr_set;
	*data = (start >> 8);
	*data = start;
	*data = (end >> 8);
	*data = end;
}

inline void lcd_clear() {
	*command = mem_write;
	for (uint32_t i = 0; i < 76800; ++i) {
		*data = 0x00;
		*data = 0x00;
		*data = 0x00;
	}
}

inline void init_tft() {
	pio_disable_pullup(PIOC, PIN_34C | PIN_35C | PIN_36C | PIN_37C | PIN_38C | PIN_39C | PIN_40C | PIN_41C | PIN_45C | PIN_47C);
	pio_enable_output(PIOC, PIN_34C | PIN_35C | PIN_36C | PIN_37C | PIN_38C | PIN_39C | PIN_40C | PIN_41C | PIN_45C | PIN_47C);
	pio_disable_pullup(PIOA, PIN_A4A);
	pio_enable_output(PIOA, PIN_A4A);
	smc_tft_lcd_setup();
	*command = 0x01;	//software reset
	delayMicroseconds(5000);
	*command = 0x29;	//display on
	delayMicroseconds(5000);
	*command = 0x11;	//sleep out
	delayMicroseconds(5000);
	lcd_set_columns(0, 239);
	lcd_set_pages(0, 319);
	lcd_clear();
}