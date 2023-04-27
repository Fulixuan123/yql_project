#ifndef _PID_H
#define	_PID_H

#include "stm32f0xx.h"
#include "main.h"

typedef struct _PID//PID参数结构体
{
    float kp,ki,kd;
    float err,lastErr;
    float integral,maxIntegral; //积分值
    float output,maxOutput;
}PID;



void PidInit(void);

float PidControlSpeed(PID* pid, float targetSpeed, float currSpeed);

float PidControlLocation(PID* pid,float target,float feedback);

void StopPidCalc(int8_t type);

void Motor_Contorl(uint8_t type);

#endif
