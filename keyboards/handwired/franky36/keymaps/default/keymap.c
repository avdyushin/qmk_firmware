/* Copyright 2024-2025 Grigory Avdyushin
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

#include QMK_KEYBOARD_H

enum layers {
    _BASE = 0,
    _LOWER,
    _RAISE,
    _NAV,
};

#define LOWER          MO(_LOWER)
#define RAISE          MO(_RAISE)
#define NAV            MO(_NAV)

#define OSM_LSFT       OSM(MOD_LSFT)       // One Shot Right Shift

#define KC_SFT_Z       SFT_T(KC_Z)         // Left Shift when held, Z when tapped
#define KC_SFT_SL      RSFT_T(KC_SLSH)     // Right Shift when held, / when tapped

#define KC_SFT_BSLS    RSFT_T(KC_BSLS)     // Right Shift when held, \ when tapped

#define KC_LWR_SPC     LT(_LOWER, KC_SPC)  // Lower layer when held, Space when tapped
#define KC_RSE_BSPC    LT(_RAISE, KC_BSPC) // Raise layer when held, Backspace when tapped
#define KC_NAV_A       LT(_NAV,KC_A)       // Navigation layer when held, A when tapped

#define KC_CMD_TAB     CMD_T(KC_TAB)       // Left Command when held, Tab when tapped
#define KC_CMD_ENT     RCMD_T(KC_ENT)      // Right Command when held, Enter when tapped

#define KC_CTL_ESC     CTL_T(KC_ESC)       // Left Control when held, Escape when tapped
#define KC_OPT_OSM_SFT ROPT_T(OSM_LSFT)    // Right Option when held, One Shot Shift when tapped

const uint16_t PROGMEM we_combo[] = {KC_W, KC_E, COMBO_END};
const uint16_t PROGMEM sd_combo[] = {KC_S, KC_D, COMBO_END};
const uint16_t PROGMEM io_combo[] = {KC_I, KC_O, COMBO_END};
const uint16_t PROGMEM kl_combo[] = {KC_K, KC_L, COMBO_END};

combo_t key_combos[] = {
    COMBO(we_combo, KC_TAB),
    COMBO(sd_combo, KC_ESC),
    COMBO(io_combo, KC_BSPC),
    COMBO(kl_combo, KC_ENT),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┐         ┌───┬───┬───┬───┬───┐
     * │ Q │ W │ E │ R │ T │         │ Y │ U │ I │ O │ P │
     * ├───┼───┼───┼───┼───┤         ├───┼───┼───┼───┼───┤
     * │ A │ S │ D │ F │ G │         │ H │ J │ K │ L │ ;:│
     * ├───┼───┼───┼───┼───┤         ├───┼───┼───┼───┼───┤
     * │⇧/Z│ X │ C │ V │ B │         │ N │ M │ ,<│ .>│⇧/?│
     * └───┴───┴───┴───┴───┘         └───┴───┴───┴───┴───┘
     *             ┌───┬───┬───┐ ┌───┬───┬───┐
     *             │CTL│CMD│SPC│ │ENT│CMD│OPT│
     *             └───┴───┴───┘ └───┴───┴───┘
     */
    [_BASE] = LAYOUT_split_3x5_3(
        KC_Q,     KC_W,    KC_E,       KC_R,       KC_T,       KC_Y,        KC_U,       KC_I,    KC_O,    KC_P,
        KC_NAV_A, KC_S,    KC_D,       KC_F,       KC_G,       KC_H,        KC_J,       KC_K,    KC_L,    KC_SCLN,
        KC_SFT_Z, KC_X,    KC_C,       KC_V,       KC_B,       KC_N,        KC_M,       KC_COMM, KC_DOT,  KC_SFT_SL,
        _______,  _______, KC_CTL_ESC, KC_CMD_TAB, KC_LWR_SPC, KC_RSE_BSPC, KC_CMD_ENT, KC_ROPT, _______, _______
    ),

    /*
     * ┌───┬───┬───┬───┬───┐         ┌───┬───┬───┬───┬───┐
     * │ 1!│ 2@│ 3#│ 4$│ 5%│         │ 6^│ 7&│ 8*│ 9(│ 0)│
     * ├───┼───┼───┼───┼───┤         ├───┼───┼───┼───┼───┤
     * │ `~│   │   │   │   │         │ ← │ ↓ │ ↑ │ → │ '"│
     * ├───┼───┼───┼───┼───┤         ├───┼───┼───┼───┼───┤
     * │ ⇧ │   │   │   │   │         │ -_│ =+│ [{│ ]}│ \|│
     * └───┴───┴───┴───┴───┘         └───┴───┴───┴───┴───┘
     *             ┌───┬───┬───┐ ┌───┬───┬───┐
     *             │   │   │   │ │   │   │   │
     *             └───┴───┴───┘ └───┴───┴───┘
     */
    [_LOWER] = LAYOUT_split_3x5_3(
        KC_1,     KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,     KC_0,
        KC_GRV,   _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, KC_QUOT,
        KC_LSFT,  _______, _______, _______, _______, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC,  KC_SFT_BSLS,
        _______,  _______, _______, _______, _______, _______, _______, _______, _______,  _______
    ),

    /*
     * ┌───┬───┬───┬───┬───┐         ┌───┬───┬───┬───┬───┐
     * │ F1│ F2│ F3│ F4│ F5│         │ F6│ F7│ F8│ F9│F10│
     * ├───┼───┼───┼───┼───┤         ├───┼───┼───┼───┼───┤
     * │   │   │   │   │   │         │   │   │   │   │   │
     * ├───┼───┼───┼───┼───┤         ├───┼───┼───┼───┼───┤
     * │   │   │   │   │   │         │   │   │   │   │   │
     * └───┴───┴───┴───┴───┘         └───┴───┴───┴───┴───┘
     *             ┌───┬───┬───┐ ┌───┬───┬───┐
     *             │   │   │   │ │   │   │   │
     *             └───┴───┴───┘ └───┴───┴───┘
     */
    [_RAISE] = LAYOUT_split_3x5_3(
        KC_F1,    KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,
        _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

    /*
     * ┌───┬───┬───┬───┬───┐         ┌───┬───┬───┬───┬───┐
     * │   │   │   │   │   │         │HOM│   │   │END│   │
     * ├───┼───┼───┼───┼───┤         ├───┼───┼───┼───┼───┤
     * │   │   │   │   │   │         │ ← │ ↓ │ ↑ │ → │   │
     * ├───┼───┼───┼───┼───┤         ├───┼───┼───┼───┼───┤
     * │   │   │   │   │   │         │PUP│   │   │PDN│   │
     * └───┴───┴───┴───┴───┘         └───┴───┴───┴───┴───┘
     *             ┌───┬───┬───┐ ┌───┬───┬───┐
     *             │   │   │   │ │   │   │   │
     *             └───┴───┴───┘ └───┴───┴───┘
     */
    [_NAV] = LAYOUT_split_3x5_3(
        _______,  _______, _______, _______, _______, KC_HOME, _______, _______, KC_END,   _______,
        _______,  _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, _______,
        _______,  _______, _______, _______, _______, KC_PGUP, _______, _______, KC_PGDN,  _______,
        _______,  _______, _______, _______, _______, _______, _______, _______, _______,  _______
    )
};

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0x00, 0x0A, 0x0A
    };

    oled_write_P(qmk_logo, false);
}

static void render_mod_status(uint8_t modifiers) {
    oled_write_P(PSTR("MODS:"), false);
    oled_write_P(PSTR("S"), (modifiers & MOD_MASK_SHIFT));
    oled_write_P(PSTR("C"), (modifiers & MOD_MASK_CTRL));
    oled_write_P(PSTR("A"), (modifiers & MOD_MASK_ALT));
    oled_write_ln_P(PSTR("G"), (modifiers & MOD_MASK_GUI));
    oled_write_ln_P(PSTR(" "), false);
}

static void render_layer_state(void) {
    oled_write_ln_P(PSTR(" "), false);
    oled_write_P("BASE ", layer_state_is(_BASE));
    oled_write_P("LOWER", layer_state_is(_LOWER));
    oled_write_P("RAISE", layer_state_is(_RAISE));
    oled_write_P("NAV  ", layer_state_is(_NAV));
    oled_write_ln_P(PSTR(" "), false);
}

static void render_capsword_state(bool on) {
    oled_write_ln_P("CAPSW", on);
}

bool oled_task_user(void) {
    render_logo();
    render_layer_state();
    render_mod_status(get_mods() | get_oneshot_mods());
    render_capsword_state(is_caps_word_on());
    return false;
}
#endif
