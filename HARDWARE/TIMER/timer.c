/**********************************************************
                       ��������
										 
���ܣ�stm32f103rct6���ƣ�20MHzʱ�ӣ� AD9954���Ҳ���Ƶ�������Χ0-130M(�ɱ༭0-200M)��
			ɨƵĬ�����Ҳ� ��
			��ʾ��12864cog
�ӿڣ����ƽӿ������AD9954.h  �����ӿ������key.h
ʱ�䣺2015/11/10
�汾��1.0
���ߣ���������
������

������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ

**********************************************************/
#include "timer.h"
#include "led.h"
#include "task_manage.h"
#include "AD9959.h"
#include "math.h"
#include "delay.h"
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void Timerx_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	//��ʱ����ʼ��					   //arr,psc
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim ��Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�	 ��������ж�
		TIM4, //TIM2
		TIM_IT_Update  |  //TIM ����(���)�ж�                  
		TIM_IT_Trigger,   //TIM �����ж�   �������жϵȣ�					��ֵַ	 ((uint16_t)0x0040)         
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����ʱ�������� ������ʱ��
							 
}

u32 count=0, NowFre=0;
float count1=1;
extern u32 SweepMinFre;
extern u32 SweepMaxFre;
extern u32 SinAmp0;
extern u32 SweepStepFre;
extern u16 SweepTime;//ms
extern u8 SweepFlag;

void TIM4_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		
		if(SweepFlag)
		{
			count++;
		//	if(count>=SweepTime*10)
			{
				//д���� 
	//			LED1 = 1;
				NowFre = 10;
				Write_Amplitude(0, 1023);
				Write_Amplitude(1, 1023);
				Write_frequence(0,NowFre); 
				Write_frequence(1,NowFre);
				delay_ms(500);
				
	//			LED1 = 0;
				NowFre = 100;
				Write_Amplitude(0, 1023);
				Write_Amplitude(1, 1023);
				Write_frequence(0,NowFre); 
				Write_frequence(1,NowFre);
				delay_ms(500);
				
	//			LED1 = 1;
				NowFre = 1000;
				Write_Amplitude(0, 1023);
				Write_Amplitude(1, 1023);
				Write_frequence(0,NowFre); 
				Write_frequence(1,NowFre);
				delay_ms(500);
				
	//			LED1 = 0;
				NowFre = 10000;
				Write_Amplitude(0, 1023);
				Write_Amplitude(1, 1023);
				Write_frequence(0,NowFre); 
				Write_frequence(1,NowFre);
				delay_ms(500);
				
		//		LED1 = 1;
				NowFre = 100000;
				Write_Amplitude(0, 1023);
				Write_Amplitude(1, 1023);
				Write_frequence(0,NowFre); 
				Write_frequence(1,NowFre);
				delay_ms(500);
				
	//			LED1 = 0;
				NowFre = 1000000;
				Write_Amplitude(0, 1023);
				Write_Amplitude(1, 1023);
				Write_frequence(0,NowFre); 
				Write_frequence(1,NowFre);
				delay_ms(500);
				
	//			LED1 = 1;
				NowFre = 10000000;                //10MHz
				Write_Amplitude(0, 1023);
				Write_Amplitude(1, 1023);
				Write_frequence(0,NowFre); 
				Write_frequence(1,NowFre);
				delay_ms(500);
			}
		}
	}
}




void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM2_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    
	TIM_DeInit(TIM2);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	TIM2_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM2_InitStructure.TIM_RepetitionCounter = 0;
	TIM2_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//=====================�����ʵ��ڡ�=====================================
	TIM2_InitStructure.TIM_Period = 799;        //(799,43)Ƶ��2048     //59,29(40K)   //44,19(80K)
	TIM2_InitStructure.TIM_Prescaler = 43;
//==============================================================
	TIM_TimeBaseInit(TIM2, &TIM2_InitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //TIM�����ȵ���ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //
    TIM_OCInitStructure.TIM_Pulse = 50;     //ռ�ձȣ�����0��OK��
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_Low;   //TIM����Ƚϼ��Ե�
    TIM_OC2Init(TIM2,&TIM_OCInitStructure);
        
    TIM_InternalClockConfig(TIM2);    //����TIMx�ڲ�ʱ��
    TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);  //ʹ�ܻ�ʧ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_UpdateDisableConfig(TIM2,DISABLE);    //ʹ�ܻ�ʧ��TIMx�����¼�
    
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //ʹ�ܻ�ʧ��ָ����TIM�ж�
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_NVIC_Configuration(void)
{
	
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
}

void TIME2_Init()
{
    TIM2_NVIC_Configuration();
    TIM2_Configuration();
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)==SET)
	{
//=================��ʱ��2�жϴ�����================================
        
//====================�����ã�������ʾ����������ʱƵ�ʡ�==================         
//          if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3))
//              led4(ON);
//          else
//              led4(OFF);
//===============================================================                
		TIM_ClearFlag(TIM2, TIM_IT_Update); //�������жϱ�־
	}
}







