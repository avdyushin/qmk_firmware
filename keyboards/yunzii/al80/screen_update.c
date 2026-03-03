#include "keyboard_screen.h"
#include "uart_mod.h"
#include "hygui.h"
#include "quantum.h"
#include "uart.h"
#include "smart_ble.h"
#include "keyboard.h"
#include "print.h"
#include "usb_main.h"
#include "usb_util.h"
#include "usb_driver.h"
#include "adc.h"
#include "os_detection.h"
#include "raw_hid.h"
#include "action_util.h"
#include "common.h"
#include "rtc.h"
#include "screen_update.h"
#include "hal.h"


APISP_T  apisp = {
    .ap_step = ISP_GET_CURRENT_VERSION
    // .ap_crc,

 };


 #define SCREEN_PROJECT_SIZE 0xD848
 #define SCREEN_PROJECT_ADDR 0x08000000

#define ISP_ACK             0x79
#define ISP_NACK            0x1f

#define  RST_PIN C4
#define  BOOT_PIN C12
 HYISP_T hyisp = {
      .step = ISP_GET_CURRENT_VERSION,
 };
  uint8_t sync_command[1] = { 0x7f };
  uint8_t global_erase_command[2] = { 0x43,0xbc };
  uint8_t global_erase_ff00[2] = { 0xff,0x00 };
  uint8_t program_command[2] = { 0x31,0xce };
  uint8_t program_command_test[2] = { 0x01,0xFE };
  uint8_t program_command_test2[2] = { 0x00,0xFF };
  uint8_t program_command_test3[2] = { 0x02,0xFD };
//   uint8_t program_command_test3[2] = { 0x01,0xFD };


  static SerialConfig screen_uart_config = {
    .speed = 115200,
     .cr1 = 0x1400,
};

  void Screen_Enter_Boot(void)
  {
    sdStop(&SD3);
    wait_ms(1);
    sdStart(&SD3, &screen_uart_config);
    wait_ms(50);


    setPinOutput(BOOT_PIN);
    writePinHigh(BOOT_PIN); //
    setPinOutput(RST_PIN);
    writePinLow(RST_PIN); //
    wait_ms(500);
    writePinHigh(RST_PIN); //
    wait_ms(10);


  }

void isp_send( uint32_t data_size , uint8_t *data ){
    // if( R_FLAG( kb_screen.kb_flag , SCREEN_OVERTIME )==FLAG_SET ){
    //     CLEAR_FLAG( kb_screen.kb_flag , SCREEN_OVERTIME );
    //     hyisp.err_num++;
    //     if( hyisp.err_num>3 ){
    //         printk("\n SCREEN_OVERTIME");
    //         CLEAR_FLAG( kb_screen.kb_flag , SCREEN_WAIT );
    //         hyisp.step = ISP_INIT;
    //       //  SET_FLAG( kb_screen.kb_flag , SCREEN_ISP_UPDATA );
    //     }
    // }
    // if( uart_mod_dma_send(data_size,data)==MOD_OK ){
    //     CLEAR_FLAG( kb_screen.kb_flag , SCREEN_RESEND );
    //     SET_FLAG( kb_screen.kb_flag , SCREEN_WAIT );
    //     kb_screen.countdown = 20;
    //     return MOD_OK;
    // }
    // else{
    //     if( R_FLAG( kb_screen.kb_flag , SCREEN_RESEND )==FLAG_SET ){
    //         hyisp.err_num++;
    //         if( hyisp.err_num>3 ){ printk("\n SCREEN_RESEND");hyisp.step = ISP_INIT; }
    //     }
    //     else{
    //         SET_FLAG( kb_screen.kb_flag , SCREEN_RESEND );
    //     }
    // }
    // return MOD_ERR;
    sdWrite(&SD3,  data,data_size);
    //设置超时机制
   // SET_FLAG( kb_screen.flag , SCREEN_WAIT );
    kb_screen.ap_countdown = 20;

}


 void isp_send_by_check(uint32_t data_size,uint8_t *data){
    uint8_t check_byte=0x00;
    for( uint16_t i=0 ; i<data_size ; i++ ){
        check_byte ^= data[i];
        hyisp.buff[i] = data[i];
    }
    hyisp.buff[data_size] = check_byte;
    isp_send(data_size+1,hyisp.buff);
}


 void isp_global_erase(void){
    switch ( hyisp.comm_step )
    {
    case ISP_COMMAND_STEP1:

    //先验证同步码值
        isp_send( 1,sync_command );
      //  isp_send( 2,global_erase_command );
        break;
    // case ISP_COMMAND_STEP2:
    //     isp_send( 2,global_erase_ff00 );

        break;
    default:
        break;
    }
}




void isp_program(void){
    // if( R_FLAG( kb_screen.kb_flag , SCREEN_OVERTIME )==FLAG_SET ){
    //     hyisp.step = ISP_REINIT;
    //     hyisp.comm_step = ISP_COMMAND_STEP1;
    //    // SET_FLAG( kb_screen.kb_flag , SCREEN_ISP_UPDATA );
    //     return;
    // }

    switch (hyisp.comm_step) {
        case ISP_COMMAND_STEP1:
            isp_send(2, program_command);
            break;

        case ISP_COMMAND_STEP2:
            hyisp.cur_addr_array[0] = (hyisp.cur_addr >> 3 * 8) & 0xFF;
            hyisp.cur_addr_array[1] = (hyisp.cur_addr >> 2 * 8) & 0xFF;
            hyisp.cur_addr_array[2] = (hyisp.cur_addr >> 1 * 8) & 0xFF;
            hyisp.cur_addr_array[3] = (hyisp.cur_addr >> 0 * 8) & 0xFF;
            isp_send_by_check(4, hyisp.cur_addr_array);
            break;

        case ISP_COMMAND_STEP3:
            isp_text_handle();
            break;

        default:
            break;
    }
}

void isp_text_handle(void){
    uint16_t send_size;
    uint8_t check_byte;

    if( hyisp.remain_size>ISP_TEXT_GAP_SIZE ){
        send_size = ISP_TEXT_GAP_SIZE-1;
    }
    else{
        send_size = hyisp.remain_size-1;
    }

    hyisp.buff[0] = send_size;
    check_byte = send_size^0x00;
    send_size = send_size+1;

    for( uint16_t i=0 ; i<send_size ; i++ ){
        // check_byte ^= screen_project[ hyisp.cur_offset+i ];   //question
        // hyisp.buff[i+1] = screen_project[ hyisp.cur_offset+i ];
    }
    hyisp.buff[send_size+1] = check_byte;

    hyisp.last_send_size = send_size;
    isp_send(send_size+2,hyisp.buff);
}


void isp_exit(void){
    // if( R_FLAG( kb_screen.kb_flag , SCREEN_OVERTIME )==FLAG_SET ){
    //     CLEAR_FLAG( kb_screen.kb_flag , SCREEN_OVERTIME );
    //     hyisp.comm_step++;
    // }
    switch ( hyisp.comm_step )
    {
        case ISP_COMMAND_STEP1:
            // GPIO_PIN_ADDR( 4 , 0 ) = 0;
            // GPIO_PIN_ADDR( 1 , 1 ) = 0;
        break;

        case ISP_COMMAND_STEP2:
            // GPIO_PIN_ADDR( 1 , 1 ) = 1;
            // kb_screen.countdown = 10;
         //   SET_FLAG( kb_screen.flag , SCREEN_WAIT );
        break;

        case ISP_COMMAND_STEP3:
            hyisp.step = ISP_GET_CURRENT_VERSION;
            screen_uart_config.speed = 921600;
            sdStart(&SD3, &screen_uart_config);
            wait_ms(50);
            // kb_screen.boot_step = SCREEN_BOOT_STEP;
            // kb_screen.boot_time = 0;
            // kb_screen.next_trigger_time=SCREEN_BOOT_SYNC_GAP;
        break;
    }
}


//这里只在ap发送数据的地方调用
// void Ap_Update_Process(uint8_t *Ap_Date, uint8_t length)
 void Ap_Update_Process(uint8_t *Ap_Date, uint8_t length)
{
    uint8_t ap_date[64];

    memcpy(ap_date, Ap_Date, length);
    switch (apisp.ap_step)
    {
        case ISP_GET_CURRENT_VERSION:
        //验证版本号
        ap_date[7] = 0x00;
        ap_date[8] = 0x01;
        raw_hid_send(ap_date, length);
        //第一步直接延时接收
        wait_us(5);
        apisp.ap_step = ISP_GO_BOOT;
            break;
        case ISP_GO_BOOT:

            raw_hid_send(ap_date, length);
            apisp.ap_step = ISP_GET_DEVICE_STATUE;
            break;
        case ISP_GET_DEVICE_STATUE:
            ap_date[7] = 0x00;
            raw_hid_send(ap_date, length);
            apisp.ap_step = ISP_UPDATE_CONFIRM;
            break;
        case ISP_UPDATE_CONFIRM:
            hyisp.cur_offset  = 0;
            hyisp.cur_addr    = (ap_date[9] << 24) |(ap_date[10] << 16)  | ( ap_date[11] << 8) |  (ap_date[12]);  //SCREEN_PROJECT_ADDR;
            hyisp.remain_size =  (ap_date[13] << 24) | (ap_date[14] << 16)  | ( ap_date[15] << 8) | (ap_date[16]);;
            //复位
            Screen_Enter_Boot();
            /*  带升级固件版本号待做*/
            apisp.ap_crc =   (ap_date[17] << 24) | (ap_date[18] << 16)  | ( ap_date[19] << 8) | (ap_date[20]);
            ap_date[7] = 0x00;
            raw_hid_send(ap_date, length);
            break;
        case ISP_START_UPDATE:
             isp_global_erase();
            break;
        case ISP_TRANS_DATA:
             isp_program();
            break;
        case ISP_TRANS_CHECKSUM:
            if( hyisp.remain_size == 0)
            {
                ap_date[7] = 0x00;
                raw_hid_send(ap_date, length);
                apisp.ap_step =ISP_END;
            }

            break;
        case ISP_END:
            isp_exit();
            break;

        default:
            break;
    }



}





#ifdef Screen_AP_UPDATE
void Mcu_Rec_Date(void)
{
    uint8_t Rec_Data;
    uint8_t ap_date[64];
    uint8_t length;

    //c
    if (!sdGetWouldBlock(&SD3)) {   //接收到数据     增加超时机制
        Rec_Data = sdGet(&SD3);
        uprintf("Rec_Data ----  %x\r\n",Rec_Data);

        switch (apisp.ap_step) {
            case ISP_START_UPDATE:
                if (Rec_Data == ISP_ACK) {
                    if (hyisp.comm_step != ISP_COMMAND_STEP2) {
                        hyisp.comm_step++;
                    } else {
                        hyisp.comm_step = ISP_COMMAND_STEP1;
                        hyisp.step      = ISP_TRANS_DATA;
                        // 上报数据
                        ap_date[0] = CMD_TRANS_DATA;
                        ap_date[6] = 0x55;
                        raw_hid_send(ap_date, length);
                    }
                } else {
                    hyisp.step = ISP_START_UPDATE;
                    ap_date[0] = CMD_START_UPDATE;
                    ap_date[6] = 0x0F;
                    raw_hid_send(ap_date, length);
                }
                break;

            case ISP_TRANS_DATA:
            if( Rec_Data==ISP_ACK ){
                if( hyisp.comm_step!=ISP_COMMAND_STEP3 ){
                    hyisp.comm_step++;
                }
                else{
                    hyisp.remain_size = hyisp.remain_size-hyisp.last_send_size;
                    hyisp.cur_offset = hyisp.cur_offset+hyisp.last_send_size;
                    hyisp.cur_addr = hyisp.cur_addr+hyisp.last_send_size;
                    if( hyisp.remain_size!=0 ){
                        hyisp.comm_step=ISP_COMMAND_STEP1;
                    }
                    else{
                        hyisp.comm_step = ISP_COMMAND_STEP1;
                        hyisp.step = ISP_TRANS_CHECKSUM;
                    }
                }
            }
            else{
                hyisp.step = ISP_START_UPDATE;
            }
                break;

            case ISP_TRANS_CHECKSUM:

                break;

            case ISP_END:

                break;

            default:
                break;
        }
    }
}
#endif
