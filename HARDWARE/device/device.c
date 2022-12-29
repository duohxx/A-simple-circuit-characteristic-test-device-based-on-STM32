#include "lcd.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "oled.h"
//#include "picture.h"
#include "key.h"
#include "adc.h"
#include "task_manage.h"
#include "AD9959.h"
#include "timer.h"
#include "device.h"
#include "chart.h"
#include "touch.h"

	extern u8 _return;
	extern page;
  u16 j,x=16,y=230,m=230,flag = 0;
	float fil=0,max,min;
	float ro;
	u16 ave=0,ri,v1ave=0,v2ave=0;
	float value[200],v1[60],v2[60],v3[60];
	u8 key_read = 0;   //read key
	float item3=0,mid=0;
	u16 Vb,Ie,rbe,B,db;
	float PA1max,PA2max,PA3max,PA4max,PA2min,PA1min;
	float PA4[60],PA2ff,PA1ff;
	u16 es[100],esmax,esmin,esdc,esac;
	u16 i;
void amp_fre()
{
			u16 ampfre[20];
			led3 = 0;
			min = 0;
			LCD_Clear(BLUE);
			LCD_ShowString(10,6,300,30,16,"Magnitude-Frequency Characteristic");
			LCD_DrawLine(15, 33, 15, 230) ;    //y轴
			LCD_DrawLine(15, 230, 310, 230)  ;   //x轴
			LCD_ShowString(30,221,30,16,12,"0   ");
			LCD_ShowString(90,221,30,16,12,"100  ");
			LCD_ShowString(150,221,30,16,12,"10k ");
			LCD_ShowString(210,221,30,16,12,"1m  ");
			LCD_ShowString(270,221,30,16,12,"100m ");

			
			Write_Amplitude(2, 1023);										
	  	Write_Amplitude(3, 1023);
	///////////////////////////////////////////////////////////////////
	
			Write_frequence(2,10); 						//10Hz 进行取点采样
			Write_frequence(3,10);
			delay_ms(100);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value(ADC_Channel_1,1);		
				delay_ms(5);
			}
			max=value[0];
			for(i=1 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[0]=max-min;
//////////////////////////////////////////////////////////
			Write_frequence(2,32);            //32Hz 进行取点采样
			Write_frequence(3,32);
			delay_ms(100);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value(ADC_Channel_1,1);		
				delay_ms(5);
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[1]=max-min;
///////////////////////////////////////////////////////
			Write_frequence(2,100);               //100Hz 进行取点采样
			Write_frequence(3,100);
			delay_ms(100);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value(ADC_Channel_1,1);		
				delay_ms(5);
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[2]=max-min;
		///////////////////////////////////////////////////////	

			Write_frequence(2,316); //316Hz 进行取点采样
			Write_frequence(3,316);
			delay_ms(100);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value(ADC_Channel_1,1);		
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[3]=max-min;
	///////////////////////////////////////////////////////		

			Write_frequence(2,1000); //1kHz 进行取点采样
			Write_frequence(3,1000);
			delay_ms(100);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value(ADC_Channel_1,1);		
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[4]=max-min;
		///////////////////////////////////////////////////////	
			Write_frequence(2,3162); //3162Hz 进行取点采样
			Write_frequence(3,3162);
			delay_ms(200);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value_fast(ADC_Channel_1,1);		
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[5]=max-min;
	///////////////////////////////////////////////////////		
			Write_frequence(2,10000); //10kHz 进行取点采样
			Write_frequence(3,10000);
			delay_ms(200);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value_fast(ADC_Channel_1,1);		
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[6]=max-min;
///////////////////////////////////////////////////////			
			Write_frequence(2,32000); //32kHz 进行取点采样
			Write_frequence(3,32000);
			delay_ms(200);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value_fast(ADC_Channel_1,1);		
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[7]=max-min;
		///////////////////////////////////////////////////////	
			Write_frequence(2,100000); //100kHz 进行取点采样
			Write_frequence(3,100000);
			delay_ms(100);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value_fast(ADC_Channel_1,1);		
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[8]=max-min;
///////////////////////////////////////////////////////
			Write_frequence(2,320000); //320kHz 进行取点采样
			Write_frequence(3,320000);
			delay_ms(200);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value_fast(ADC_Channel_1,1);		
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[9]=max-min;
///////////////////////////////////////////////////////
			Write_frequence(2,1000000); //1mHz 进行取点采样
			Write_frequence(3,1000000);
			delay_ms(100);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value_fast(ADC_Channel_1,1);		
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[10]=max-min;
			///////////////////////////////////////////////////////
			Write_frequence(2,3200000); //3.2mHz 进行取点采样
			Write_frequence(3,3200000);
			delay_ms(100);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value_fast(ADC_Channel_1,1);		
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[11]=max-min;
/////////////////////////////////////////////////////
			Write_frequence(2,10000000); //10mHz 进行取点采样
			Write_frequence(3,10000000);
			delay_ms(100);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value_fast(ADC_Channel_1,1);		
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[12]=max-min;
///////////////////////////////////////////////////////
			Write_frequence(2,32000000); //32mHz 进行取点采样
			Write_frequence(3,32000000);
			delay_ms(100);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value_fast(ADC_Channel_1,1);		
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[13]=max-min;
///////////////////////////////////////////////////////
			Write_frequence(2,100000000); //100mHz 进行取点采样
			Write_frequence(3,100000000);
			delay_ms(100);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value_fast(ADC_Channel_1,1);		
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[14]=max-min;
	///////////////////////////////////////////////////////
			Write_frequence(2,320000000); //320mHz 进行取点采样
			Write_frequence(3,320000000);
			delay_ms(100);
			for(i=0 ; i<50 ; i++)												//取50个点
			{
				value[i]=Get_ADC_Value_fast(ADC_Channel_1,1);		
			}
			max=value[0];
			for(i=0 ; i<49 ; i++)    			//取差值
			{	
					if(max<value[i])
					{
					max=value[i];
					}
			}
			ampfre[15]=max-min;

			x=10;
			POINT_COLOR = BLACK;
			y=220;
			
			for(i=0 ; i<16 ; i++)       //画出幅频特性曲线图
			{
	  	m=y;
			x+=15;
			y=220-(ampfre[i]*160/4096);
	  	LCD_DrawLine(x, m, x+15, y);
	//			LCD_ShowNum(290,15+x,y,5,12);
			}	
			POINT_COLOR = LGRAY;
	    LCD_DrawLine(20, 107, 260, 107);
			POINT_COLOR = BLACK;
		//		flag = 1;
}

void Rin()
{
		WriFre1000();
			x=15;
			underpainting();  //矩形区域为：(10,70,240,230);
			LCD_ShowString(250,170,50,16,16,"Rin :");
			LCD_ShowString(40,26,50,16,16,"PAGE 1");
			LCD_ShowString(100,26,150,16,16,"Rin MEASUREMENT");
			for(i=0 ; i<114 ; i++)
			{
				value[i]=Get_ADC_Value(ADC_Channel_2,1);		    //红线
			}
//			for(i=0 ; i<60 ; i++)
//			{	
//				v1[i]=Get_ADC_Value(ADC_Channel_1,1);
//			}
//			for(i=0 ; i<60 ; i++)
//			{	
//				v2[i]=Get_ADC_Value(ADC_Channel_3,1);
//			}
			
			y=value[1];POINT_COLOR =  BLACK;
			for(i=0 ; i<110 ; i++)                 //显示信号形状      
			{
				x+=2;
				m=y;
				y=230-(value[i]*160/4096);
				LCD_DrawLine(x, m, x+2, y);
			}	
			POINT_COLOR = BLACK;

			PA1max = value[0];
			for(i=0 ; i<100 ; i+=2)            //算出最大最小值
			{
				if(PA1max < value[i])
					PA1max = value[i];
			}
			
			PA1min = value[0];
			for(i=0 ; i<100 ; i+=2)
			{
				if(PA1min > value[i])
					PA1min = value[i];
			}
			PA1max = PA1max*3300/4096;
			PA1min = PA1min*3300/4096;
			PA1ff = PA1max - PA1min;          //峰峰值
			ri = (PA1ff*5600)/(620-PA1ff); 
			LCD_ShowNum(250,90,PA1ff,5,16);
			LCD_ShowNum(250,140,PA1max,5,16);
			LCD_ShowNum(250,190,ri,5,16);
   //   flag = 1;
		delay_ms(200);
}


void Rout()
{
//			led3 = 0;

		Write_frequence(2,1000); 
		Write_frequence(3,1000);
	  Write_Amplitude(2, 100);
		Write_Amplitude(3, 100);

			x=15;
				LCD_Clear(BLUE);
	LCD_ShowString(40,26,50,16,16,"PAGE 2");
	LCD_ShowString(100,26,150,16,16,"Rout MEASUREMENT");
	LCD_Fill(10,70,240,230,LIGHTBLUE);
	LCD_ShowString(250,70,50,16,16,"V1p-p:");
	
	LCD_ShowString(250,120,50,16,16,"V2p-p:");
			
			for(i=0 ; i<110 ; i++)
			{	 
				v1[i]=Get_ADC_Value(ADC_Channel_1,1);
			}
			led2 = 1;                                //继电器转换
			delay_ms(1000);delay_ms(1000);
			for(i=0 ; i<110 ; i++)
			{	
				v3[i]=Get_ADC_Value(ADC_Channel_1,1);
			}
			
			
			y=v3[1];POINT_COLOR =  BLACK;
			for(i=0 ; i<110 ; i++)
			{
				x+=2;
				m=y;
				y=230-(v3[i]*160/4096);
				LCD_DrawLine(x, m, x+2, y);
			}						
			y=v1[1];
				x=15;POINT_COLOR = RED;
			for(i=0 ; i<110 ; i++)
			{	
				x+=2;
				m=y;
				y=230-(v1[i]*160/4096);
				LCD_DrawLine(x, m, x+2, y);
			}		
			POINT_COLOR =  BLACK;
			PA1max = v1[0];
			for(i=0 ; i<110 ; i+=2)
			{
				if(PA1max < v1[i])
					PA1max = v1[i];
			}
			
			PA1min = v1[0];
			for(i=0 ; i<110 ; i+=2)
			{
				if(PA1min > v1[i])
					PA1min = v1[i];
			}
			
			PA2max = v3[0];
			for(i=0 ; i<110 ; i+=2)
			{
				if(PA2max < v3[i])
					PA2max = v3[i];
			}
			
			PA2min = v3[0];
			for(i=0 ; i<110 ; i+=2)
			{
				if(PA2min > v3[i])
					PA2min = v3[i];
			}
			PA2ff = PA2max - PA2min;
			PA1ff = PA1max - PA1min;
			LCD_ShowNum(250,90,PA2ff,5,16);
			LCD_ShowNum(250,140,PA1ff,5,16);
			ro = (((PA1ff-PA2ff)*22000*4700)*1.0)/(((PA2ff*4700)-(PA1ff*22000)));
	//		ro = 2*1.0/3*100*1.0;
			POINT_COLOR = BLACK;
			LCD_ShowString(250,170,60,3,16,"Rout:");
			LCD_ShowNum(250,190,ro,5,16);
			//flag = 1;
			led2 = 0;
}

void judgeC()
{

		Write_Amplitude(2, 1023);
		Write_Amplitude(3, 1023);
		Write_frequence(2,1000); 
		Write_frequence(3,1000);
		led3 = 1;
//			delay_ms(500);
			delay_ms(500);

					x=15;
				LCD_Clear(BLUE);
//				LCD_ShowString(10,6,200,16,12,"R");
//				LCD_ShowString(10,20,200,16,16,"ESTIMATE");	

			LCD_ShowString(40,10,50,16,16,"PAGE 6");
			LCD_ShowString(100,10,50,16,16,"FAULT DIAGNOSIS");	
			
					for(i=0 ; i<60 ; i++)
			{
				es[i]=Get_ADC_Value(ADC_Channel_3,1);		
			}
			
////*************************************
//			POINT_COLOR = GREEN;
//			for(i=0 ; i<199 ; i++)
//			{
//			x++;
//	  	m=y;
//			y=260-(es[i]*1200/4096);

//	//			if(i>193)break;
//	  	LCD_DrawLine( x, m, x++, y);
//			}
//			POINT_COLOR = BLACK;
////*************************************		
			
			
			esmax = es[0];
			for(i=0 ; i<59 ; i+=2)
			{
				if(esmax < es[i])
					esmax = es[i];
			}
			
			esmin = es[0];
			for(i=0 ; i<59 ; i+=2)
			{
				if(esmin > es[i])
					esmin = es[i];
			}
			esdc = (esmin+esmax)/2;
			esac = esmax - esmin;
			
			esdc = esdc*3300/4096;
			esac = esac*3300/4096;
			esmax = esmax*3300/4096;
			esmin = esmin*3300/4096;
			
			LCD_ShowString(20,40,200,6,16,"esmax:");
			LCD_ShowNum(80,40,esmax,5,16);
			LCD_ShowString(20,60,200,6,16,"esmin:");
			LCD_ShowNum(80,60,esmin,5,16);
			LCD_ShowString(20,80,200,6,16,"esdc:");
			LCD_ShowNum(80,80,esdc,5,16);
			LCD_ShowString(20,100,200,6,16,"esac:");
			LCD_ShowNum(80,100,esac,5,16);
			
//		  if(esdc>1400 && esdc<1700 && esac>1350 && esac < 1600)//7.3v为正常
//			{
//				LCD_ShowString(130,10,200,6,16,"NORMAL");
//			}
			if(esdc<1300 &&esdc>1150 && esac < 1450 && esac > 1300)//7.3v为正常
			{
				LCD_ShowString(130,30,200,16,16,"R2 disconnection");
			}
			
			if(esdc>1900 && esdc<2100 && esac < 160)//7.3v为正常
			{
				LCD_ShowString(130,50,200,16,16,"R1 disconnection");
			}
			if(esdc>2400 && esdc<2650 && esac>30 && esac < 160)//7.3v为正常
			{
				LCD_ShowString(130,70,200,16,16,"R1 shortcut");
			}
			

			if(esdc>1900 && esdc<2100 && esac < 160)//7.3v为正常
			{
				LCD_ShowString(130,90,200,16,16,"R2 shortcut");
			}
			
			if(esdc>2700 && esac < 160)//7.3v为正常
			{
				LCD_ShowString(130,90,200,16,16,"R3 shortcut");
			}
			
//			if(esdc>2400 && esac < 160)//7.3v为正常
//			{
//				LCD_ShowString(130,110,200,16,16,"R3 shortcut");
//			}
			
			if(esdc>0 && esdc<200 && esac>0 && esac < 200)//7.3v为正常
			{
				LCD_ShowString(130,110,200,16,16,"R3 disconnection");
			}
//			else if(esdc>2400 && esdc<2600 && esac>10 && esac < 160)//7.3v为正常
//			{
//				LCD_ShowString(130,70,200,16,16,"or R3 shortcut");
//			}
			
			if(esac>10	&& esac<100 && esdc>1900 && esdc < 2200)//7.3v为正常
			{
				LCD_ShowString(130,130,200,16,16,"R4 disconnection");
			}
			if(esmin<10)//7.3v为正常
			{
				LCD_ShowString(130,150,200,16,16,"R4 shortcut");
			}
//				if(esmin<10)//7.3v为正常
//			{
//				LCD_ShowString(130,170,200,16,16,"C1 disconnection");
//			}
//			if(esdc<2000 && esdc>1700 && esac<200)//7.3v为正常
//			{
//				LCD_ShowString(130,190,200,16,16,"C2 disconnection");
//			}
			delay_ms(1000);				flag = 1;
}

void estimate_device()
{
  	Write_Amplitude(2, 1023);
		Write_Amplitude(3, 1023);
		Write_frequence(2,1000000); 
		Write_frequence(3,1000000);
		OLED_ShowString(60,0,"highFre",16);
		OLED_ShowNum(0,40,1000,7,16);
		OLED_ShowString(60,40,"MHz", 16);		
			x=15;
			LCD_Clear(BLUE);
			LCD_ShowString(40,26,50,16,16,"PAGE 5");
			LCD_ShowString(100,26,150,16,16,"CAPAITY DIAGNOSIS");
			LCD_Fill(10,70,240,230,LIGHTBLUE);
		  	delay_ms(100);
//				delay_ms(200);
			for(i=0 ; i<220 ; i++)
			{
				value[i]=Get_ADC_Value_fast(ADC_Channel_3,1);		
				delay_ms(5);
			}
////*************************************
//			POINT_COLOR = GREEN;
//			for(i=0 ; i<199 ; i++)
//			{
//			x++;
//	  	m=y;
//			y=260-(value[i]*1200/4096);

//				if(i>193)break;
//	  	LCD_DrawLine(x, m, x++, y);
//			}
//			POINT_COLOR = BLACK;
////*************************************			
			
			
			for(i=0 ; i<190 ; i++)    //取差值
			{
				max=value[i];
				min=value[i];
				for(j=0;j<10;j++)
				{
					if(max<value[i+j])
					{
					max=value[i+j];
					}
					if(min>value[i+j])
					{
					min=value[i+j];
					}
				}
			value[i]=max-min;
				//fil=0;
			}
			
			for(i=0 ; i<190 ; i++)   //滤波
			{
				for(j=0;j<5;j++)
				{
					fil+=value[i+j];
				}
			value[i]=fil/5;
				fil=0;
			}
			max=value[0];
			for(i=5 ; i<150 ; i++)   //取最大
			{
				if(max<value[i])
					{
					max=value[i];
					}
			}
			LCD_ShowString(250,70,40,4,16,"amp:");
			LCD_ShowNum(250,90,2*max,5,16);
			max = 2*max;
			if(max>600 && max<800)
				LCD_ShowString(170,40,200,3,16,"NORMAL");
			if(max>210 && max<230)
				LCD_ShowString(170,40,200,3,16,"C1 open");
//			if(max<400)
//				LCD_ShowString(170,40,200,3,16,"C2 open");
			if(max>180 && max<220)
				LCD_ShowString(170,40,200,3,16,"C3 open");
//			if(max>400 && max<550)
//				LCD_ShowString(170,40,200,3,16,"C3 Double");
			if(max>178 && max<192)
				LCD_ShowString(170,40,200,3,16,"C1 Double");
			if(max>190 && max<220)
				LCD_ShowString(170,40,200,3,16,"C3 Double");
			POINT_COLOR = RED;
			for(i=0 ; i<220 ; i++)
			{
			x++;  
	  	m=y;
			y=230-(value[i]*160/4096);
				if(y<5)y=5;
				if(y>270)y=270;
				if(i>193)break;
	  	LCD_DrawLine(x, m, x++, y);
			}	
			POINT_COLOR = BLACK;
				flag = 1;
}

void gain()
{
	Write_Amplitude(2, 100);
	Write_Amplitude(3, 100);
	Write_frequence(2,1000); 
	Write_frequence(3,1000);
	LCD_Clear(BLUE);
	LCD_ShowString(40,26,50,16,16,"PAGE 3");
	LCD_ShowString(100,26,150,16,16,"GAIN MEASUREMENT");
	LCD_Fill(10,70,240,230,LIGHTBLUE);
	LCD_ShowString(250,70,65,16,16,"Vin p-p:");
	LCD_ShowString(250,120,65,16,16,"Vout p-p:");
	LCD_ShowString(250,170,50,16,16,"GAIN:");
				for(i=0 ; i<110 ; i++)
			{
				value[i]=Get_ADC_Value(ADC_Channel_2,1);		    //输入端
			}
			for(i=0 ; i<110 ; i++)
			{	
				v1[i]=Get_ADC_Value(ADC_Channel_1,1);						 //输出端
			}
			x=15;
			y=value[1];POINT_COLOR =  BRRED;
			for(i=0 ; i<110 ; i++)                 //显示输入端信号的形状
			{
				x+=2;
				m=y;
				y=230-(value[i]*160/4096);
				LCD_DrawLine(x, m, x+2, y);
			}	
			x=15;
			y=v1[1];POINT_COLOR =  RED;
			for(i=0 ; i<110 ; i++)                 //显示输出端信号的形状
			{
				x+=2;
				m=y;
				y=230-(v1[i]*160/4096);
				LCD_DrawLine(x, m, x+2, y);
			}	
			POINT_COLOR = BLACK;

			PA1max = value[0];
			for(i=0 ; i<100 ; i+=2)            //算出最大最小值
			{
				if(PA1max < value[i])
					PA1max = value[i];
			}
			PA1min = value[0];
			for(i=0 ; i<100 ; i+=2)
			{
				if(PA1min > value[i])
					PA1min = value[i];
			}
			PA2max = v1[0];               //算增益的时候再用
			PA2min = v1[0];
			for(i=0 ; i<60 ; i+=2)
			{
				if(PA2max < v1[i])
					PA2max = v1[i];
				if(PA2min > v1[i])
					PA2min = v1[i];
			}
			PA1max = PA1max*3300/4096;
			PA1min = PA1min*3300/4096;
			PA2max = PA2max*3300/4096;
			PA2min = PA2min*3300/4096;
			B=(PA2ff/PA1ff)*1.761*2;
			PA1ff = PA1max - PA1min;          //峰峰值
			PA2ff = PA2max - PA2min;          //峰峰值
			LCD_ShowNum(250,90,PA1ff,5,16);
			LCD_ShowNum(250,140,PA2ff,5,16);
			LCD_ShowNum(250,190,B,5,16);
			delay_ms(200);
      flag = 1;
}


