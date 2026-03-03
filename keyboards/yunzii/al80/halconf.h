// Copyright 2026 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define HAL_USE_SPI TRUE
#define SPI_USE_WAIT TRUE
#define SPI_SELECT_MODE SPI_SELECT_MODE_PAD

#define HAL_USE_ADC TRUE

// This enables interrupt-driven mode
#define PAL_USE_WAIT TRUE
#define USB_USE_WAIT TRUE

#define HAL_USE_SERIAL TRUE

#define HAL_USE_PWM TRUE
#define HAL_USE_PAL TRUE

#include_next <halconf.h>
