// Copyright 2026 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [2] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [3] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
};
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Win
    [0] = LAYOUT(
        KC_ESC,   KC_F1,      KC_F2,    KC_F3,     KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,       KC_F9,        KC_F10,       KC_F11,   KC_F12,   KC_DELETE,   KC_MUTE,
        KC_GRV,   KC_1,       KC_2,     KC_3,      KC_4,     KC_5,     KC_6,     KC_7,     KC_8,        KC_9,         KC_0,         KC_MINS,  KC_EQL,   KC_BSPC,     KC_PAGE_UP,
        KC_TAB,   KC_Q,       KC_W,     KC_E,      KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,        KC_O,         KC_P,         KC_LBRC,  KC_RBRC,  KC_BSLS,     KC_PAGE_DOWN,
        KC_ESC,   KC_A,       KC_S,     KC_D,      KC_F,     KC_G,     KC_H,     KC_J,     KC_K,        KC_L,         KC_SCLN,      KC_QUOT,  KC_ENT,
        KC_LSFT,  KC_Z,       KC_X,     KC_C,      KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,     KC_DOT,       KC_SLSH,      KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LGUI,    KC_LALT,                       KC_SPC,             MO(1),    KC_RCTL,                                 KC_LEFT,  KC_DOWN,  KC_RIGHT
    ),
    [1] = LAYOUT(
        _______,  KC_BRID,    KC_BRIU,  LGUI_TAB,  KC_MYCM,  KC_MAIL,  KC_WHOM,  KC_MPRV,  KC_MPLY,     KC_MNXT,      KC_MUTE,      KC_VOLD,  KC_VOLU,  KC_SHUT_SC,  KC_MUTE,
        _______,  KC_BLE1,    KC_BLE2,  KC_BLE3,   KC_24G,   _______,  _______,  _______,  KC_OLED_HM,  KC_OLED_IMG,  KC_OLED_GIF,  _______,  _______,  KC_SHUTRGB,  KC_VAL_UP,
        _______,  _______,    _______,  _______,   _______,  _______,  _______,  _______,  _______,     _______,      _______,      _______,  _______,  RM_NEXT,     KC_VAL_DN,
        _______,  KC_MACMODE, _______,  _______,   _______,  _______,  _______,  _______,  _______,     _______,      _______,      _______,  RM_HUEU,
        _______,  _______,    _______,  _______,   _______,  _______,  _______,  _______,  _______,     _______,      _______,      _______,  RM_VALU,
        _______,  KC_WIN,     _______,                      KC_RESET,            _______,  KC_BAT,                                  RM_SPDD,  RM_VALD,  RM_SPDU
    ),
    // Mac
    [2] = LAYOUT(
        KC_ESC,   KC_F1,      KC_F2,    KC_F3,     KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,       KC_F9,        KC_F10,       KC_F11,   KC_F12,   KC_DELETE,   KC_MUTE,
        KC_GRV,   KC_1,       KC_2,     KC_3,      KC_4,     KC_5,     KC_6,     KC_7,     KC_8,        KC_9,         KC_0,         KC_MINS,  KC_EQL,   KC_BSPC,     KC_PAGE_UP,
        KC_TAB,   KC_Q,       KC_W,     KC_E,      KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,        KC_O,         KC_P,         KC_LBRC,  KC_RBRC,  KC_BSLS,     KC_PAGE_DOWN,
        KC_ESC,   KC_A,       KC_S,     KC_D,      KC_F,     KC_G,     KC_H,     KC_J,     KC_K,        KC_L,         KC_SCLN,      KC_QUOT,  KC_ENT,
        KC_LSFT,  KC_Z,       KC_X,     KC_C,      KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,     KC_DOT,       KC_SLSH,      KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LALT,    KC_LGUI,                       KC_SPC,             MO(3),    KC_RCTL,                                 KC_LEFT,  KC_DOWN,  KC_RIGHT
    ),
    [3] = LAYOUT(
        QK_BOOT,  KC_BRID,    KC_BRIU,  LCTL_UP,   KC_LPAD_, _______,  _______,  KC_MPRV,  KC_MPLY,     KC_MNXT,      KC_MUTE,      KC_VOLD,  KC_VOLU,  KC_SHUT_SC,  KC_MUTE,
        _______,  KC_BLE1,    KC_BLE2,  KC_BLE3,   KC_24G,   _______,  _______,  _______,  KC_OLED_HM,  KC_OLED_IMG,  KC_OLED_GIF,  _______,  _______,  KC_SHUTRGB,  KC_VAL_UP,
        _______,  _______,    _______,  _______,   _______,  _______,  _______,  _______,  _______,     _______,      _______,      _______,  _______,  RM_NEXT,     KC_VAL_DN,
        _______,  _______, KC_WINMODE,  _______,   _______,  _______,  _______,  _______,  _______,     _______,      _______,      _______,  RM_HUEU,
        _______,  _______,    _______,  _______,   _______,  _______,  _______,  _______,  _______,     _______,      _______,      _______,  RM_VALU,
        _______,  _______,    _______,                      KC_RESET,            _______,  KC_BAT,                                  RM_SPDD,  RM_VALD,  RM_SPDU
    ),
};
