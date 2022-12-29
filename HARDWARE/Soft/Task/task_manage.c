
#include "task_manage.h"
#include "delay.h"
#include "key.h"
#include "AD9959.h"
////#include "AD9954.h" 
#include <stdio.h>
#include <ctype.h>
#include <cstring>
#include "oled.h"
#include "led.h"
#define OUT_KEY  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//读取按键0
#define FLASH_SAVE_ADDR  0x0801F000  				//设置FLASH 保存地址(必须为偶数)

u8 Task_Index = 0;//界面切换
u8 Param_Mode = 0;//参数类型切换
u8 fre_buf[StrMax]; //参数转换字符缓存
u8 display[StrMax]; //参数显示缓存
u8 P_Index = 0; //编辑位置
u8 Task_First=1;//第一次进入标记
u8 _return=0;
u32 SinFre[5] = {1000, 1000, 1000, 1000};
u32 sinfre =  1000;
u32 sina = 10000;
u32 SinAmp[5] = {1023, 1023, 1023, 1023};
u32 SinAmp0=100;
u32 SinPhr[5] = {0, 4095, 4095*3, 4095*2};
u32 SinFre0[4] = {1000};
//扫频参数
u32 SweepMinFre = 10;
u32 SweepMaxFre = 1000000;
u32 SweepStepFre =  4000;
u16 SweepTime = 3;//ms
u8 SweepFlag = 0;

u8 Task_Delay(u32 delay_time, u8* delay_ID)
{
	static u8 Time_Get = 0;
	static u32 Time_Triger;
    if(Time_Get == 0)
    {
      Time_Triger = SysTimer + delay_time;
      Time_Get = 1;
    }
    if(SysTimer >= Time_Triger)
    { 
      Time_Get = 0;
			*delay_ID = 1;		//	后续代码已被执行一遍
			return 1;
    }
		return 0;
}


u8 TaskCycleDelay(u32 delay_time, u8* Last_delay_ID, u8* Self_delay_ID)
{
	static u8 Time_Get = 0;
	static u32 Time_Triger;
	
	if(!(*Last_delay_ID))
		return 0;
	if(Time_Get == 0)
	{
		Time_Triger = SysTimer + delay_time;
		Time_Get = 1;
	}
	if(SysTimer >= Time_Triger)
	{ 
		Time_Get = 0;
		*Last_delay_ID = 0;
		*Self_delay_ID = 1;		//	后续代码已被执行一遍
		return 1;
	}
	return 0;
}

////把数据放到另一个缓存，显示，FloatNum小数点位数，CursorEn光标使能
//void Copybuf2dis(u8 *source, u8 dis[StrMax], u8 dispoint, u8 FloatNum, u8 CursorEn)
//{
//	int i, len;
//	
//	
//	len = strlen(source);
//	i = len - FloatNum;//整数个数
//	Clear_AllStr(dis);
//	if(FloatNum>0)dis[i] = '.';
//	for (i = 0; i < len; i++)
//	{
//		if(i < (len-FloatNum)) dis[i] = source[i];
//		else 
//		{ dis[i+1] = source[i]; }
//	}
//	
//	if(CursorEn)
//	{
//		if(dispoint < (len-FloatNum)) dis[dispoint] += 128;
//		else dis[dispoint+1] += 128;	
//	}
//}
//
extern count;
extern count1;
extern NowFre;
void Set_PointFre(u32 Key_Value, u8* Task_ID)
{
	//按键判断
	//界面切换
	if(Key_Value == K_5_S)
	{
		Task3_SweepFre(Key_Value);
	}
	
	if(Key_Value == K_1_S)
	{
		Task5_Input(Key_Value);
	}
	if(Key_Value == K_2_S)
	{
		Task0_PointFre(Key_Value);
	}if(Key_Value == K_3_S)
	{
		Task4_Input(Key_Value);
	}
	


	OLED_ShowString(0,0,"AD9959",16);
	
	
	
}
//任务

void Task0_PointFre(u32 Key_Value)//正弦波 (10M) 0-100 000 000
{
	SweepFlag = 0;
	  sina = sinfre/1000;
	  Write_frequence(0,sinfre); 
		Write_frequence(1,sinfre);
		Write_frequence(2,sinfre); 
		Write_frequence(3,sinfre);
	  Write_Amplitude(0, SinAmp0);
		Write_Amplitude(1, SinAmp0);
	  Write_Amplitude(2, SinAmp0);
		Write_Amplitude(3, SinAmp0);
		OLED_Clear();
	  OLED_ShowString(60,0,"FM",16);
		OLED_ShowNum(0,40,sinfre,7,16);
		OLED_ShowString(10,20,"OUTPUT:", 12);
		OLED_ShowString(60,40,"Hz", 16);
}


void Task3_SweepFre()//扫频
{
	SweepFlag = 1;

//	if(Key_Value == K_5_S)
//	{
//		OLED_Clear();
//	}

//	  OLED_ShowString(80,0,"SweepFre",12);
//		OLED_ShowString(60,20,"Hz Min", 12);
////		OLED_ShowNum(0,34,SweepMaxFre,7,12);
//		OLED_ShowString(60,34,"Hz Max", 12);
////		OLED_ShowNum(0,48,SweepStepFre,7,12);
//		OLED_ShowString(60,48,"Hz Step", 12);
//	

 

//			OLED_ShowNum(0,34,SweepMaxFre,7,12);
//	  	OLED_ShowNum(0,20,SweepMinFre,7,12);
//		OLED_ShowNum(0,48,SweepStepFre,7,12);
//		SweepTime = ( 200*(1/SweepStepFre) )*( SweepMaxFre-SweepMinFre );
		
}

//

void Task4_Input(u32 Key_Value)
{
		SweepFlag = 0;
		OLED_Clear();
		Write_Amplitude(0, 700);
		Write_Amplitude(1, 700);
		Write_Amplitude(2, 700);
		Write_Amplitude(3, 700);
		Write_frequence(0,1000); 
		Write_frequence(1,1000);
		Write_frequence(2,1000); 
		Write_frequence(3,1000);
		OLED_ShowString(60,0,"INPUT",16);
		OLED_ShowNum(0,40,1000,7,16);
		OLED_ShowString(60,40,"Hz", 16);
}

void Task5_Input(u32 Key_Value)
{
		SweepFlag = 0;
	  OLED_Clear();
		Write_Amplitude(0, 1023);
		Write_Amplitude(1, 1023);
		Write_Amplitude(2, 1023);
		Write_Amplitude(3, 1023);
		Write_frequence(0,1000000); 
		Write_frequence(1,1000000);
		Write_frequence(2,1000000); 
		Write_frequence(3,1000000);
		OLED_ShowString(60,0,"highFre",16);
		OLED_ShowNum(0,40,1000,7,16);
		OLED_ShowString(60,40,"MHz", 16);
}

//void fre_buf_change(u8 *strbuf)
//{
//	int i;
//	for (i = 0 ; i < strlen(strbuf); i++)
//	 if(strbuf[i]==0x20) strbuf[i] = '0';
//	for (i = 0 ; i < strlen(fre_buf); i++)
//	 if(fre_buf[i]==0x20) fre_buf[i] = '0';
//}
//void Clear_AllStr(u8 *str)
//{
//	u8 i, len;
//	len = strlen(str);
//	for(i = 0; i < len; i++)
//	str[i] = NULL;
//}

