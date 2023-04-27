#include "bsp_motor.h"
#include "bsp_debug.h"
#include "tim.h"
#include "FreeRTOS.h"


extern int64_t Pos;      //�ۻ�λ��
extern uint32_t acc;      //�����˶��ۻ�λ��
uint8_t a = 0;
extern uint16_t g_ccr;   //�����ٶ�
extern receive_data R_M1;  
extern send_data S_M1;
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)          //�ж����
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
			Pos++;       //��ת+1			
			}
	 else
	  {
		   Pos--;      //��ת-1		   
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
			switch (status)                                                /* �жϹ��� */
    {
        case R_Enable:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 				/* �豸״̬ */
             HAL_GPIO_WritePin(Enable_x_GPIO_Port,Enable_x_Pin,GPIO_PIN_RESET); //ʹ��
				     S_M1.S_status = S_Enable;
            break;

        case  R_Disabel:                                                  
             HAL_GPIO_WritePin(Enable_x_GPIO_Port,Enable_x_Pin,GPIO_PIN_SET);   //ʧ��
						 HAL_TIMEx_OCN_Stop_IT(&htim3,TIM_CHANNEL_4);                       //�ر�����Ƚ��ж�ͨ��
				     acc = 0;
				     S_M1.S_status =  S_Disabel;
            break;
				
				 case  R_Forward:                                                       //�㶯��ת
             HAL_GPIO_WritePin(Enable_x_GPIO_Port,Enable_x_Pin,GPIO_PIN_RESET); //ʹ��
				     HAL_GPIO_WritePin(DIR_y_GPIO_Port,DIR_y_Pin,GPIO_PIN_RESET);         //�رշ���
				     HAL_TIMEx_OCN_Start_IT(&htim3,TIM_CHANNEL_4);                      //������Ƚ��ж�ͨ��
            S_M1.S_status = S_Forward;
				    break;
				
				 case  R_Backward:                                                      //�㶯��ת
             HAL_GPIO_WritePin(Enable_x_GPIO_Port,Enable_x_Pin,GPIO_PIN_RESET); //ʹ��
				     HAL_GPIO_WritePin(DIR_y_GPIO_Port,DIR_y_Pin,GPIO_PIN_SET);         //�򿪷���
					   HAL_TIMEx_OCN_Start_IT(&htim3,TIM_CHANNEL_4);                      //������Ƚ��ж�ͨ��
             S_M1.S_status = S_Backward;
						break;
				
				 case  R_RelF:                                                          //������ת
             HAL_GPIO_WritePin(Enable_x_GPIO_Port,Enable_x_Pin,GPIO_PIN_RESET); //ʹ��
				     HAL_GPIO_WritePin(DIR_y_GPIO_Port,DIR_y_Pin,GPIO_PIN_RESET);         //�رշ���
				 if(acc <= R_M1.R_length)
					 {
						  S_M1.S_status = S_RelF;
					   HAL_TIMEx_OCN_Start_IT(&htim3,TIM_CHANNEL_4);                      //������Ƚ��ж�ͨ��         
				   }
					 else
					 {
					   HAL_TIMEx_OCN_Stop_IT(&htim3,TIM_CHANNEL_4);
						 acc = 0;
						 R_M1.R_status = R_Enable;
					 }
					 break;
				 
				 case  R_RelB:                                                          //������ת
             HAL_GPIO_WritePin(Enable_x_GPIO_Port,Enable_x_Pin,GPIO_PIN_RESET); //ʹ��
				     HAL_GPIO_WritePin(DIR_y_GPIO_Port,DIR_y_Pin,GPIO_PIN_SET);         //�رշ���
				 if(acc <= R_M1.R_length)
					 {
						 S_M1.S_status = S_RelB;
					   HAL_TIMEx_OCN_Start_IT(&htim3,TIM_CHANNEL_4);                      //������Ƚ��ж�ͨ��         
				   }
					 else
					 {
					   HAL_TIMEx_OCN_Stop_IT(&htim3,TIM_CHANNEL_4);
						 acc = 0;
					   R_M1.R_status = R_Enable;
					 }
            break;
				
				 case  R_Stop:                                                          //ֹͣ
             HAL_GPIO_WritePin(Enable_x_GPIO_Port,Enable_x_Pin,GPIO_PIN_SET); //ʹ��				     
					   HAL_TIMEx_OCN_Stop_IT(&htim3,TIM_CHANNEL_4);                      //������Ƚ��ж�ͨ��
				     acc = 0;
				     S_M1.S_status =S_Stop;

            break;
				
				
    }

}







