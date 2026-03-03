/*
    Copyright (C) 2023 Jacky
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
        http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#pragma once
#include "quantum.h"

void sc_ble_battary(uint8_t batt_level);
void WIRELESS_START(uint32_t mode);
void WIRELESS_STOP(void);
void WIRELESS_STOPOWER(void);
void WIRELESS_PAIR(uint32_t mode);
void encode_boot(void);
void Module_UpdataHandle(void);




typedef struct  {
  uint8_t   eeconfig_last_wireless_mode;
  uint8_t   eeconfig_nkro_flag;
  uint8_t   eeconfig_encode_toggle;
  uint8_t   eeconfig_lock_win_flag;
  uint8_t   eeconfig_shut_up_Flag;
  uint8_t   eeconfig_shut_up_screen_Flag;
}MyVariables;

MyVariables  variable_data;






