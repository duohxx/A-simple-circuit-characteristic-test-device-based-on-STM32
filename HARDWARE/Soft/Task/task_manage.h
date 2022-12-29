#ifndef __task_manage_H
#define __task_manage_H
#include "stm32f10x.h"
#include "lcd.h"
#include "key.h"

#define Interface 3//��������
#define StrMax    10//�������ݴ�С

extern u32 SysTimer;
extern u32 SinFre[5];
extern u32 SinAmp[5];
extern u32 SinPhr[5];

u8 Task_Delay(u32 delay_time, u8* delay_ID);
u8 TaskCycleDelay(u32 delay_time, u8* Last_delay_ID, u8* Self_delay_ID);
void welcome_KW(void);
void Main_Menu(u32 Key_Value, u8* Task_ID);
void LCD_Show_ModeCEInfo(u16 x0, u8 start_info, u8 current_deal_info);
void Copybuf2dis(u8 *source, u8 dis[StrMax], u8 dispoint, u8 FloatNum, u8 CursorEn);
void Set_PointFre(u32 Key_Value, u8* Task_ID);
void Sweep_Fre(u32 Key_Value, u8* Task_ID);
void Move_Pha(u32 Key_Value, u8* Task_ID);
void Jump_Fre(u32 Key_Value, u8* Task_ID);

void Task0_PointFre(u32 Key_Value);
void Task1_Amp_Pha(u32 Key_Value);
void Task2_Triangular(u32 Key_Value);
void Task3_SweepFre();
void fre_buf_change(u8 *strbuf);
void Clear_AllStr(u8 *str);
void Task4_Input(u32 Key_Value);
void Task5_Input(u32 Key_Value);

#endif
