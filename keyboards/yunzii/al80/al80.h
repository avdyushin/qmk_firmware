#pragma once


#include "quantum.h"


/************************默认图层定义************************/
// #define _LAYER_WINDOWS DF(0)
// #define _LAYER_IOS     DF(2)

/************************端口定义************************/







#define _LAYER_WINDOWS DF(0)
#define _LAYER_IOS     DF(2)





extern uint32_t Effect_Flag;

/************************标志定义************************/
typedef struct
{
  	uint32_t link_break_flag : 1;   //无线模式连接上了断开
	uint32_t adc_one_flag : 1;		//只测一次adc值标志            
	uint32_t ble_report_flag : 1;	//刚上电只蓝牙上报一次电量
	uint32_t light_flag : 1;		//亮完模式灯效才可以亮充电检测灯效
	uint32_t sleep_flag : 1;		//软关机
	uint32_t scroll_flag : 1;		//scroll按键灯标志
	uint32_t power_look_flag : 1;	//电量查询标志效标志
	uint32_t reset_flag : 1;		//复位标志
	uint32_t reset_light : 1;		//复位灯效标志
	uint32_t backlight : 1;				//锁住win标志
	uint32_t mode_one : 1;			//按键灯效只进一次	
	uint32_t low_power_flag : 1;	
	uint32_t begin_low_power : 1;
	uint32_t press_24G_flag : 1;	
	uint32_t press_ble_flag : 1;
	uint32_t short_ble_flag : 1;
	uint32_t usb_suspend_flag : 1;
	uint32_t power_low_flag : 1;
	uint32_t caps_flag : 1;
	uint32_t lock_win_app_flag : 1;
	uint32_t short_24_flag : 1;
	uint32_t bit22 : 1;
	uint32_t bit23 : 1;
	uint32_t bit24 : 1;
	uint32_t bit25 : 1;
	uint32_t bit26 : 1;
	uint32_t bit27 : 1;
	uint32_t bit28 : 1;
	uint32_t bit29 : 1;
	uint32_t bit30 : 1;
	uint32_t bit31 : 1;
} kb_flag32_t;


extern uint8_t lock_win_flag;



enum kb_mode_t {
  KB_MODE_USB=0,
  KB_MODE_BLE,
  KB_MODE_24G,
  KB_MODE_DEFALT
};


void swif_debounce(void);
uint8_t Rtc_Config_Api(void);
void adc_test(void);
void ble_send_batt(void);
void POWER_EnterSleep_First(void);
void POWER_EnterSleep(void);
void sleep_mode(void);
void gpio_disable_init(void);
void gpio_init(void);
void three_mode(void);
bool get_plug_mode(void);          
enum kb_mode_t get_kb_mode(void);  

