#ifndef LCD_FUNCS_H
#define LCD_FUNCS_H
#include "lcd.h"

// register select
#define LCD_RS_DDR DDRB
#define LCD_RS_PORT PORTB
#define LCD_RS PB1

// read/write
#define LCD_RW_DDR DDRB
#define LCD_RW_PORT PORTB
#define LCD_RW PB2

// enable
#define LCD_E_DDR DDRB
#define LCD_E_PORT PORTB
#define LCD_E PB3

// data bus pins
#define LCD_DB_DDR  DDRD
#define LCD_DB_PORT PORTD
#define LCD_DB_PIN PIND

void setup_lcd_funcs(lcd_funcs_t *funcs);

#endif // LCD_FUNCS_H
