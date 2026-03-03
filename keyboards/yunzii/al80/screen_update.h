#pragma once


#include "quantum.h"













typedef struct {
    uint8_t ap_step;
    uint8_t  ap_crc;
}APISP_T;


#define   CMD_GET_CURRENT_VERSION     0xC0
#define   CMD_GO_BOOT                 0xC1
#define   CMD_GET_DEVICE_STATUE       0xC2
#define   CMD_UPDATE_CONFIRM          0xC3
#define   CMD_START_UPDATE            0xC4
#define   CMD_TRANS_DATA              0xC5
#define   CMD_TRANS_CHECKSUM          0xC6
#define   CMD_END                     0xC7



enum{
    ISP_GET_CURRENT_VERSION,
    ISP_GO_BOOT,
    ISP_GET_DEVICE_STATUE,
    ISP_UPDATE_CONFIRM,
    ISP_START_UPDATE,
    ISP_TRANS_DATA,
    ISP_TRANS_CHECKSUM,
    ISP_END
};



extern APISP_T  apisp;
extern uint8_t sync_command[1];
extern uint8_t program_command_test[2];
extern uint8_t program_command_test2[2];
extern uint8_t program_command_test3[2];
extern uint8_t global_erase_command[2];

#define ISP_TEXT_GAP_SIZE       8

enum{
    ISP_UPDATA,
    ISP_SEND_OVERTIME,
};



enum{
    ISP_COMMAND_STEP1,
    ISP_COMMAND_STEP2,
    ISP_COMMAND_STEP3,
    ISP_COMMAND_STEP4,
};

typedef struct{
    uint8_t step;
    uint8_t comm_step;
    uint32_t write_addr;
    uint32_t err_num;
    uint32_t remain_size;
    uint32_t cur_addr;
    uint8_t cur_addr_array[4];
    uint32_t cur_offset;
    uint32_t last_send_size;
    uint8_t buff[16];
}HYISP_T;







 void Ap_Update_Process(uint8_t *Ap_Date, uint8_t length);
 void isp_global_erase(void);
 void isp_program(void);
 void isp_exit(void); 
 void isp_text_handle(void);
 void isp_send_by_check(uint32_t data_size,uint8_t *data);
 void isp_send( uint32_t data_size , uint8_t *data );
 void Mcu_Rec_Date(void);
 void Screen_Enter_Boot(void);
//void Ap_Update_Process(void);