// Copyright 2026 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

enum custom_keycodes {
#ifdef VIA_ENABLE
    KC_USB = QK_KB_0,
#else
    KC_USB = SAFE_RANGE,
#endif
    KC_BLE1,
    KC_BLE2,
    KC_BLE3,
    KC_24G,
    KC_BAT,
    KC_RESET,
    KC_WIN,
    KC_WINMODE,
    KC_MACMODE,
    KC_SHUTRGB,
    KC_RRWIN,
    KC_MODEPLUS,
    KC_Mctl,
    KC_Lpad,
    KC_HUEPLS,
    ENC_TG,
    KC_VAL_UP,
    KC_VAL_DN,
    KC_SPD_UP,
    KC_SPD_DN,
    KC_LOCK,
    KC_OLED_HM,
    KC_OLED_IMG,
    KC_OLED_GIF,
    KC_SHUT_SC
};

#define LGUI_TAB LGUI(KC_TAB)
#define LCTL_UP LCTL(KC_UP)
#define KC_LPAD_ KC_Lpad

enum kb_mode_c {
    Kb_Usb,
    Kb_Bluetooth,
    Kb_Wireless
};

#define BLE_PIN C14
#define WIRELESS_PIN C15
#define RENUM_PIN A8
#define RGB_PIN B7
#define LCD_PIN C9
#define PLUGIN_PIN B9
