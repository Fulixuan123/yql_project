#ifndef __DJ_CON_H
#define __DJ_CON_H

#include "main.h"

#define ENCODER_TIM htim2
#define PWM_TIM     htim3
#define GAP_TIM     htim1

typedef struct _Motor
{
    int lastCount;   //��һ�μ���ֵ
    int totalCount;  //�ܼ���ֵ
    int overflowNum; //�������
    float speed;         //���ת��
    uint8_t direct;      //��ת����
}Motor;

void ControlDjStatus(int status, int type);


void Motor_Init(void);

#endif
