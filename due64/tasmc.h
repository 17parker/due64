#pragma once

#define LCD_WR PIN_45C	//active HIGH
#define LCD_RS PIN_47C	//active HIGH
//Commands for the TFT
#define COL_ADDR_SET 0x2A
#define PAGE_ADDR_SET 0x2B
#define MEM_WRITE 0x2C

//Each byte in the frame buffer selects one color from 256 possible colors
//The 256 possible colors will be a hard-coded const array
//The update one half of the frame buffer while the other is being shown
volatile uint8_t frame_buffer[320][240] = { 0 };
volatile uint8_t* const frame_buffer1 = frame_buffer[0];
volatile uint8_t* const frame_buffer2 = frame_buffer[160]; //midpoint of the arr


inline void lcd_strobe_write();
inline void lcd_set_command();
inline void lcd_set_data();
inline void lcd_set_columns(uint8_t start, uint8_t end);
inline void lcd_set_pages(uint16_t start, uint16_t end);
inline void lcd_clear();
inline void lcd_display_on();
inline void lcd_sleep_out();
inline void lcd_software_reset();
inline void init_tft();
inline void print_frame();
inline void lcd_write(uint8_t data);
inline void lcd_test();


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

inline void lcd_write(uint8_t data) {
	*((uint32_t*)0x60000000) = data;
}

inline void print_frame() {
	const uint32_t* line_ptr = f;
	lcd_set_pages(310, 317);
	lcd_set_columns(206, 237);
	pio_output_write(PIOC, (MEM_WRITE) << 1);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
	for (uint8_t i = 0; i < 8; ++i) {
		for (uint8_t j = 0; j < 32; ++j) {
			if (((*line_ptr) >> j) & 1) {
				pio_output_write(PIOD, 0xFF);
				lcd_strobe_write();
				pio_output_write(PIOD, 0xFF);
				lcd_strobe_write();
				pio_output_write(PIOD, 0xFF);
				lcd_strobe_write();
			}
			else {
				pio_output_write(PIOD, 0x00);
				lcd_strobe_write();
				pio_output_write(PIOD, 0x00);
				lcd_strobe_write();
				pio_output_write(PIOD, 0x00);
				lcd_strobe_write();
			}
		}
		++line_ptr;
	}
}

//Set the RS (D/CX) pin LOW to send command
inline void lcd_set_command() { pio_output_write_LOW(PIOC, LCD_RS); }
//Set the RS (D/CX) pin HIGH to send data
inline void lcd_set_data() { pio_output_write_HIGH(PIOC, LCD_RS); }
inline void lcd_strobe_write() {
	pio_output_write_LOW(PIOC, LCD_WR);
	pio_output_write_HIGH(PIOC, LCD_WR);
}

inline void lcd_set_columns(uint8_t start, uint8_t end) {
	//This sends 1 byte to specify command and 4 bytes of data (2 for start, 2 for end)
	pio_output_write(PIOC, (COL_ADDR_SET) << 1);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
	pio_output_write(PIOC, 0x00);
	lcd_strobe_write();
	pio_output_write(PIOC, start << 1);
	lcd_strobe_write();
	pio_output_write(PIOC, 0x00);
	lcd_strobe_write();
	pio_output_write(PIOC, end << 1);
	lcd_strobe_write();
}

//The values for pages goes from 0-319, and anything over 255 is over 1 byte, so this needs 2 bytes
inline void lcd_set_pages(uint16_t start, uint16_t end) {
	lcd_set_command();
	pio_output_write(PIOC, (PAGE_ADDR_SET) << 1);
	lcd_strobe_write();
	lcd_set_data();
	pio_output_write(PIOC, start >> 7);
	lcd_strobe_write();
	pio_output_write(PIOC, start << 1);
	lcd_strobe_write();
	pio_output_write(PIOD, end >> 7);
	lcd_strobe_write();
	pio_output_write(PIOD, end << 1);
	lcd_strobe_write();
}

inline void lcd_clear() {
	lcd_set_command();
	pio_output_write(PIOC, (MEM_WRITE) << 1);
	lcd_strobe_write();
	lcd_set_data();
	for (uint32_t pixels = 0; pixels <= 76800; ++pixels) {
		//Blue
		pio_output_write(PIOC, 0x00);
		lcd_strobe_write();
		//Green
		pio_output_write(PIOC, 0x00);
		lcd_strobe_write();
		//Red
		pio_output_write(PIOC, 0x00);
		lcd_strobe_write();
	}
}

inline void lcd_display_on() {
	pio_output_write(PIOC, 0x29 << 1);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
	delayMicroseconds(5000);
}

inline void lcd_sleep_out() {
	lcd_set_command();
	pio_output_write(PIOC, 0x11 << 1);
	lcd_strobe_write();
	lcd_set_data();
	delayMicroseconds(5000);		//There needs to be a short delay to allow voltages to stabilize
}

inline void lcd_software_reset() {
	lcd_set_command();
	pio_output_write(PIOC, 0x01 << 1);
	lcd_strobe_write();
	lcd_set_data();
	delayMicroseconds(5000);
}

inline void lcd_test() {
	lcd_write(0x00);
	lcd_write(0xff);
	lcd_write(0x00);
}

inline void init_tft() {
	pio_disable_pullup(PIOC, PIN_33C | PIN_34C | PIN_35C | PIN_36C | PIN_37C | PIN_38C | PIN_39C | PIN_40C | PIN_45C | PIN_47C);
	pio_enable_output(PIOC, PIN_33C | PIN_34C | PIN_35C | PIN_36C | PIN_37C | PIN_38C | PIN_39C | PIN_40C | PIN_45C | PIN_47C);
	pio_disable_pio(PIOC, PIN_33C | PIN_34C | PIN_35C | PIN_36C | PIN_37C | PIN_38C | PIN_39C | PIN_40C | PIN_45C | PIN_47C);
	pio_output_write_HIGH(PIOC, PIN_33C | PIN_34C | PIN_35C | PIN_36C | PIN_37C | PIN_38C | PIN_39C | PIN_40C | PIN_45C | PIN_47C);
	pio_enable_output_write(PIOC, PIN_33C | PIN_34C | PIN_35C | PIN_36C | PIN_37C | PIN_38C | PIN_39C | PIN_40C | PIN_47C);
	pio_disable_output_write(PIOC, LCD_WR);
	lcd_software_reset();
	lcd_display_on();
	lcd_sleep_out();
	lcd_set_columns(0, 239);
	lcd_set_pages(0, 319);
	lcd_clear();
	lcd_set_command();
	pio_output_write(PIOC, MEM_WRITE << 1);
	lcd_strobe_write();
	lcd_set_data();
}