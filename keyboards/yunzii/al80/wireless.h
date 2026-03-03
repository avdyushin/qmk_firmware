// Copyright 2026 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include QMK_KEYBOARD_H

void wireless_start(uint32_t mode);
void wireless_stop(void);
void wireless_battery(uint8_t level);
