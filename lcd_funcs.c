#include "lcd_funcs.h"
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

static void init_pins(){
	LCD_RS_DDR |= (1 << LCD_RS);
	LCD_RW_DDR |= (1 << LCD_RW);
	LCD_E_DDR |= (1 << LCD_E);
	LCD_DB_DDR = 0xff;
}

static void set_rs(bool val){
	if(!val)
		LCD_RS_PORT &= ~(1 << LCD_RS);
	else
		LCD_RS_PORT |= (1 << LCD_RS);
}

static void set_rw(bool val){
	if(!val)
		LCD_RW_PORT &= ~(1 << LCD_RW);
	else
		LCD_RW_PORT |= (1 << LCD_RW);
}

static void set_enable(bool val){
	if(!val)
		LCD_E_PORT &= ~(1 << LCD_E);
	else
		LCD_E_PORT |= (1 << LCD_E);
}

static void write_data(uint8_t data){
	LCD_DB_PORT = data;
}

static uint8_t read_data(){
	// set port d as input
	LCD_DB_DDR = 0;
	write_data(0); // disalbes pull ups
	__asm__ __volatile__("nop");
	// read the values of the pins
	uint8_t val = LCD_DB_PIN;
	// set port d as output
	LCD_DB_DDR = 0xff;
	return val;
}

static void delay(int us){
	while(us--)
		_delay_us(1);
}

void setup_lcd_funcs(lcd_funcs_t *funcs){
	funcs->init_pins = init_pins;
	funcs->set_rs = set_rs;
	funcs->set_rw = set_rw;
	funcs->set_enable = set_enable;
	funcs->write_data = write_data;
	funcs->read_data = read_data;
	funcs->delay_us = delay;
}
