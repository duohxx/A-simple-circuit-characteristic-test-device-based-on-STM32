/**********************************************************
                       康威电子
										 
功能：stm32f103rct6控制，20MHz时钟， AD9954正弦波点频输出，范围0-130M(可编辑0-200M)，
			扫频默认正弦波 。
			显示：12864cog
接口：控制接口请参照AD9954.h  按键接口请参照key.h
时间：2015/11/10
版本：1.0
作者：康威电子
其他：

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ

**********************************************************/
#include "timer.h"
#include "led.h"
#include "task_manage.h"
#include "AD9959.h"
#include "math.h"
#include "delay.h"
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void Timerx_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	//定时器初始化					   //arr,psc
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim 分频因子
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断	 允许更新中断
		TIM4, //TIM2
		TIM_IT_Update  |  //TIM 更新(溢出)中断                  
		TIM_IT_Trigger,   //TIM 触发中断   （捕获中断等）					地址值	 ((uint16_t)0x0040)         
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx（定时器）外设 开启定时器
							 
}

u32 count=0, NowFre=0;
float count1=1;
extern u32 SweepMinFre;
extern u32 SweepMaxFre;
extern u32 SinAmp0;
extern u32 SweepStepFre;
extern u16 SweepTime;//ms
extern u8 SweepFlag;

void TIM4_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		
		if(SweepFlag)
		{
			count++;
		//	if(count>=SweepTime*10)
			{
				//写数据 
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
//=====================采样率调节↓=====================================
	TIM2_InitStructure.TIM_Period = 799;        //(799,43)频率2048     //59,29(40K)   //44,19(80K)
	TIM2_InitStructure.TIM_Prescaler = 43;
//==============================================================
	TIM_TimeBaseInit(TIM2, &TIM2_InitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //
    TIM_OCInitStructure.TIM_Pulse = 50;     //占空比（大于0就OK）
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_Low;   //TIM输出比较级性低
    TIM_OC2Init(TIM2,&TIM_OCInitStructure);
        
    TIM_InternalClockConfig(TIM2);    //设置TIMx内部时钟
    TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);  //使能或失能TIMx在CCR2上的预装载寄存器
    TIM_UpdateDisableConfig(TIM2,DISABLE);    //使能或失能TIMx更新事件
    
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //使能或失能指定的TIM中断
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
//=================定时器2中断处理函数================================
        
//====================测试用，可以用示波器测量定时频率↓==================         
//          if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3))
//              led4(ON);
//          else
//              led4(OFF);
//===============================================================                
		TIM_ClearFlag(TIM2, TIM_IT_Update); //清除溢出中断标志
	}
}







