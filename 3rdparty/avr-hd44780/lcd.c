#include "lcd.h"

#include <stdarg.h>
#include <stdio.h>
#include <util/delay.h>

#ifdef LCD_FF_MOD
#  include "os.h"
#  include "hw.h"
static void sDelay(const uint8_t ms)
{
    if (osTaskIsSchedulerRunning())
    {
        osTaskDelay(ms);
    }
    else
    {
        const uint32_t us = (uint32_t)ms * 1000;
        hwDelay(us);
    }
}
#endif


void lcd_send(uint8_t value, uint8_t mode);
void lcd_write_nibble(uint8_t nibble);

static uint8_t lcd_displayparams;
#ifndef LCD_FF_MOD
static char lcd_buffer[LCD_COL_COUNT + 1];
#endif

void lcd_command(uint8_t command) {
  lcd_send(command, 0);
}

void lcd_write(uint8_t value) {
  lcd_send(value, 1);
}

void lcd_send(uint8_t value, uint8_t mode) {
#ifdef LCD_FF_MOD
    if (mode)
    {
        PIN_HIGH(FF_HD44780_RS_PIN);
    }
    else
    {
        PIN_LOW(FF_HD44780_RS_PIN);
    }
    PIN_LOW(FF_HD44780_RW_PIN);
#else
  if (mode) {
    LCD_PORT = LCD_PORT | (1 << LCD_RS);
  } else {
    LCD_PORT = LCD_PORT & ~(1 << LCD_RS);
  }

  LCD_PORT = LCD_PORT & ~(1 << LCD_RW);
#endif
  lcd_write_nibble(value >> 4);
  lcd_write_nibble(value);
}

void lcd_write_nibble(uint8_t nibble) {
#ifdef LCD_FF_MOD
    if (nibble & 0x01) { PIN_HIGH(FF_HD44780_D4_PIN); } else { PIN_LOW(FF_HD44780_D4_PIN); }
    if (nibble & 0x02) { PIN_HIGH(FF_HD44780_D5_PIN); } else { PIN_LOW(FF_HD44780_D5_PIN); }
    if (nibble & 0x04) { PIN_HIGH(FF_HD44780_D6_PIN); } else { PIN_LOW(FF_HD44780_D6_PIN); }
    if (nibble & 0x08) { PIN_HIGH(FF_HD44780_D7_PIN); } else { PIN_LOW(FF_HD44780_D7_PIN); }

    PIN_LOW( FF_HD44780_E_PIN);
    PIN_HIGH(FF_HD44780_E_PIN);
    PIN_LOW( FF_HD44780_E_PIN);
    sDelay(1);
#else
  LCD_PORT = (LCD_PORT & 0xff & ~(0x0f << LCD_D0)) | ((nibble & 0x0f) << LCD_D0);

  LCD_PORT = LCD_PORT & ~(1 << LCD_EN);
  LCD_PORT = LCD_PORT | (1 << LCD_EN);
  LCD_PORT = LCD_PORT & ~(1 << LCD_EN);
  _delay_ms(0.04);
#endif
}

void lcd_init(void) {
  // Configure pins as output
#ifdef LCD_FF_MOD
  PIN_OUTPUT(FF_HD44780_RS_PIN);
  PIN_OUTPUT(FF_HD44780_RW_PIN);
  PIN_OUTPUT(FF_HD44780_E_PIN);
  PIN_OUTPUT(FF_HD44780_D4_PIN);
  PIN_OUTPUT(FF_HD44780_D5_PIN);
  PIN_OUTPUT(FF_HD44780_D6_PIN);
  PIN_OUTPUT(FF_HD44780_D7_PIN);
#else
  LCD_DDR = LCD_DDR
    | (1 << LCD_RS)
    | (1 << LCD_RW)
    | (1 << LCD_EN)
    | (1 << LCD_D0)
    | (1 << LCD_D1)
    | (1 << LCD_D2)
    | (1 << LCD_D3);
#endif

  // Wait for LCD to become ready (docs say 15ms+)
  _delay_ms(15);

#ifdef LCD_FF_MOD
  PIN_LOW(FF_HD44780_RS_PIN);
  PIN_LOW(FF_HD44780_RW_PIN);
  PIN_LOW(FF_HD44780_E_PIN);
#else
  LCD_PORT = LCD_PORT
    & ~(1 << LCD_EN)
    & ~(1 << LCD_RS)
    & ~(1 << LCD_RW);
#endif

  _delay_ms(4.1);

  lcd_write_nibble(0x03); // Switch to 4 bit mode
  _delay_ms(4.1);

  lcd_write_nibble(0x03); // 2nd time
  _delay_ms(4.1);

  lcd_write_nibble(0x03); // 3rd time
  _delay_ms(4.1);

  lcd_write_nibble(0x02); // Set 8-bit mode (?)

  lcd_command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);

  lcd_displayparams = LCD_CURSOROFF | LCD_BLINKOFF;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_on(void) {
  lcd_displayparams |= LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_off(void) {
  lcd_displayparams &= ~LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_clear(void) {
  lcd_command(LCD_CLEARDISPLAY);
#ifdef LCD_FF_MOD
  sDelay(2);
#else
  _delay_ms(2);
#endif
}

void lcd_return_home(void) {
  lcd_command(LCD_RETURNHOME);
#ifdef LCD_FF_MOD
  sDelay(2);
#else
  _delay_ms(2);
#endif
}

void lcd_enable_blinking(void) {
  lcd_displayparams |= LCD_BLINKON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_disable_blinking(void) {
  lcd_displayparams &= ~LCD_BLINKON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_enable_cursor(void) {
  lcd_displayparams |= LCD_CURSORON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_disable_cursor(void) {
  lcd_displayparams &= ~LCD_CURSORON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_scroll_left(void) {
  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void lcd_scroll_right(void) {
  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void lcd_set_left_to_right(void) {
  lcd_displayparams |= LCD_ENTRYLEFT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_set_right_to_left(void) {
  lcd_displayparams &= ~LCD_ENTRYLEFT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_enable_autoscroll(void) {
  lcd_displayparams |= LCD_ENTRYSHIFTINCREMENT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_disable_autoscroll(void) {
  lcd_displayparams &= ~LCD_ENTRYSHIFTINCREMENT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_create_char(uint8_t location, uint8_t *charmap) {
  lcd_command(LCD_SETCGRAMADDR | ((location & 0x7) << 3));
  for (int i = 0; i < 8; i++) {
    lcd_write(charmap[i]);
  }
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
  static uint8_t offsets[] = { 0x00, 0x40, 0x14, 0x54 };

  if (row > 1) {
    row = 1;
  }

  lcd_command(LCD_SETDDRAMADDR | (col + offsets[row]));
}

#ifndef LCD_FF_MOD
void lcd_puts(char *string) {
  for (char *it = string; *it; it++) {
    lcd_write(*it);
  }
}
#endif // !LCD_FF_MOD

#ifndef LCD_FF_MOD
void lcd_printf(char *format, ...) {
  va_list args;

  va_start(args, format);
  vsnprintf(lcd_buffer, LCD_COL_COUNT + 1, format, args);
  va_end(args);

  lcd_puts(lcd_buffer);
}
#endif // !LCD_FF_MOD
