#include "lcd.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "timer.h"
#include "key.h"
#include "adc.h"
#include "task_manage.h"
#include "AD9959.h"
#include "chart.h"
#include "device.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32_dsp.h"
#include "table_fft.h"
#include "touch.h"
extern page;
extern flag;

void WriFre1000()
{
		Write_Amplitude(2, 1023);
		Write_Amplitude(3, 1023);
		Write_frequence(2,1000); 
		Write_frequence(3,1000);
}

void initShow()
{
//A simple circuit characteristic test device based on STM32
	LCD_Clear(BLUE);
	LCD_ShowString(40,26,300,16,16,"A simple circuit characteristic");
	LCD_ShowString(60,56,300,16,16,"test device based on STM32");
	LCD_DrawLine(5, 75, 315, 75)  ;   //x轴
	LCD_ShowString(10,86,100,16,12,"FUNCTION:");
	LCD_ShowString(20,116,100,16,16,"Rin           ");
	LCD_ShowString(20,146,100,16,16,"Rout          ");
	LCD_ShowString(20,176,100,16,16,"Gain          ");
	LCD_ShowString(20,206,100,16,16,"Amp-Fre Chara ");
	LCD_ShowString(160,116,100,16,16,"Detect Circurt");
	LCD_ShowString(160,146,100,16,16,"Analyze C     ");
	LCD_ShowString(160,176,100,16,16,"Spectrometer  ");
	

}

void underpainting()
{
	LCD_Clear(BLUE);
//	LCD_ShowString(40,26,50,16,16,"SCCTD");
	LCD_Fill(10,70,240,230,LIGHTBLUE);
	LCD_ShowString(250,70,50,16,16,"Vp-p:");
	
	LCD_ShowString(250,120,50,16,16,"Vmax:");
}




#define PI2 6.28318530717959
#define Fs 1024*2                   //采样频率 Hz
#define NPT 1024                    //采样点数

u16 ADC_input[NPT];

u32 input[NPT];
u32 output[NPT],Mag[NPT];

float Fn[NPT/2];     				   //各个点的频率
float Mag_max;
u16 ADC_DataNum = 0;
s16 Fn_Num;

COEFS fir_coefs;/*coefficients structure*/
u16 ADC_flag = 0;

extern __IO uint16_t ADC_ConvertedValue[];    // ADC1转换的电压值通过MDA方式传到SRAM	
/**********************原程序*********************************************/

/***************计算幅值*****************/
void PowerMag(u16 FFT_NPT)  
{

	s16 lx, ly;
	u32 i;
	float mag, x, y;
	
	for (i = 0; i < FFT_NPT/2; i++ )
	{

		lx = (output[i]<<16)>>16;
		ly = (output[i]>>16);
		
		 x = FFT_NPT*((float)lx)/32768;
		 y = FFT_NPT*((float)ly)/32768;
		mag = sqrt(x*x+y*y)/FFT_NPT;		
		Mag[i] = (u32)(mag*65536);		
	}
	Mag[0] = Mag[0]/2;	
}

/***********取最大值************/
void Compare_Max()
{
	u16 i;
	Mag_max = Mag[1]; 
    for(i=1; i < NPT/2; i++)
    {
		if(Mag_max < Mag[i])
		{
			Fn_Num = i;
			Mag_max = Mag[i];
		}
    }

}

/********FFT变换**********/
void DSP_FFT1024()
{
	u16 i;	
    
	for (i = 0; i <NPT ; i++)
	{
         input[i] = ADC_input[i]<<16;
    }
	
	cr4_fft_1024_stm32(output,input,NPT);   //FFT变换
	PowerMag(NPT);

}


u16 ii=0,xx=16,yy=230,mm=230,keyread = 0;
void spectrometer()
{
	u16 i;   
	LCD_Clear(WHITE);
	LCD_ShowString(40,26,50,16,16,"PAGE 4");

	LCD_ShowString(6,4,200,12,16,"OSCILLOSCOPE");
	LCD_ShowString(6,124,200,12,16,"SPECTROMETER");
	LCD_Fill(10,20,310,120,LIGHTBLUE);
	LCD_Fill(10,140,310,240,LIGHTGREEN);
	LCD_DrawLine(15, 33, 15, 230) ;    //y轴
	LCD_DrawLine(15, 230, 310, 230)  ;   //x轴
	xx=16;
			for(i=0 ; i<1024 ; i++)
			{
				ADC_input[i]=Get_ADC_Value(ADC_Channel_4,1);		    //输入端
			}
			DSP_FFT1024();
      Compare_Max();
			mm=220;
		for(i = 0; i < 290; i+=1)
       {  
				xx+=1;	
				mm=yy;
				yy = 225 - Mag[i]*85/Mag_max;
//				y = 235 - ADC_input[i]*200/4096;
				LCD_DrawLine(xx, mm, xx+1, yy);
				}  
			xx=16;
			for(i = 0; i < 280; i++)
       {  
				xx+=1;	
				mm=yy;
				yy = 115 - ADC_input[i]*90/4096;
				LCD_DrawLine(xx, mm, xx+1, yy);
				}  
			delay_ms(1000);flag = 1;
}

void DMA1_Channel1_IRQHandler()  
{  
    int  i;
    if(DMA_GetITStatus(DMA_IT_TC))                      //判断DMA传输完成中断  
    {      
            for(i=0;i<NPT;i++)
            {
                ADC_input[i] = ADC_ConvertedValue[i];
            }
            ADC_flag = 1;
    }
    DMA_ClearITPendingBit(DMA_IT_TC);                   //清除DMA中断标志位  
}  

//清空屏幕并在右上角显示"RST"
void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);//清屏   
 	POINT_COLOR=BLUE;//设置字体为蓝色 
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
  	POINT_COLOR=RED;//设置画笔蓝色 
}
////////////////////////////////////////////////////////////////////////////////
//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  
//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  
//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_fill_circle(uRow,uCol,size,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   
////////////////////////////////////////////////////////////////////////////////
 //5个触控点的颜色(电容触摸屏用)												 
//const u16 POINT_COLOR_TBL[OTT_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED};  
//电阻触摸屏测试函数
void rtp_test(void)
{
	u8 key;
	u8 i=0;	  
	while(1)
	{
//	 	key=KEY_Scan(0);
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]<16)Load_Drow_Dialog();//清除
				else TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//画图	  			   
			}
		}else delay_ms(10);	//没有按键按下的时候 	    
//		if(key==KEY0_PRES)	//KEY0按下,则执行校准程序
		{
//			LCD_Clear(WHITE);	//清屏
//		    TP_Adjust();  		//屏幕校准 
//			TP_Save_Adjdata();	 
//			Load_Drow_Dialog();
		}
	}
}


void Show_Picture(void)
{
	u32 i,j,k=0;
	u16 picH,picL;
	LCD_Clear(WHITE);//清屏

	{
		
		LCD_Set_Window(0,0,320,240);//设置一个自动换行显示区域窗口

		LCD_WriteRAM_Prepare();     	//开始写入GRAM	
						
		for(i=0;i<240;i++)
		for(j=0;j<320;j++)
		{
			picH=gImage_sss[k++];
			picL=gImage_sss[k++];
	
			LCD->LCD_RAM=(picH<<8)+picL; 
			
		}			
	}		
} 
