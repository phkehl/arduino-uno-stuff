/*!
    \file
    \brief flipflip's Arduino Uno stuff: configuration (see \ref CONFIG)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup CONFIG Configuration
    \ingroup FF

    These are the configuration defaults. Do not change them but instead
    set them in your local config.h file.

    @{
*/

#ifndef __DEFCONFIG_H__
#define __DEFCONFIG_H__

#include "arduinopins.h"


/* *************************************************************************** */

//! system load signal on pin (see \ref ARDUINOPINS, set to _NONE to disable)
#if !defined(FF_HW_LOAD_PIN) || defined(__DOXYGEN__)
#  define FF_HW_LOAD_PIN _A0
#endif

//! system tick signal on pin (see \ref ARDUINOPINS, set to _NONE to disable)
#if !defined(FF_HW_TICK_PIN) || defined(__DOXYGEN__)
#  define FF_HW_TICK_PIN _A1
#endif

//! serial port baudrate (don't set this too low!)
#if !defined(FF_HW_RXTX_BAUDRATE) || defined(__DOXYGEN__)
#  define FF_HW_RXTX_BAUDRATE 115200
#endif

//! serial port input buffer size (set to 0 to disable any input and free pin 0)
#if !defined(FF_HW_RX_BUFSIZE) || defined(__DOXYGEN__)
#  define FF_HW_RX_BUFSIZE 100
#endif

//! serial port output buffer size (set to 0 to disable any output and free pin 1)
#if !defined(FF_HW_TX_BUFSIZE) || defined(__DOXYGEN__)
#  define FF_HW_TX_BUFSIZE 125
#endif

//! number of runtime counters for hwTic() and hwToc(), will use Timer/Counter1 hardware if > 0
#if !defined(FF_HW_NUM_TICTOC) || defined(__DOXYGEN__)
#  define FF_HW_NUM_TICTOC 1
#endif

//! use watchdog
#if !defined(FF_HW_USE_WATCHDOG) || defined(__DOXYGEN__)
#  define FF_HW_USE_WATCHDOG 1
#endif

//! measure Vcc and print in hw monitor? (only on ATmega 168 / 328P, i.e. Arduino Uno)
#if !defined(FF_HW_MEAS_VCC) || defined(__DOXYGEN__)
#  if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168P__)
#    define FF_HW_MEAS_VCC 1
#  else
#    define FF_HW_MEAS_VCC 0
#  endif
#endif

//! measure temperature and print in hw monitor? (only on 328P, i.e. newer Arduino Uno)
#if !defined(FF_HW_MEAS_TEMP) || defined(__DOXYGEN__)
#  if defined(__AVR_ATmega328P__)
#    define FF_HW_MEAS_TEMP 1
#  else
#    define FF_HW_MEAS_TEMP 0
#  endif
#endif

//! temperature sensor calibration value (offset) (for hwReadTemp() and #FF_HW_MEAS_TEMP)
#if !defined(FF_HW_TEMP_OFFS) || defined(__DOXYGEN__)
#  define FF_HW_TEMP_OFFS 100
#endif

//! random number generator initialisation (0 = none, 1 = using watchdog jitter, 99 = combine all)
#if !defined(FF_HW_RANDSEED) || defined(__DOXYGEN__)
#  define FF_HW_RANDSEED 99
#endif


/* *************************************************************************** */

//! idle thread stack size
#if !defined(FF_OS_IDLE_STACK) || defined(__DOXYGEN__)
#  define FF_OS_IDLE_STACK 70
#endif

//! reserve heap memory (for malloc() etc.)
#if !defined(FF_OS_HEAP_SIZE) || defined(__DOXYGEN__)
#  define FF_OS_HEAP_SIZE 0
#endif


/* *************************************************************************** */

//! system task monitor period in 1000ticks (i.e. seconds)
#if !defined(FF_SYS_MON_PERIOD) || defined(__DOXYGEN__)
#  define FF_SYS_MON_PERIOD 5
#endif

//! verbose system monitor?
#if !defined(FF_SYS_MON_VERBOSE) || defined(__DOXYGEN__)
#  define FF_SYS_MON_VERBOSE 1
#endif

//! system task stack size (maybe needs to be increased with more tasks or more complex sysRegisterMonFunc() function)
#if !defined(FF_SYS_TASK_STACK) || defined(__DOXYGEN__)
#  if (FF_SYS_MON_VERBOSE > 0)
#    define FF_SYS_TASK_STACK 250
#  else
#    define FF_SYS_TASK_STACK 150
#  endif
#endif

//! system task priority, should be higher (i.e. a smaller number) than any user task priority
#if !defined(FF_SYS_TASK_PRIO) || defined(__DOXYGEN__)
#  define FF_SYS_TASK_PRIO 3
#endif

//! string size for #SYS_MON_FUNC_t argument (increasing this may need increasing #FF_SYS_TASK_STACK)
#if !defined(FF_SYS_MON_STRLEN) || defined(__DOXYGEN__)
#  define FF_SYS_MON_STRLEN 64
#endif

//! software watchdog period [#SYS_TASK_PERIOD]
#if !defined(FF_SYS_SW_WATCHDOG) || defined(__DOXYGEN__)
#  define FF_SYS_SW_WATCHDOG 0
#endif


/* *************************************************************************** */

//! enable DEBUG() output
#if !defined(FF_DEBUG_LEVEL) || defined(__DOXYGEN__)
#  define FF_DEBUG_LEVEL 1
#endif


/* *************************************************************************** */

//! hsv2rgb() method to use (1 = classic, 2 = correctd by lookup table, which will cost some SRAM)
#if !defined(FF_HSV2RGB_METHOD) || defined(__DOXYGEN__)
#  define FF_HSV2RGB_METHOD 1
#endif


/* *************************************************************************** */

//! SPI master speed for the \ref WS2801 in kHz (125, 250, 500, 1000, 2000, 4000, 8000)
#if !defined (FF_WS2801_SPI_SPEED) || defined(__DOXYGEN__)
#  define FF_WS2801_SPI_SPEED 250
#endif


/* *************************************************************************** */

//! number of LEDs
#if !defined (FF_LEDFX_NUM_LED) || defined(__DOXYGEN__)
#  define FF_LEDFX_NUM_LED 0
#endif

//! LED colour ordering (123 = RGB, 213 = GRB, 231 = GBR, etc.)
#if !defined (FF_LEDFX_ORDER) || defined(__DOXYGEN__)
#  define FF_LEDFX_ORDER 123
#endif

//! LED matrix x dimension (default #FF_LEDFX_NUM_LED)
#if !defined (FF_LEDFX_NUM_X) || defined(__DOXYGEN__)
#  define FF_LEDFX_NUM_X FF_LEDFX_NUM_LED
#endif

//! LED matrix y dimension (default 1)
#if !defined (FF_LEDFX_NUM_Y) || defined(__DOXYGEN__)
#  define FF_LEDFX_NUM_Y 1
#endif

//! LED matrix arrangement (see \ref LEDFX for details)
#if !defined (FF_LEDFX_XY_ARR) || defined(__DOXYGEN__)
#  define FF_LEDFX_XY_ARR 1
#endif


/* *************************************************************************** */

//! use UBX-NAV-PVT messages (0 = no, 1 = yes)
#if !defined (FF_UBX_NAV_PVT_USE) || defined(__DOXYGEN__)
#  define FF_UBX_NAV_PVT_USE 1
#endif

//! use UBX-NAV-AOPSTATUS messages (0 = no, 1 = yes)
#if !defined (FF_UBX_NAV_AOPSTATUS_USE) || defined(__DOXYGEN__)
#  define FF_UBX_NAV_AOPSTATUS_USE 0
#endif

//! use UBX-NAV-CLOCK messages (0 = no, 1 = yes)
#if !defined (FF_UBX_NAV_CLOCK_USE) || defined(__DOXYGEN__)
#  define FF_UBX_NAV_CLOCK_USE 0
#endif

//! use UBX-NAV-POSECEF messages (0 = no, 1 = yes)
#if !defined (FF_UBX_NAV_POSECEF_USE) || defined(__DOXYGEN__)
#  define FF_UBX_NAV_POSECEF_USE 0
#endif

//! use UBX-NAV-VELECEF messages (0 = no, 1 = yes)
#if !defined (FF_UBX_NAV_VELECEF_USE) || defined(__DOXYGEN__)
#  define FF_UBX_NAV_VELECEF_USE 0
#endif

//! use UBX-NAV-STATUS messages (0 = no, 1 = yes)
#if !defined (FF_UBX_NAV_STATUS_USE) || defined(__DOXYGEN__)
#  define FF_UBX_NAV_STATUS_USE 0
#endif

//! use UBX-INF-ERROR messages (0 = no, 1 = yes, 2 = yes and debug print via ERROR())
#if !defined (FF_UBX_INF_ERROR_USE) || defined(__DOXYGEN__)
#  define FF_UBX_INF_ERROR_USE 2
#endif

//! use UBX-INF-WARNING messages (0 = no, 1 = yes, 2 = yes and debug print via WARNING())
#if !defined (FF_UBX_INF_WARNING_USE) || defined(__DOXYGEN__)
#  define FF_UBX_INF_WARNING_USE 2
#endif

//! use UBX-INF-NOTICE messages (0 = no, 1 = yes, 2 = yes and debug print via PRINT())
#if !defined (FF_UBX_INF_NOTICE_USE) || defined(__DOXYGEN__)
#  define FF_UBX_INF_NOTICE_USE 2
#endif

//! use UBX-INF-TEST messages (0 = no, 1 = yes, 2 = yes and debug print via NOTICE())
#if !defined (FF_UBX_INF_TEST_USE) || defined(__DOXYGEN__)
#  define FF_UBX_INF_TEST_USE 0
#endif

//! use UBX-INF-DEBUG messages (0 = no, 1 = yes, 2 = yes and debug print via DEBUG())
#if !defined (FF_UBX_INF_DEBUG_USE) || defined(__DOXYGEN__)
#  define FF_UBX_INF_DEBUG_USE 0
#endif


/* *************************************************************************** */

//! GNSS task priority, should be higher (i.e. a smaller number) than #FF_SYS_TASK_PRIO
#if !defined (FF_GNSS_TASK_PRIO) || defined(__DOXYGEN__)
#  define FF_GNSS_TASK_PRIO 2
#endif

//! GNSS task stack size
#if !defined(FF_GNSS_TASK_STACK) || defined(__DOXYGEN__)
#  define FF_GNSS_TASK_STACK 125
#endif

//! GNSS parser to use (1 = \ref UBX)
#if !defined(FF_GNSS_PARSER) || defined(__DOXYGEN__)
#  define FF_GNSS_PARSER 1
#endif


/* *************************************************************************** */

//! aliexpress matrix operation mode (1 = simple, 2 = 4 brightness levels)
#if !defined(FF_ALIMATRIX_MODE) || defined(__DOXYGEN__)
#  define FF_ALIMATRIX_MODE 1
#endif


/* *************************************************************************** */

//! HD44780 number of lines
#if !defined(FF_HD44780_LINES) || defined(__DOXYGEN__)
#  define FF_HD44780_LINES 2
#endif

//! HD44780 number of columns
#if !defined(FF_HD44780_COLUMNS) || defined(__DOXYGEN__)
#  define FF_HD44780_COLUMNS 16
#endif

//! HD44780 RS pin
#if !defined(FF_HD44780_RS_PIN) || defined(__DOXYGEN__)
#  define FF_HD44780_RS_PIN  _D8
#endif

//! HD44780 RW pin
#if !defined(FF_HD44780_RW_PIN) || defined(__DOXYGEN__)
#  define FF_HD44780_RW_PIN  _D7
#endif

//! HD44780 E pin
#if !defined(FF_HD44780_E_PIN) || defined(__DOXYGEN__)
#  define FF_HD44780_E_PIN  _D6
#endif

//! HD44780 D4 pin
#if !defined(FF_HD44780_D4_PIN) || defined(__DOXYGEN__)
#  define FF_HD44780_D4_PIN  _D5
#endif

//! HD44780 D5 pin
#if !defined(FF_HD44780_D5_PIN) || defined(__DOXYGEN__)
#  define FF_HD44780_D5_PIN  _D4
#endif

//! HD44780 D6 pin
#if !defined(FF_HD44780_D6_PIN) || defined(__DOXYGEN__)
#  define FF_HD44780_D6_PIN  _D3
#endif

//! HD44780 D7 pin
#if !defined(FF_HD44780_D7_PIN) || defined(__DOXYGEN__)
#  define FF_HD44780_D7_PIN  _D2
#endif


/* *************************************************************************** */

//! tone generator enabled
#if !defined(FF_TONE_ENABLE) || defined(__DOXYGEN__)
#  define FF_TONE_ENABLE  0
#endif

//! tone generator output pin
#if !defined(FF_TONE_PIN) || defined(__DOXYGEN__)
#  define FF_TONE_PIN  _D8
#endif


/* *************************************************************************** */

//! rotary encoder input enabled
#if !defined(FF_ROTENC_ENABLE) || defined(__DOXYGEN__)
#  define FF_ROTENC_ENABLE  0
#endif

//! rotary encoder input pin 1 (signal 1)
#if !defined(FF_ROTENC_PIN1) || defined(__DOXYGEN__)
#  define FF_ROTENC_PIN1  _D2
#endif

//! rotary encoder input pin 2 (signal 2)
#if !defined(FF_ROTENC_PIN2) || defined(__DOXYGEN__)
#  define FF_ROTENC_PIN2  _D4
#endif

//! rotary encoder input pin 3 (button)
#if !defined(FF_ROTENC_PIN3) || defined(__DOXYGEN__)
#  define FF_ROTENC_PIN3  _D3
#endif

//! rotary encoder maximum number of events that can be queued
#if !defined(FF_ROTENC_NUMQ) || defined(__DOXYGEN__)
#  define FF_ROTENC_NUMQ  10
#endif


/* *************************************************************************** */

//! SPI master speed for the \ref SK9822 in kHz (125, 250, 500, 1000, 2000, 4000, 8000)
#if !defined (FF_SK9822_SPI_SPEED) || defined(__DOXYGEN__)
#  define FF_SK9822_SPI_SPEED 250
#endif


/* *************************************************************************** */

//! I2C master speed \ref I2C in kHz (100..400)
#if !defined (FF_I2C_MASTER_SPEED) || defined(__DOXYGEN__)
#  define FF_I2C_MASTER_SPEED 100
#endif


/* *************************************************************************** */

//! SSD1306 display size for \ref SSD1306 (64x84, 120x64, 128x32, 96x16)
#if !defined (FF_SSD1306_DISPLAY_SIZE) || defined(__DOXYGEN__)
#  define FF_SSD1306_DISPLAY_SIZE 6448
#endif

//! SSD1306 display address for \ref SSD1306 (0x3c or 0x3d)
#if !defined (FF_SSD1306_DISPLAY_ADDR) || defined(__DOXYGEN__)
#  define FF_SSD1306_DISPLAY_ADDR 0x3c
#endif

// SSD1306 display orientation for \ref SSD1306 (0, 1, 2, 3)
//#if !defined (FF_SSD1306_DISPLAY_ORIENT) || defined(__DOXYGEN__)
//#  define FF_SSD1306_DISPLAY_ORIENT 0
//#endif


/* *************************************************************************** */

#endif // __DEFCONFIG_H__

//@}
// eof
