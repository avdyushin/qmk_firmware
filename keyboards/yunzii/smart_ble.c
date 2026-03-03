/* Copyright 2023 Jacky Jia www.i-chip.cn
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
/*
    Copyright (C) 2020 Yaotian Feng, Codetector<codetector@codetector.cn>
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


#include "uart.h"
#include "ch.h"
#include "hal.h"
#include "host.h"
#include "host_driver.h"
#include "report.h"
#include "smart_ble.h"
#include "print.h"

#include "quantum.h"

#include "smart_ble.h"
#include "keyboard.h"
#include "analog.h"

#include "usb_main.h"
#include "usb_util.h"
#include "usb_driver.h"
#include "adc.h"
#include "al80/al80.h"
#include "os_detection.h"
#include "raw_hid.h"


/**********

已改：

----- Static Function Prototypes -----
static uint8_t ap2_ble_leds(void);
static void    ap2_ble_mouse(report_mouse_t *report);
static void    ap2_ble_extra(report_extra_t *report);
static void    ap2_ble_keyboard(report_keyboard_t *report);

static void ap2_ble_swtich_ble_driver(void);

-------------------- Static Local Variables ----------------
static host_driver_t ap2_ble_driver = {
    ap2_ble_leds, ap2_ble_keyboard, NULL, ap2_ble_mouse, ap2_ble_extra
};
14:07
BLE的驱动定义变了
14:07

//改定义在C:\qmk_20240305\tmk_core\protocol\host_driver.h
typedef struct {
    uint8_t (*keyboard_leds)(void);
    void (*send_keyboard)(report_keyboard_t *);
    void (*send_nkro)(report_nkro_t *);
    void (*send_mouse)(report_mouse_t *);
    void (*send_extra)(report_extra_t *);
} host_driver_t;


 ***********/

#define WIRELESS_MODULE_WAKE_UP_BYTES_NUM 60
#define WIRELESS_MODULE_SLEEP_TIME 30000
extern uint8_t  lock_keyboard;
uint8_t ble_led_state = 0;
bool wireless_connected=false;
bool suspend = false;

uint8_t last_wireless_mode=0;

uint8_t ble_command[22]= {0x55, 0x20, 0x00, 0x00, 'S', 'm','a','r','t','B','L','E',0,0,0,0,0,0,0,0,0,0};
const char ble_name[15]= {'Y', 'U','N', 'Z', 'I','I',' ', 'A','L','8','0',' ','B','T','\0'};
uint32_t send_packetimer;
extern uint8_t sleep_first_flag;
extern uint32_t keyborad_Idtimer;
extern uint8_t keyboard_Idle;
extern enum kb_mode_t kb_mode;
uint8_t packet_send = 0;



static  uint32_t keyboard_test_time;
static uint32_t time_test1;
 uint32_t rgb_wireless_timer;
 uint32_t first_sleep_timer;
/* -------------------- Static Function Prototypes -------------------------- */
static uint8_t ap2_ble_leds(void);
static void    ap2_ble_mouse(report_mouse_t *report);
static void    ap2_ble_extra(report_extra_t *report);
static void    ap2_ble_keyboard(report_keyboard_t *report);
static void    ap2_send_nkro(report_nkro_t *report);

static void ap2_ble_swtich_ble_driver(void);

/* -------------------- Static Local Variables ------------------------------ */
static host_driver_t ap2_ble_driver = {
    ap2_ble_leds, ap2_ble_keyboard,ap2_send_nkro,ap2_ble_mouse, ap2_ble_extra
};//注册回调函数


static host_driver_t *last_host_driver = NULL;

/* -------------------- Public Function Implementation ---------------------- */

void smart_ble_wakeup(void) {
    uint32_t mode_flag = 0; // Set initial value
    if (sleep_first_flag) {
        sleep_first_flag = false;
        for (int i = 0; i < WIRELESS_MODULE_WAKE_UP_BYTES_NUM; i++) {
            uart_write(0x00);
        }
        wait_ms(10);

        if (kb_mode == KB_MODE_BLE) {
            mode_flag = last_wireless_mode;
        }
        if (kb_mode == KB_MODE_24G) {
            mode_flag = 4;
        }

        ble_command[0] = 0x55;                               // sync
        ble_command[1] = 20;                                 // len
        ble_command[2] = 0;                                  // ble command
        ble_command[3] = mode_flag;                               // ble mode 0~4 usb ble 1 2 3 & 24g
        strcpy((char *restrict)(ble_command + 4), ble_name); // 4  5
        // ble_command[18]='-';   //6
        ble_command[18] = '0' + mode_flag; // 7
        ble_command[19] = 0;          // 8
        uart_transmit(ble_command, sizeof(ble_command));
        wait_ms(5);
        uart_transmit(ble_command, sizeof(ble_command));
        wait_ms(5);
    }
}

void smart_ble_startup(void) {
    ap2_ble_swtich_ble_driver();
}

void smart_ble_broadcast(uint8_t port) {

}

void smart_ble_connect(uint8_t port) {

}

void smart_ble_disconnect(void) {
    /* Skip if the driver is already enabled */
    if (host_get_driver() != &ap2_ble_driver) {
        return;
    }

    clear_keyboard();
    host_set_driver(last_host_driver);
}


void WIRELESS_PAIR(uint32_t mode)
{


    smart_ble_startup();
    wireless_connected=false;

    if(mode<1 || mode>4)
    {
        mode=1;
    }

    if((mode>=1) && (mode<=4)) {                      // remember last wireless mode    切换到蓝牙可以记住上次蓝牙的模式
        if (mode==4)
        {
            // uint32_t temp = eeconfig_read_kb();
            // eeconfig_update_kb(temp|4);
            uint32_t temp;
            eeconfig_read_kb_datablock(&variable_data, 0, EECONFIG_KB_DATA_SIZE);
            temp                                      = variable_data.eeconfig_last_wireless_mode;
            variable_data.eeconfig_last_wireless_mode = temp | 4;
            eeconfig_update_kb_datablock(&variable_data, 0, EECONFIG_KB_DATA_SIZE);
            last_wireless_mode=temp|4;
        }
        else
        {
            variable_data.eeconfig_last_wireless_mode = mode;
            eeconfig_update_kb_datablock(&variable_data, 0, EECONFIG_KB_DATA_SIZE);
            // eeconfig_update_kb(mode);
            last_wireless_mode=mode;
        }


    //  eeconfig_update_kb(mode);
    // last_wireless_mode=mode;
    // eeconfig_update_kb(mode);
        for (int i=0;i<WIRELESS_MODULE_WAKE_UP_BYTES_NUM;i++)
        {
            uart_write(0x00);
        }
        wait_ms(350);  //make sure module power on
    }




    uart_write(0x55);
    uart_write(0x03);
    uart_write(0);  //battery level id
    uart_write(mode);
    uart_write(0x01);
    wait_ms(10);
    uart_write(0x55);
    uart_write(0x03);
    uart_write(0);  //battery level id
    uart_write(mode);
    uart_write(0x01);
    wait_ms(10);
}


void WIRELESS_START(uint32_t mode)
{
            smart_ble_startup();
            wireless_connected=false;
            if(mode<1 || mode>4)
            {
                mode=1;
            }
            // last_wireless_mode=mode;
            // eeconfig_update_kb(mode);
            if((mode>=1) && (mode<=4)) {                      // remember last wireless mode    切换到蓝牙可以记住上次蓝牙的模式
                if (mode==4)
                {
                   // uint32_t temp = eeconfig_read_kb();
                    // eeconfig_update_kb(temp|4);
                    uint32_t temp;
                    eeconfig_read_kb_datablock(&variable_data, 0, EECONFIG_KB_DATA_SIZE);
                    temp                                      = variable_data.eeconfig_last_wireless_mode;
                    variable_data.eeconfig_last_wireless_mode = temp | 4;
                    eeconfig_update_kb_datablock(&variable_data, 0, EECONFIG_KB_DATA_SIZE);
                    last_wireless_mode=temp|4;
                }
                else
                {
                    variable_data.eeconfig_last_wireless_mode = mode;
                    eeconfig_update_kb_datablock(&variable_data, 0, EECONFIG_KB_DATA_SIZE);
                    // eeconfig_update_kb(mode);
                    last_wireless_mode=mode;
                }
            }
            for (int i=0;i<WIRELESS_MODULE_WAKE_UP_BYTES_NUM;i++)
            {
                uart_write(0x00);
            }
            wait_ms(350);  //make sure module power on


            ble_command[0]=0x55;  //sync
            ble_command[1]=20;   //len
            ble_command[2]=0;       //ble command
            ble_command[3]=mode;    //ble mode 0~4 usb ble 1 2 3 & 24g
            strcpy((char * restrict)(ble_command+4),ble_name);  //4  5
            // ble_command[18]='-';   //6
            ble_command[18]='0'+mode;   //7
            ble_command[19]=0;     //8
            uart_transmit(ble_command,sizeof(ble_command));
            wait_ms(10);
            uart_transmit(ble_command,sizeof(ble_command));
            wait_ms(10);
}



void WIRELESS_STOP(void)
{
           //uprintf("%ld",last_wireless_mode);
            // for (int i=0;i<WIRELESS_MODULE_WAKE_UP_BYTES_NUM;i++)
            // {
            //     uart_write(0x00);
            // }
              //make sure module power on
           wireless_connected=false;
            for (int i=0;i<WIRELESS_MODULE_WAKE_UP_BYTES_NUM;i++)
            {
                uart_write(0x00);
            }
            wait_ms(100);
            smart_ble_disconnect();
            wait_ms(20);
            ble_command[0]=0x55;  //sync
            ble_command[1]=2;   //len
            ble_command[2]=0;       //ble command
            ble_command[3]=0;    //ble mode 0~4 usb ble 1 2 3 & 24g
            uart_transmit(ble_command,4);

}

void WIRELESS_STOPOWER(void)
{
           //uprintf("%ld",last_wireless_mode);
            // for (int i=0;i<WIRELESS_MODULE_WAKE_UP_BYTES_NUM;i++)
            // {
            //     uart_write(0x00);
            // }
              //make sure module power on
           wireless_connected=false;
            for (int i=0;i<WIRELESS_MODULE_WAKE_UP_BYTES_NUM;i++)
            {
                uart_write(0x00);
            }
            wait_ms(100);
           // smart_ble_disconnect();
            wait_ms(20);
            ble_command[0]=0x55;  //sync
            ble_command[1]=2;   //len
            ble_command[2]=0;       //ble command
            ble_command[3]=0;    //ble mode 0~4 usb ble 1 2 3 & 24g
            uart_transmit(ble_command,4);

}









/* ------------------- Static Function Implementation ----------------------- */
static void ap2_ble_swtich_ble_driver(void) {

    if (host_get_driver() == &ap2_ble_driver) {
        return;
    }
    clear_keyboard();
    last_host_driver = host_get_driver();
    ble_led_state = host_keyboard_leds();
    host_set_driver(&ap2_ble_driver);

}

void sc_ble_battary(uint8_t batt_level) {
    if(wireless_connected)
    {
       // smart_ble_wakeup();//从睡眠模式中唤醒
        uart_write(0x55);
        uart_write(0x02);
        uart_write(0x09);  //battery level id
        uart_write(batt_level);
        wait_ms(10);
    }
}

// uint8_t host_keyboard_leds(void) {
//     if (!driver) return 0;
//     return (*driver->keyboard_leds)();
// }

// led_t host_ble_led_state(void) {
//     return (led_t)host_keyboard_leds();
// }
 #ifdef Module_Updata
void Module_UpdataHandle(void)
{
    static enum {UART_READY, UART_0X55_RECEIVED, UART_LENS_REVEIVED,UART_Module, UART_REPORT_ID_RECEIVED} uart_state = UART_READY;
    static uint8_t 	uart_command[40];
    static uint8_t uart_lens =0;
    static uint8_t uart_buff_index=0;
    uint8_t c;
    uint8_t Pc_updata[64];
   //当前在usb模式下且在升级的过程中
   if( kb_mode ==KB_MODE_USB)
   {
        if (uart_available())
        {
            c= uart_read();     // 0x55 0x03 0x01 mode data  根据command判断
            switch (uart_state) {
                case UART_READY:
                    if (c==0x55)
                    {
                        uart_state=UART_0X55_RECEIVED;
                        uart_buff_index=0;
                        uart_command[uart_buff_index++]=c;
                    }
                    break;

                case UART_0X55_RECEIVED:
                    if (c ==0x55)
                        break;
                    else if((c >= 0x02) && (c <= 0x20))
                    {
                        uart_lens=c;
                        uart_command[uart_buff_index++]=c;
                        uart_state= UART_LENS_REVEIVED;
                    }
                    else
                        uart_state= UART_READY;
                    break;

                case UART_LENS_REVEIVED:
                        if(c == 0x0a)//模组升级指令
                        {
                            uart_command[uart_buff_index++]=c;
                            uart_state= UART_Module;
                        }
                    else
                        uart_state= UART_READY;
                    break;
    //question
                case UART_Module:    //这里逻辑异常，更改点20240226
                    uart_command[uart_buff_index++]=c;
                    if (uart_buff_index >= uart_lens + 2)//所有的数据存储完毕，开始补包   //20240226
                    {
                        memset(Pc_updata, 0, sizeof(Pc_updata));
                        for(uint8_t i = 0; i < uart_lens; i++)
                        {
                          Pc_updata[i] = uart_command[i+2];//拆包前两个字节

                        }
                        //剩余的字节全部用FF填充
                        for(uint8_t i = uart_lens; i < (64-uart_lens); i++)
                        {
                          Pc_updata[i] = 0xff;

                        }
                        raw_hid_send(Pc_updata, RAW_EPSIZE);
                        // raw_hid_send(Pc_updata, sizeof(Pc_updata));
                        uart_state= UART_READY;    //20240226 数据全部发送完成后状态切换到初始状态
                    }
                    break;
                default:
                    uart_state = UART_READY;
                    break;
            }


        }
   }


}
 #endif

static uint8_t ap2_ble_leds(void) {  //返回的就是灯的状态
static enum {UART_READY, UART_0X55_RECEIVED, UART_LENS_REVEIVED,UART_WORKMODE, UART_REPORT_ID_RECEIVED} uart_state = UART_READY;
static uint8_t 	uart_command[40];
static uint8_t uart_lens =0;

static uint8_t uart_workmode =0;
static uint8_t uart_buff_index=0;
uint8_t c;
    //uprintf("%lu %lu %lu %lu\r\n",timer_read32(),timer_elapsed32(0),timer_elapsed32(0x10000),timer_elapsed32(0x10000000));
    if (uart_available())
    {
        c= uart_read();     // 0x55 0x03 0x01 mode data  根据command判断
        //uart_write(c);
        switch (uart_state) {
            case UART_READY:
                if (c==0x55)
				{
					uart_state=UART_0X55_RECEIVED;
					uart_buff_index=0;
					uart_command[uart_buff_index++]=c;
				}
				break;

            case UART_0X55_RECEIVED:
				if (c ==0x55)
					break;
				else if (c == 0x03)  // UART_COMMAND_LENS)
				{
					uart_lens=c;
					uart_command[uart_buff_index++]=c;
					uart_state= UART_LENS_REVEIVED;
				} else
					uart_state= UART_READY;
				break;

   			case UART_LENS_REVEIVED:
				if  (c <= 2)    //UART_command
                    {

						uart_command[uart_buff_index++]=c;
						uart_state= UART_WORKMODE;
					}
                else
				    uart_state= UART_READY;

				break;

            case UART_WORKMODE:
				if  (c <= 4)    //UART_mode
                    {
                        uart_workmode = c;
						uart_command[uart_buff_index++]=c;
						uart_state= UART_REPORT_ID_RECEIVED;
					}
                else
				    uart_state= UART_READY;

				break;

            case UART_REPORT_ID_RECEIVED:

                   // uprintf("ENTER here\r\n");
                uart_command[uart_buff_index++]=c;
                if (uart_buff_index>=uart_lens+2)
                {
                    switch (uart_command[2])
                    {
                        case 0:


                            //获取的模式与当前的模式一致,2.4G和蓝牙分两种
                            if(last_wireless_mode <= 3)
                            {
                                 if(last_wireless_mode ==  uart_workmode)
                                 {
                                    wireless_connected=(c==0);
                                 }

                            }
                            else{
                                  if(uart_workmode == 4)
                                  {

                                    wireless_connected=(c==0);
                                  }

                            }

                            break;
                        case 1:
                                if(last_wireless_mode <= 3)
                                {
                                    if(last_wireless_mode ==  uart_workmode)
                                    {
                                            ble_led_state=c;
                                            //这里有没有打印数据
                                            // uprintf("caps press\r\n");

                                    }

                                }
                                else{
                                    if(uart_workmode == 4)
                                    {

                                           ble_led_state=c;
                                    }

                                }
                                break;
                        case 2:
                             if(last_wireless_mode > 3 && (uart_workmode == 4))
                             {
                                if (c==0xaa){
                                   suspend=true;

                                }
                                else if (c==0xbb)
                                {
                                    suspend=false;
                                }
                             }


                            break;
                        default:
                            break;
                    }

                    uart_state= UART_READY;
                }
				break;

            default:
                uart_state = UART_READY;
                break;
        }


    }

    return ble_led_state;
}

static void ap2_ble_mouse(report_mouse_t *report) {
    static uint8_t last_report[sizeof(report_mouse_t)]={0};
    keyborad_Idtimer   = timer_read32();
    keyboard_Idle = 1;

//  &&  (!lock_keyboard)
    if(wireless_connected  )
    {
        // if (!has_mouse_report_changed((report_mouse_t *)last_report, report)) return;
        if(memcmp(last_report,report,sizeof(report_mouse_t)))
        {
            smart_ble_wakeup();
            memcpy(last_report,report,sizeof(report_mouse_t));
            uart_write(0x55);
             uart_write(0x06);

            // uart_write(sizeof(report_mouse_t));
            uart_transmit(last_report,sizeof(report_mouse_t));
        }



          if(timer_elapsed32(keyboard_test_time) < 8  && (timer_elapsed32(keyboard_test_time) > 0))
          {
             time_test1 =  8-(timer_elapsed32(keyboard_test_time));

             //蓝牙下才延时
               if(kb_mode != KB_MODE_24G)
               wait_ms(time_test1);
                 else{
                wait_ms(2);
               }
               // uprintf("%ld\r\n",time_test1);

          }
          else{
              if(kb_mode != KB_MODE_24G)
              wait_ms(8);
              else{
                wait_ms(2);
               }
          //   uprintf("%d\r\n",222);
          }
          keyboard_test_time = timer_read32();

          //防止宏录制异常，更新心跳包计数以及一二级时间
         rgb_wireless_timer  = timer_read32();
         first_sleep_timer   = timer_read32();
         packet_send = 0;
    }




}


static void ap2_ble_extra(report_extra_t *report) {
    keyborad_Idtimer   = timer_read32();
    keyboard_Idle = 1;

// &&  (!lock_keyboard)
    if(wireless_connected )
    {
        smart_ble_wakeup();
        uart_write(0x55);
        uart_write(sizeof(report_extra_t));
        uart_transmit((uint8_t *)report,sizeof(report_extra_t));


          if(timer_elapsed32(keyboard_test_time) < 8  && (timer_elapsed32(keyboard_test_time) > 0))
          {
             time_test1 =  8-(timer_elapsed32(keyboard_test_time));

             //蓝牙下才延时
               if(kb_mode != KB_MODE_24G)
               wait_ms(time_test1);
                 else{
                wait_ms(2);
               }
               // uprintf("%ld\r\n",time_test1);

          }
          else{
              if(kb_mode != KB_MODE_24G)
              wait_ms(8);
              else{
                wait_ms(2);
               }
          //   uprintf("%d\r\n",222);
          }
          keyboard_test_time = timer_read32();

          //防止宏录制异常，更新心跳包计数以及一二级时间
         rgb_wireless_timer  = timer_read32();
         first_sleep_timer   = timer_read32();
         packet_send = 0;
    }

}

/*!
 * @brief  Send keyboard HID report for Bluetooth driver
 */


static void ap2_ble_keyboard(report_keyboard_t *report) {

    keyborad_Idtimer   = timer_read32();
    keyboard_Idle = 1;


    //   uprintf("keyboard_test_time:%ld\r\n",timer_elapsed32(keyboard_test_time));
//   &&  (!lock_keyboard)
    if(wireless_connected)
    {
        //
        smart_ble_wakeup();


        // if (keymap_config.nkro)
        // {
        //     uart_write(0x55);
        //     uart_write(0x12);  //nkro_report should be 0x20(32)
        //     uart_transmit((uint8_t *)report,0x12);
        // }
        // else
        {
            uart_write(0x55);
            uart_write(0x09);
            uart_write(0x01);
            uart_transmit((uint8_t *)report,KEYBOARD_REPORT_SIZE);
        }

//防止宏录制异常，更新心跳包计数以及一二级时间
         rgb_wireless_timer  = timer_read32();
         first_sleep_timer   = timer_read32();
         packet_send = 0;


          if(timer_elapsed32(keyboard_test_time) < 8  && (timer_elapsed32(keyboard_test_time) > 0))
          {
             time_test1 =  8-(timer_elapsed32(keyboard_test_time));

             //蓝牙下才延时
               if(kb_mode != KB_MODE_24G)
                wait_ms(time_test1);
               else{
                wait_ms(2);
               }
          }
          else{
              if(kb_mode != KB_MODE_24G)
              wait_ms(8);
              else{
                wait_ms(2);
               }
          }
          keyboard_test_time = timer_read32();
    }



}

// void special_key_handle(report_nkro_t *report)
// {
//     // uint8_t result;
//     //   if(report[19] & 0x08 )
//     //   {
//     //     uart_write(0x22);

//     //   }

// }
uint8_t   JP_flag;
report_keyboard_t *special_report;
static void ap2_send_nkro(report_nkro_t *report)
{

    keyborad_Idtimer   = timer_read32();
    keyboard_Idle = 1;


    //   uprintf("keyboard_test_time:%ld\r\n",timer_elapsed32(keyboard_test_time));
//  &&  (!lock_keyboard)
    if(wireless_connected )
    {
        smart_ble_wakeup();
        //  special_key_handle(report);

        // if (report->bits[17] & 0x08) {
        //     JP_flag = 1;
        //    // special_report->keys[7] = 0x8b;
        //     uart_write(0x55);
        //     uart_write(0x09);
        //     uart_write(0x01);

        //     uart_write(0x00);
        //     uart_write(0x00);
        //     uart_write(0x00);
        //     uart_write(0x00);
        //     uart_write(0x00);
        //     uart_write(0x00);
        //     uart_write(0x00);
        //     uart_write(0x8b);
        //     wait_ms(2);
        //    // uart_transmit((uint8_t *)special_report,KEYBOARD_REPORT_SIZE);

        // }
        // else if(JP_flag && ((report->bits[17] & 0x08) == 0))
        // {
        //       JP_flag = 0;
        //   //    special_report->keys[7] = 0;
        //       uart_write(0x55);
        //       uart_write(0x09);
        //       uart_write(0x01);
        //       uart_write(0x00);
        //       uart_write(0x00);
        //       uart_write(0x00);
        //       uart_write(0x00);
        //       uart_write(0x00);
        //       uart_write(0x00);
        //       uart_write(0x00);
        //       uart_write(0x00);
        //       wait_ms(2);
        //     //  uart_transmit((uint8_t *)special_report,KEYBOARD_REPORT_SIZE);
        // }

        //前六个按键转为按键


        uart_write(0x55);
        uart_write(0x12); // nkro_report should be 0x20(32)
        uart_transmit((uint8_t *)report, 0x12);

        // 防止宏录制异常，更新心跳包计数以及一二级时间
        rgb_wireless_timer = timer_read32();
        first_sleep_timer  = timer_read32();
        packet_send        = 0;

        if (timer_elapsed32(keyboard_test_time) < 8 && (timer_elapsed32(keyboard_test_time) > 0)) {
            time_test1 = 8 - (timer_elapsed32(keyboard_test_time));

            // 蓝牙下才延时
            if (kb_mode != KB_MODE_24G)
                wait_ms(time_test1);
               else{
                wait_ms(2);
               }
          }
          else{
              if(kb_mode != KB_MODE_24G)
              wait_ms(8);
              else{
                wait_ms(2);
               }
          }
          keyboard_test_time = timer_read32();
    }

}
