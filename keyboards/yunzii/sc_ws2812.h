/*
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

#pragma once

#include "quantum/color.h"

/*
 * The WS2812 datasheets define T1H 900ns, T0H 350ns, T1L 350ns, T0L 900ns. Hence, by default, these
 * are chosen to be conservative and avoid problems rather than for maximum throughput; in the code,
 * this is done by default using a WS2812_TIMING parameter that accounts for the whole window (1250ns)
 * and defining T1H and T0H; T1L and T0L are obtained by subtracting their low counterparts from the window.
 *
 * However, there are certain "WS2812"-like LEDs, like the SK6812s, which work in a similar
 * communication topology but use different timings for the window and the T1L, T1H, T0L and T0H.
 * This means that, albeit the same driver being applicable, the timings must be adapted.
 */


void sc_ws2812_init(void);
void sc_ws2812_set_color_all(uint8_t red, uint8_t green, uint8_t blue);

/* User Interface
 *
 * Input:
 *         ledarray:           An array of GRB data describing the LED colors
 *         number_of_leds:     The number of LEDs to write
 *
 * The functions will perform the following actions:
 *         - Set the data-out pin as output
 *         - Send out the LED data
 *         - Wait 50us to reset the LEDs
 */
void sc_ws2812_setleds(rgb_led_t *ledarray, uint16_t number_of_leds);
