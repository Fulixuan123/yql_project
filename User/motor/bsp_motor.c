#include "bsp_motor.h"
#include "bsp_debug.h"
#include "tim.h"
#include "FreeRTOS.h"


extern int64_t Pos;      //累积位置
extern uint32_t acc;      //单次运动累积位置
uint8_t a = 0;
extern uint16_t g_ccr;   //定义速度
extern receive_data R_M1;  
extern send_data S_M1;
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)          //中断输出
{
	if(htim == &htim3)
	{
			a++;
		  if(a % 2 == 0)
		{		
       acc++;
       a = 0;			
	  	if(DIR_y_Pin == 0)
		{
			Pos++;       //正转+1			
			}
	 else
	  {
		   Pos--;      //反转-1		   
	   }
     }
		S_M1.S_position = Pos;
	
		__IO uint16_t cout = 0;
		cout = __HAL_TIM_GetCompare(&htim3,TIM_CHANNEL_4);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,(cout + g_ccr)%0xFFFF);	
 	}	
	}



void motor_status(uint8_t status)

{
			switch (status)                                                /* 判断功能 */
    {
        case R_Enable:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 				/* 设备状态 */
             HAL_GPIO_WritePin(Enable_x_GPIO_Port,Enable_x_Pin,GPIO_PIN_RESET); //使能
				     S_M1.S_status = S_Enable;
            break;

        case  R_Disabel:                                                  
             HAL_GPIO_WritePin(Enable_x_GPIO_Port,Enable_x_Pin,GPIO_PIN_SET);   //失能
						 HAL_TIMEx_OCN_Stop_IT(&htim3,TIM_CHANNEL_4);                       //关闭输出比较中断通道
				     acc = 0;
				     S_M1.S_status =  S_Disabel;
            break;
				
				 case  R_Forward:                                                       //点动正转
             HAL_GPIO_WritePin(Enable_x_GPIO_Port,Enable_x_Pin,GPIO_PIN_RESET); //使能
				     HAL_GPIO_WritePin(DIR_y_GPIO_Port,DIR_y_Pin,GPIO_PIN_RESET);         //关闭方向
				     HAL_TIMEx_OCN_Start_IT(&htim3,TIM_CHANNEL_4);                      //打开输出比较中断通道
            S_M1.S_status = S_Forward;
				    break;
				
				 case  R_Backward:                                                      //点动反转
             HAL_GPIO_WritePin(Enable_x_GPIO_Port,Enable_x_Pin,GPIO_PIN_RESET); //使能
				     HAL_GPIO_WritePin(DIR_y_GPIO_Port,DIR_y_Pin,GPIO_PIN_SET);         //打开方向
					   HAL_TIMEx_OCN_Start_IT(&htim3,TIM_CHANNEL_4);                      //打开输出比较中断通道
             S_M1.S_status = S_Backward;
						break;
				
				 case  R_RelF:                                                          //增量正转
             HAL_GPIO_WritePin(Enable_x_GPIO_Port,Enable_x_Pin,GPIO_PIN_RESET); //使能
				     HAL_GPIO_WritePin(DIR_y_GPIO_Port,DIR_y_Pin,GPIO_PIN_RESET);         //关闭方向
				 if(acc <= R_M1.R_length)
					 {
						  S_M1.S_status = S_RelF;
					   HAL_TIMEx_OCN_Start_IT(&htim3,TIM_CHANNEL_4);                      //打开输出比较中断通道         
				   }
					 else
					 {
					   HAL_TIMEx_OCN_Stop_IT(&htim3,TIM_CHANNEL_4);
						 acc = 0;
						 R_M1.R_status = R_Enable;
					 }
					 break;
				 
				 case  R_RelB:                                                          //增量反转
             HAL_GPIO_WritePin(Enable_x_GPIO_Port,Enable_x_Pin,GPIO_PIN_RESET); //使能
				     HAL_GPIO_WritePin(DIR_y_GPIO_Port,DIR_y_Pin,GPIO_PIN_SET);         //关闭方向
				 if(acc <= R_M1.R_length)
					 {
						 S_M1.S_status = S_RelB;
					   HAL_TIMEx_OCN_Start_IT(&htim3,TIM_CHANNEL_4);                      //打开输出比较中断通道         
				   }
					 else
					 {
					   HAL_TIMEx_OCN_Stop_IT(&htim3,TIM_CHANNEL_4);
						 acc = 0;
					   R_M1.R_status = R_Enable;
					 }
            break;
				
				 case  R_Stop:                                                          //停止
             HAL_GPIO_WritePin(Enable_x_GPIO_Port,Enable_x_Pin,GPIO_PIN_SET); //使能				     
					   HAL_TIMEx_OCN_Stop_IT(&htim3,TIM_CHANNEL_4);                      //打开输出比较中断通道
				     acc = 0;
				     S_M1.S_status =S_Stop;

            break;
				
				
    }

}







