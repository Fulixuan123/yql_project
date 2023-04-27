
#ifndef __DEBUG_H
#define __DEBUG_H

#include "main.h"

#define M_NUM 2
#define EN_CRC             0

#define FRAME_HEAD   0xC5       /* 帧头 */
#define FRAME_END    0x5C       /* 帧尾 */
#define RX_LEN            32



typedef enum
{
    R_STATUS      			= 0x01,
  	R_SPEED       			= 0x02,
    R_INC_POSITIONING  		= 0x03,
    R_ABS_POSITIONING       = 0x04,

}receive_type;

/* 锟斤拷锟阶刺� */
typedef enum
{
    R_Enable               = 0x01,
    R_Disabel              = 0x02,
    R_Forward              = 0x03,
    R_Backward             = 0x04,
    R_RelF                 = 0x05,
    R_RelB                 = 0x06,
	R_Stop                 = 0x07,      /* 停止 */
}receive_state;


typedef struct
{
	int16_t  R_speed;
    uint32_t R_length;
    uint32_t R_abslength;                 /* 绝对定位的长度 */
    uint8_t  R_status;
}receive_data;



/* 锟斤拷锟斤拷锟斤拷锟� */
typedef enum
{
    S_STATUS         = 0x01,
    S_SPEED          = 0x02,
	S_POS            = 0x03,
    S_ERR            = 0x04,
}send_type;

/* 锟斤拷锟阶刺� */
typedef enum
{
    S_Enable               = 0x01,
    S_Disabel              = 0x02,
    S_Forward              = 0x03,
    S_Backward             = 0x04,
}send_state;


typedef struct
{
     uint8_t S_status;
	 uint16_t S_speed;
     int64_t S_position;
	 uint8_t S_err;
}send_data;



void debug_handle(void);

void debug_init(void);
void debug_send_state(uint8_t state, uint8_t type);
void debug_send_speed( uint16_t speed, uint8_t type);
void debug_send_pos(int64_t position, uint8_t type);
void debug_send_err(uint8_t err, uint8_t type);

uint8_t GetTargetStatus(uint8_t type);


uint16_t GetTargetSpeed(uint8_t type);


uint64_t GetTargetPosition(uint8_t type);


#endif
