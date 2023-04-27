#ifndef __DJ_CON_H
#define __DJ_CON_H

#include "main.h"

#define ENCODER_TIM htim2
#define PWM_TIM     htim3
#define GAP_TIM     htim1

typedef struct _Motor
{
    int lastCount;   //上一次计数值
    int totalCount;  //总计数值
    int overflowNum; //溢出次数
    float speed;         //电机转速
    uint8_t direct;      //旋转方向
}Motor;

void ControlDjStatus(int status, int type);


void Motor_Init(void);

#endif
