# QUANTUM_LIB_SRC += uart.c
UART_DRIVER_REQUIRED = yes
ANALOG_DRIVER_REQUIRED = yes
SPI_DRIVER_REQUIRED = yes
MCU_LDSCRIPT = STM32F103xB
DEFERRED_EXEC_ENABLE = yes
# ENCODER_ENABLE = yes

# Enter lower-power sleep mode when on the ChibiOS idle thread
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
#OLED_ENABLE = yes
#OLED_TRANSPORT = spi

SRC +=  common.c \
		adc.c \
		aw20216s.c\
		rtc.c\
		uart_mod.c\
		keyboard_screen.c\
		smart_ble.c\
		rgblight_custom.c\
		smart.c\
#		sc_ws2812.c

DEBOUNCE_TYPE = sym_defer_g
