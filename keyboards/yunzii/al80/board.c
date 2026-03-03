// Copyright 2026 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

void bootloader_jump(void) {
    BKP->DR1 = RTC_BOOTLOADER_JUST_UPLOADED;//0x5343;
    NVIC_SystemReset();
}

void mcu_reset(void) {
    NVIC_SystemReset();
}
