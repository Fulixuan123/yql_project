#include "dj_pid.h"
#include "bsp_debug.h"
#include "tim.h"
#include "dj_con.h"
#include "string.h"

enum para
{
	KP = 0,
	KI,
	KD
};
#define PID_PARA_NUM 3
#define GRATING_PERIOD 20 // 光栅最小分辨率 单位um
#define PID_CALC_TIME 20
#define SPEED_RECORD_NUM 20 // 经测试，50Hz个采样值进行滤波的效果比较好
#define RELOAD_VALUE __HAL_TIM_GetAutoreload(&htim2)

float M0_speed_Record[SPEED_RECORD_NUM]={0};
float M1_speed_Record[SPEED_RECORD_NUM]={0};

unsigned char g_filterFlag;
float g_pidSpeedPara[PID_PARA_NUM * 2] = {1.0, 0.0, 0.0, 1.0, 0.0, 0.0}; //Xkp, Xki, Xkd Ykp, Yki, Ykd
float g_pidPosPara[PID_PARA_NUM * 2] = {1.0, 0.0, 0.0, 1.0, 0.0, 0.0}; //Xkp, Xki, Xkd Ykp, Yki, Ykd

PID pid_speed[M_NUM];
PID pid_position[M_NUM];



//void Motor_Contorl(TIM_HandleTypeDef *htim);

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//
//    Motor_Contorl(htim);//我把测速和PID封装成一个函数了
//}

void PidInit(void){
	for(int i = 0; i < M_NUM; i++){
		pid_speed[i].err = 0;
		pid_speed[i].integral = 0;
		pid_speed[i].maxIntegral = 20000;
		pid_speed[i].maxOutput = __HAL_TIM_GetAutoreload(&htim3);
		pid_speed[i].lastErr = 0;
		pid_speed[i].output = 0;
		pid_speed[i].kp = g_pidSpeedPara[KP + 3*i];
		pid_speed[i].ki = g_pidSpeedPara[KI + 3*i];
		pid_speed[i].kd = g_pidSpeedPara[KD + 3*i];

		pid_position[i].err = 0;
		pid_position[i].integral = 0;
		pid_position[i].maxIntegral = 20000;
		pid_position[i].maxOutput = __HAL_TIM_GetAutoreload(&htim3);
		pid_position[i].lastErr = 0;
		pid_position[i].output = 0;
		pid_position[i].kp = g_pidPosPara[KP + 3*i];
		pid_position[i].ki = g_pidPosPara[KI + 3*i];
		pid_position[i].kd = g_pidPosPara[KD + 3*i];

	}


}

/*
 * 进行速度的平均滤波
 * 输入新采样到的速度，存放速度的数组，
 * 返回滤波后的速度
 */
float Speed_Low_Filter(float new_Spe,float *speed_Record)
{
    float sum = 0.0f, filter_speed;
    if(g_filterFlag < SPEED_RECORD_NUM){
    	for(uint8_t i=SPEED_RECORD_NUM-1;i>0;i--)//将现有数据后移一位
		{
			speed_Record[i] = speed_Record[i-1];
			sum += speed_Record[i];
		}
    	g_filterFlag++;
    	speed_Record[0] = new_Spe;//第一位是新的数据
		sum += new_Spe;
		filter_speed = (float)(sum/g_filterFlag);
    } else if(g_filterFlag >= SPEED_RECORD_NUM){
    	for(uint8_t j=SPEED_RECORD_NUM-1; j>0; j--)//将现有数据后移一位
		{
			speed_Record[j] = speed_Record[j-1];
			sum += speed_Record[j];
		}
		speed_Record[0] = new_Spe;//第一位是新的数据
		sum += new_Spe;
		filter_speed = (float)(sum/SPEED_RECORD_NUM);
    }

    return filter_speed;//返回均值
}


/* get speed data to calc */
float PidControlSpeed(PID* pid, float targetSpeed, float currSpeed){
	pid->err = targetSpeed	- currSpeed;

    if(pid->err < 100 && pid->err > -100) pid->err = 0;//pid死区
    pid->integral += pid->err;

    if(pid->ki * pid->integral < -pid->maxIntegral) {
    	pid->integral = -pid->maxIntegral / pid->ki;//积分限幅
    }
    else if(pid->ki * pid->integral > pid->maxIntegral)
    	pid->integral = pid->maxIntegral / pid->ki;

    if(targetSpeed == 0) pid->integral = 0; // 刹车时清空i

    pid->output = (pid->kp * pid->err) + (pid->ki * pid->integral) + (pid->kd * (pid->err - pid->lastErr));//全量式PID

    //输出限幅
    if(targetSpeed >= 0)//正转时
    {
        if(pid->output < 0) pid->output = 0;
        else if(pid->output > pid->maxOutput) pid->output = pid->maxOutput;
    }
    else if(targetSpeed < 0)//反转时
    {
        if(pid->output < -pid->maxOutput) pid->output = -pid->maxOutput;
        else if(pid->output > 0) pid->output = 0;
    }

    pid->lastErr = pid->err;
    if(targetSpeed == 0) pid->output = 0; // 刹车时直接输出0
    return pid->output;

}

/* get position data to calc */
float PidControlLocation(PID* pid,float target,float feedback)
{

    pid->err = target - feedback;
     if(pid->err < 100 && pid->err > -100)
    	 pid->err = 0;//pid死区
    pid->integral += pid->err;

    if(pid->ki * pid->integral < -pid->maxIntegral) pid->integral = -pid->maxIntegral / pid->ki;//积分限幅
    else if(pid->ki * pid->integral > pid->maxIntegral) pid->integral = pid->maxIntegral / pid->ki;

    pid->output = (pid->kp * pid->err) + (pid->ki * pid->integral) + (pid->kd * (pid->err - pid->lastErr));//全量式PID

    //输出限幅
    if(pid->output > pid->maxOutput) pid->output = pid->maxOutput;
    if(pid->output < -pid->maxOutput) pid->output = -pid->maxOutput;

    pid->lastErr = pid->err;

    return pid->output;
}



extern uint8_t g_djEnableFlag[M_NUM];
static int timeCount;
extern Motor motor[M_NUM];
extern send_data S_M[M_NUM];

void Motor_Contorl(uint8_t type)
{
	static uint16_t Target_Speed = 0;
	float Target_duty;
	uint64_t Target_Position = 0;

	if(type == 0) {
		motor[type].totalCount = ((int)__HAL_TIM_GetCounter(&htim2) + ((int)__HAL_TIM_GetAutoreload(&htim2) * motor[type].overflowNum)) * GRATING_PERIOD;

		motor[type].speed = (motor[type].totalCount - motor[type].lastCount) / PID_CALC_TIME;  // speed ms

		motor[type].speed = Speed_Low_Filter(motor[type].speed, M0_speed_Record);

	} else {
		motor[type].totalCount = ((int)__HAL_TIM_GetCounter(&htim3) + ((int)__HAL_TIM_GetAutoreload(&htim3) * motor[type].overflowNum)) * GRATING_PERIOD;
		motor[type].speed = (motor[type].totalCount - motor[type].lastCount) / PID_CALC_TIME;

		motor[type].speed = Speed_Low_Filter(motor[type].speed, M1_speed_Record);
	}
	S_M[type].S_speed = motor[type].speed;
	S_M[type].S_position = motor[type].totalCount;
	motor[type].lastCount = motor[type].totalCount;

	Target_Position = GetTargetPosition(type);
	//Target_Speed = GetTargetSpeed(type);

	if(g_djEnableFlag[type] == 0)
		return;

	if(timeCount++ % 2 == 0){
		Target_Speed = PidControlLocation(&pid_position[type],Target_Position, motor[type].totalCount);//位置环 Target_Position是目标位置，自行定义即可
	}


	Target_duty = PidControlSpeed(&pid_speed[type], Target_Speed, motor[type].speed);//速度环

    if(Target_duty > 0)
    {
    	if(type == 0) {
            __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, RELOAD_VALUE);
            __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, RELOAD_VALUE - Target_duty);
    	} else if(type == 1) {
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1, RELOAD_VALUE);
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_2, RELOAD_VALUE - Target_duty);
    	}
    }
    else
    {
    	if(type == 0) {
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, RELOAD_VALUE);
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, RELOAD_VALUE + Target_duty);
		} else if(type == 1) {
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_2, RELOAD_VALUE);
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1, RELOAD_VALUE + Target_duty);
    	}
    }
    if (timeCount >= 9999){
    	timeCount = 0;
    }
    return;
}

void StopPidCalc(int8_t type){
	memset(M0_speed_Record, 0,sizeof(float)*SPEED_RECORD_NUM);
	memset(M1_speed_Record, 0,sizeof(float)*SPEED_RECORD_NUM);
	__HAL_TIM_SetCounter(&htim2, 0);
	__HAL_TIM_SetCounter(&htim3, 0);
	for(int i = 0; i < M_NUM; i++){
		pid_speed[i].err = 0;
		pid_speed[i].integral = 0;
		pid_speed[i].lastErr = 0;
		pid_speed[i].output = 0;

		pid_position[i].err = 0;
		pid_position[i].integral = 0;
		pid_position[i].lastErr = 0;
		pid_position[i].output = 0;

	}
	g_djEnableFlag[type] = 0;
	// memset(motor, 0,sizeof(motor));
	// memset(pid_speed, 0,sizeof(pid_speed));
	// memset(pid_position, 0,sizeof(pid_speed));
}





