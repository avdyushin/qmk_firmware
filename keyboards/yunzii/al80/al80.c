// // /* Copyright 2022 Jacky
// //  *
// //  * This program is free software: you can redistribute it and/or modify
// //  * it under the terms of the GNU General Public License as published by
// //  * the Free Software Foundation, either version 2 of the License, or
// //  * (at your option) any later version.
// //  *
// //  * This program is distributed in the hope that it will be useful,
// //  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// //  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// //  * GNU General Public License for more details.
// //  *
// //  * You should have received a copy of the GNU General Public License
// //  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
// //  */
#include "config.h"
#include "quantum.h"
#include "uart.h"
#include "smart_ble.h"
#include "keyboard.h"
#include "print.h"
#include "usb_main.h"
#include "usb_util.h"
#include "usb_driver.h"
#include "adc.h"
#include "al80.h"
#include "os_detection.h"
#include "raw_hid.h"
#include "action_util.h"
#include "common.h"
#include "rtc.h"
#include "uart.h"
#include "keyboard_screen.h"
#include "hygui.h"
#include "spi_master.h"
#include "chibios_config.h"

uint16_t key_nums_variable = 0;
extern matrix_row_t matrix[MATRIX_ROWS];
#ifdef RGB_MATRIX_ENABLE
#    include "rgb_matrix.h"
#endif
#ifdef RGBLIGHT_ENABLE
#    include "rgblight.h"
#endif

// // /************************默认图层定义************************/


// /****************函数声明************************/

uint8_t Sleep_First_PressFlag = 0;
extern matrix_row_t raw_matrix[MATRIX_ROWS]; // raw values
extern uint32_t exti_flag;
extern uint32_t pr1;
extern uint8_t                keyboard_led_state;
extern uint8_t ble_led_state ;
extern uint32_t send_packetimer;
extern uint8_t packet_send;
uint8_t temp_test_cout;
extern uint32_t test_isrsource;
extern uint8_t test_variable;
/****************变量定义************************/
//闪烁计时
static uint8_t blink_period = 5;
static uint8_t blink_counter  = 0;
static uint8_t wakeup_pack24G  = 0;
static uint8_t lowpower_flag                    = 1;
static uint8_t power_period   = 2;
static uint8_t power_counter  = 0;
uint8_t shut_up_Flag = 0 ;
static uint8_t ble24G_period   = 5;
static uint8_t BT_24G_Shine;
static uint8_t  ble_mode;
static uint8_t  reset_flag;
static uint8_t  current_layer;
uint8_t suspend_count = 0;
uint16_t Shine_Handle;
uint8_t Shut_Up_Screen = 0;
uint32_t Effect_Flag = 0;
// bool     sleep_24G_flag = false; // 2.4G进入休眠标志位
static uint8_t current_layer = 0;
bool     usb_wakeup_flag;
// static uint8_t old_effect;
// static uint8_t Default_Effect_Flag;
// static uint8_t  encode_toggle = 0;
// static uint8_t  encode_toggle_flag = 0;
// static uint8_t  encode_Press_flag = 0;
#ifdef Lock_Keyboard
uint8_t  lock_keyboard = 0;
#endif
// static uint32_t encode_toggle_timer          = 0;    //旋钮计时
#ifdef FN_Double_Knock
static uint32_t Doubele_Fn_timer          = 0;    //fn计时
static uint32_t First_timer          = 0;    //fn计时
uint8_t  FN_Count = 0;
static uint32_t Media_timer          = 0;    //fn计时
#endif
// static uint32_t Power_Full_timer          = 0;    //fn计时

uint8_t        sleep_shutled_flag       = 0;
// uint8_t  Power_Full_Flag = 0;
#ifdef Screen_AP
uint8_t	 time_50ms_count;
#endif
uint8_t  test_number = 0;
uint8_t  Media_Flag = 0;
 uint8_t Plug_In_Flag;
uint8_t lowpower_effect;
uint8_t chag_via_flag;
uint8_t chag_first_flag;
// static uint8_t Double_Fn_Flag = 0;
uint8_t lock_win_flag = 1;
static uint8_t Valorspd_counter  = 0;
static uint8_t Valorspd_period   = 3;
static uint8_t blink_num                    = 0;
//电池待更新标志
static uint8_t battery_Flag = 0;
//按键休眠 不分连接以解决切模式异常
 uint8_t keyboard_Idle = 0;
static uint8_t ble24G_counter  = 0;
static uint8_t temp = 0;
// static uint8_t temp_test = 0;
static uint8_t last_temp = 0 ;
static uint8_t second_temp = 0 ;
 uint16_t sleep_switch_driverflag = 0 ;
//充电灯闪烁
/*切换全键无冲变量*/
//static uint8_t BT_Switch_Flag = 1;
uint8_t isr_Trig;
uint8_t nkro_flag = 1;
uint8_t default_layer  = 1;
extern uint32_t rgb_wireless_timer;
extern uint32_t first_sleep_timer;
 static uint8_t Test_Battery_value;
static uint8_t connect_buff = 0;
bool ble_flag = 0;
static uint32_t ble_bat_timer;
uint8_t Light_Count;
static uint32_t long_ble_24g_timer          = 0;    //蓝牙2.4g长按计时
static uint32_t blink_ble_24g_timer         = 0;    //蓝牙2.4闪烁计时
uint32_t keyborad_Idtimer         = 0;    //按键空闲计时
uint32_t keyborad_Chrgtimer         = 0;    //充电计时
// uint32_t encoder_longpresstimer         = 0;    //充电计时
uint32_t test_twoled         = 0;    //按键空闲计时
static uint32_t pair_timeout                = 20000;//20000;    //配对超时时间
static uint32_t pair_succeed_timer          = 0;    //按键无线配对成功灯长亮时间
static uint32_t first_sleep_wakeup_timer          = 0;    //一级休眠起来的时间
static uint32_t long_press_reset_timer      = 0;    //记录长按复位键的时
static uint8_t release_count = 0;           //usb休眠唤醒添加释放包，防止hold键
static uint32_t usb_suspend_time;
// static uint8_t WinLayer_Flag;
// static uint8_t MacLayer_Flag;
static uint32_t usb_suspend_time;
uint8_t test_count  = 0;
/****************标志变量************************/
kb_flag32_t	flag32 = {0};
uint8_t usb_suspend_flag = 2;             //usb检测到suspend标志位
//adc检测变量
static uint16_t adc_value = 0;
static uint16_t adc_vref = 0;
uint16_t battery_value = 0;//20240317
// uint16_t battery_value_test = 0;
bool bat_test_flag = false;
static  uint32_t battery_test_time;
extern bool suspend;
extern bool wireless_connected;
extern uint8_t last_wireless_mode;
 enum kb_mode_t kb_mode= KB_MODE_DEFALT;//每次上电都可以进入下面的上电执行一次的地
extern rgb_led_t leds[6];
#define LOOP_10HZ_PERIOD    100
deferred_token loop10hz_token  = INVALID_DEFERRED_TOKEN;
uint32_t loop_10Hz(uint32_t trigger_time, void *cb_arg);
uint8_t status_caps = Caps_OFF;
uint8_t batt_charge_status = batt_default;        //电池状态
//uint8_t low_batt_status = 0;



uint8_t change[3] = {0X01, 0X02, 0X03};
uint8_t up[3] = {0XA5, 0X00, 0X21};
uint8_t down[3] = {0XA5, 0X00, 0X22};
uint8_t toggle[3] = {0XA5, 0X00, 0X10};
uint8_t restart[3] = {0XA5, 0X00, 0Xff};
uint8_t ISP[3] = {0XA5, 0X00, 0Xfe};
uint8_t DOWMLOAD_PIC[4] = {0XA5, 0X02, 0XF0,0X01};
uint8_t DOWMLOAD_GIF[4] = {0XA5, 0X02, 0XF0,0X02};
uint8_t DOWMLOAD_EXIT[4] = {0XA5, 0X02, 0XF1,0X01};
uint8_t SET_PIC[3] = {0XA5, 0X02, 0XE0};
uint8_t SET_GIF[3] = {0XA5, 0X02, 0XE1};

uint8_t SET_BLACK_PIC[32] = {0x00, 0x00, 0x00, 0x00,\
                             0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff,\
                             0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff,\
                             0x00, 0x00, 0x00, 0x00
                            };


static const SerialConfig uart_config = {
    .speed = 921600,
};


#ifdef RGBLIGHT_ENABLE

const rgblight_segment_t PROGMEM _SecondOff_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_WHITE}
);
const rgblight_segment_t PROGMEM __SecondOn_layer_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_BLACK}
);

const rgblight_segment_t PROGMEM __power1_layer_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_RED}
);



const rgblight_segment_t PROGMEM __power4_layer_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_BLUE}
);
const rgblight_segment_t PROGMEM __power2_layer_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_YELLOW}
);
const rgblight_segment_t PROGMEM __power3_layer_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_RED}
);
const rgblight_segment_t PROGMEM __power5_layer_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_BLACK}
);


const rgblight_segment_t PROGMEM __power6_layer_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_GREEN}
);
const rgblight_segment_t PROGMEM __power7_layer_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_BLACK}
);


const rgblight_segment_t PROGMEM __power8_layer_layer11[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_GREEN}
);


const rgblight_segment_t PROGMEM __power8_layer_layer12[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_RED}
);

const rgblight_segment_t PROGMEM __power8_layer_layer13[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_BLACK}
);


const rgblight_segment_t PROGMEM __power8_layer_layer14[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_BLACK}
);



const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
     _SecondOff_layer,
    __SecondOn_layer_layer,
    __power1_layer_layer,

    __power4_layer_layer,
    __power2_layer_layer,
    __power3_layer_layer,
    __power5_layer_layer,
    __power6_layer_layer,
    __power7_layer_layer,
    __power8_layer_layer11,
    __power8_layer_layer12,
    __power8_layer_layer13,
    __power8_layer_layer14
);

#endif

/*全键无冲切换*/
void key_nkro_toggle(void) {
    //全键无冲带记忆
    if (nkro_flag != keymap_config.nkro) {
        nkro_flag = keymap_config.nkro;
        // dynamic_keymap_set_keycode(1, 1, 0, nkro_flag);
        variable_data.eeconfig_nkro_flag = keymap_config.nkro;
        eeconfig_update_kb_datablock(&variable_data, 0, EECONFIG_KB_DATA_SIZE);
    }
}

//插入检测
bool get_plug_mode(void)
{
    if (readPin(PLUG_IN))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//增加限制
uint8_t count_ble;
uint8_t count_24G;
uint8_t count_USB;
void get_mode(void)
{
    if (!readPin(BLE) )
    {
        kb_mode = KB_MODE_BLE;
        Keyboard_mode = 1;

    }
    else if((!readPin(TwoMode)))
    {
          kb_mode = KB_MODE_24G;
          Keyboard_mode = 4;
    }
    else
    {
          kb_mode = KB_MODE_USB;
          Keyboard_mode = 0;
    }



    // if (!readPin(SYS_SW) && (WinLayer_Flag == 0))
    // {
    //     WinLayer_Flag = 1;
    //     MacLayer_Flag = 0;
    //     set_single_persistent_default_layer(2);
    //   //  uprintf("win\r\n");
    // }

    // if (readPin(SYS_SW) && (MacLayer_Flag == 0))
    // {
    //     MacLayer_Flag = 1;
    //     WinLayer_Flag = 0;
    //     set_single_persistent_default_layer(0);
    //     //uprintf("mac\r\n");
    // }
}


#ifdef  mk856_special_isr
void special_key_debounce(void)
{
   //中断源1 左旋    2 2.4G  3蓝牙
   // Interrupt source 1 USB, 2 2.4G, 3 Bluetooth
   uint8_t temp_status;

   if(isr_specal_Trig)
   {
         if(isr_specal_Trig == 1)
         {
              temp_status =  readPin(encoder_left);
         }
         else if(isr_specal_Trig == 2)
         {
              temp_status =  readPin(TwoMode);
         }
        else if(isr_specal_Trig == 3)
         {
              temp_status =  readPin(BLE);
         }

        wait_ms(5);
        if( (isr_specal_Trig == 1 && (temp_status !=  readPin(encoder_left)))  ||  (isr_specal_Trig == 2 && (temp_status !=  readPin(TwoMode)))  || (isr_specal_Trig == 3 && (temp_status !=  readPin(BLE)))  )
        {
            //中断源再加进去

            setPinInput(BLE);
            setPinInput(TwoMode);
            palEnableLineEvent(BLE, PAL_EVENT_MODE_BOTH_EDGES);
            palEnableLineEvent(TwoMode, PAL_EVENT_MODE_BOTH_EDGES);


            setPinInput(encoder_left);
            setPinInput(encoder_right);
            palEnableLineEvent(encoder_left, PAL_EVENT_MODE_RISING_EDGE);
            palEnableLineEvent(encoder_right, PAL_EVENT_MODE_RISING_EDGE);

            const long unsigned int row_pins[] = MATRIX_ROW_PINS;
            pin_t col_pins[] = MATRIX_COL_PINS;
                for(uint8_t x=0; x<MATRIX_COLS; x++)
                {
                    pin_t pin;
                    pin = col_pins[x];
                    if (pin != NO_PIN)
                    {
                        setPinOutput(pin);
                        writePinLow(pin);
                    }
                }


                for(uint8_t x=0; x<MATRIX_ROWS; x++)
                {
                    pin_t pin;
                    pin = row_pins[x];
                    if (pin != NO_PIN)
                    {
                        setPinInputHigh(pin);
                        palEnableLineEvent(pin, PAL_EVENT_MODE_FALLING_EDGE);
                    }
                }
                PWR->CR |= (1<<0)|(1<< 10) |(1<< 11)|(3<<18);
                PWR->CR |= PWR_CR_CWUF | PWR_CR_CSBF;
                SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
                __WFI();
                SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
        }
   }
    isr_specal_Trig = 0;
    EXTI->IMR  &= ~0xffff;
    EXTI->EMR  &= ~0xffff;
    EXTI->RTSR &= ~0xffff;
    EXTI->FTSR &= ~0xffff;


}
#endif
layer_state_t default_layer_state_set_kb(layer_state_t state)
{
    default_layer = state;
    return state;
}



void keyboard_pre_init_kb(void) {
    setPinOutput(RENUM);
    writePinHigh(RENUM);
    // setPinOutput(B8);
    // writePinLow(B8);
    // setPinOutput(ARGB_LEFT_EN);
    // writePinLow(ARGB_LEFT_EN);
}




// //键盘初始化
void keyboard_post_init_kb(void)
{
    AFIO->MAPR |= AFIO_MAPR_USART3_REMAP_PARTIALREMAP;
    uart_init(460800);
    setPinOutput(ARGB_LEFT_EN);
    writePinLow(ARGB_LEFT_EN);
    // palSetLineMode(C10, PAL_MODE_ALTERNATE(UART_TX_PAL_MODE) | PAL_OUTPUT_TYPE_PUSHPULL | PAL_OUTPUT_SPEED_HIGHEST);
    // palSetLineMode(C11, PAL_MODE_ALTERNATE(UART_RX_PAL_MODE) | PAL_OUTPUT_TYPE_PUSHPULL | PAL_OUTPUT_SPEED_HIGHEST);


    // CHECK ME IF IT WORKS

    palSetLineMode(C10, UART_TX_PAL_MODE);
    palSetLineMode(C11, UART_RX_PAL_MODE);

    sdStart(&SD3, &uart_config);
    wait_ms(400);
    debug_enable=true;
    debug_keyboard=true;
    debug_matrix=true;
    debug_mouse=true;
    rgb_matrix_set_color_all(0,0,0);
    rgblight_layers = my_rgb_layers;
    AFIO->MAPR = (AFIO->MAPR & ~AFIO_MAPR_SWJ_CFG_Msk);
    AFIO->MAPR|= AFIO_MAPR_SWJ_CFG_DISABLE;
     eeconfig_read_kb_datablock(&variable_data, 0, EECONFIG_KB_DATA_SIZE);    //读出数据
    keymap_config.nkro  = variable_data.eeconfig_nkro_flag; /*是否开启全全键无冲*/
    lock_win_flag = variable_data.eeconfig_lock_win_flag;
    SET_FLAG( kb_screen.flag , PK_WINLOCK_STATUS );
    // encode_toggle = variable_data.eeconfig_encode_toggle;
    shut_up_Flag = variable_data.eeconfig_shut_up_Flag;
    last_wireless_mode  = variable_data.eeconfig_last_wireless_mode;
    Shut_Up_Screen      = variable_data.eeconfig_shut_up_screen_Flag;
    if (!Shut_Up_Screen) {
        setPinOutput(LCD_SWITCH);
        writePinHigh(LCD_SWITCH);
    }
    else{
        setPinOutput(LCD_SWITCH);
        writePinLow(LCD_SWITCH);
    }
    eeconfig_update_keymap(&keymap_config);
    setPinInput(PLUG_IN);
    wait_ms(1);
    wireless_connected=false;
    flag32.mode_one = 1;
    pair_succeed_timer = timer_read32();
//ADC初始化
    adc_init();     //adc初始化
    get_adc_value();
    get_adc_vref();
    Rtc_Config_Api();
    current_layer = get_highest_layer(layer_state | default_layer);
    if(current_layer == 2)
    {
         f_os_mode = 1;
         SET_FLAG( kb_screen.flag , PK_OS_TYPE );
    }
    else if(current_layer == 0)
    {
         f_os_mode = 0;
         SET_FLAG( kb_screen.flag , PK_OS_TYPE );
    }
    loop10hz_token = defer_exec(LOOP_10HZ_PERIOD, loop_10Hz, NULL);
}


uint8_t limit_flag;
uint8_t power_count_flag = 1;;
//1s执行10次
enum kb_mode_t new_kb_mode = KB_MODE_DEFALT;
uint32_t loop_10Hz(uint32_t trigger_time, void *cb_arg)
{
    blink_counter = (blink_counter+1) % (blink_period*2);
    ble24G_counter = (ble24G_counter+1) % (ble24G_period*2);
    power_counter = (power_counter+1) % (power_period*2);
    Valorspd_counter = (Valorspd_counter+1) % (Valorspd_period*2);
    if(USB_DRIVER.state == USB_ACTIVE)
    {
        if(usb_wakeup_flag == true){
            clear_keyboard();
            send_keyboard_report();
            usb_wakeup_flag = false;
        }
    }

    if(0 == blink_counter)
    {

          if (release_count) {
            if (USB_DRIVER.state == USB_ACTIVE) {
                release_count--;
                if (keymap_config.nkro == 1) {
                    report_nkro_t send_nkro_report;
                    memset(&send_nkro_report, 0, sizeof(report_nkro_t));
                    host_nkro_send(&send_nkro_report);
                } else {
                    report_keyboard_t send_6nkro_report;
                    memset(&send_6nkro_report, 0, sizeof(report_keyboard_t));
                    host_keyboard_send(&send_6nkro_report);
                }
            }
        }


        if(power_count_flag)
        {
            power_count_flag++;
            if(power_count_flag > 3)
            {
                power_count_flag = 0;
            }
        }
    }

    if(!default_layer)
    {
       default_layer = 1;
    }

    if(timer_elapsed32(keyborad_Idtimer) >= 2000)
    {

        keyboard_Idle = 0;
    }

     if(0 == Valorspd_counter)
     {

        if(blink_num > 0)
        {
            blink_num--;
        }

     }



    if(!limit_flag)
    {
        limit_flag = 1;
        setPinOutput(ARGB_LEFT_EN);
        writePinHigh(ARGB_LEFT_EN);
    }





     if (Sleep_First_PressFlag)
     {
        if( (matrix[0] == 0) &&  (matrix[1] == 0)  &&  (matrix[2] == 0)&&  (matrix[3] == 0)&&  (matrix[4] == 0)&&  (matrix[5] == 0))
        {
            setPinOutput(RENUM);
            writePinHigh(RENUM);
            init_usb_driver(&USB_DRIVER);
            Sleep_First_PressFlag = 0;
        }
        else if(Sleep_First_PressFlag && timer_elapsed32(first_sleep_wakeup_timer) > 6000)
        {
            setPinOutput(RENUM);
            writePinHigh(RENUM);
            init_usb_driver(&USB_DRIVER);
            Sleep_First_PressFlag = 0;
        }

    }
    if(lowpower_flag)
    lowpower_flag++;

    if((lowpower_flag > 10))
    {
        lowpower_flag = 0;
    }

    if(kb_screen.screen_boot_time)
    {
         kb_screen.screen_boot_time++;
    }





   current_layer = get_highest_layer(layer_state | default_layer);
    /*全键无冲切换*/
    key_nkro_toggle();

    // if(encode_Press_flag)
    // {
    //     if(timer_elapsed32(encode_toggle_timer) > 2000)
    //     {
    //         encode_toggle  = !encode_toggle;
    //         //存储功能
    //         // dynamic_keymap_set_keycode(0, 4, 9, encode_toggle);//虚拟按键
    //         variable_data.eeconfig_encode_toggle = encode_toggle;
    //         //增加常亮1s白色计时
    //         if(!BT_24G_Shine)
    //         {
    //             encode_toggle_flag = 1;
    //             encoder_longpresstimer = timer_read32();
    //         }
    //         eeconfig_update_kb_datablock(&variable_data);
    //         encode_Press_flag = 0;
    //     }
    // }




   // uprintf("Effect_Flag:%08lx\r\n",Effect_Flag);
    //    uprintf("battery_value_test:%d\r\n",battery_value_test);
        // uprintf(" keymap_config.nkro:%d\r\n", keymap_config.nkro);
 //    "FN_Count:%d\r\n",FN_Count);
    if((bat_test_flag == false && !power_count_flag))
    {
        adc_value = get_adc_value();    //获取adc值
        adc_vref =  get_adc_vref();
        battery_value = (adc_value*1764/adc_vref);
        // battery_value_test =  battery_value;zzzzzzzz
        battery_test_time = timer_read32();
        temp               = batt_level();
        // temp_test          = temp;
        last_temp          = temp;
        bat_test_flag = true;              //把bat_test_flag置1，确保只进一次
        if(kb_mode==KB_MODE_USB)
        {
            Batt_Baifen = 100;
            SET_FLAG( kb_screen.flag , PK_BATT_QUANTITY);
        }
        else
        {
            Batt_Baifen = temp;
            SET_FLAG( kb_screen.flag , PK_BATT_QUANTITY);
        }

    }

    adc_test();         //adc检测
    ble_send_batt();    //蓝牙发送百分比电量

//状态切换进去一次


    if( (batt_charge_status==batt_is_charging||batt_charge_status==batt_charge_full) && pre_batt_status!=batt_charge_status ){
    	pre_batt_status = batt_charge_status;
    	temp_u8 = 1;
        SET_FLAG( kb_screen.flag , PK_BATT_STATUS );
    }
    else if( batt_charge_status==batt_no_charge && pre_batt_status!=batt_charge_status )
    {
    	pre_batt_status = batt_charge_status;
    	temp_u8 = 0;
        SET_FLAG( kb_screen.flag , PK_BATT_STATUS );
    }
    // else if( low_batt_status==1 && pre_batt_status!=batt_charge_status){
    // 	pre_batt_status = batt_charge_status;
    // 	temp_u8 = 1;
    //     SET_FLAG( kb_screen.flag , PK_BATT_STATUS );
    // }

#ifdef FN_Double_Knock
//防止按键按下时间过长
    if(timer_elapsed32(First_timer) > 400)
    {
       FN_Count = 0;
    }


    if(timer_elapsed32(Media_timer) > 100 && (Media_Flag == 1) && (FN_Count == 0))
    {
        Media_Flag = 2;

        current_layer = get_highest_layer(layer_state | default_layer);
        if(current_layer == 0)
        {
            set_single_persistent_default_layer(4);
        }
        else if(current_layer == 2)
        {
            set_single_persistent_default_layer(6);
        }
        else if(current_layer == 4)
        {
            set_single_persistent_default_layer(0);
        }
        else if(current_layer == 6)
        {
            set_single_persistent_default_layer(2);
        }
    }
 #endif

   //检测电池发送，空闲才发送
    if((battery_Flag == 1) && (!keyboard_Idle) )
    {
        battery_Flag = 0;
        sc_ble_battary(temp);
    }

//question
      get_mode();
//    上电进来一次默认为usb
    if (kb_mode != new_kb_mode)  //ONLY DO IT ONCE WHEN MODE SWITCHED
    {
         new_kb_mode = kb_mode;
         BT_24G_Shine = 0;   //，模式切换进来一次，BT_24G_Shine = 0；
        if(kb_mode==KB_MODE_BLE)
        {
            last_wireless_mode &= 3;
            WIRELESS_START(last_wireless_mode);
            flag32.link_break_flag =0;
            rgb_wireless_timer = timer_read32();
            first_sleep_timer = timer_read32();
            packet_send = 0;
        }


        else if(kb_mode == KB_MODE_24G)
        {
            WIRELESS_START(4);
            flag32.link_break_flag =0;
            rgb_wireless_timer = timer_read32();
            first_sleep_timer = timer_read32();
            packet_send = 0;
        }


        else if(kb_mode == KB_MODE_USB)
        {
            WIRELESS_STOP();
            // init_usb_driver(&USB_DRIVER);
            pair_succeed_timer = timer_read32();
            blink_counter = 0;
        }
        SET_FLAG( kb_screen.flag , PK_CONN_TYPE );
        // if(!limit_flag)
        // {
        //     limit_flag = 1;
        //     setPinOutput( );
        //     writePinHigh(ARGB_LEFT_EN);
        // }

        blink_ble_24g_timer = 0; //蓝牙回连超时     配对也是这个
        pair_succeed_timer = timer_read32();
        //重新计时连接时间
        blink_ble_24g_timer = timer_read32(); //重新计时回连时间
        pair_timeout = 20000;
    //    BT_Switch_Flag = 1;
        ble24G_period = 5;
        ble24G_counter  = 0;
        flag32.mode_one = 1;
    }






    if(1 == flag32.press_24G_flag)
    {
        if(timer_elapsed32(long_ble_24g_timer) > BLE_24G_TIMEOUT)
        {

            flag32.link_break_flag =0;
            flag32.press_24G_flag = 0;
            WIRELESS_PAIR(4);
            blink_ble_24g_timer = timer_read32();
            ble24G_period = 2;
            ble24G_counter  = 0;
            pair_timeout = 60000;
          //  BT_Switch_Flag = 0;
            flag32.mode_one = 1;
        }
    }


    if(1 == flag32.press_ble_flag)
    {
        if(timer_elapsed32(long_ble_24g_timer) > BLE_24G_TIMEOUT)
        {
            flag32.link_break_flag =0;

            // ble_led_state = 0;
            flag32.press_ble_flag = 0;
            WIRELESS_PAIR(last_wireless_mode);
            blink_ble_24g_timer = timer_read32();
            ble24G_period = 2;
            ble24G_counter  = 0;
            pair_timeout = 60000;
          //  BT_Switch_Flag = 0;
            flag32.mode_one = 1;
        }

    }
    //以上




        //长按3s复位键处理
        if(1 == flag32.reset_flag)
        {
            if(timer_elapsed32(long_press_reset_timer) > RESET_TIMEOUT)
            {

                flag32.reset_flag = 0;
                blink_num = 3;
                Valorspd_counter = 0;
                reset_flag = 1;
            }
        }


        if(!wireless_connected)
        {

            if( (flag32.link_break_flag == 1))
            {
                flag32.link_break_flag = 0;
                ble24G_period = 5;
                ble24G_counter = 0;
                flag32.mode_one = 1;
                connect_buff = 0;
                pair_timeout = 20000;
               // BT_Switch_Flag = 1;
                blink_ble_24g_timer = timer_read32(); //重新计时回连时间
            }
        }
        if(wireless_connected)
        {
            if(connect_buff == 0)
            {
                connect_buff = 1;
                rgb_wireless_timer = timer_read32();
                first_sleep_timer = timer_read32();
                packet_send = 0;
            }
        }
       sleep_mode();


    return LOOP_10HZ_PERIOD;
}




uint8_t test01 = 0;
uint8_t unchrg_falg = 1;
uint8_t chrg_falg = 1;
uint8_t chrgfull_falg = 1;
uint8_t chrg_battest = 1;
uint8_t lowpower_effect;
uint16_t count_full;
bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max)
{
    if (Shine_Handle == 1) {
        Shine_Handle++;
    }
    if (suspend_count) {
        rgblight_set_layer_state(1, true);
        rgb_matrix_set_color_all(0, 0, 0);
        return false;
    }

    if (sleep_switch_driverflag) {
        sleep_switch_driverflag = sleep_switch_driverflag - 1;
        if (sleep_switch_driverflag == 0) {
            setPinOutput(ARGB_LEFT_EN);
            writePinHigh(ARGB_LEFT_EN);
            rgblight_set_layer_state(1, false);
        }
    }

    if(sleep_switch_driverflag == 0)
    { //|| lock_keyboard
               if(shut_up_Flag)//主动关闭
               {
                    if( MK923_R_FLAG(Effect_Flag,Caps_Nums_Statue)   || MK923_R_FLAG(Effect_Flag,Reset_Statue)   || MK923_R_FLAG(Effect_Flag,Power_Indication_Statue)   || MK923_R_FLAG(Effect_Flag,Win_Lock_Statue)
                    || MK923_R_FLAG(Effect_Flag,Chrg_Statue)   || MK923_R_FLAG(Effect_Flag,Lower_Power_Statue)   || MK923_R_FLAG(Effect_Flag,Mode_Statue) )
                    {
                          rgb_matrix_set_color_all(0,0,0);
                          if(MK923_R_FLAG(Effect_Flag,Mode_Statue) == 0  && ( MK923_R_FLAG(Effect_Flag,Reset_Statue) == 0)    && ( MK923_R_FLAG(Effect_Flag,Lower_Power_Statue) == 0)  )   //没有跑模式的灯效
                          {
                              rgblight_set_layer_state(12,true);
                          }
                          else{
                              rgblight_set_layer_state(12,false);
                          }

                    }
                    else{
                        if(!Shine_Handle)
                        {
                           Shine_Handle = 1;
                        }
                        if(readPin(ARGB_LEFT_EN)&& (Shine_Handle ==5))
                        {
                            Shine_Handle = 0;
                            setPinOutput(ARGB_LEFT_EN);
                            writePinLow(ARGB_LEFT_EN);
                        }
                        rgb_matrix_set_color_all(0,0,0);
                         if(MK923_R_FLAG(Effect_Flag,Mode_Statue) == 0  && ( MK923_R_FLAG(Effect_Flag,Reset_Statue) == 0)    && ( MK923_R_FLAG(Effect_Flag,Lower_Power_Statue) == 0)  )   //没有跑模式的灯效
                        {
                            rgblight_set_layer_state(12,true);
                        }
                    }
               }
               else{
                    if((!lowpower_flag && ((temp < 10)) && (!power_count_flag)  && (get_plug_mode() == false) && (bat_test_flag == true))
                    || (!get_plug_mode() &&  Light_Count && ((kb_mode == KB_MODE_BLE) || (kb_mode == KB_MODE_24G))) )
                    {
                            rgb_matrix_set_color_all(0,0,0);
                           if(MK923_R_FLAG(Effect_Flag,Mode_Statue) == 0  && ( MK923_R_FLAG(Effect_Flag,Reset_Statue) == 0)    && ( MK923_R_FLAG(Effect_Flag,Lower_Power_Statue) == 0)  )   //没有跑模式的灯效
                          {
                              rgblight_set_layer_state(12,true);
                          }
                          else{
                              rgblight_set_layer_state(12,false);
                          }
                    }
                    else{
                          rgblight_set_layer_state(12,false);
                    }
               }


#ifdef Lock_Keyboard
            if(lock_keyboard)
            {
                rgb_matrix_set_color(34,255,255,255);
            }
 #endif

            // //旋钮的优先级低于模式
            // if(!BT_24G_Shine)
            // {
            //     if(encode_toggle_flag)
            //     {
            //       if(timer_elapsed32(encoder_longpresstimer) < 1200)
            //       {
            //                 rgblight_set_layer_state(12,true);
            //       }
            //       else
            //       {
            //                  encode_toggle_flag = 0;
            //                 rgblight_set_layer_state(12,false);
            //       }
            //     }
            // }
            // else //防止闪灯期间切回模式等效
            // {
            //              encode_toggle_flag = 0;
            //              rgblight_set_layer_state(12,false);
            // }


                if (blink_num != 0) {
                    if (MK923_R_FLAG(Effect_Flag, Reset_Statue) == 0) {
                        MK923_SET_FLAG(Effect_Flag, Reset_Statue);
                      //  uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                    }
                    BT_24G_Shine = 0;
                    rgblight_set_layer_state(6,false);
                    rgblight_set_layer_state(3,false);
                    rgblight_set_layer_state(4,false);
                    rgblight_set_layer_state(5,false);
                    rgblight_set_layer_state(7,false);
                    rgblight_set_layer_state(8,false);
                    rgblight_set_layer_state(9,false);
                    rgblight_set_layer_state(10,false);
                    rgblight_set_layer_state(11,false);
                    if (Valorspd_counter < Valorspd_period)
                    {

                        rgblight_set_layer_state(0,true); //亮
                        rgblight_set_layer_state(1,false);
                    }
                    else
                    {
                        rgblight_set_layer_state(0,false); //亮
                        rgblight_set_layer_state(1,true);
                    }
                }
                else {
                    if (MK923_R_FLAG(Effect_Flag, Reset_Statue)) {
                        MK923_CLEAR_FLAG(Effect_Flag, Reset_Statue);
                      //  uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                    }
                    if(reset_flag)
                    {
                        rgblight_set_layer_state(0,false);
                        rgblight_set_layer_state(1,false);
                        eeconfig_init();
                        mcu_reset();
                    }
                }



           if( ((host_keyboard_leds() & 0x02) == 0x02) && (wireless_connected || ((kb_mode == KB_MODE_USB))  ))
            {
               if (MK923_R_FLAG(Effect_Flag, Caps_Nums_Statue) == 0) {
                   MK923_SET_FLAG(Effect_Flag, Caps_Nums_Statue);
                 //  uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
               }
                rgb_matrix_set_color(44,255,255,255);
                if(status_caps == Caps_OFF)
                {
                     status_caps = Caps_ON;
                    SET_FLAG( kb_screen.flag , PK_CAPS_STATUS );
                }

            }
            else{
                if (MK923_R_FLAG(Effect_Flag, Caps_Nums_Statue)) {
                    MK923_CLEAR_FLAG(Effect_Flag, Caps_Nums_Statue);
                   // uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                }
                if(status_caps == Caps_ON)
                {
                     status_caps = Caps_OFF;
                    SET_FLAG( kb_screen.flag , PK_CAPS_STATUS );
                }

            }

            // if(status_caps != Default_Status)
            // {
            //    status_caps = Default_Status;
            //    SET_FLAG( kb_screen.flag , PK_CAPS_STATUS );
            // }



            if(get_plug_mode())
            {
                if(temp < 100)
                {
                    batt_charge_status =  batt_is_charging;
                    count_full = 0;
                }
                else
                {
                    count_full++;
                    if(count_full > 300)
                    {
                       batt_charge_status =  batt_charge_full;
                    }
                }
            }
            else
            {
               count_full = 0;
               batt_charge_status =  batt_no_charge;
            }



            //电量指示
            if(!get_plug_mode() &&  Light_Count && ((kb_mode == KB_MODE_BLE) || (kb_mode == KB_MODE_24G))  )   //
            {
                if (MK923_R_FLAG(Effect_Flag, Power_Indication_Statue) == 0) {
                    MK923_SET_FLAG(Effect_Flag, Power_Indication_Statue);
                   // uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                }
                // rgb_matrix_set_color_all(0,0,0);
                for (uint8_t i = 0; i < Light_Count; i++) {
                    rgb_matrix_set_color(15 + i, 255, 255, 255);
                }

                if(Light_Count > 0 && (Light_Count <= 3))
                {
                    for(uint8_t i = 0;i < Light_Count; i++ )  //紅      紅綠可以混成黃色
                    {
                        rgb_matrix_set_color(15+i,255,0,0);
                    }
                }

                if(Light_Count > 3 && (Light_Count <= 7))
                {
                    for(uint8_t i = 0;i < Light_Count; i++ )  //黃    紅綠可以混成黃色
                    {
                        rgb_matrix_set_color(15+i,255,255,0);
                    }
                }

                if(Light_Count > 7 && (Light_Count <= 10))
                {
                    for(uint8_t i = 0;i < Light_Count; i++ )  //黃    紅綠可以混成黃色
                    {
                        rgb_matrix_set_color(15+i,0,255,0);
                    }
                }

            } else {
                if (MK923_R_FLAG(Effect_Flag, Power_Indication_Statue)) {
                    MK923_CLEAR_FLAG(Effect_Flag, Power_Indication_Statue);
                  //  uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                }
            }

            if(0 == lock_win_flag && (wireless_connected || (kb_mode == KB_MODE_USB)))
            {
                if(default_layer == 1 ||(default_layer == 16))
                {

                      if(MK923_R_FLAG(Effect_Flag,Win_Lock_Statue) == 0)
                     {
                          MK923_SET_FLAG(Effect_Flag,Win_Lock_Statue);
                       // uprintf("Effect_Flag:%ld\r\n",Effect_Flag);
                     }
                   rgb_matrix_set_color(71,255,255,255);
                }
                else{
                     if(MK923_R_FLAG(Effect_Flag,Win_Lock_Statue))
                     {
                         MK923_CLEAR_FLAG(Effect_Flag,Win_Lock_Statue);
                        // uprintf("Effect_Flag:%ld\r\n",Effect_Flag);
                     }
                }
             //
            }
            else{
                     if(MK923_R_FLAG(Effect_Flag,Win_Lock_Statue))
                     {
                         MK923_CLEAR_FLAG(Effect_Flag,Win_Lock_Statue);
                    //     uprintf("Effect_Flag:%ld\r\n",Effect_Flag);
                     }
            }



        // //充电灯效
        //     if(get_plug_mode() && (temp < 100) && (bat_test_flag == true) && Test_Battery_value)
        //     {
        //        rgb_matrix_set_color(33,255,0,0);
        //     }
        //低电灯效

        //  充电与充满灯效    //防止上电时看起来灯效会覆盖太快，限制下时间

        if(!BT_24G_Shine && (blink_num == 0))
        {
                if(get_plug_mode()  && (!lowpower_flag))
                {
                  //  low_batt_status = 0;
                    //增加一个充电标志，防止via切换充电灯效

                   //低电处理
                    if(lowpower_effect == 1)
                    {
                        lowpower_effect = 2;
                        rgblight_set_layer_state(11,false);
                        rgblight_set_layer_state(10,false);
                    }

                    if (temp < 100) // chrg in
                    {
                        chag_via_flag = 1;
                        // 这里开始计时
                        if (!chag_first_flag) {
                            chag_first_flag    = 1;
                            keyborad_Chrgtimer = timer_read32();
                        }
                        if (timer_elapsed32(keyborad_Chrgtimer) < 5000) {
                            if(MK923_R_FLAG(Effect_Flag,Chrg_Statue) == 0)
                            {
                                MK923_SET_FLAG(Effect_Flag,Chrg_Statue);
                               // uprintf("Effect_Flag:%ld\r\n",Effect_Flag);
                            }
                            rgblight_set_layer_state(9, true);
                        } else {
                            if (MK923_R_FLAG(Effect_Flag, Chrg_Statue)) {
                                MK923_CLEAR_FLAG(Effect_Flag, Chrg_Statue);
                              //  uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                            }
                            rgblight_set_layer_state(9, false);
                            chag_via_flag = 0;
                        }
                    } else {
                        if (MK923_R_FLAG(Effect_Flag, Chrg_Statue)) {
                            MK923_CLEAR_FLAG(Effect_Flag, Chrg_Statue);
                           // uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                        }
                        chag_first_flag = 0;
                        chag_via_flag   = 0;
                        rgblight_set_layer_state(9, false);
                    }

                }
                else
                {

                    chag_first_flag = 0;
                    chag_via_flag = 0;
                    Plug_In_Flag = 0;
                    rgblight_set_layer_state(9,false);
                    // Power_Full_Flag = 0;
                    //低电报警
                    lowpower_effect = 1;

                    if (!lowpower_flag && ((temp < 10))) {
                        if (MK923_R_FLAG(Effect_Flag, Lower_Power_Statue) == 0) {
                            MK923_SET_FLAG(Effect_Flag, Lower_Power_Statue);
                            //("Effect_Flag:%ld\r\n", Effect_Flag);
                        }
                        if (power_counter < power_period) {
                            rgblight_set_layer_state(10, true); // 亮红
                            rgblight_set_layer_state(11, false);
                        } else {
                            rgblight_set_layer_state(10, false); // 亮黑
                            rgblight_set_layer_state(11, true);
                        }
                        //    low_batt_status = 1;
                    } else {
                        if (MK923_R_FLAG(Effect_Flag, Lower_Power_Statue)) {
                            MK923_CLEAR_FLAG(Effect_Flag, Lower_Power_Statue);
                           // uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                        }
                        //    low_batt_status = 0;
                    }
                }
        }
        else
        {
            //模式灯效中关闭低电
                    chag_via_flag = 0;
                    rgblight_set_layer_state(10,false);  //亮红
                    rgblight_set_layer_state(11,false);
                    rgblight_set_layer_state(9,false);
        }
        if(1 == flag32.mode_one)
        {
            if (blink_num == 0) {
            three_mode();
            }
        }


        // 被动打开
        if (shut_up_Flag == 1 && (readPin(ARGB_LEFT_EN) == 0) && ( MK923_R_FLAG(Effect_Flag, Reset_Statue)   || MK923_R_FLAG(Effect_Flag, Caps_Nums_Statue)
        || MK923_R_FLAG(Effect_Flag, Power_Indication_Statue) || MK923_R_FLAG(Effect_Flag, Win_Lock_Statue) || MK923_R_FLAG(Effect_Flag, Chrg_Statue) || MK923_R_FLAG(Effect_Flag, Lower_Power_Statue) || MK923_R_FLAG(Effect_Flag, Mode_Statue))) {
            setPinOutput(ARGB_LEFT_EN);
            writePinHigh(ARGB_LEFT_EN);
            rgb_matrix_set_color_all(0, 0, 0);
            rgblight_set_layer_state(12, true);
        }
    }



    return false;
}

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//     if (record->event.pressed) {
//         key_nums_variable++;

//         if((keyboard_report->keys[2] == 0) ||  (keyboard_report->keys[3] == 0)  ||  (keyboard_report->keys[4] == 0)||
//           (keyboard_report->keys[5] == 0)||  (keyboard_report->keys[6] == 0)||  (keyboard_report->keys[7] == 0) )
//         {
//             add_key_byte(keyboard_report, keycode);
//             keymap_config.nkro = 0;
//         }else
//         {
//              add_key_bit(nkro_report, keycode);
//              keymap_config.nkro = 1;
//         }
//     } else {
//         key_nums_variable--;
//         for (int i = 0; i < KEYBOARD_REPORT_KEYS; i++) {
//             if (keyboard_report->keys[i] == keycode) {
//                 keyboard_report->keys[i] = 0;
//                 keymap_config.nkro = 0;
//                 return true;
//             }
//         }
//             if ((keycode >> 3) < NKRO_REPORT_BITS) {
//                 if (nkro_report->bits[keycode >> 3] & 1 << (keycode & 7)) {
//                     nkro_report->bits[keycode >> 3] = 0;
//                     keymap_config.nkro = 1;
//                 }
//             }

//     }
//     return true;
// }


bool process_record_kb(uint16_t keycode, keyrecord_t* record)
{

    if(record->event.pressed)
    {
        rgb_wireless_timer  = timer_read32();
        blink_ble_24g_timer = timer_read32();
        first_sleep_timer   = timer_read32();
  //      uprintf("keycode :%d\r\n",keycode);
       if (!wireless_connected)
       {
            for (int i=0;i<5;i++)
            {
                uart_write(0x00);
            }
       }
       packet_send = 0;
    }

    // if (keycode == KC_INTERNATIONAL_5)   //jp1
    //  {
    //     if (record->event.pressed) {

    //         if(keymap_config.nkro)
    //         {
    //             keymap_config.nkro = !keymap_config.nkro;
    //             Six_key_Flag = 1;
    //         }
    //     }
    //     else{
    //         // if(Six_key_Flag)
    //         // {
    //         //     Six_key_Flag = 0;
    //         //     keymap_config.nkro = !keymap_config.nkro;
    //         // }

    //     }
    // }

    // while (!sdGetWouldBlock(&SD3))
    // {
    //     uint8_t byte = sdGet(&SD3);
    // //    uprintf("byte = %d\r\n",byte);
    // }


    if(keycode == KC_Screen_Home)
    {
       if(record->event.pressed)
        {
             SET_FLAG( kb_screen.flag , PK_GO_HOME );
        }
        return false;
    }

    if(keycode == KC_Screen_Tog)
    {
       if(record->event.pressed)
        {
            SET_FLAG( kb_screen.flag , PK_TOGGLE_PIC );
        }
        return false;
    }

    if(keycode == KC_Screen_Gif)
    {
       if(record->event.pressed)
        {
           SET_FLAG( kb_screen.flag , PK_GO_GIF);
        }
        return false;
    }

    if(keycode == KC_SHUT_SC)
    {
       if(record->event.pressed)
        {
            Shut_Up_Screen = !Shut_Up_Screen;
            variable_data.eeconfig_shut_up_screen_Flag = Shut_Up_Screen;
            eeconfig_update_kb_datablock(&variable_data, 0, EECONFIG_KB_DATA_SIZE);
            if(Shut_Up_Screen)
            {
                setPinOutput(LCD_SWITCH);
                writePinLow(LCD_SWITCH);
            }
            else{
                setPinOutput(LCD_SWITCH);
                writePinHigh(LCD_SWITCH);
            }
        }
        return false;
    }






#ifdef Lock_Keyboard
    if(lock_keyboard)//锁键盘
    {
       // if((keycode != KC_LOCK  ||(keycode != MO(1))   ||(keycode != MO(3)))) //防止上锁后解锁不了
       if(keycode != MO(1) && (keycode != MO(3))  && (keycode != MO(5))   && (keycode != MO(7)) && (keycode != KC_LOCK))
        {
             if(record->event.pressed)
             {
                return false;
             }

        }
    }


    if(keycode == KC_LOCK)
    {
       if(record->event.pressed)
        {
            //松开按键，清掉fn
            lock_keyboard = ~lock_keyboard;
        }
        return false;
    }
#endif
//








#ifdef FN_Double_Knock
//增加双击fn切层的功能
    if(keycode ==  MO(5) || (keycode ==  MO(7)) ||  keycode ==  MO(1) || (keycode ==  MO(3)))
    {

        if(record->event.pressed)
        {

             First_timer = timer_read32();
             FN_Count++;
            if(timer_elapsed32(Doubele_Fn_timer) < 200 && (FN_Count == 1))//第三次快速按下时清0
            {
                FN_Count = 0;
                Media_Flag = 0;
                return true; //第三次直接推出去

            }

            //第一次松开到第二次按下的时间间隔小于
            if(timer_elapsed32(Doubele_Fn_timer) < 200 && (FN_Count == 2))
            {
                //开始计时，超过200ms
                //切层
               FN_Count = 0;
               Media_Flag = 1;
               Media_timer = timer_read32();
            }
            if(FN_Count == 2)
            {
                FN_Count = 0;
            }
        }
        else{
            //  if(timer_elapsed32(First_timer) > 400)
            //  {
            //     FN_Count = 0;
            //  }
            //松开开始计时
            Doubele_Fn_timer = timer_read32();
        }
        return true;
    }
#endif


// //增加长按切换旋钮的功能 ENC_TG
//     if (keycode == ENC_TG) {
//         if (record->event.pressed) {
//             encode_toggle_timer = timer_read32();
//             encode_Press_flag = 1;
//         }
//         else{
//                if(timer_elapsed32(encode_toggle_timer) <= 2000)
//                {//防止长按松开会切换功能
//                     if (!encode_toggle)
//                     {
//                         tap_code(KC_MUTE);//按下时的功能
//                     }
//                     else{
//                         rgb_matrix_step();
//                     }
//                     encode_toggle_timer = timer_read32();
//                }
//                encode_Press_flag = 0;
//         }
//        return false;
//     }

// 0    1    2     3   4    5    6     7
// win      mac       win        mac
// 1    2    4     8   16  32    64   128    default


   //windows和IOS切换处理
    if(keycode== KC_WINMODE)
    {
        if(record->event.pressed)
        {
            // if(default_layer == 4)
            // {
            //   set_single_persistent_default_layer(0); //Sets the default layer and writes it to persistent memory (EEPROM).
            // }
            // else if(default_layer == 64)
            // {
            //     set_single_persistent_default_layer(4);
            // }

           set_single_persistent_default_layer(0);
           f_os_mode = 0;
           SET_FLAG( kb_screen.flag , PK_OS_TYPE );

        }
        return false;
    }



// 0    1    2     3   4    5    6     7
// win      mac       win        mac
// 1    2    4     8   16  32    64   128    default



    if(keycode==KC_MACMODE)
    {
        if (record->event.pressed)
        {
            // //fn双击以后切到第6层
            //   if(default_layer == 1)
            //   {
            //      set_single_persistent_default_layer(2); //Sets the default layer and writes it to persistent memory (EEPROM)
            //   }
            //   else if(default_layer == 16)
            //   {
            //     set_single_persistent_default_layer(6);
            //   }

              set_single_persistent_default_layer(2);
              f_os_mode = 1;
              SET_FLAG( kb_screen.flag , PK_OS_TYPE );
        }
        return false;
    }


    if ((keycode == KC_SHUTRGB) && record->event.pressed )
    {
        shut_up_Flag = ! shut_up_Flag;
        if(!readPin(ARGB_LEFT_EN) && (shut_up_Flag == 0)) //主动关闭
        {
            setPinOutput(ARGB_LEFT_EN);
            writePinHigh(ARGB_LEFT_EN);
        }
        variable_data.eeconfig_shut_up_Flag = shut_up_Flag;
        eeconfig_update_kb_datablock(&variable_data, 0, EECONFIG_KB_DATA_SIZE);
    }


       //电量检测
    if(keycode == KC_BAT)
    {
        if (record->event.pressed)
        {
            Test_Battery_value = 1;
            Light_Count = temp/10 +1;
            if(Light_Count > 10)
            Light_Count = 10;
        }
        else
        {
            Test_Battery_value = 0;
            Light_Count = 0;
        }
    }






    //按键切换模式
     if( (keycode>=KC_BLE1) && (keycode<=KC_BLE3) && (kb_mode==KB_MODE_BLE))
    {
        flag32.link_break_flag =0;
        flag32.ble_report_flag = 0;
        if(last_wireless_mode != keycode-KC_USB)
        {
            if (record->event.pressed)
            {
                ble_mode = keycode-KC_USB;
                WIRELESS_START(ble_mode);
                blink_ble_24g_timer = timer_read32();
                long_ble_24g_timer=timer_read32();
                flag32.mode_one = 1;
                ble24G_period = 5;
            //   uprintf("problem2\r\n");
                ble24G_counter  = 0;
                pair_timeout = 20000;
              //  BT_Switch_Flag = 1;
                //解决蓝牙互切之间也可以不松手直接配对
                long_ble_24g_timer=timer_read32();
                flag32.press_ble_flag = 1;

            }
        }
        else
        {
            if (record->event.pressed)
            {
                long_ble_24g_timer=timer_read32();
                flag32.press_ble_flag = 1;
            }
            else
            {
                if(timer_elapsed32(long_ble_24g_timer) <= BLE_24G_TIMEOUT)
                {
                    flag32.press_ble_flag = 0;
                }
            }

        }
        return false;
    }

    if (keycode == KC_24G && (kb_mode==KB_MODE_24G))
    {
        {
            if(record->event.pressed)
            {
                long_ble_24g_timer = timer_read32();
                flag32.press_24G_flag = 1;

            }
            else
            {
                if (timer_elapsed32(long_ble_24g_timer) <= BLE_24G_TIMEOUT)    //长按广播配对
                {
                    flag32.press_24G_flag = 0;
                }
            }
        }

        return false;
    }

    if (keycode == KC_Mctl) {

        // uprintf("mctl\r\n");
        if (record->event.pressed)
            register_code16(KC_MCTL);
        else
            unregister_code16(KC_MCTL);
    }
    if (keycode == KC_Lpad) {

       //  uprintf("lpad\r\n");
        if (record->event.pressed)
            register_code16(KC_LPAD);
        else
            unregister_code16(KC_LPAD);
    }


//question

    if(((keycode==KC_WIN)) && record->event.pressed)
    {

        lock_win_flag = !lock_win_flag;

        SET_FLAG( kb_screen.flag , PK_WINLOCK_STATUS );
      //  dynamic_keymap_set_keycode(1, 2, 0,lock_win_flag);
        variable_data.eeconfig_lock_win_flag = lock_win_flag;
        eeconfig_update_kb_datablock(&variable_data, 0, EECONFIG_KB_DATA_SIZE);

    }




    if(record->event.pressed && ((keycode == KC_LGUI) || (keycode == KC_RGUI)))
    {
        if(default_layer == 1 || (default_layer == 16))  //第0层为1，第5层为16
        {
            if(lock_win_flag== 1)
            return true;
            else
            return false;

        }
    }


    switch(keycode)
    {
        case KC_RESET:
            if (record->event.pressed)  //恢复出厂设置
            {
                flag32.reset_flag = 1;
                long_press_reset_timer = timer_read32();  //按下按键时就给个标志位，开始计时
            }
            else                        //松手进入一次else
            {
                flag32.reset_flag = 0;
            }
        break;
        default:
            break;

    }
    return process_record_user(keycode, record);
}






bool encoder_update_kb(uint8_t index, bool clockwise) {
    // #ifdef Lock_Keyboard
    // if(!lock_keyboard)
    {
        // if (encode_toggle == 1) { /* First encoder */
        //     if (clockwise) {
        //         rgb_matrix_increase_val();
        //     } else {
        //         rgb_matrix_decrease_val();
        //     }
        // } else if (encode_toggle == 0) { /* Second encoder */
        //     if (clockwise) {
        //         tap_code(KC_VOLU);
        //     } else {
        //         tap_code(KC_VOLD);
        //     }
        // }
    }
// #endif

    return false;
}




 void three_mode(void)
{
    switch(kb_mode)
    {
        case KB_MODE_USB:
                        if (MK923_R_FLAG(Effect_Flag, Mode_Statue)) {
                            MK923_CLEAR_FLAG(Effect_Flag, Mode_Statue);
                         //   uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                        }

                        rgblight_set_layer_state(6,false);
                        rgblight_set_layer_state(3,false);
                        rgblight_set_layer_state(4,false);
                        rgblight_set_layer_state(5,false);
                        rgblight_set_layer_state(7,false);
                        rgblight_set_layer_state(8,false);

        break;

        case KB_MODE_BLE:

                rgblight_set_layer_state(7,false);
                rgblight_set_layer_state(8,false);
                if(!wireless_connected)
                {
                    if(timer_elapsed32(blink_ble_24g_timer) <= pair_timeout)
                    {
                        if (MK923_R_FLAG(Effect_Flag, Mode_Statue) == 0) {
                            MK923_SET_FLAG(Effect_Flag, Mode_Statue);
                           // uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                        }
                        switch (last_wireless_mode)
                        {
                            case 1:
                                 rgblight_set_layer_state(4,false);
                                 rgblight_set_layer_state(5,false);
                                if(ble24G_counter < ble24G_period)
                                {
                                    // if(BT_Switch_Flag)
                                    // rgb_matrix_set_color(52,0,255,0); //蓝牙回连绿色，配对蓝色
                                    // else
                                    // rgb_matrix_set_color(52,0,0,255);

                                    rgblight_set_layer_state(3,true);
                                    rgblight_set_layer_state(6,false);
                                }
                                else
                                {
                                     rgblight_set_layer_state(6,true);
                                     rgblight_set_layer_state(3,false);
                                    //rgb_matrix_set_color(52,0,0,0);
                                }
                            break;

                            case 2:
                                   rgblight_set_layer_state(3,false);
                                   rgblight_set_layer_state(5,false);
                                if(ble24G_counter < ble24G_period)
                                {
                                    rgblight_set_layer_state(6,false);
                                    rgblight_set_layer_state(4,true);;

                                }
                                else
                                {
                                     rgblight_set_layer_state(4,false);
                                     rgblight_set_layer_state(6,true);

                                }
                            break;

                            case 3:
                                 rgblight_set_layer_state(4,false);
                                 rgblight_set_layer_state(3,false);
                                if(ble24G_counter < ble24G_period)
                                {
                                    rgblight_set_layer_state(6,false);
                                    rgblight_set_layer_state(5,true);;
                                }
                                else
                                {
                                     rgblight_set_layer_state(5,false);
                                     rgblight_set_layer_state(6,true);

                                }
                            break;
                            default:
                            break;
                        }
                    }
                    else{
                        if (MK923_R_FLAG(Effect_Flag, Mode_Statue)) {
                            MK923_CLEAR_FLAG(Effect_Flag, Mode_Statue);
                           // uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                        }
                        rgblight_set_layer_state(6,false);
                        rgblight_set_layer_state(3,false);
                        rgblight_set_layer_state(4,false);
                        rgblight_set_layer_state(5,false);
                        POWER_EnterSleep();
                         BT_24G_Shine = 0;
                    }
                    pair_succeed_timer = timer_read32();
                    if (blink_num == 0) {
                      BT_24G_Shine = 1;
                    }
                }
                else
                {
                     rgblight_set_layer_state(6,false);
                    flag32.link_break_flag = 1;
                    if(timer_elapsed32(pair_succeed_timer) <= PAIR_SUCCEED_TIME)
                    {
                        if (MK923_R_FLAG(Effect_Flag, Mode_Statue) == 0) {
                            MK923_SET_FLAG(Effect_Flag, Mode_Statue);
                           // uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                        }
                          rgblight_set_layer_state(6,false);
                        switch (last_wireless_mode)
                        {
                            case 1:
                                     rgblight_set_layer_state(3,true);

                            break;

                            case 2:

                                      rgblight_set_layer_state(4,true);
                            break;

                            case 3:

                                      rgblight_set_layer_state(5,true);
                            break;
                            default:
                                break;
                        }
                        BT_24G_Shine = 1; //蓝牙没有连上
                    }
                    else
                    {
                        if (MK923_R_FLAG(Effect_Flag, Mode_Statue)) {
                            MK923_CLEAR_FLAG(Effect_Flag, Mode_Statue);
                          //  uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                        }
                         BT_24G_Shine = 0;
                        rgblight_set_layer_state(3,false);
                        rgblight_set_layer_state(4,false);
                        rgblight_set_layer_state(5,false);
                        rgblight_set_layer_state(6,false);
                        flag32.mode_one = 0;
                    }
                }
        break;

        case KB_MODE_24G:

                    rgblight_set_layer_state(6,false);
                    rgblight_set_layer_state(3,false);
                    rgblight_set_layer_state(4,false);
                    rgblight_set_layer_state(5,false);
                if(!wireless_connected)
                {
                    if (MK923_R_FLAG(Effect_Flag, Mode_Statue) == 0) {
                        MK923_SET_FLAG(Effect_Flag, Mode_Statue);
                       // uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                    }
                    if(timer_elapsed32(blink_ble_24g_timer) <= pair_timeout)
                    {
                        if(ble24G_counter < ble24G_period)
                        {
                                rgblight_set_layer_state(8,false);
                                rgblight_set_layer_state(7,true);
                        }
                        else
                        {
                               rgblight_set_layer_state(7,false);
                               rgblight_set_layer_state(8,true);
                        }
                    }
                    else{
                                if (MK923_R_FLAG(Effect_Flag, Mode_Statue)) {
                                    MK923_CLEAR_FLAG(Effect_Flag, Mode_Statue);
                                  //  uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                                }
                                rgblight_set_layer_state(7,false);
                                rgblight_set_layer_state(8,false);
                                POWER_EnterSleep();
                                BT_24G_Shine = 0;
                    }
                    pair_succeed_timer = timer_read32();
                    if (blink_num == 0) {
                      BT_24G_Shine = 1;
                    }
                }
                else
                {
                    flag32.link_break_flag = 1;
                    if(timer_elapsed32(pair_succeed_timer) <= PAIR_SUCCEED_TIME)
                    {
                        if (MK923_R_FLAG(Effect_Flag, Mode_Statue) == 0) {
                            MK923_SET_FLAG(Effect_Flag, Mode_Statue);
                          //  uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                        }
                          rgblight_set_layer_state(8,false);
                          rgblight_set_layer_state(7,true);
                    }
                    else
                    {
                        if (MK923_R_FLAG(Effect_Flag, Mode_Statue)) {
                            MK923_CLEAR_FLAG(Effect_Flag, Mode_Statue);
                          //  uprintf("Effect_Flag:%ld\r\n", Effect_Flag);
                        }
                        rgblight_set_layer_state(7, false);
                        rgblight_set_layer_state(8, false);
                        BT_24G_Shine = 0;
                    }

                   if(wakeup_pack24G)
                   {
                        wakeup_pack24G = 0;
                        tap_code(KC_F24);
                   }
                }
        break;
        default:
        break;
    }

}





void gpio_disable_init(void)
{
    setPinOutput(ARGB_LEFT_EN);
    writePinLow(ARGB_LEFT_EN);

    setPinOutput(LCD_SWITCH);
    writePinLow(LCD_SWITCH);
      //  setPinInput(ARGB_LEFT_EN);
    palSetLineMode(OSC_IN,PAL_MODE_INPUT_ANALOG);  //0.23ma
    palSetLineMode(OSC_OUT,PAL_MODE_INPUT_ANALOG);

    setPinOutput(C4);
    writePinLow(C4); //reset

        //测试是否可以减少屏幕的功耗
    setPinOutput(C10);   //tx
    writePinLow(C10);

    setPinOutput(C12);
    writePinLow(C12); //boot
}
//question
void gpio_init(void)
{
    //插入检测引脚
   setPinInput(PLUG_IN);
    //晶振引脚初始
   setPinInput(OSC_IN);
   setPinOutput(OSC_OUT);
   if (!Shut_Up_Screen) {
       setPinOutput(LCD_SWITCH);
       writePinHigh(LCD_SWITCH);
   }
   kb_screen.screen_boot_time = 1;
   kb_screen.screen_boot_step = 0;
    if(kb_mode==KB_MODE_USB)
    {
        Batt_Baifen = 100;
        SET_FLAG( kb_screen.flag , PK_BATT_QUANTITY);
    }
    else
    {
        Batt_Baifen = temp;
        SET_FLAG( kb_screen.flag , PK_BATT_QUANTITY);
    }

    setPinOutput(C4);
    writePinHigh(C4); //reset

    // CHECK ME IF IT WORKS
    palSetLineMode(C10, UART_TX_PAL_MODE);

    setPinOutput(C12);
    writePinLow(C12); //boot
}





void usb_suspend_power_down(void)
{
    sleep_switch_driverflag = 1;
    release_count = 4;
    cancel_deferred_exec(loop10hz_token);
    gpio_disable_init();
    setPinInput(BLE);
    setPinInput(TwoMode);
    palEnableLineEvent(BLE, PAL_EVENT_MODE_BOTH_EDGES);
    palEnableLineEvent(TwoMode, PAL_EVENT_MODE_BOTH_EDGES);

    setPinInput(encoder_left);
    setPinInput(encoder_right);
    palEnableLineEvent(encoder_left, PAL_EVENT_MODE_RISING_EDGE);
    palEnableLineEvent(encoder_right, PAL_EVENT_MODE_RISING_EDGE);
    #if (DIODE_DIRECTION == ROW2COL)

    pin_t col_pins[] = MATRIX_COL_PINS;

    for(uint8_t x=0; x<MATRIX_COLS; x++)
    {
        pin_t pin;
        pin = col_pins[x];
        if (pin != NO_PIN)
        {
            setPinOutput(pin);
            writePinLow(pin);

        }
    }


    const long unsigned int row_pins[] = MATRIX_ROW_PINS;
    for(uint8_t x=0; x<MATRIX_ROWS; x++)
    {
        pin_t pin;
        pin = row_pins[x];
        if (pin != NO_PIN)
        {
            setPinInputHigh(pin);
            palEnableLineEvent(pin, PAL_EVENT_MODE_BOTH_EDGES);
        }
    }
#else

    const long unsigned int row_pins[] = MATRIX_ROW_PINS;
    for(uint8_t x=0; x<MATRIX_ROWS; x++)
    {
        pin_t pin;
        pin = row_pins[x];
        if (pin != NO_PIN)
        {
            setPinOutput(pin);
            writePinLow(pin);
        }
    }

    const long unsigned int col_pins[] = MATRIX_COL_PINS;
    for(uint8_t x=0; x<MATRIX_COLS; x++)
    //for(uint8_t x=0; x<9; x++)
    {
        pin_t pin;
        pin = col_pins[x];
        setPinInputHigh(pin);
        palEnableLineEvent(pin, PAL_EVENT_MODE_BOTH_EDGES);
    }
#endif
   ADC1->CR2 &= ~ADC_CR2_ADON;
    _pal_lld_enablepadevent(0,18,PAL_EVENT_MODE_BOTH_EDGES);

    PWR->CR |= PWR_CR_CWUF | PWR_CR_CSBF;
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
     SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    #ifdef  mk856_special_isr
    special_key_debounce();
    #endif
//休眠防抖
//休眠防抖
    swif_debounce();
    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    EXTI->IMR &= ~(1<<18);
    EXTI->EMR &= ~(1<<18);
    EXTI->RTSR &= ~(1<<18);
    EXTI->FTSR &= ~(1<<18);
    EXTI->PR = (1<<18);
    setPinInput(BLE);
    setPinInput(TwoMode);
    stm32_clock_init();
    clear_keyboard();
    send_keyboard_report();
    usbWakeupHost(&USB_DRIVER);  //解决休眠无法唤醒的问题
    restart_usb_driver(&USB_DRIVER);
    init_usb_driver(&USB_DRIVER); //Should not enter SLEEP when USB mode
    matrix_init();
    adc_init();
    gpio_init();
    get_mode();
    if(kb_mode == KB_MODE_BLE)
    {
        WIRELESS_START(last_wireless_mode);
    }
    if(kb_mode == KB_MODE_24G)
    {
        WIRELESS_START(4);
    }



    usb_suspend_flag = 2;
    power_counter = 0;
    loop10hz_token = defer_exec(LOOP_10HZ_PERIOD, loop_10Hz, NULL);
}




void POWER_EnterSleep_First(void) {
    sleep_switch_driverflag = 1;
    cancel_deferred_exec(loop10hz_token);

    gpio_disable_init();
    usb_disconnect(); // Don't enter SLEEP when USB mode
    palSetLineMode(RENUM, PAL_MODE_INPUT_ANALOG);
    palSetLineMode(A11, PAL_MODE_INPUT_ANALOG);
    palSetLineMode(A12, PAL_MODE_INPUT_ANALOG);
    Sleep_First_PressFlag = 1;


    setPinInput(BLE);
    setPinInput(TwoMode);
    palEnableLineEvent(BLE, PAL_EVENT_MODE_BOTH_EDGES);
    palEnableLineEvent(TwoMode, PAL_EVENT_MODE_BOTH_EDGES);

    setPinInput(encoder_left);
    setPinInput(encoder_right);
    palEnableLineEvent(encoder_left, PAL_EVENT_MODE_RISING_EDGE);
    palEnableLineEvent(encoder_right, PAL_EVENT_MODE_RISING_EDGE);
#if (DIODE_DIRECTION == ROW2COL)

    pin_t col_pins[] = MATRIX_COL_PINS;

    for(uint8_t x=0; x<MATRIX_COLS; x++)
    {
        pin_t pin;
        pin = col_pins[x];
        if (pin != NO_PIN)
        {
            setPinOutput(pin);
            writePinLow(pin);
        }
    }

    const long unsigned int row_pins[] = MATRIX_ROW_PINS;
   for(uint8_t x=0; x<MATRIX_ROWS; x++)
    {
       pin_t pin;
       pin = row_pins[x];
        if (pin != NO_PIN)
        {
            setPinInputHigh(pin);
            palEnableLineEvent(pin, PAL_EVENT_MODE_FALLING_EDGE);
        }
    }
   setPinInput(PLUG_IN);
   palEnableLineEvent(PLUG_IN, PAL_EVENT_MODE_BOTH_EDGES);
#else

    const long unsigned int row_pins[] = MATRIX_ROW_PINS;
    for(uint8_t x=0; x<MATRIX_ROWS; x++)
    {
        pin_t pin;
        pin = row_pins[x];
        if (pin != NO_PIN)
        {
            setPinOutput(pin);
            writePinLow(pin);
        }
    }

    const long unsigned int col_pins[] = MATRIX_COL_PINS;
    for(uint8_t x=0; x<MATRIX_COLS; x++)
    //for(uint8_t x=0; x<9; x++)
    {
        pin_t pin;
        pin = col_pins[x];
        setPinInputHigh(pin);
        palEnableLineEvent(pin, PAL_EVENT_MODE_BOTH_EDGES);
    }
#endif


    ADC1->CR2 &= ~ADC_CR2_ADON;   //0.9ma
    //writePinLow(DRIVER_1_EN);
    /* going to anabiosis*/
    //chSysLock();
    PWR->CR |= (1<<0)|(1<< 10) |(1<< 11)|(3<<18);
    /* Clear Wake-up flag */
    PWR->CR |= PWR_CR_CWUF | PWR_CR_CSBF;

    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    //}
    /* Request Wait For Interrupt */
    __WFI();
     SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
     #ifdef  mk856_special_isr
    special_key_debounce();
#endif
//休眠防抖
//休眠防抖
    swif_debounce();
    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    setPinInput(BLE);
    setPinInput(TwoMode);

    stm32_clock_init();
    matrix_init();
    matrix_scan();

    adc_init();
    get_mode();
    first_sleep_wakeup_timer = timer_read32();
    rgb_wireless_timer = timer_read32();
     first_sleep_timer = timer_read32();
     packet_send = 0;
    //配对超时和回连超时计时清0

    blink_ble_24g_timer = 0; //蓝牙回连超时     配对也是这个
    pair_succeed_timer = timer_read32();//重新计时连接时间
    blink_ble_24g_timer = timer_read32(); //重新计时回连时间
    flag32.mode_one = 1;
    blink_counter  = 0;
    ble24G_counter = 0;
    power_counter = 0;
    loop10hz_token = defer_exec(LOOP_10HZ_PERIOD, loop_10Hz, NULL);
  }





void POWER_EnterSleep(void) {
    sleep_switch_driverflag = 1;
    cancel_deferred_exec(loop10hz_token);
    WIRELESS_STOPOWER();
    wait_ms(10);
    usb_disconnect();    //Don't enter SLEEP when USB mode
    gpio_disable_init();
    palSetLineMode(RENUM,PAL_MODE_INPUT_ANALOG);
    if(kb_mode == KB_MODE_24G)
    wakeup_pack24G = 1;

    if(get_plug_mode() == false)
    {
        palSetLineMode(A11,PAL_MODE_INPUT_ANALOG);
        palSetLineMode(A12,PAL_MODE_INPUT_ANALOG);
    }
//开关设置

    setPinInput(BLE);
    setPinInput(TwoMode);
    palEnableLineEvent(BLE, PAL_EVENT_MODE_BOTH_EDGES);
    palEnableLineEvent(TwoMode, PAL_EVENT_MODE_BOTH_EDGES);
    //旋钮唤醒
    setPinInput(encoder_left);
    setPinInput(encoder_right);
    palEnableLineEvent(encoder_left, PAL_EVENT_MODE_RISING_EDGE);
    palEnableLineEvent(encoder_right, PAL_EVENT_MODE_RISING_EDGE);
#if (DIODE_DIRECTION == ROW2COL)

    pin_t col_pins[] = MATRIX_COL_PINS;

    for(uint8_t x=0; x<MATRIX_COLS; x++)
    {
        pin_t pin;
        pin = col_pins[x];
        if (pin != NO_PIN)
        {
            setPinOutput(pin);
            writePinLow(pin);
        }
    }



    const long unsigned int row_pins[] = MATRIX_ROW_PINS;
    for(uint8_t x=0; x<MATRIX_ROWS; x++)
    {
        pin_t pin;
        pin = row_pins[x];
        if (pin != NO_PIN)
        {
            setPinInputHigh(pin);
            palEnableLineEvent(pin, PAL_EVENT_MODE_FALLING_EDGE);
        }
    }
    palEnableLineEvent(PLUG_IN, PAL_EVENT_MODE_BOTH_EDGES);
#else
    const long unsigned int row_pins[] = MATRIX_ROW_PINS;
    for(uint8_t x=0; x<MATRIX_ROWS; x++)
    {
        pin_t pin;
        pin = row_pins[x];
        if (pin != NO_PIN)
        {
            setPinOutput(pin);
            writePinLow(pin);
        }
    }

    const long unsigned int col_pins[] = MATRIX_COL_PINS;
    for(uint8_t x=0; x<MATRIX_COLS; x++)
    //for(uint8_t x=0; x<9; x++)
    {
        pin_t pin;
        pin = col_pins[x];
        setPinInputHigh(pin);
        palEnableLineEvent(pin, PAL_EVENT_MODE_BOTH_EDGES);
    }
#endif


    ADC1->CR2 &= ~ADC_CR2_ADON;   //0.9ma

    //writePinLow(DRIVER_1_EN);
    /* going to anabiosis*/
    //chSysLock();
    PWR->CR |= (1<<0)|(1<< 10) |(1<< 11)|(3<<18);
    /* Clear Wake-up flag */
    PWR->CR |= PWR_CR_CWUF | PWR_CR_CSBF;

    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    //}
    /* Request Wait For Interrupt */
    __WFI();
     SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
     //增加特殊按键消抖处理

#ifdef  mk856_special_isr
    special_key_debounce();
#endif
//休眠防抖

    swif_debounce();
    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;


    setPinInput(BLE);
    setPinInput(TwoMode);
    setPinOutput(RENUM);
    writePinHigh(RENUM);
    gpio_init();
    stm32_clock_init();
    init_usb_driver(&USB_DRIVER); //Should not enter SLEEP when USB mode                //Should not enter SLEEP when USB mode
    matrix_init();
    adc_init();


     get_mode();
    if(kb_mode == KB_MODE_BLE)
    {
        WIRELESS_START(last_wireless_mode);
    }
    if(kb_mode == KB_MODE_24G)
    {
        WIRELESS_START(4);
    }
    rgb_wireless_timer = timer_read32();
    first_sleep_timer = timer_read32();
    packet_send = 0;
    //配对超时和回连超时计时清0
    blink_ble_24g_timer = 0; //蓝牙回连超时     配对也是这个
    pair_succeed_timer = timer_read32();
    //重新计时连接时间
    blink_ble_24g_timer = timer_read32(); //重新计时回连时间
    flag32.mode_one = 1;
    blink_counter  = 0;
    ble24G_counter = 0;
    //低电
    ble24G_period = 5;
    pair_timeout = 20000;
  //  BT_Switch_Flag = 1;
    power_counter = 0;
    loop10hz_token = defer_exec(LOOP_10HZ_PERIOD, loop_10Hz, NULL);
 }



uint8_t sleep_first_flag;
uint16_t testime_flag;
void sleep_mode(void)
{

    //软关机休眠
    if((false == get_plug_mode() ) && (temp == 0 ) && bat_test_flag)
    {
        POWER_EnterSleep();
        BT_24G_Shine = 0;
    }
   //回连超时休眠
    if (!wireless_connected  &&((kb_mode == KB_MODE_BLE ) || (kb_mode == KB_MODE_24G) ) )
    {

        if(timer_elapsed32(blink_ble_24g_timer) >= pair_timeout)
        {
            //增加滅燈操作
            rgblight_set_layer_state(6,false);
            rgblight_set_layer_state(3,false);
            rgblight_set_layer_state(4,false);
            rgblight_set_layer_state(5,false);
            rgblight_set_layer_state(7,false);
            rgblight_set_layer_state(8,false);
            POWER_EnterSleep();
            BT_24G_Shine = 0;
        }
    }


    if (wireless_connected)   //DO IT WHEN WIRELESS MODE
    {
//测试如果改成10分钟  600s   580s
// If the test is changed to 10 minutes, 600s, 580s
        if(timer_elapsed32(first_sleep_timer) >= 8000 )  //&& (packet_send < 15)
        {
            first_sleep_timer = timer_read32();
            packet_send++;
            for (int i=0;i<5;i++)
            {
                uart_write(0x00);
            }
        }

        if(timer_elapsed32(rgb_wireless_timer) > RGB_MATRIX_WIRELESS_TIME)
        {
            suspend_count = 0;
            config_time_alarm();
            POWER_EnterSleep_First();
            BT_24G_Shine = 0;
            exti_stop_config();
            //如果唤醒源不是按键唤醒，才进去下面
            // If the wake-up source is not a button press, then proceed to the next step.
            if(exti_flag)
            {
                exti_flag = 0;
               POWER_EnterSleep();
               BT_24G_Shine = 0;
            }
            else
            {

              gpio_init();
              sleep_first_flag = 1;
            }
        }
        else if(timer_elapsed32(rgb_wireless_timer) > (RGB_MATRIX_WIRELESS_TIME - 200))
        {
            suspend_count = 1;
          //黑屏，防止休眠起来会卡顿
          // Black screen to prevent lag when waking up from sleep mode.
          // rgb_matrix_set_color_all(0,0,0);
            rgblight_set_layer_state(0,false);
            for(uint8_t i = 2;i < 12; i++)
            {
              rgblight_set_layer_state(i,false);
            }
           rgblight_set_layer_state(1,true);
        }
    }



    //有线跟随休眠
    if(USB_DRIVER.state == USB_SUSPENDED && kb_mode == KB_MODE_USB)
    {
        if(usb_suspend_flag == 0)   //USB之前时ACTIVE的状态
        {
            usb_suspend_time = timer_read32();
            usb_suspend_flag = 1;   //USB之前是ACTIVE,现在是SUSPENED
        }

    }
    else if(USB_DRIVER.state == USB_ACTIVE && kb_mode == KB_MODE_USB)
    {
        usb_suspend_flag = 0;   //USB是ACTIVE的状态
      //   usb_suspend_time = timer_read32();
    }
    else
    {
        usb_suspend_flag = 2;
    }

    if(timer_elapsed32(usb_suspend_time) > 500)
    {
       if(get_plug_mode() == true && kb_mode == KB_MODE_USB )
        {
            if(usb_suspend_flag == 1)
            {

                usb_suspend_power_down();
                BT_24G_Shine = 0;
                usb_wakeup_flag = true;
            }
        }
    }


    // if((blink_counter == 5) && (Sleep_First_PressFlag))
    // {
    //     Sleep_First_PressFlag = 0;
    //     init_usb_driver(&USB_DRIVER); //Should not enter SLEEP when USB mode
    // }


    //2.4G跟随休眠
    if(suspend == true)
    {
        suspend_count++;
        if(suspend_count == 2)
        {
            suspend_count =0;
            POWER_EnterSleep();
            BT_24G_Shine = 0;
            suspend = false;
        }
    }
}





void adc_test(void)
{

    if((timer_elapsed32(battery_test_time) > 30000))
    {
        adc_value = get_adc_value();    //获取adc值
        adc_vref =  get_adc_vref();
        battery_value = (adc_value*1764/adc_vref);
        temp = batt_level();
        battery_test_time = timer_read32();



    //电量一点点增加或者减少

        if(get_plug_mode() == true)     //插usb
        {
            if(temp > last_temp +1)     //暴涨超2%
            {
                second_temp = temp;
                if(last_temp != second_temp)
                {
                    last_temp++;
                    temp = last_temp;
                }
            }
            else if(last_temp == temp || temp == last_temp +1)  //电量正常不加或是只增长1%
            {
                last_temp = temp;
            }
        }
        else
        {
            if(last_temp == temp || temp == last_temp -1)   //电量正常不减少或是只减少1%
            {
                last_temp = temp;
            }
            else if(temp < last_temp -1)    //暴跌超2%
            {
                second_temp = temp;
                if(last_temp != second_temp)
                {
                    last_temp--;
                    temp = last_temp;
                }
            }
            else if(temp > last_temp)   //这是开灯测的电量，突然关灯会导致adc值暴涨
            {
                temp = last_temp;
            }
        }
            if(kb_mode==KB_MODE_USB)
            {
              Batt_Baifen = 100;
              SET_FLAG( kb_screen.flag , PK_BATT_QUANTITY);
            }
            else
            {
                Batt_Baifen = temp;
                SET_FLAG( kb_screen.flag , PK_BATT_QUANTITY);
            }

    }
}
void ble_send_batt(void)
{
    if(kb_mode==KB_MODE_BLE && wireless_connected) //no battery info when 24G or connecting
    {
        if(ble_flag == 0  && !power_count_flag)
        {

             battery_Flag = 1;
            if(!keyboard_Idle)
            {
              battery_Flag = 0;
              sc_ble_battary(temp);

            }


            ble_flag = 1;
            ble_bat_timer = timer_read32();
        }

        if(timer_elapsed32(ble_bat_timer) > 30000)
        {
            if(get_plug_mode() == true)     //插usb
            {
                if(temp > last_temp +1)     //暴涨超2%
                {
                    second_temp = temp;
                    if(last_temp != second_temp)
                    {
                        last_temp++;
                        temp = last_temp;
                    }
                }
                else if(last_temp == temp || temp == last_temp +1)  //电量正常不加或是只增长1%
                {
                    last_temp = temp;
                }
            }
            else
            {
                if(last_temp == temp || temp == last_temp -1)   //电量正常不减少或是只减少1%
                {
                    last_temp = temp;
                }
                else if(temp < last_temp -1)    //暴跌超2%
                {
                    second_temp = temp;
                    if(last_temp != second_temp)
                    {
                        last_temp--;
                        temp = last_temp;
                    }
                }
                else if(temp > last_temp)   //这是开灯测的电量，突然关灯会导致adc值暴涨
                {
                    temp = last_temp;
                }
            }
             battery_Flag = 1;
            if(!keyboard_Idle)
            {
              battery_Flag = 0;
              sc_ble_battary(temp);
            }

            ble_bat_timer = timer_read32();
        }
    }
}





static uint16_t count_flag;
uint8_t Rtc_Config_Api(void)
{
   rcu_backup_clock_Enable();//备份区域的时钟使能
   rcu_power_clock_Enable();//电源管理时钟使能
   backup_write_Enable();//备份区允许访问
  // bkp_reset();//备份域复位
   rcu_inlowclock_on();//使能内部低速时钟
 //  wait_inlowclock_sta();//等待内部时钟稳定
   while(!(RCC->CSR  & RCC_BDCR_LSERDY) &&  count_flag<250)
   {
     count_flag++;
     wait_ms(10);
   }
   if(count_flag >=250)
   {
    return 1;
   }
   rcu_clock_source();  //RTC时钟源选择
   RCU_Clock_enable();//使能RTC时钟
   rtc_register_sync_wait();//等待寄存器域APB1时钟同步
    RTC->CRH |= RTC_CRH_ALRIE;
    while(!(RTC->CRL & RTC_CRL_RTOFF));  //RTC operation OFF
    RTC->CRL |= RTC_CRL_CNF;       //Configuration Flag
    RTC->CRL |= RTC_CRL_CNF;
    RTC->PRLL =  0x9C3F;//0x9C3F//0x9C3F;//0x9C3F;  //RTC Prescaler Reload Value Low
    RTC->CRL &= ~RTC_CRL_CNF;       //clear reset Flag
    while(!(RTC->CRL & RTC_CRL_RTOFF));  //RTC operation OFF
    rcu_backup_clock_Enable();//备份区域的时钟使能
    rcu_power_clock_Enable();//电源管理时钟使能
    backup_write_Enable();//备份区允许访问
    RTC->CRL |= RTC_CRL_CNF;       //Configuration Flag
    RTC->CNTL = 0x00;
    RTC->CRL &= ~RTC_CRL_CNF;
    while(!(RTC->CRL & RTC_CRL_RTOFF));  //RTC operation OFF
    rcu_backup_clock_Enable();//备份区域的时钟使能
    rcu_power_clock_Enable();//电源管理时钟使能
    backup_write_Enable();//备份区允许访问
    RTC->CRL |= RTC_CRL_CNF;       //Configuration Flag
    RTC->ALRH = 0;
    RTC->ALRL = Second_level_sleeptime;//0x483;//0x483;//5分钟//0x474;//0x4b0;   //时间时15s
    RTC->CRL &= ~RTC_CRL_CNF;
    while(!(RTC->CRL & RTC_CRL_RTOFF));  //RTC operation OFF
    RTC->CRL &= ~RTC_CRL_CNF;
    exti_stop_config();
    return 0;
}



uint8_t  isr_specal_Trig;
//休眠防抖
// Sleep stabilization
void swif_debounce(void)
{
    uint8_t isr_source[MATRIX_ROWS];
    uint8_t i;
    const long unsigned int row_pins[] = MATRIX_ROW_PINS;
    //休眠防抖处理,前提是中断源是按键唤醒
    // Sleep debouncing requires that the interrupt source is a button wake-up.
    if(isr_Trig)
    {
        while(1)
        {
            wait_ms(5);
            //读6次row口的电平
            for(uint8_t j = 0; j< 10; j++)
            {
                for(i = 0; i < MATRIX_ROWS; i++)
                {
                    isr_source[i] = readPin(row_pins[i]);
                    if(!isr_source[i])
                    break;
                }
                if(i < MATRIX_ROWS)
                {
                    break;
                }
            }


            if(i < MATRIX_ROWS)
            {
                break;
            }
            else
            {
                pin_t col_pins[] = MATRIX_COL_PINS;
                for(uint8_t x=0; x<MATRIX_COLS; x++)
                {
                    pin_t pin;
                    pin = col_pins[x];
                    if (pin != NO_PIN)
                    {
                        setPinOutput(pin);
                        writePinLow(pin);
                    }
                }


                for(uint8_t x=0; x<MATRIX_ROWS; x++)
                {
                    pin_t pin;
                    pin = row_pins[x];
                    if (pin != NO_PIN)
                    {
                        setPinInputHigh(pin);
                        palEnableLineEvent(pin, PAL_EVENT_MODE_FALLING_EDGE);
                    }
                }
                PWR->CR |= (1<<0)|(1<< 10) |(1<< 11)|(3<<18);
                PWR->CR |= PWR_CR_CWUF | PWR_CR_CSBF;
                SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
                __WFI();
                SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

            }
        }
          //  test_count = 100;
    }

    EXTI->IMR  &= ~0xffff;
    EXTI->EMR  &= ~0xffff;
    EXTI->RTSR &= ~0xffff;
    EXTI->FTSR &= ~0xffff;
    isr_Trig = 0;
}

uint8_t  ap_message_buff[64];
// #ifdef Module_Updata
bool via_command_kb_(uint8_t *data, uint8_t length) {
    static uint16_t packcount = 0;
    uint8_t         modele_updata[66];

#ifdef Screen_AP
//	uint32_t	data_addr;
    uint8_t		data_len;
    memcpy(ap_message_buff, data, length);
   	data_len	= ap_message_buff[3];
	//data_addr 	= 0;
	// data_addr	= ap_message_buff[3];
	// data_addr	= (data_addr<<8)|ap_message_buff[2];
	ap_message_buff[6] = 0x55;
#endif


    //  uprintf("enter here4\r\n");

    if (data[0] == 0x0a) {
        switch (data[1]) {
            case 0x55: // connect
                for (uint8_t i = 0; i < 60; i++) {
                    uart_write(0x00);
                }
                wait_ms(5);
                modele_updata[0] = 0x55;
                modele_updata[1] = 0x40;
                for (uint8_t i = 2; i < 66; i++) {
                    modele_updata[i] = data[i - 2];
                }
                uart_transmit(modele_updata, sizeof(modele_updata));
                wait_ms(3);
                break;

            case 0x20: // disable EP1
                modele_updata[0] = 0x55;
                modele_updata[1] = 0x40;
                for (uint8_t i = 2; i < 66; i++) {
                    modele_updata[i] = data[i - 2];
                }
                uart_transmit(modele_updata, sizeof(modele_updata));
                wait_ms(3);
                break;

            case 0x00:
                for (uint8_t i = 0; i < 60; i++) {
                    uart_write(0x00);
                }
                wait_ms(5);
                modele_updata[0] = 0x55;
                modele_updata[1] = 0x40;
                for (uint8_t i = 2; i < 66; i++) {
                    modele_updata[i] = data[i - 2];
                }
                uart_transmit(modele_updata, sizeof(modele_updata));
                wait_ms(3);
                break;

            case 0x01: // check version
                modele_updata[0] = 0x55;
                modele_updata[1] = 0x40;
                for (uint8_t i = 2; i < 66; i++) {
                    modele_updata[i] = data[i - 2];
                }
                uart_transmit(modele_updata, sizeof(modele_updata));
                wait_ms(3);
                break;

            case 0x02: // DFU start
                packcount = 0;

                modele_updata[0] = 0x55;
                modele_updata[1] = 0x40;
                for (uint8_t i = 2; i < 30; i++) {
                    modele_updata[i] = data[i - 2];
                }
                uart_transmit(modele_updata, sizeof(modele_updata));
                wait_ms(3);
                break;

            case 0x03:
                packcount        = 0;
                modele_updata[0] = 0x55;
                modele_updata[1] = 0x40;
                for (uint8_t i = 2; i < 66; i++) {
                    modele_updata[i] = data[i - 2];
                }
                uart_transmit(modele_updata, sizeof(modele_updata));
                wait_ms(3);
                break;

            case 0x04:
                modele_updata[0] = 0x55;
                modele_updata[1] = 0x40;
                for (uint8_t i = 2; i < 66; i++) {
                    modele_updata[i] = data[i - 2];
                }
                uart_transmit(modele_updata, sizeof(modele_updata));
                wait_ms(3);
                break;

            case 0x10:
                //打印发下来的数据
                // if( ( (((packcount+1)*62)/256) >   ((packcount*62)/256)  )  || ((packcount+1)*62 >= bin_size)   )
                // {
                //     data[2]=((packcount*62)/256)%256;
                //     data[3]=((packcount*62)/256)/256;
                //     raw_hid_send(data, length);
                // }
                packcount++;
                //透传module数据，增加2个字节
                //在串口接收处拆掉2个字节，上传pc
                modele_updata[0] = 0x55;
                modele_updata[1] = 0x40;
                for (uint8_t i = 2; i < 66; i++) {
                    modele_updata[i] = data[i - 2];
                }
                uart_transmit(modele_updata, sizeof(modele_updata));
                wait_ms(3);
                break;

            default:
                if(data[1] <= 0x40)
                {
                    modele_updata[0] = 0x55;
                    modele_updata[1] = 0x40;
                    for (uint8_t i = 2; i < 66; i++) {
                        modele_updata[i] = data[i - 2];
                    }
                    uart_transmit(modele_updata, sizeof(modele_updata));
                    wait_ms(3);
                    break;
                }
                else
                {
                    return false;
                }
        }

        return true;
    }

#ifdef Screen_AP
    else if(ap_message_buff[0]  == 0x40  || (ap_message_buff[0]  == 0x41) || (ap_message_buff[0]  == 0x42  ) )  //ap report id
    {
       // raw_hid_send(ap_message_buff, sizeof(ap_message_buff));
        // return true;
         switch(ap_message_buff[0])	//cmd
        {
                case 	AP_W_SCREEN_INFO:
                       rgb_wireless_timer  = timer_read32();
                    if( R_FLAG( kb_screen.flag , PK_MOD_SEM )!=NEED_UPDATE ){
                         sdWrite(&SD3,  ap_message_buff+7,data_len);
                         SET_FLAG( kb_screen.flag , PK_MOD_SEM );
                         kb_screen.ap_countdown = 100;
                        ap_message_buff[6] = 0x55;
                    }
                    else{
                        ap_message_buff[6] = 0x0f;
                        // ap_message_buff[6] = 0x0f;
                    }

                    raw_hid_send(ap_message_buff, sizeof(ap_message_buff));
                    // for(uint32_t i=0 ; i< sizeof(ap_message_buff) ; i++ ){
		            //     uprintf("%x\r\n",ap_message_buff[i]);
                    // }


                    break;

            case 	AP_W_SCREEN_DATA:
                       rgb_wireless_timer  = timer_read32();
                    if( R_FLAG( kb_screen.flag , PK_MOD_SEM )==NEED_UPDATE ){
                         kb_screen.ap_countdown = 100;
                         sdWrite(&SD3,  ap_message_buff+7,data_len);
                         ap_message_buff[6] = 0x55;
                         wait_us(5);
                    }
                    else{
                        ap_message_buff[6] = 0x0f;
                        // ap_message_buff[6] = 0x0f;
                    }

                     raw_hid_send(ap_message_buff, sizeof(ap_message_buff));
                    //  setPinOutput(ARGB_LEFT_EN);
                    //  writePinLow(ARGB_LEFT_EN);

                    break;
            case 	AP_GIVE_SCREEN_SEM:
                     rgb_wireless_timer  = timer_read32();
                    if( R_FLAG( kb_screen.flag , PK_MOD_SEM )==NEED_UPDATE ){
                        CLEAR_FLAG( kb_screen.flag , PK_MOD_SEM );
                        ap_message_buff[6] = 0x55;
                    }
                    else{
                        ap_message_buff[6] = 0x0f;
                    }

                    raw_hid_send(ap_message_buff, sizeof(ap_message_buff));

                    break;



                    default:
                    ap_message_buff[6] = 0x55;

                    break;


        }
       return true;

    }

#endif



    return false;
}
// #endif

void housekeeping_task_user(void) {
#ifdef Module_Updata
    //增加module升级
    Module_UpdataHandle();
#endif

#ifdef Screen_AP_UPDATE
    Mcu_Rec_Date();
#endif

#ifdef  LCD_Driver
    //增加屏控接收数据
    keyboard_screen_process();
#endif
}

//作为临时定时器使用    周期为1ms
#ifdef Screen_AP
void matrix_scan_kb (void)
{
    time_50ms_count++;
   	if(time_50ms_count >= 50) //50ms
    {
        // if(kb_screen.screen_boot_step==0){
        //     kb_screen.screen_boot_time++;
        // }
        if( R_FLAG( kb_screen.flag , PK_MOD_SEM )==NEED_UPDATE ){
            if( kb_screen.ap_countdown!=0 ){
                kb_screen.ap_countdown--;
            }
            else{
                CLEAR_FLAG( kb_screen.flag , PK_MOD_SEM );
            }
        }
     time_50ms_count = 0;
    }
}

#endif

// DEPRECATED - DO NOT USE

#define CS1_SW1 SW1_CS1
#define CS2_SW1 SW1_CS2
#define CS3_SW1 SW1_CS3
#define CS4_SW1 SW1_CS4
#define CS5_SW1 SW1_CS5
#define CS6_SW1 SW1_CS6
#define CS7_SW1 SW1_CS7
#define CS8_SW1 SW1_CS8
#define CS9_SW1 SW1_CS9
#define CS10_SW1 SW1_CS10
#define CS11_SW1 SW1_CS11
#define CS12_SW1 SW1_CS12
#define CS13_SW1 SW1_CS13
#define CS14_SW1 SW1_CS14
#define CS15_SW1 SW1_CS15
#define CS16_SW1 SW1_CS16
#define CS17_SW1 SW1_CS17
#define CS18_SW1 SW1_CS18

#define CS1_SW2 SW2_CS1
#define CS2_SW2 SW2_CS2
#define CS3_SW2 SW2_CS3
#define CS4_SW2 SW2_CS4
#define CS5_SW2 SW2_CS5
#define CS6_SW2 SW2_CS6
#define CS7_SW2 SW2_CS7
#define CS8_SW2 SW2_CS8
#define CS9_SW2 SW2_CS9
#define CS10_SW2 SW2_CS10
#define CS11_SW2 SW2_CS11
#define CS12_SW2 SW2_CS12
#define CS13_SW2 SW2_CS13
#define CS14_SW2 SW2_CS14
#define CS15_SW2 SW2_CS15
#define CS16_SW2 SW2_CS16
#define CS17_SW2 SW2_CS17
#define CS18_SW2 SW2_CS18

#define CS1_SW3 SW3_CS1
#define CS2_SW3 SW3_CS2
#define CS3_SW3 SW3_CS3
#define CS4_SW3 SW3_CS4
#define CS5_SW3 SW3_CS5
#define CS6_SW3 SW3_CS6
#define CS7_SW3 SW3_CS7
#define CS8_SW3 SW3_CS8
#define CS9_SW3 SW3_CS9
#define CS10_SW3 SW3_CS10
#define CS11_SW3 SW3_CS11
#define CS12_SW3 SW3_CS12
#define CS13_SW3 SW3_CS13
#define CS14_SW3 SW3_CS14
#define CS15_SW3 SW3_CS15
#define CS16_SW3 SW3_CS16
#define CS17_SW3 SW3_CS17
#define CS18_SW3 SW3_CS18

#define CS1_SW4 SW4_CS1
#define CS2_SW4 SW4_CS2
#define CS3_SW4 SW4_CS3
#define CS4_SW4 SW4_CS4
#define CS5_SW4 SW4_CS5
#define CS6_SW4 SW4_CS6
#define CS7_SW4 SW4_CS7
#define CS8_SW4 SW4_CS8
#define CS9_SW4 SW4_CS9
#define CS10_SW4 SW4_CS10
#define CS11_SW4 SW4_CS11
#define CS12_SW4 SW4_CS12
#define CS13_SW4 SW4_CS13
#define CS14_SW4 SW4_CS14
#define CS15_SW4 SW4_CS15
#define CS16_SW4 SW4_CS16
#define CS17_SW4 SW4_CS17
#define CS18_SW4 SW4_CS18

#define CS1_SW5 SW5_CS1
#define CS2_SW5 SW5_CS2
#define CS3_SW5 SW5_CS3
#define CS4_SW5 SW5_CS4
#define CS5_SW5 SW5_CS5
#define CS6_SW5 SW5_CS6
#define CS7_SW5 SW5_CS7
#define CS8_SW5 SW5_CS8
#define CS9_SW5 SW5_CS9
#define CS10_SW5 SW5_CS10
#define CS11_SW5 SW5_CS11
#define CS12_SW5 SW5_CS12
#define CS13_SW5 SW5_CS13
#define CS14_SW5 SW5_CS14
#define CS15_SW5 SW5_CS15
#define CS16_SW5 SW5_CS16
#define CS17_SW5 SW5_CS17
#define CS18_SW5 SW5_CS18

#define CS1_SW6 SW6_CS1
#define CS2_SW6 SW6_CS2
#define CS3_SW6 SW6_CS3
#define CS4_SW6 SW6_CS4
#define CS5_SW6 SW6_CS5
#define CS6_SW6 SW6_CS6
#define CS7_SW6 SW6_CS7
#define CS8_SW6 SW6_CS8
#define CS9_SW6 SW6_CS9
#define CS10_SW6 SW6_CS10
#define CS11_SW6 SW6_CS11
#define CS12_SW6 SW6_CS12
#define CS13_SW6 SW6_CS13
#define CS14_SW6 SW6_CS14
#define CS15_SW6 SW6_CS15
#define CS16_SW6 SW6_CS16
#define CS17_SW6 SW6_CS17
#define CS18_SW6 SW6_CS18

#define CS1_SW7 SW7_CS1
#define CS2_SW7 SW7_CS2
#define CS3_SW7 SW7_CS3
#define CS4_SW7 SW7_CS4
#define CS5_SW7 SW7_CS5
#define CS6_SW7 SW7_CS6
#define CS7_SW7 SW7_CS7
#define CS8_SW7 SW7_CS8
#define CS9_SW7 SW7_CS9
#define CS10_SW7 SW7_CS10
#define CS11_SW7 SW7_CS11
#define CS12_SW7 SW7_CS12
#define CS13_SW7 SW7_CS13
#define CS14_SW7 SW7_CS14
#define CS15_SW7 SW7_CS15
#define CS16_SW7 SW7_CS16
#define CS17_SW7 SW7_CS17
#define CS18_SW7 SW7_CS18

#define CS1_SW8 SW8_CS1
#define CS2_SW8 SW8_CS2
#define CS3_SW8 SW8_CS3
#define CS4_SW8 SW8_CS4
#define CS5_SW8 SW8_CS5
#define CS6_SW8 SW8_CS6
#define CS7_SW8 SW8_CS7
#define CS8_SW8 SW8_CS8
#define CS9_SW8 SW8_CS9
#define CS10_SW8 SW8_CS10
#define CS11_SW8 SW8_CS11
#define CS12_SW8 SW8_CS12
#define CS13_SW8 SW8_CS13
#define CS14_SW8 SW8_CS14
#define CS15_SW8 SW8_CS15
#define CS16_SW8 SW8_CS16
#define CS17_SW8 SW8_CS17
#define CS18_SW8 SW8_CS18

#define CS1_SW9 SW9_CS1
#define CS2_SW9 SW9_CS2
#define CS3_SW9 SW9_CS3
#define CS4_SW9 SW9_CS4
#define CS5_SW9 SW9_CS5
#define CS6_SW9 SW9_CS6
#define CS7_SW9 SW9_CS7
#define CS8_SW9 SW9_CS8
#define CS9_SW9 SW9_CS9
#define CS10_SW9 SW9_CS10
#define CS11_SW9 SW9_CS11
#define CS12_SW9 SW9_CS12
#define CS13_SW9 SW9_CS13
#define CS14_SW9 SW9_CS14
#define CS15_SW9 SW9_CS15
#define CS16_SW9 SW9_CS16
#define CS17_SW9 SW9_CS17
#define CS18_SW9 SW9_CS18

#define CS1_SW10 SW10_CS1
#define CS2_SW10 SW10_CS2
#define CS3_SW10 SW10_CS3
#define CS4_SW10 SW10_CS4
#define CS5_SW10 SW10_CS5
#define CS6_SW10 SW10_CS6
#define CS7_SW10 SW10_CS7
#define CS8_SW10 SW10_CS8
#define CS9_SW10 SW10_CS9
#define CS10_SW10 SW10_CS10
#define CS11_SW10 SW10_CS11
#define CS12_SW10 SW10_CS12
#define CS13_SW10 SW10_CS13
#define CS14_SW10 SW10_CS14
#define CS15_SW10 SW10_CS15
#define CS16_SW10 SW10_CS16
#define CS17_SW10 SW10_CS17
#define CS18_SW10 SW10_CS18

#define CS1_SW11 SW11_CS1
#define CS2_SW11 SW11_CS2
#define CS3_SW11 SW11_CS3
#define CS4_SW11 SW11_CS4
#define CS5_SW11 SW11_CS5
#define CS6_SW11 SW11_CS6
#define CS7_SW11 SW11_CS7
#define CS8_SW11 SW11_CS8
#define CS9_SW11 SW11_CS9
#define CS10_SW11 SW11_CS10
#define CS11_SW11 SW11_CS11
#define CS12_SW11 SW11_CS12
#define CS13_SW11 SW11_CS13
#define CS14_SW11 SW11_CS14
#define CS15_SW11 SW11_CS15
#define CS16_SW11 SW11_CS16
#define CS17_SW11 SW11_CS17
#define CS18_SW11 SW11_CS18

#define CS1_SW12 SW12_CS1
#define CS2_SW12 SW12_CS2
#define CS3_SW12 SW12_CS3
#define CS4_SW12 SW12_CS4
#define CS5_SW12 SW12_CS5
#define CS6_SW12 SW12_CS6
#define CS7_SW12 SW12_CS7
#define CS8_SW12 SW12_CS8
#define CS9_SW12 SW12_CS9
#define CS10_SW12 SW12_CS10
#define CS11_SW12 SW12_CS11
#define CS12_SW12 SW12_CS12
#define CS13_SW12 SW12_CS13
#define CS14_SW12 SW12_CS14
#define CS15_SW12 SW12_CS15
#define CS16_SW12 SW12_CS16
#define CS17_SW12 SW12_CS17
#define CS18_SW12 SW12_CS18

#define aw_led aw20216s_led_t
#define g_aw_leds g_aw20216s_leds

const aw_led g_aw_leds[RGB_MATRIX_LED_COUNT] = {
    {0, CS1_SW1, CS2_SW1, CS3_SW1},    //14   esc
    {0, CS1_SW2, CS2_SW2, CS3_SW2},
    {0, CS1_SW3, CS2_SW3, CS3_SW3},
    {0, CS1_SW4, CS2_SW4, CS3_SW4},
    {0, CS1_SW5, CS2_SW5, CS3_SW5},
    {0, CS1_SW6, CS2_SW6, CS3_SW6},
    {0, CS1_SW7, CS2_SW7, CS3_SW7},
    {0, CS1_SW8, CS2_SW8, CS3_SW8},
    {1, CS1_SW1, CS2_SW1, CS3_SW1},
    {1, CS1_SW2, CS2_SW2, CS3_SW2},
    {1, CS1_SW3, CS2_SW3, CS3_SW3},
    {1, CS1_SW4, CS2_SW4, CS3_SW4},
    {1, CS1_SW5, CS2_SW5, CS3_SW5},
    {1, CS1_SW6, CS2_SW6, CS3_SW6},



    {0, CS4_SW1, CS5_SW1, CS6_SW1},    //15  ~
    {0, CS4_SW2, CS5_SW2, CS6_SW2},
    {0, CS4_SW3, CS5_SW3, CS6_SW3},
    {0, CS4_SW4, CS5_SW4, CS6_SW4},
    {0, CS4_SW5, CS5_SW5, CS6_SW5},
    {0, CS4_SW6, CS5_SW6, CS6_SW6},
    {0, CS4_SW7, CS5_SW7, CS6_SW7},
    {0, CS4_SW8, CS5_SW8, CS6_SW8},
    {1, CS4_SW1, CS5_SW1, CS6_SW1},
    {1, CS4_SW2, CS5_SW2, CS6_SW2},
    {1, CS4_SW3, CS5_SW3, CS6_SW3},
    {1, CS4_SW4, CS5_SW4, CS6_SW4},
    {1, CS4_SW5, CS5_SW5, CS6_SW5},
    {1, CS4_SW6, CS5_SW6, CS6_SW6},
    {1, CS4_SW7, CS5_SW7, CS6_SW7},


    {0, CS7_SW1, CS8_SW1, CS9_SW1},    //15   tab
    {0, CS7_SW2, CS8_SW2, CS9_SW2},
    {0, CS7_SW3, CS8_SW3, CS9_SW3},
    {0, CS7_SW4, CS8_SW4, CS9_SW4},
    {0, CS7_SW5, CS8_SW5, CS9_SW5},
    {0, CS7_SW6, CS8_SW6, CS9_SW6},
    {0, CS7_SW7, CS8_SW7, CS9_SW7},
    {0, CS7_SW8, CS8_SW8, CS9_SW8},
    {1, CS7_SW1, CS8_SW1, CS9_SW1},
    {1, CS7_SW2, CS8_SW2, CS9_SW2},
    {1, CS7_SW3, CS8_SW3, CS9_SW3},
    {1, CS7_SW4, CS8_SW4, CS9_SW4},
    {1, CS7_SW5, CS8_SW5, CS9_SW5},
    {1, CS7_SW6, CS8_SW6, CS9_SW6},
    {1, CS7_SW7, CS8_SW7, CS9_SW7},


    {0, CS10_SW1, CS11_SW1, CS12_SW1}, //14   caps
    {0, CS10_SW2, CS11_SW2, CS12_SW2},
    {0, CS10_SW3, CS11_SW3, CS12_SW3},
    {0, CS10_SW4, CS11_SW4, CS12_SW4},
    {0, CS10_SW5, CS11_SW5, CS12_SW5},
    {0, CS10_SW6, CS11_SW6, CS12_SW6},
    {0, CS10_SW7, CS11_SW7, CS12_SW7},
    {0, CS10_SW8, CS11_SW8, CS12_SW8},
    {1, CS10_SW1, CS11_SW1, CS12_SW1},
    {1, CS10_SW2, CS11_SW2, CS12_SW2},
    {1, CS10_SW3, CS11_SW3, CS12_SW3},
    {1, CS10_SW4, CS11_SW4, CS12_SW4},
    // {1, CS10_SW5, CS11_SW5, CS12_SW5},
    {1, CS10_SW6, CS11_SW6, CS12_SW6},

    {0, CS13_SW1, CS14_SW1, CS15_SW1},  //lshift  14
    // {0, CS13_SW2, CS14_SW2, CS15_SW2},
    {0, CS13_SW3, CS14_SW3, CS15_SW3},
    {0, CS13_SW4, CS14_SW4, CS15_SW4},
    {0, CS13_SW5, CS14_SW5, CS15_SW5},
    {0, CS13_SW6, CS14_SW6, CS15_SW6},
    {0, CS13_SW7, CS14_SW7, CS15_SW7},
    {0, CS13_SW8, CS14_SW8, CS15_SW8},
    {1, CS13_SW1, CS14_SW1, CS15_SW1},
    {1, CS13_SW2, CS14_SW2, CS15_SW2},
    {1, CS13_SW3, CS14_SW3, CS15_SW3},
    {1, CS13_SW4, CS14_SW4, CS15_SW4},
    {1, CS13_SW5, CS14_SW5, CS15_SW5},
    {1, CS13_SW6, CS14_SW6, CS15_SW6},


    {0, CS16_SW1, CS17_SW1, CS18_SW1}, //12   lctrl
    {0, CS16_SW2, CS17_SW2, CS18_SW2},
    {0, CS16_SW3, CS17_SW3, CS18_SW3},
    {0, CS16_SW7, CS17_SW7, CS18_SW7},
    {0, CS16_SW8, CS17_SW8, CS18_SW8},
    {1, CS16_SW1, CS17_SW1, CS18_SW1},
    {1, CS16_SW2, CS17_SW2, CS18_SW2},
    {1, CS16_SW3, CS17_SW3, CS18_SW3},
    {1, CS16_SW4, CS17_SW4, CS18_SW4},
    {1, CS16_SW5, CS17_SW5, CS18_SW5},
    {1, CS16_SW6, CS17_SW6, CS18_SW6},
    {1, CS16_SW7, CS17_SW7, CS18_SW7}
};
