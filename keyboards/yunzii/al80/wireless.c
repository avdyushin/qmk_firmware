// Copyright 2026 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "wireless.h"
#include "uart.h"

static uint8_t sc_ble_leds(void) {
    return 0;
}

static void sc_ble_mouse(report_mouse_t *report) {
    static uint8_t last_report[sizeof(report_mouse_t)] = {0};

    if (!has_mouse_report_changed((report_mouse_t *)last_report, report)) return;
    memcpy(last_report, report, sizeof(report_mouse_t));
    uart_write(0x55);
    uart_write(sizeof(report_mouse_t));
    uart_transmit(last_report, sizeof(report_mouse_t));
}

static void sc_ble_extra(report_extra_t *report) {
    uart_write(0x55);
    uart_write(sizeof(report_extra_t));
    uart_transmit((uint8_t *)report, sizeof(report_extra_t));
}

static void sc_ble_keyboard(report_keyboard_t *report) {
    uart_write(0x55);
    uart_write(0x09);
    uart_write(0x01);
    uart_transmit((uint8_t *)report, KEYBOARD_REPORT_SIZE);
}

static void sc_send_nkro(report_nkro_t *report) {
    uart_write(0x55);
    uart_write(0x12);
    uart_transmit((uint8_t *)report, 0x12);
}

static host_driver_t *last_host_driver = NULL;
static host_driver_t  sc_ble_driver    = {sc_ble_leds, sc_ble_keyboard, sc_send_nkro, sc_ble_mouse, sc_ble_extra};

void startup(void) {
    if (host_get_driver() == &sc_ble_driver) {
        return;
    }
    clear_keyboard();
    last_host_driver = host_get_driver();
    host_set_driver(&sc_ble_driver);
}

void disconnect(void) {
    if (host_get_driver() != &sc_ble_driver) {
        return;
    }
    clear_keyboard();
    host_set_driver(last_host_driver);
}

void wakeup(int wait) {
    for (int i = 0; i < 60; ++i) {
        uart_write(0x00);
    }
    wait_ms(wait);
}

void wireless_start(uint32_t mode) {
    uint8_t ble_command[24];
    startup();
    wakeup(350);
    if (mode < 1 || mode > 4) {
        mode = 1;
    }
    ble_command[0] = 0x55;
    ble_command[1] = 20;
    ble_command[2] = 0;
    ble_command[3] = mode;
    strcpy((char *restrict)(ble_command + 4), PRODUCT);
    ble_command[strlen(PRODUCT) + 4] = '-';
    ble_command[strlen(PRODUCT) + 5] = '0' + mode;
    ble_command[strlen(PRODUCT) + 6] = 0;
    uart_transmit(ble_command, sizeof(ble_command));
}

void wireless_stop(void) {
    uint8_t ble_command[4];
    wakeup(100);
    disconnect();
    wait_ms(20);
    ble_command[0] = 0x55;
    ble_command[1] = 2;
    ble_command[2] = 0;
    ble_command[3] = 0;
    uart_transmit(ble_command, sizeof(ble_command));
}

void wireless_battery(uint8_t level) {
    uint8_t ble_command[4];
    ble_command[0] = 0x55;
    ble_command[1] = 2;
    ble_command[2] = 0x09;
    ble_command[3] = level;
    uart_transmit(ble_command, sizeof(ble_command));
    wait_ms(20);
}
