#include "dj_con.h"
#include "bsp_debug.h"
#include "stm32f0xx_hal.h"
#include "tim.h"
#include "dj_pid.h"

#define DJ_DUTY 500
#define RELOAD_VALUE __HAL_TIM_GetAutoreload(&htim1)
extern receive_data R_M[M_NUM];
extern send_data S_M[M_NUM];
Motor motor[M_NUM];

void Motor_Init(void)
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2);      //开启编码器定时器
    __HAL_TIM_ENABLE_IT(&htim2,TIM_IT_UPDATE);           //开启编码器定时器更新中断,防溢出处理

    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);      //开启编码器定时器
    __HAL_TIM_ENABLE_IT(&htim3,TIM_IT_UPDATE);           //开启编码器定时器更新中断,防溢出处理

    //HAL_TIM_Base_Start_IT(&htim14);                       //开启20ms定时器中断

    // start pwm wave output

    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_2);

	HAL_TIM_Base_Start_IT(&htim14);                       //开启20ms定时器中断

    // init  motor
    for (uint8_t i = 0; i < M_NUM ;i++){
        motor[i].lastCount = 0;                                   //结构体内容初始化
        motor[i].totalCount = 0;
        motor[i].overflowNum = 0;
        motor[i].speed = 0;
        motor[i].direct = 0;
    }

}



uint8_t g_djEnableFlag[M_NUM] = {0, 0};

void ControlDjStatus(int status, int type){
	if(type < 0 || type > 1){
		return;
	}

	if(type == 0){
		switch (status){
			
		case R_Enable:
			HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);

			g_djEnableFlag[type] = 1;
			S_M[type].S_status = S_Enable;
			break;

		case R_Disabel:
			HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);
			StopPidCalc(type);
			S_M[type].S_status = S_Disabel;
			break;

		case R_Forward:
			if (HAL_GPIO_ReadPin(limit_x_up_GPIO_Port, limit_x_up_Pin) == GPIO_PIN_RESET){
				break;
			}
			g_djEnableFlag[type] = 0;
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, RELOAD_VALUE);
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, RELOAD_VALUE - DJ_DUTY);

			S_M[type].S_status = S_Forward;
			break;

		case R_Backward:
			if (HAL_GPIO_ReadPin(limit_x_down_GPIO_Port, limit_x_down_Pin) == GPIO_PIN_RESET){
				break;
			}
			g_djEnableFlag[type] = 0;
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, RELOAD_VALUE);
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, RELOAD_VALUE - DJ_DUTY);

			S_M[type].S_status = S_Backward;
			break;

		case R_RelF:
			if (HAL_GPIO_ReadPin(limit_x_up_GPIO_Port, limit_x_up_Pin) == GPIO_PIN_RESET){
				break;
			}
			R_M[type].R_abslength = motor[type].totalCount + R_M[type].R_length;
			g_djEnableFlag[type] = 1;
//			HAL_TIM_Base_Start_IT(&htim14);                       //开启20ms定时器中断
			S_M[type].S_status = S_Forward;
//			if (motor[type].totalCount <= R_M[type].R_abslength) {
//				g_djEnableFlag[type] = 1;
//				HAL_TIM_Base_Start_IT(&htim14);                       //开启20ms定时器中断
//				S_M[type].S_status = S_Forward;
//			} else {
//				HAL_GPIO_WritePin(Y_positive_GPIO_Port,Y_positive_Pin,GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(Y_negative_GPIO_Port,Y_negative_Pin,GPIO_PIN_RESET);
//				StopPidCalc(type);
//				S_M[type].S_status = S_Enable;
//			}
			break;

		case R_RelB:
			if (HAL_GPIO_ReadPin(limit_x_down_GPIO_Port, limit_x_down_Pin) == GPIO_PIN_RESET){
				break;
			}
			R_M[type].R_abslength = motor[type].totalCount - R_M[type].R_length;
			g_djEnableFlag[type] = 1;
//			HAL_TIM_Base_Start_IT(&htim14);                       //开启20ms定时器中断
			S_M[type].S_status = S_Backward;
//			if (motor[type].totalCount <= R_M[type].R_abslength) {
//				g_djEnableFlag[type] = 1;
//				HAL_TIM_Base_Start_IT(&htim14);                       //开启20ms定时器中断
//				S_M[type].S_status = S_Backward;
//			} else {
//				HAL_GPIO_WritePin(Y_positive_GPIO_Port,Y_positive_Pin,GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(Y_negative_GPIO_Port,Y_negative_Pin,GPIO_PIN_RESET);
//				StopPidCalc(type);
//				S_M[type].S_status = S_Enable;
//			}
			break;

		case R_Stop:
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, RELOAD_VALUE);
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, RELOAD_VALUE);
			StopPidCalc(type);
			S_M[type].S_status = S_Disabel;
			break;

		}
	}else

	if(type == 1){
//		if (HAL_GPIO_ReadPin(limit_y_up_GPIO_Port, limit_y_up_Pin) == GPIO_PIN_RESET){
//			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1, RELOAD_VALUE);
//			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_2, RELOAD_VALUE);
//		}
		switch (status){
			
		case R_Enable:
			HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_2);
			g_djEnableFlag[type] = 1;
			S_M[type].S_status = S_Enable;
			break;

		case R_Disabel:
			HAL_TIM_PWM_Stop(&htim15,TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim15,TIM_CHANNEL_2);
			StopPidCalc(type);
			S_M[type].S_status = S_Disabel;

			break;

		case R_Forward:
			if (HAL_GPIO_ReadPin(limit_y_up_GPIO_Port, limit_y_up_Pin) == GPIO_PIN_RESET){
				break;
			}
			g_djEnableFlag[type] = 0;
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1, RELOAD_VALUE);
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_2, RELOAD_VALUE - DJ_DUTY);
			S_M[type].S_status = S_Forward;
			break;

		case R_Backward:
			if (HAL_GPIO_ReadPin(limit_y_down_GPIO_Port, limit_y_down_Pin) == GPIO_PIN_RESET){
				break;
			}
			g_djEnableFlag[type] = 0;
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_2, RELOAD_VALUE);
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1, RELOAD_VALUE - DJ_DUTY);
			S_M[type].S_status = S_Backward;
			break;

		case R_RelF:
			if (HAL_GPIO_ReadPin(limit_y_up_GPIO_Port, limit_y_up_Pin) == GPIO_PIN_RESET){
				break;
			}
			R_M[type].R_abslength = motor[type].totalCount + R_M[type].R_length;
			g_djEnableFlag[type] = 1;
//			HAL_TIM_Base_Start_IT(&htim14);                       //开启20ms定时器中断
			S_M[type].S_status = S_Forward;
//			if (motor[type].totalCount <= R_M[type].R_abslength) {
//				g_djEnableFlag[type] = 1;
//				HAL_TIM_Base_Start_IT(&htim14);                       //开启20ms定时器中断
//				S_M[type].S_status = S_Forward;
//			} else {
//				HAL_GPIO_WritePin(Y_positive_GPIO_Port,Y_positive_Pin,GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(Y_negative_GPIO_Port,Y_negative_Pin,GPIO_PIN_RESET);
//				StopPidCalc(type);
//				S_M[type].S_status = S_Enable;
//			}
			break;

		case R_RelB:
			if (HAL_GPIO_ReadPin(limit_y_down_GPIO_Port, limit_y_down_Pin) == GPIO_PIN_RESET){
				break;
			}
			R_M[type].R_abslength = motor[type].totalCount - R_M[type].R_length;
			g_djEnableFlag[type] = 1;
			S_M[type].S_status = S_Backward;
//			if (motor[type].totalCount <= R_M[type].R_abslength) {
//				g_djEnableFlag[type] = 1;
//				HAL_TIM_Base_Start_IT(&htim14);                       //开启20ms定时器中断
//				S_M[type].S_status = S_Backward;
//			} else {
//				HAL_GPIO_WritePin(Y_positive_GPIO_Port,Y_positive_Pin,GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(Y_negative_GPIO_Port,Y_negative_Pin,GPIO_PIN_RESET);
//				StopPidCalc(type);
//				S_M[type].S_status = S_Enable;
//			}
			break;

		case R_Stop:
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1, RELOAD_VALUE);
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_2, RELOAD_VALUE);
			StopPidCalc(type);
			S_M[type].S_status = S_Disabel;
			break;

		}
		

	}
	

}
