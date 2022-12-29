#include "key.h"
#include "sys.h" 
#include "lcd.h"
#include "adc.h"
#include "led.h"
#include "delay.h"
//#include "Delay.h"
//////////////////////
u32 KEY_Sys_Timer;
u32 Keycode;
//////////////////////
u32 SystemTime;
u32 KEY_Time;
u32 key_Trgtime;
u32 KEY_Trg;
u32 KEY_Cont=0;
u32 Trg_state = NO;
								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//初始化KEY0-->GPIOA.13,KEY1-->GPIOA.15  上拉输入
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//PE2~4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOE2,3,4

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0
//*************************************************************

//		GPIO_InitTypeDef GPIOStructure;
	
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9 + GPIO_Pin_12 + GPIO_Pin_13 + GPIO_Pin_14 + GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

//	TIM_DeInit(TIM3);
//	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
//	TIM_TimeBaseInitStruct.TIM_Period = 999;
//	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
//	
//	TIM_ITConfig(TIM3, TIM_IT_Update|TIM_IT_Trigger, ENABLE);
//	TIM_Cmd(TIM3, ENABLE);
//	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Init(&NVIC_InitStruct);
	
}



u32 Change_GPIOCode(void)
{
	u32 GPIO_Data;
	GPIO_Data=(((GPIOB->IDR)>>11)&0xffff)|(~(KEY1BIT+KEY2BIT +KEY3BIT +KEY4BIT+KEY5BIT));
	return GPIO_Data;
}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		
//if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==1||KEY4==0||KEY5==0||K_1_S||K_2_S||K_3_S||K_4_S||K_5_S))  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==1||KEY4==0||KEY5==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
		else if(KEY2==0)return 3;
		else if(KEY3==1)return 4;
		else if(KEY4==0)return 5;
		else if(KEY5==0)return 6;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==0&&KEY4==1&&KEY5==1)key_up=1; 	    
 	return 0;// 无按键按下
}


//extern u8 key_read;
extern u16 flag;
//extern u16 x;
//u8 page=0;
//extern u8 startshow;
void ShiftScreen(u8 key_read)
{
			

}
extern u16 item3;

void estimate(void)
{
//				while(1)
//				{
//				u16 i;
//					x=15;
//				LCD_Clear(WHITE);
//				LCD_ShowString(10,6,200,16,12,"Electronic Contest");
//				LCD_ShowString(10,20,200,16,16,"ESTIMATE");		
//			
//					for(i=0 ; i<60 ; i++)
//			{
//				es[i]=Get_ADC_Value(ADC_Channel_2,1);		
//			}
//			
//			esmax = es[0];
//			for(i=0 ; i<60 ; i+=2)
//			{
//				if(esmax < es[i])
//					esmax = es[i];
//			}
//			
//			esmin = es[0];
//			for(i=0 ; i<60 ; i+=2)
//			{
//				if(esmin > es[i])
//					esmin = es[i];
//			}
//			esdc = (esmin+esmax)/2;
//			esac = esmax - esmin;
//			
//			esdc = esdc*3300/4096;
//			esac = esac*3300/4096;
//			esmax = esmax*3300/4096;
//			esmin = esmin*3300/4096;
//			
//			LCD_ShowString(20,40,200,6,16,"esmax:");
//			LCD_ShowNum(80,40,esmax,5,16);
//			LCD_ShowString(20,60,200,6,16,"esmin:");
//			LCD_ShowNum(80,60,esmin,5,16);
//			LCD_ShowString(20,80,200,6,16,"esdc:");
//			LCD_ShowNum(80,80,esdc,5,16);
//			LCD_ShowString(20,100,200,6,16,"esac:");
//			LCD_ShowNum(80,100,esac,5,16);
//			
//		  if(esmax>2360 && esmax<2490)//7.3v为正常
//			{
//				LCD_ShowString(130,50,200,6,16,"NORMAL");
//			}
//			else if(esmax < 100)//7.3v为正常
//			{
//				LCD_ShowString(130,50,200,16,16,"R3 shortcut");
////				LCD_ShowString(130,80,200,16,16,"OR R4 shortcut");
//			}
//			else if(esmax>1400 && esmax<1650)//7.3v为正常
//			{
//				LCD_ShowString(130,50,200,16,16,"C1 disconnection");
//			}
//			else if(esmax>1650 && esmax<1800)//7.3v为正常
//			{
//				LCD_ShowString(130,50,200,16,16,"C2 disconnection");
//			}  		
//			if(esmax>2300 && esmax<2500)
//			{
//				LCD_ShowString(130,70,200,16,16,"R1 shortcut");
//			}
//			if(esmax>2500 && esmax<2550)
//			{
//				LCD_ShowString(130,90,200,16,16,"R2 shortcut");
//			}
//			if(esmax>2650 && esmax<2810)
//			{
//				LCD_ShowString(130,90,200,16,16,"R3 shortcut");
//			}
//			if(esmax>2380 && esmax<2520)
//			{
//				LCD_ShowString(130,110,200,16,16,"R4 shortcut");
//			}
//			if(esmax>2540 && esmax<2630)
//			{
//				LCD_ShowString(130,130,200,16,16,"R2 disconnection");
//			}
//			if(esmax>2550 && esmax<2660)
//			{
//				LCD_ShowString(130,150,200,16,16,"R1 disconnection");
//			}
//			delay_ms(1000);
//			delay_ms(1000);
//		}
}

u16 value1[200];
extern max;
extern min;
extern fil;
extern db;
extern mid;
void flag0(void)
{
	u16 i,j,x,y,m;
	x=15;
			LCD_Clear(WHITE);
			LCD_ShowString(10,6,200,16,12,"Electronic Contest");
			LCD_ShowString(10,20,200,16,12,"SweepFre");
			LCD_DrawLine(15, 33, 15, 230) ;    //y轴
			LCD_DrawLine(15, 230, 310, 230)  ;   //x轴
			
			LCD_ShowString(30,221,40,16,12,"0k");
			LCD_ShowString(80,221,30,16,12,"200k");
			LCD_ShowString(130,221,30,16,12,"400k");
			LCD_ShowString(180,221,30,16,12,"600k");
			LCD_ShowString(230,221,30,16,12,"800k");
			LCD_ShowString(280,221,30,16,12,"1m");
		  	delay_ms(300);
//				delay_ms(200);
			for(i=0 ; i<200 ; i++)
			{
				value1[i]=Get_ADC_Value(ADC_Channel_1,1);		
				delay_ms(5);
			}

			for(i=0 ; i<190 ; i++)    //取差值
			{
				max=value1[i];
				min=value1[i];
				for(j=0;j<10;j++)
				{
					if(max<value1[i+j])
					{
					max=value1[i+j];
					}
					if(min>value1[i+j])
					{
					min=value1[i+j];
					}
				}
			value1[i]=max-min;
				//fil=0;
			}
			
			for(i=0 ; i<190 ; i++)   //滤波
			{
				for(j=0;j<5;j++)
				{
					fil+=value1[i+j];
				}
			value1[i]=fil/5;
				fil=0;
			}
			max=value1[0];
			for(i=5 ; i<150 ; i++)   //取最大
			{
				if(max<value1[i])
					{
					max=value1[i];
					}
			}
			for(i=5;i<150;i++)
			{
				if(value1[i]>(max*0.7))
				{
					db = i;
					break;
				}
			}			
			mid = 1000000*db/200;
			LCD_ShowString(170,10,200,3,16,"3dB:");
			LCD_ShowNum(215,10,2*mid,5,16);
	//		LCD_ShowNum(215,40,max,5,16);

			POINT_COLOR = RED;
			for(i=0 ; i<199 ; i++)
			{
			x++;
	  	m=y;
			y=260-(value1[i]*1200/4096);
				if(y<5)y=5;
				if(y>270)y=270;
				if(i>193)break;
	  	LCD_DrawLine(x, m, x++, y);
			}	
			POINT_COLOR = BLACK;
			
}







u32 KeyRead(void)
{
    u32 ReadData = Change_GPIOCode()^0XFFFFFFFF;   // 1
    KEY_Trg = ReadData & (ReadData ^ KEY_Cont);      // 2
    KEY_Cont = ReadData;

	if(!Trg_state) 
		key_Trgtime = KEY_Sys_Timer + T_10mS;
	if(KEY_Trg)
		Trg_state = YES;
	if(Trg_state)
	{
		if((KEY_Sys_Timer > key_Trgtime)&&(KEY_Sys_Timer<(((u32)0-T_10mS))))  //??
		{
			switch(KEY_Cont)
			{
				case K_1:{Keycode = K_1_S; break;}
				case K_2:{Keycode = K_2_S; break;} 	
				case K_3:{Keycode = K_3_S; break;}
				case K_4:{Keycode = K_4_S; break;}
				case K_5:{Keycode = K_5_S; break;}
				default:{Keycode = K_NO; break;}
			}	
			
		}
	}
//	if(KEY_Time >= T_1S)	  	//
//	{ 
//		KEY_Time = T_1S-T_330mS;
//		switch(KEY_Cont)
//		{
//			case K_1:{Keycode = K_1_L; break;}
//			case K_2:{Keycode = K_2_L; break;} 	
//			case K_3:{Keycode = K_3_L; break;}
//			case K_4:{Keycode = K_4_L; break;}
//			case K_5:{Keycode = K_5_L; break;}
//			default:{Keycode = K_NO; break;} 
//		}
//	}
	return Keycode;
}


void KEY_EXIT(void)
{
	if(Keycode != K_NO)
	{
		Trg_state = NO;
		Keycode = K_NO;
	}	
}