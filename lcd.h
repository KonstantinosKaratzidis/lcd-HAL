#ifndef LCD_H
#define LCD_H
#include <stdint.h>
#include <stdbool.h>

typedef enum {
  LCD_DATA_LENGTH_4 = 0,
	LCD_DATA_LENGTH_8 = 1
} lcd_data_length_t;

typedef enum {
	LCD_NUM_LINES_1 = 0,
	LCD_NUM_LINES_2 = 1
} lcd_num_lines_t;

typedef enum {
	LCD_FONT_5x8 = 0,
	LCD_FONT_5x11 = 1
} lcd_font_t;

typedef enum {
	LCD_LOW = 0,
	LCD_HIGH = 1
} lcd_value_t;

typedef struct {
	void    (*init_pins)();
	void    (*set_rs)(bool val);
	void    (*set_rw)(bool val);
	void    (*set_enable)(bool val);
	void    (*write_data)(uint8_t);
	uint8_t (*read_data)();
	void    (*delay_us)(int us);
} lcd_funcs_t;

typedef struct {
	lcd_funcs_t funcs;
	struct {
		uint8_t shift:1;
		uint8_t incr_decr:1;
		uint8_t code:1;
		uint8_t _empty:5;
	} _entry_mode;

	struct {
		uint8_t blink:1;
		uint8_t cursor:1;
		uint8_t on_off:1;
		uint8_t code:1;
		uint8_t _empty:4;
	} _display_control;

	struct {
		uint8_t _unused:2;
		uint8_t left_right:1;
		uint8_t shift_move:1;
		uint8_t code:1;
		uint8_t _empty:3;
	} _cursor_display_shift;

	struct {
		uint8_t _unused:2;
		uint8_t font:1;
		uint8_t num_lines:1;
		uint8_t data_length:1;
		uint8_t code:1;
		uint8_t _empty:2;
	} _function;
} lcd_ctx_t;

typedef enum {
	LCD_LEFT = 0,
	LCD_RIGHT = 1
} lcd_direction_t;

typedef struct {
	lcd_data_length_t length;
	lcd_num_lines_t lines;
	lcd_font_t font;
} lcd_settings_t;

// initialize the lcd contenxt and the lcd display
void lcd_init(lcd_ctx_t *ctx, lcd_settings_t *settings, lcd_funcs_t *funcs);

void lcd_on(lcd_ctx_t *, bool);
void lcd_clear(lcd_ctx_t*);
void lcd_home(lcd_ctx_t*);
void lcd_cursor(lcd_ctx_t *, bool);
void lcd_cursor_blink(lcd_ctx_t *, bool);
void lcd_cursor_shift(lcd_ctx_t *, lcd_direction_t);
void lcd_display_shift(lcd_ctx_t *, lcd_direction_t);
void lcd_set_ddram_addr(lcd_ctx_t *, uint8_t addr);
void lcd_set_cgram_addr(lcd_ctx_t *, uint8_t addr);
void lcd_write_char(lcd_ctx_t *, uint8_t c);
uint8_t lcd_read_addr(lcd_ctx_t *);
uint8_t lcd_read_data(lcd_ctx_t *);

#endif // LCD_H
