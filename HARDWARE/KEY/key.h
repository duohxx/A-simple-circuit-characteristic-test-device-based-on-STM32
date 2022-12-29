#ifndef __KEY_H
#define __KEY_H	 
#include "system.h"

//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define KEY3 PAin(0)	//PA0  WK_UP
#define KEY5  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)//读取按键0
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//读取按键0
#define KEY0  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//读取按键2 
#define KEY3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键3(WK_UP) 

#define K_1_S		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)       //K_M_S  -->> 按键_中键_短按
#define K_2_S		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) 
#define K_3_S		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) 
#define K_4_S		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) 
#define K_5_S		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) 



#define KEY_UP 		4
#define KEY_LEFT	3
#define KEY_DOWN	2
#define KEY_RIGHT	1

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数	
void ShiftScreen(u8 key_read);
void estimate(void);
void flag0(void);


#define YES	  1
#define NO	  0

#define T_10mS	((u32)10)
#define T_1S	((u32)1000)
#define T_330mS	((u32)330)
///////////////////////////////
#define KEY1BIT GPIO_Pin_0
#define KEY2BIT GPIO_Pin_1
#define KEY3BIT GPIO_Pin_2
#define KEY4BIT GPIO_Pin_3
#define KEY5BIT GPIO_Pin_4

////////原始码值///////////////
#define K_1		KEY1BIT
#define K_2		KEY2BIT
#define K_3		KEY3BIT
#define K_4		KEY4BIT
#define K_5		KEY5BIT

//////////////////////////////

////////码值重映射////////////
////////码值重映射////////////
//#define K_1_S		0X01        //K_M_S  -->> 按键_中键_短按
//#define K_2_S		0X02
//#define K_3_S		0X03
//#define K_4_S		0X04
//#define K_5_S		0X05

//#define K_1_L		0X0B        //K_M_S  -->> 按键_中键_短按
//#define K_2_L		0X0C
//#define K_3_L		0X0D
//#define K_4_L		0X0E
//#define K_5_L		0X0F

#define K_NO		0X00000000
///////////////////////////////////


extern u32 KEY_Sys_Timer;
extern u32 KEY_Time;
extern u32 key_Trgtime;
extern u32 KEY_Trg;
extern u32 KEY_Cont;
extern u32 Trg_state;
extern u32 Keycode;

void key_init(void);
u32 KeyRead(void);
void KEY_EXIT(void);


#endif
