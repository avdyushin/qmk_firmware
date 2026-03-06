// Copyright 2026 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "al80.h"
#include "uart.h"
#include "wireless.h"

int get_mode(void) {
    if (!gpio_read_pin(BLE_PIN)) {
        return Kb_Bluetooth;
    } else if (!gpio_read_pin(WIRELESS_PIN)) {
        return Kb_Wireless;
    } else {
        return Kb_Usb;
    }
}

void keyboard_pre_init_kb(void) {
    AFIO->MAPR = (AFIO->MAPR & ~AFIO_MAPR_SWJ_CFG_Msk);
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_DISABLE; // disable JTAG (GD32 different to STM32)
    gpio_set_pin_output(RENUM_PIN);
    gpio_write_pin_high(RENUM_PIN); // Enable USB
    gpio_set_pin_output(RGB_PIN);
    gpio_write_pin_low(RGB_PIN);
    gpio_set_pin_output(LCD_PIN);
    gpio_write_pin_high(LCD_PIN);
    gpio_set_pin_input(PLUGIN_PIN);
    uart_init(921600);
    wait_ms(400);
}

void keyboard_post_init_kb(void) {
    int mode = get_mode();
    switch (mode) {
        case Kb_Bluetooth:
            wireless_start(1);
            break;
        case Kb_Wireless:
            wireless_start(4);
            break;
        case Kb_Usb:
            wireless_stop();
            break;
    }
}

deferred_token task_token = INVALID_DEFERRED_TOKEN;

uint32_t task_callback(uint32_t trigger_timer, void *cb_arg) {
    int mode = get_mode();
    uprintf("Mode %d\r\n", mode);
    return 1000;
}

void keyboard_post_init_user(void) {
    debug_enable = true;
    debug_matrix = true;

    task_token = defer_exec(1000, task_callback, NULL);
}

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    if (record->event.pressed) {
        if ((keycode >= KC_BLE1) && (keycode <= KC_24G)) {
            wireless_start(keycode - KC_USB);
            return false;
        } else if (keycode == KC_USB) {
            wireless_stop();
            return false;
        } else if (keycode == KC_WINMODE) {
            set_single_persistent_default_layer(0);
            return false;
        } else if (keycode == KC_MACMODE) {
            set_single_persistent_default_layer(2);
            return false;
        }
    }
    return process_record_user(keycode, record);
}
