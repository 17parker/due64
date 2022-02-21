#pragma once
#include "sprites.h"

#define LCD_WR PIN_45C	//active HIGH
#define LCD_RS PIN_47C	//active HIGH
//Commands for the TFT
const uint32_t col_addr_set = 0x2A;
const uint32_t page_addr_set = 0x2B;
const uint32_t mem_write = 0x2C;

volatile uint8_t frame_buffer[320][240] = { 0 };
volatile uint8_t* const bus = (uint8_t*)0x60000000;
extern volatile uint32_t current_data;
volatile uint8_t update_buttons_flag = 0;

//Set the RS (D/CX) pin LOW to send command
inline void lcd_set_command() { pio_output_write_LOW(PIOC, LCD_RS); }
//Set the RS (D/CX) pin HIGH to send data
inline void lcd_set_data() { pio_output_write_HIGH(PIOC, LCD_RS); }
inline void lcd_mem_write() {
	lcd_set_command();
	*bus = mem_write;
	lcd_set_data();
}
inline void lcd_set_columns(uint8_t start, uint8_t end);
inline void lcd_set_pages(uint16_t start, uint16_t end);
inline void lcd_clear();
inline void init_tft();
inline void init_frame_buffer();
inline void draw_frame_buffer();
inline void update_frame_count_buffer();
inline void draw_frame_count_label();
inline void draw_frame_num();
inline void draw_buttons();

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
extern volatile uint8_t tene0;
extern volatile uint8_t tene1;
extern volatile uint8_t tene2;
extern volatile uint8_t tene3;
extern volatile uint8_t tene4;
extern volatile uint8_t tene5;
extern volatile uint8_t tene6;
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

inline void update_frame_count_buffer() {
	num_buff[0] = num[tene0];
	num_buff[1] = num[tene1];
	num_buff[2] = num[tene2];
	num_buff[3] = num[tene3];
	num_buff[4] = num[tene4];
	num_buff[5] = num[tene5];
	num_buff[6] = num[tene6];
}

inline void draw_frame_count_label() {
	lcd_set_columns(206, 237);
	lcd_set_pages(310, 317);
	lcd_mem_write();
	const uint32_t* volatile line_ptr = f;
	for (uint8_t i = 0; i < 8; ++i) {
		for (uint8_t j = 0; j < 32; ++j) {
			if (((*line_ptr) >> j) & 1) {
				*bus = 0xff;
				*bus = 0xff;
				*bus = 0xff;
			}
			else {
				*bus = 0x00;
				*bus = 0x00;
				*bus = 0x00;
			}
		}
		++line_ptr;
	}
}

inline void draw_buttons() {
	lcd_set_pages(0, 320);
	lcd_set_columns(1, 16);
	lcd_mem_write();
	const uint8_t* volatile ptr;
	if (current_data & 1)
		ptr = button_A;
	else
		ptr = ublack;
	for (uint16_t i = 0; i < 768; ++i)
		*bus = *ptr++;
	if ((current_data >> 1) & 1)
		ptr = button_A;
	else
		ptr = ublack;
	for (uint16_t i = 0; i < 768; ++i)
		*bus = *ptr++;
}

inline void draw_frame_num() {
	lcd_set_pages(310, 317);
	lcd_set_columns(168, 171);
	lcd_mem_write();
	for (uint8_t i = 0; i <= 31; ++i) {
		*bus = 0x00;
		*bus = 0x00;
		*bus = 0x00;
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[0] >> i) & 1) {
			*bus = 0xff;
			*bus = 0xff;
			*bus = 0xff;
		}
		else {
			*bus = 0x00;
			*bus = 0x00;
			*bus = 0x00;
		}
	}
	lcd_set_columns(173, 176);
	lcd_mem_write();
	for (uint8_t i = 0; i <= 31; ++i) {
		*bus = 0x00;
		*bus = 0x00;
		*bus = 0x00;
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[1] >> i) & 1) {
			*bus = 0xff;
			*bus = 0xff;
			*bus = 0xff;
		}
		else {
			*bus = 0x00;
			*bus = 0x00;
			*bus = 0x00;
		}
	}
	lcd_set_columns(178, 181);
	lcd_mem_write();
	for (uint8_t i = 0; i <= 31; ++i) {
		*bus = 0x00;
		*bus = 0x00;
		*bus = 0x00;
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[2] >> i) & 1) {
			*bus = 0xff;
			*bus = 0xff;
			*bus = 0xff;
		}
		else {
			*bus = 0x00;
			*bus = 0x00;
			*bus = 0x00;
		}
	}
	lcd_set_columns(183, 186);
	lcd_mem_write();
	for (uint8_t i = 0; i <= 31; ++i) {
		*bus = 0x00;
		*bus = 0x00;
		*bus = 0x00;
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[3] >> i) & 1) {
			*bus = 0xff;
			*bus = 0xff;
			*bus = 0xff;
		}
		else {
			*bus = 0x00;
			*bus = 0x00;
			*bus = 0x00;
		}
	}
	lcd_set_columns(188, 191);
	lcd_mem_write();
	for (uint8_t i = 0; i <= 31; ++i) {
		*bus = 0x00;
		*bus = 0x00;
		*bus = 0x00;
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[4] >> i) & 1) {
			*bus = 0xff;
			*bus = 0xff;
			*bus = 0xff;
		}
		else {
			*bus = 0x00;
			*bus = 0x00;
			*bus = 0x00;
		}
	}
	lcd_set_columns(193, 196);
	lcd_mem_write();
	for (uint8_t i = 0; i <= 31; ++i) {
		*bus = 0x00;
		*bus = 0x00;
		*bus = 0x00;
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[5] >> i) & 1) {
			*bus = 0xff;
			*bus = 0xff;
			*bus = 0xff;
		}
		else {
			*bus = 0x00;
			*bus = 0x00;
			*bus = 0x00;
		}
	}
	lcd_set_columns(198, 201);
	lcd_mem_write();
	for (uint8_t i = 0; i <= 31; ++i) {
		*bus = 0x00;
		*bus = 0x00;
		*bus = 0x00;
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[6] >> i) & 1) {
			*bus = 0xff;
			*bus = 0xff;
			*bus = 0xff;
		}
		else {
			*bus = 0x00;
			*bus = 0x00;
			*bus = 0x00;
		}
	}
}

inline void init_frame_buffer() {
	for (uint16_t i = 0; i < 320; ++i)
		for (uint8_t j = 0; j < 240; ++j)
			frame_buffer[i][j] = 115;
}

inline void draw_frame_buffer() {
	for (uint16_t i = 0; i < 320; ++i)
		for (uint8_t j = 0; j < 240; ++j) {
			*bus = color_palette[frame_buffer[i][j]][0];
			*bus = color_palette[frame_buffer[i][j]][1];
			*bus = color_palette[frame_buffer[i][j]][2];
		}
}

inline void lcd_set_columns(uint8_t start, uint8_t end) {
	//This sends 1 byte to specify command and 4 bytes of data (2 for start, 2 for end)
	lcd_set_command();
	*bus = col_addr_set;
	lcd_set_data();
	*bus = 0x00;
	*bus = start;
	*bus = 0x00;
	*bus = end;
}

//The values for pages goes from 0-319, and anything over 255 is over 1 byte, so this needs 2 bytes
inline void lcd_set_pages(uint16_t start, uint16_t end) {
	lcd_set_command();
	*bus = page_addr_set;
	lcd_set_data();
	*bus = (start >> 8);
	*bus = start;
	*bus = (end >> 8);
	*bus = end;
}

inline void lcd_clear() {
	lcd_mem_write();
	for (uint32_t i = 0; i < 76800; ++i) {
		*bus = 0x00;
		*bus = 0x00;
		*bus = 0x00;
	}
}

inline void init_tft() {
	pio_disable_pullup(PIOC, PIN_34C | PIN_35C | PIN_36C | PIN_37C | PIN_38C | PIN_39C | PIN_40C | PIN_41C | PIN_45C | PIN_47C);
	pio_enable_output(PIOC, PIN_34C | PIN_35C | PIN_36C | PIN_37C | PIN_38C | PIN_39C | PIN_40C | PIN_41C | PIN_45C | PIN_47C);
	pio_enable_pio(PIOC, PIN_47C);
	pio_output_write_HIGH(PIOC, PIN_47C);
	smc_tft_lcd_setup();
	lcd_set_command();
	*bus = 0x01;	//software reset
	delayMicroseconds(5000);
	*bus = 0x29;	//display on
	delayMicroseconds(5000);
	*bus = 0x11;	//sleep out
	delayMicroseconds(5000);
	lcd_set_columns(0, 239);
	lcd_set_pages(0, 319);
	lcd_clear();
	lcd_mem_write();
}