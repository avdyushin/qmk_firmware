/* Copyright 2022 Jacky
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once


//eeconfig
// #define EECONFIG_KB_DATA_SIZE 3
#define EECONFIG_KB_DATA_SIZE  6

#define RGBLIGHT_DISABLE_KEYCODES

// #define WS2812_PWM_DRIVER PWMD4
// #define WS2812_PWM_CHANNEL 3
// #define WS2812_PWM_PAL_MODE 2
// #define WS2812_DMA_STREAM STM32_DMA1_STREAM7
// #define WS2812_DMA_CHANNEL 7

#define SC_WS2812_DI_PIN A10
#define SC_WS2812_LED_COUNT 4

// #define RGB_MATRIX_LED_COUNT 68


// #define DRIVER_1_LED_TOTAL 44

#define DRIVER_1_LED_TOTAL 45
// #define DRIVER_2_LED_TOTAL 38
#define DRIVER_2_LED_TOTAL 39
#define RGB_MATRIX_LED_COUNT (DRIVER_1_LED_TOTAL + DRIVER_2_LED_TOTAL)
#define SPI_DRIVER SPID1
#define AW_SPI_MODE 3


/* SPI Config for LED Driver */

#define SPI_SCK_PIN A5
#define SPI_MOSI_PIN A6
#define SPI_MISO_PIN A7

#define DRIVER_1_CS B6
#define AW20216S_CS_PIN_1 B6
#define DRIVER_2_CS C8
#define AW20216S_CS_PIN_2 C8
#define DRIVER_1_EN B7
#define DRIVER_2_EN B7
#define AW20216S_EN_PIN B7

#define DRIVER_COUNT 2

//#define DYNAMIC_KEYMAP_LAYER_COUNT 6
#define Module_Updata
#define RGB_MATRIX_KEYPRESSES // reacts to keypresses
//#define RGB_MATRIX_KEYRELEASES         // reacts to keyreleases (instead of keypresses)
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS // enable framebuffer effects
#define RGB_MATRIX_SOLID_REACTIVE_GRADIENT_MODE
#define RGB_MATRIX_TYPING_HEATMAP_SLIM



// #define RGB_DISABLE_WHEN_USB_SUSPENDED // USB待机时候停止灯效
//#define RGBLIGHT_SLEEP
// #define RGB_MATRIX_TIMEOUT 0    //rgb矩阵灯自动熄灭时间

//#define RGBLIGHT_DISABLE_KEYCODES

/* UART Config  */
#define SERIAL_DRIVER SD1
// #define SD1_TX_PAL_MODE PAL_MODE_ALTERNATE_OPENDRAIN
#define SD1_TX_PAL_MODE PAL_MODE_ALTERNATE_PUSHPULL
// #define WS2812_EXTERNAL_PULLUP
// #define SD1_TX_PAL_MODE PAL_MODE_ALTERNATE_OPENDRAIN
// #define SD1_TX_PAL_MODE PAL_MODE_ALTERNATE_PUSHPULL
// #define WS2812_EXTERNAL_PULLUP
#define UART_RX_PAL_MODE PAL_MODE_ALTERNATE_PUSHPULL
#define UART_TX_PAL_MODE PAL_MODE_ALTERNATE_PUSHPULL

/*DEBUG CONFIG*/
// #define DEBUG_MATRIX_SCAN_RATE

#define MOUSEKEY_INTERVAL 5

#define WEAR_LEVELING_LOGICAL_SIZE 8192
#define WEAR_LEVELING_BACKING_SIZE (WEAR_LEVELING_LOGICAL_SIZE * 2)


/*DEBOUNCE TIME ms, default 5*/
#define DEBOUNCE 4

#define RGBLIGHT_LAYERS
#define RGBLIGHT_MAX_LAYERS  20

//#define WS2812_EXTERNAL_PULLUP

// #define RGBLIGHT_DISABLE_KEYCODES   //代码函数控制功能
//#define RGB_MATRIX_DEFAULT_VAL 0      //指示灯初始值 0
// #define RGBLIGHT_DEFAULT_MODE  RGBLIGHT_MODE_STATIC_LIGHT
//#define RGBLIGHT_DEFAULT_MODE RGBLIGHT_EFFECT_BREATHING+1
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 220
#define RGBLIGHT_DEFAULT_MODE   RGBLIGHT_MODE_RAINBOW_MOOD+2

//#define RGBLIGHT_HUE_STEP 32      //色调
//#define RGBLIGHT_VAL_STEP 32      //亮度  //步进值，比如说灯，按下fn + ↑ 一次增加32的亮度

#define RGB_MATRIX_HUE_STEP 10
#define RGB_MATRIX_SAT_STEP 10

#define RGB_MATRIX_VAL_STEP 56

#define ENCODER_DEFAULT_POS 0x3

// //20240301
// #define RGB_MATRIX_DEFAULT_MODE (RGB_MATRIX_CYCLE_OUT_IN - 1)
// #define RGB_MATRIX_DEFAULT_VAL      87
#define RGB_MATRIX_DEFAULT_SPD     128
#define RGB_MATRIX_SPD_STEP 64

// #define RGBLIGHT_DEFAULT_MODE 3

#define RGBLIGHT_LIMIT_VAL     220
#define RGBLIGHT_DEFAULT_VAL   220
#define RGBLIGHT_VAL_STEP      56
