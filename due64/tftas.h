#pragma once

//LCD_RS - REGISTER SELECT -AKA C/D or D/CX
//LCD_RS - LOW = command, HIGH = data
//The special pins (from PC1 to PC5):
//33, 34, 35, 36, 37
#define LCD_RD PIN_33C	//PC1, send from LOW to HIGH to get data to read on the bus
#define LCD_WR PIN_34C	//PC2, send from LOW to HIGH to write data/command
#define LCD_RS PIN_35C	//PC3, set LOW to send command, set HIGH to send data
#define LCD_CS PIN_36C	//PC4, set LOW to start comms, set HIGH to end comms - CAN BE GROUNDED (UNNECESSARY)
#define LCD_RST PIN_37C	//PC5, Reset pin (active LOW) - UNUSED

//Data pins (from PD0 to PD7):
//25, 26, 27, 28, 14, 15, 29, 11
#define PORTD_DATA 0b11111111	//using PD0 to PD7
#define PORTC_PINS 0b111110		//using PC1 to PC5

//Commands for the TFT
#define COL_ADDR_SET 0x2A
#define PAGE_ADDR_SET 0x2B
#define MEM_WRITE 0x2C
volatile uint8_t* const bus = (uint8_t*)0x60000000;


inline void lcd_strobe_write();
inline void lcd_strobe_read();
inline void lcd_set_command();
inline void lcd_set_data();
inline void lcd_send_0_byte_command(uint8_t comm);
inline void lcd_send_1_byte_command(uint8_t comm, uint8_t data);
inline void lcd_send_2_byte_command(uint8_t comm, uint8_t upper_byte, uint8_t lower_byte);
inline void lcd_send_3_byte_command(uint8_t comm, uint8_t upper_byte, uint8_t middle_byte, uint8_t lower_byte);
inline void lcd_set_columns(uint8_t start, uint8_t end);
inline void lcd_set_pages(uint16_t start, uint16_t end);
inline void lcd_clear();
inline void lcd_display_on();
inline void lcd_sleep_out();
inline void lcd_software_reset();
inline void init_tft();
inline void print_frame();
inline void update_num_buffer();
inline void display_nums();

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


inline void print_frame() {
	const uint32_t* line_ptr = f;
	lcd_set_pages(310, 317);
	lcd_set_columns(206, 237);
	pio_output_write(PIOD, MEM_WRITE);
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

inline void update_num_buffer() {
	num_buff[0] = num[tene0];
	num_buff[1] = num[tene1];
	num_buff[2] = num[tene2];
	num_buff[3] = num[tene3];
	num_buff[4] = num[tene4];
	num_buff[5] = num[tene5];
	num_buff[6] = num[tene6];
}

inline void display_nums() {
	lcd_set_pages(310, 317);
	lcd_set_columns(168, 171);
	pio_output_write(PIOD, MEM_WRITE);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
	for (uint8_t i = 0; i <= 31; ++i) {
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[0] >> i) & 1) {
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
	lcd_set_columns(173, 176);
	pio_output_write(PIOD, MEM_WRITE);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
	for (uint8_t i = 0; i <= 31; ++i) {
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[1] >> i) & 1) {
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
	lcd_set_columns(178, 181);
	pio_output_write(PIOD, MEM_WRITE);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
	for (uint8_t i = 0; i <= 31; ++i) {
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[2] >> i) & 1) {
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
	lcd_set_columns(183, 186);
	pio_output_write(PIOD, MEM_WRITE);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
	for (uint8_t i = 0; i <= 31; ++i) {
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[3] >> i) & 1) {
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
	lcd_set_columns(188, 191);
	pio_output_write(PIOD, MEM_WRITE);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
	for (uint8_t i = 0; i <= 31; ++i) {
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[4] >> i) & 1) {
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
	lcd_set_columns(193, 196);
	pio_output_write(PIOD, MEM_WRITE);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
	for (uint8_t i = 0; i <= 31; ++i) {
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[5] >> i) & 1) {
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
	lcd_set_columns(198, 201);
	pio_output_write(PIOD, MEM_WRITE);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
	for (uint8_t i = 0; i <= 31; ++i) {
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
	}
	for (uint8_t i = 0; i <= 31; ++i) {
		if ((num_buff[6] >> i) & 1) {
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
}

//Set the RS (D/CX) pin LOW to send command
inline void lcd_set_command() { pio_output_write_LOW(PIOC, LCD_RS); }
//Set the RS (D/CX) pin HIGH to send data
inline void lcd_set_data() { pio_output_write_HIGH(PIOC, LCD_RS); }
inline void lcd_strobe_write() {
	pio_output_write_HIGH(PIOC, LCD_WR);
	pio_output_write_LOW(PIOC, LCD_WR);
}
inline void lcd_strobe_read() {
	pio_output_write_LOW(PIOC, LCD_RD);
	pio_output_write_HIGH(PIOC, LCD_RD);
}


inline void lcd_send_0_byte_command(uint8_t comm) {
	pio_output_write(PIOD, comm);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
}

inline void lcd_send_1_byte_command(uint8_t comm, uint8_t data) {
	pio_output_write(PIOD, comm);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
	pio_output_write(PIOD, data);
	lcd_strobe_write();
}

inline void lcd_send_2_byte_command(uint8_t comm, uint8_t upper_byte, uint8_t lower_byte) {
	lcd_set_command();
	pio_output_write(PIOD, comm);
	lcd_strobe_write();
	lcd_set_data();
	pio_output_write(PIOD, upper_byte);
	lcd_strobe_write();
	pio_output_write(PIOD, lower_byte);
	lcd_strobe_write();
}

inline void lcd_send_3_byte_command(uint8_t comm, uint8_t upper_byte, uint8_t middle_byte, uint8_t lower_byte) {
	lcd_set_command();
	pio_output_write(PIOD, comm);
	lcd_strobe_write();
	lcd_set_data();
	pio_output_write(PIOD, upper_byte);
	lcd_strobe_write();
	pio_output_write(PIOD, middle_byte);
	lcd_strobe_write();
	pio_output_write(PIOD, lower_byte);
	lcd_strobe_write();
}

inline void lcd_set_columns(uint8_t start, uint8_t end) {
	//This sends 1 byte to specify command and 4 bytes of data (2 for start, 2 for end)
	pio_output_write(PIOD, COL_ADDR_SET);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
	pio_output_write(PIOD, 0x00);
	lcd_strobe_write();
	pio_output_write(PIOD, start);
	lcd_strobe_write();
	pio_output_write(PIOD, 0x00);
	lcd_strobe_write();
	pio_output_write(PIOD, end);
	lcd_strobe_write();
}

//The values for pages goes from 0-319, and anything over 255 is over 1 byte, so this needs 2 bytes
inline void lcd_set_pages(uint16_t start, uint16_t end) {
	lcd_set_command();
	pio_output_write(PIOD, PAGE_ADDR_SET);
	lcd_strobe_write();
	lcd_set_data();
	pio_output_write(PIOD, start >> 8);
	lcd_strobe_write();
	pio_output_write(PIOD, start);
	lcd_strobe_write();
	pio_output_write(PIOD, end >> 8);
	lcd_strobe_write();
	pio_output_write(PIOD, end);
	lcd_strobe_write();
}

inline void lcd_clear() {
	pio_output_write(PIOD, MEM_WRITE);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
	for (uint32_t pixels = 0; pixels <= 76800; ++pixels) {
		//Blue
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		//Green
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
		//Red
		pio_output_write(PIOD, 0x00);
		lcd_strobe_write();
	}
}

inline void lcd_display_on() {
	pio_output_write(PIOD, 0x29);
	lcd_set_command();
	lcd_strobe_write();
	lcd_set_data();
	delayMicroseconds(5000);
}

inline void lcd_sleep_out() {
	lcd_set_command();
	pio_output_write(PIOD, 0x11);
	lcd_strobe_write();
	lcd_set_data();
	delayMicroseconds(5000);		//There needs to be a short delay to allow voltages to stabilize
}

inline void lcd_software_reset() {
	lcd_set_command();
	pio_output_write(PIOD, 0x01);
	lcd_strobe_write();
	lcd_set_data();
	delayMicroseconds(5000);
}


inline void init_tft() {
	pio_enable_pio(PIOC, PORTC_PINS);
	pio_disable_pullup(PIOC, PORTC_PINS);
	pio_enable_output(PIOC, PORTC_PINS);
	pio_enable_output_write(PIOC, PORTC_PINS);
	pio_enable_pio(PIOD, PORTD_DATA);
	pio_disable_pullup(PIOD, PORTD_DATA);
	pio_enable_output(PIOD, PORTD_DATA);
	pio_enable_output_write(PIOD, PORTD_DATA);
	pio_output_write_HIGH(PIOC, 0xFFFFFFFF);
	pio_output_write_HIGH(PIOD, 0xFFFFFFFF);
	lcd_software_reset();
	lcd_display_on();
	lcd_sleep_out();
	lcd_set_columns(0, 239);
	lcd_set_pages(0, 319);
	lcd_clear();
}