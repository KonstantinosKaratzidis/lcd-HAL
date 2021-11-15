#include <string.h>
#include "lcd.h"

#define AS_BYTE(x) ((uint8_t) (*((uint8_t*) &(x))))

static
void command(lcd_ctx_t *ctx, lcd_value_t rs, lcd_value_t rw, uint8_t data);

// void lcd_init(
// 		lcd_ctx_t *ctx, lcd_data_length_t length, lcd_num_lines_t lines,
// 		lcd_font_t font){
void lcd_init(lcd_ctx_t *ctx, lcd_settings_t *settings, lcd_funcs_t *funcs){
	memset(ctx, 0, sizeof(lcd_ctx_t));
	memcpy(&ctx->funcs, funcs, sizeof(lcd_funcs_t));
	// initialize pins to a known state
	funcs->init_pins();
	funcs->set_rs(LCD_LOW);
	funcs->set_rw(LCD_LOW);
	funcs->set_enable(LCD_LOW);
	funcs->write_data(0);

	// intialize private data
	ctx->_entry_mode.code = 1;
	ctx->_display_control.code = 1;
	ctx->_display_control.on_off = 1;
	ctx->_cursor_display_shift.code = 1;

	ctx->_function.code = 1;
	ctx->_function.font = settings->font;
	ctx->_function.num_lines = settings->lines;
	ctx->_function.data_length = settings->length;

	// set function of lcd display
	command(ctx, LCD_LOW, LCD_LOW, AS_BYTE(ctx->_function));
	// set dispaly on, clear it and return to home
	lcd_on(ctx, true);
	lcd_clear(ctx);
	lcd_home(ctx);
}

static void pulse_enable(lcd_ctx_t *ctx){
	const lcd_funcs_t *funcs = &ctx->funcs;
	funcs->set_enable(LCD_LOW);
	funcs->delay_us(50);
	funcs->set_enable(LCD_HIGH);
	funcs->delay_us(450);
	funcs->set_enable(LCD_LOW);
	funcs->delay_us(50);
}

void command(lcd_ctx_t *ctx, lcd_value_t rs, lcd_value_t rw, uint8_t data){
	const lcd_funcs_t *funcs = &ctx->funcs;
	funcs->set_rs(rs);
	funcs->set_rw(rw);
	funcs->write_data(data);
	pulse_enable(ctx);
}

void lcd_cursor(lcd_ctx_t *ctx, bool val){
	ctx->_display_control.cursor = val ? 1 : 0;
	command(ctx, LCD_LOW, LCD_LOW, AS_BYTE(ctx->_display_control));
}

void lcd_cursor_blink(lcd_ctx_t *ctx, bool val){
	ctx->_display_control.blink = val ? 1 : 0;
	command(ctx, LCD_LOW, LCD_LOW, AS_BYTE(ctx->_display_control));
}

void lcd_clear(lcd_ctx_t *ctx){
	command(ctx, LCD_LOW, LCD_LOW, 0b10);
}

void lcd_home(lcd_ctx_t *ctx){
	command(ctx, LCD_LOW, LCD_LOW, 1);
	ctx->funcs.delay_us(2000);
}

void lcd_on(lcd_ctx_t *ctx, bool val){
	ctx->_display_control.on_off = val;
	command(ctx, LCD_LOW, LCD_LOW, AS_BYTE(ctx->_display_control));
}

void lcd_cursor_shift(lcd_ctx_t *ctx, lcd_direction_t direction){
	ctx->_cursor_display_shift.shift_move = 0;
	ctx->_cursor_display_shift.left_right = direction;
	command(ctx, LCD_LOW, LCD_LOW, AS_BYTE(ctx->_cursor_display_shift));
}

void lcd_display_shift(lcd_ctx_t * ctx, lcd_direction_t direction){
	ctx->_cursor_display_shift.shift_move = 1;
	ctx->_cursor_display_shift.left_right = direction;
	command(ctx, LCD_LOW, LCD_LOW, AS_BYTE(ctx->_cursor_display_shift));
}

void lcd_set_ddram_addr(lcd_ctx_t *ctx, uint8_t addr){
	command(ctx, LCD_LOW, LCD_LOW, (1 << 7) | (addr & 0b01111111));
}

void lcd_set_cgram_addr(lcd_ctx_t *ctx, uint8_t addr){
	command(ctx, LCD_LOW, LCD_LOW, (1 << 6) | (addr & 0b00111111));
}

void lcd_write_char(lcd_ctx_t * ctx, uint8_t c){
	command(ctx, LCD_HIGH, LCD_LOW, c);
}

uint8_t lcd_read_addr(lcd_ctx_t *ctx){
	const lcd_funcs_t *funcs = &ctx->funcs;
	funcs->set_rs(false);
	funcs->set_rw(true);
	funcs->set_enable(true);
	uint8_t data;
	while((data = funcs->read_data()) & 0b10000000)
		;
	funcs->set_enable(false);
	return data & 0b01111111;
}

uint8_t lcd_read_data(lcd_ctx_t *ctx){
	const lcd_funcs_t *funcs = &ctx->funcs;
	funcs->set_rs(true);
	funcs->set_rw(true);
	funcs->set_enable(true);
	funcs->delay_us(50);
	uint8_t data = funcs->read_data();
	funcs->set_enable(false);
	return data;
}
