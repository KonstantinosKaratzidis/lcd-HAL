#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "lcd.h"
#include "lcd_funcs.h"

#define DELAY_MS 100

void lcd_write(lcd_ctx_t *ctx, const char *txt){
	for(const char *c = txt; *c != '\0'; c++)
		lcd_write_char(ctx, *c);
}

void lcd_write_delay(lcd_ctx_t *ctx, const char *txt, int delay_ms){
	for(const char *c = txt; *c != '\0'; c++){
		lcd_write_char(ctx, *c);
		int remaining = delay_ms;
		while(remaining--){
			_delay_ms(1);
		}
	}
}

void setup(lcd_ctx_t *ctx){
	lcd_funcs_t funcs;
	lcd_settings_t settings = {
		.length = LCD_DATA_LENGTH_8,
		.lines = LCD_NUM_LINES_2,
		.font = LCD_FONT_5x8
	};

	setup_lcd_funcs(&funcs);
	lcd_init(ctx, &settings, &funcs);
}

int main(void) {
	lcd_ctx_t lcd_ctx, *ctx;
	ctx = &lcd_ctx;
	setup(ctx);

	lcd_write(ctx, "Hello!");

	for(uint8_t i = 0; i < 6; i++){
		lcd_set_ddram_addr(ctx, 0 + i);
		uint8_t data = lcd_read_data(ctx);
		lcd_set_ddram_addr(ctx, 0x40 + i);
		lcd_write_char(ctx, data);
	}
	

	while(1)
		;
  return 0;
}
