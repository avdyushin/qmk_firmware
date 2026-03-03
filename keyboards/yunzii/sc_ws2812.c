#include "ws2812.h"
#include "sc_ws2812.h"
#include "gpio.h"
#include "chibios_config.h"
#include "quantum.h"
#include "uart.h"
#include "keyboard.h"
#include "print.h"
#include "os_detection.h"
#include "raw_hid.h"

/* Adapted from https://github.com/bigjosh/SimpleNeoPixelDemo/ */

#ifndef SC_NOP_FUDGE
#    if defined(STM32F0XX) || defined(STM32F1XX) || defined(GD32VF103) || defined(STM32F3XX) || defined(STM32F4XX) || defined(STM32L0XX) || defined(WB32F3G71xx) || defined(WB32FQ95xx)
#        define SC_NOP_FUDGE 0.4
#    elif defined(STM32WBXX)
#        define SC_NOP_FUDGE 0.5
#    else
#        error("NOP_FUDGE configuration required")
#        define SC_NOP_FUDGE 1 // this just pleases the compile so the above error is easier to spot
#    endif
#endif

// Push Pull or Open Drain Configuration
// Default Push Pull
#ifndef SC_WS2812_EXTERNAL_PULLUP
#    define SC_WS2812_OUTPUT_MODE PAL_MODE_OUTPUT_PUSHPULL
#else
#    define SC_WS2812_OUTPUT_MODE PAL_MODE_OUTPUT_OPENDRAIN
#endif

// The reset gap can be 6000 ns, but depending on the LED strip it may have to be increased
// to values like 600000 ns. If it is too small, the pixels will show nothing most of the time.
#ifndef SC_WS2812_RES
#    define SC_WS2812_RES (1000 * WS2812_TRST_US) // Width of the low gap between bits to cause a frame to latch
#endif

#define SC_NUMBER_NOPS 6
#define SC_CYCLES_PER_SEC (CPU_CLOCK / SC_NUMBER_NOPS * SC_NOP_FUDGE)
#define SC_NS_PER_SEC (1000000000L) // Note that this has to be SIGNED since we want to be able to check for negative values of derivatives
#define SC_NS_PER_CYCLE (SC_NS_PER_SEC / SC_CYCLES_PER_SEC)
#define SC_NS_TO_CYCLES(n) ((n) / SC_NS_PER_CYCLE)

#define sc_wait_ns(x)                                  \
    do {                                            \
        for (int i = 0; i < SC_NS_TO_CYCLES(x); i++) { \
            __asm__ volatile("nop\n\t"              \
                             "nop\n\t"              \
                             "nop\n\t"              \
                             "nop\n\t"              \
                             "nop\n\t"              \
                             "nop\n\t");            \
        }                                           \
    } while (0)

void sc_sendByte(uint8_t byte) {
    // WS2812 protocol wants most significant bits first
    for (unsigned char bit = 0; bit < 8; bit++) {
        bool is_one = byte & (1 << (7 - bit));
        // using something like wait_ns(is_one ? T1L : T0L) here throws off timings
        if (is_one) {
            // 1
            writePinHigh(SC_WS2812_DI_PIN);
            sc_wait_ns(WS2812_T1H);
            writePinLow(SC_WS2812_DI_PIN);
            sc_wait_ns(WS2812_T1L);
        } else {
            // 0
            writePinHigh(SC_WS2812_DI_PIN);
            sc_wait_ns(WS2812_T0H);
            writePinLow(SC_WS2812_DI_PIN);
            sc_wait_ns(WS2812_T0L);
        }
    }
}

void sc_ws2812_init(void) {
   //battery_test_time = timer_read32();
   // wait_ms(1500);
   palSetLineMode(SC_WS2812_DI_PIN, SC_WS2812_OUTPUT_MODE);
}

void sc_ws2812_set_color_all(uint8_t red, uint8_t green, uint8_t blue) {
    chSysLock();
    #if (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_GRB)
        sc_sendByte(green);
        sc_sendByte(red);
        sc_sendByte(blue);
    #elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_RGB)
        sc_sendByte(red);
        sc_sendByte(green);
        sc_sendByte(blue);
    #elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_BGR)
        sc_sendByte(blue);
        sc_sendByte(green);
        sc_sendByte(red);
    #endif

    #ifdef RGBW
        // sc_sendByte(white); // unsupported
    #endif
    sc_wait_ns(SC_WS2812_RES);
    chSysUnlock();
}

// Setleds for standard RGB
void sc_ws2812_setleds(rgb_led_t *ledarray, uint16_t leds) {
    static bool s_init = false;
    if (!s_init) {
        sc_ws2812_init();
        s_init = true;
    }
   // uprintf("%d\r\n",123);
    // this code is very time dependent, so we need to disable interrupts
    chSysLock();

    for (uint8_t i = 0; i < leds; i++) {
        // WS2812 protocol dictates grb order
#if (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_GRB)
        sc_sendByte(ledarray[i].g);
        sc_sendByte(ledarray[i].r);
        sc_sendByte(ledarray[i].b);
#elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_RGB)
        sc_sendByte(ledarray[i].r);
        sc_sendByte(ledarray[i].g);
        sc_sendByte(ledarray[i].b);
#elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_BGR)
        sc_sendByte(ledarray[i].b);
        sc_sendByte(ledarray[i].g);
        sc_sendByte(ledarray[i].r);
#endif

#ifdef RGBW
        sc_sendByte(ledarray[i].w);
#endif
    }

    sc_wait_ns(SC_WS2812_RES);

    chSysUnlock();
}
