/*************************************
电路特性测试系统

功能描述：本系统以STM32F103ZET6为核心处理器，通过控制AD9959产生正弦波，配
合ILI9341、按键、继电器、VCA821程控放大器实现电路特性测试功能。
此系统能测量电路的输入电阻、输出电阻、增益、幅频特性曲线、能判断
电路故障原因，还能对信号进行频谱分析、滤波。
该系统实时性高、稳定，且人际互动界面友好，可通过触屏和按键进行控
置。

作者：韩铎
**************************************/
#include "lcd.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "oled.h"
#include "key.h"
#include "adc.h"
#include "task_manage.h"
#include "AD9959.h"
#include "timer.h"
#include "device.h"
#include "chart.h"
#include "ott2001a.h"	 
#include "touch.h"
#include "includes.h"  

u8 initshow;

//START 任务
//设置任务优先级
#define START_TASK_PRIO      			10 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				64
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);	

//触摸屏任务
//设置任务优先级
#define TOUCH_TASK_PRIO       		 	6
//设置任务堆栈大小
#define TOUCH_STK_SIZE  				128
//任务堆栈	
OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//任务函数
void touch_task(void *pdata);

//按键扫描任务
//设置任务优先级
#define KEY_TASK_PRIO       			5 
//设置任务堆栈大小
#define KEY_STK_SIZE  					128
//任务堆栈	
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//任务函数
void key_task(void *pdata);

//主任务
//设置任务优先级
#define MAIN_TASK_PRIO       			4 
//设置任务堆栈大小
#define MAIN_STK_SIZE  					256
//任务堆栈	
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//任务函数
void main_task(void *pdata);

OS_EVENT * startshow_ECB;			//按键邮箱事件块	  
OS_EVENT * page_ECB;			//按键邮箱事件块	  
OS_EVENT * initshow_ECB;			//按键邮箱事件块
OS_EVENT * sem_start;		//按键信号量指针	 

int main()
{

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组	
	Init_AD9959();				//DDS初始化
	tp_dev.init();				//触摸屏初始化
	LED_Init();					//LED初始化
	uart_init(115200);	 	//串口初始化为115200
  LCD_Init();			  //初始化液晶 
	KEY_Init();	 		 //按键初始化
	ADCx_Init();			//ADC电路初始化
	LCD_Display_Dir(1);       //横屏
	LCD->LCD_RAM = BLACK;			//设置画笔颜色
	LCD_Clear(BLUE);        //清屏
	led2 = 0;
	led5 = 0;	
	Show_Picture();    //显示起始图片
	while(1)     //屏幕校准
	{
		tp_dev.scan(1);//扫描物理坐标
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)//按键按下了一次(此时按键松开了.)
		{tp_dev.sta&=~(1<<6);//标记按键已经被处理过了.
			break;
		}
	}
  delay_ms(500); 
	TP_Adjust();  	   //屏幕校准  
	OSInit();  	 				//初始化UCOSII
  OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();      //UCOSII开始运行
	while(1);
}

void start_task(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
	u8 err;	    	    
	pdata = pdata; 	
  startshow_ECB = OSMboxCreate((void*)0);	//创建邮箱（startshow）
	page_ECB = OSMboxCreate((void*)0);	//创建邮箱（page）
	sem_start = OSSemCreate(0);  //创建信号量
	OSStatInit();						//初始化统计任务.这里会延时1秒钟左右	
 	OS_ENTER_CRITICAL();				//进入临界区(无法被中断打断)    			   
 	OSTaskCreate(touch_task,(void *)0,(OS_STK*)&TOUCH_TASK_STK[TOUCH_STK_SIZE-1],TOUCH_TASK_PRIO);	 //创建触屏任务				    				   
 	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);	 		//创建主任务		    				   
 	OSTaskCreate(key_task,(void *)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);	 	//创建按键任务			   
 	OSTaskSuspend(START_TASK_PRIO);		//挂起起始任务.
	OS_EXIT_CRITICAL();					//退出临界区(可以被中断打断)
}

void key_task(void *pdata)  //按键任务
{	
	u8 key_read=0;
  u8 startshow=0;
	u8 page=0;
  POINT_COLOR=BLACK;	//设置背景颜色为黑色
	while(1)   //进入循环
	{
		key_read = KEY_Scan(1);   //按键扫描
		if(key_read != 0)      //当有按键按下的时候
		{
			if(key_read == 5)    //当key5被按下
			{
				page = 0;    //进入扫频模式
				startshow = 1;  
			}
			else if(key_read == 1)    //当key1被按下
			{
				page ++;  //到下一页
				if(page > 6)   //当到最后一页时
				{
					page = 0;  //转入第一页
				}
				startshow = 1;
				delay_ms(500);
			}
			else if(key_read == 2)   
			{
				startshow = 1;     //重新测
			}
			else if(key_read == 4)    
			{
			}
			else if(key_read == 6)   //当key3被按下
			{
				page --; //页面跳到上一页
				if(page < 0) //当转到第一页时
				{
					page = 6;  //转到最后一页
				}
				startshow = 1;
				delay_ms(200); //延时200毫秒去抖
			}	
			OSMboxPost(page_ECB,(void*)page);//发送消息
			OSMboxPost(startshow_ECB,(void*)startshow);//发送消息
			OSSemPost(sem_start);  //当有按键按下时，信号量开始
		}
 		delay_ms(20);
	}
}

void touch_task(void *pdata) //触屏任务
{	  	

  u8 startshow=0;	
	u8 page=0;
	while(1)  //进入触屏任务循环
		{
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
			if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>(lcddev.width-34) && tp_dev.y[0]<26)
				{	
					initshow = 0;
				}
				if(tp_dev.x[0]>296 && tp_dev.y[0]>36 && tp_dev.y[0]<60)
				{	
					startshow = 1;
			  OSMboxPost(startshow_ECB,(void*)startshow);//发送消息
				}
				if(tp_dev.x[0]>20 && tp_dev.x[0]<120 && tp_dev.y[0]>114 && tp_dev.y[0]<138)
				{
					page = 1;  startshow = 1;
					OSMboxPost(page_ECB,(void*)page);//发送消息
			  OSMboxPost(startshow_ECB,(void*)startshow);//发送消息
				}	
				if(tp_dev.x[0]>20 && tp_dev.x[0]<120 && tp_dev.y[0]>142 && tp_dev.y[0]<166)
				{
					page = 2; startshow = 1;
					OSMboxPost(page_ECB,(void*)page);//发送消息
			  OSMboxPost(startshow_ECB,(void*)startshow);//发送消息
				}	
				if(tp_dev.x[0]>20 && tp_dev.x[0]<120 && tp_dev.y[0]>174 && tp_dev.y[0]<202)
				{
					page = 3; startshow = 1;
					OSMboxPost(page_ECB,(void*)page);//发送消息
			  OSMboxPost(startshow_ECB,(void*)startshow);//发送消息
				}	
				if(tp_dev.x[0]>20 && tp_dev.x[0]<120 && tp_dev.y[0]>204 && tp_dev.y[0]<234)
				{
					page = 0;  startshow = 1;
					OSMboxPost(page_ECB,(void*)page);//发送消息
			  OSMboxPost(startshow_ECB,(void*)startshow);//发送消息
				}	
				if(tp_dev.x[0]>160 && tp_dev.x[0]<260 && tp_dev.y[0]>114 && tp_dev.y[0]<138)
				{
					page = 6; startshow = 1;
					OSMboxPost(page_ECB,(void*)page);//发送消息
			  OSMboxPost(startshow_ECB,(void*)startshow);//发送消息
				}	
				if(tp_dev.x[0]>160 && tp_dev.x[0]<260 && tp_dev.y[0]>142 && tp_dev.y[0]<166)
				{
					page = 5;  startshow = 1;
					OSMboxPost(page_ECB,(void*)page);//发送消息
			  OSMboxPost(startshow_ECB,(void*)startshow);//发送消息
				}	
				if(tp_dev.x[0]>160 && tp_dev.x[0]<260 && tp_dev.y[0]>174 && tp_dev.y[0]<202)
			  {
					page = 4; startshow = 1;
					OSMboxPost(page_ECB,(void*)page);//发送消息
			  OSMboxPost(startshow_ECB,(void*)startshow);//发送消息
				}			
				OSSemPost(sem_start);
				delay_ms(100);		  
			}
		}
	}
}  

//主任务
void main_task(void *pdata)
{							 
	OS_CPU_SR cpu_sr=0;
	u8  key_read=0;		
	u8 i;
	u8 err;
  u8 startshow;
	u8 page=0;
	startshow = 0;
	i = 0;
	initshow = 0;
	POINT_COLOR=BLACK;
	while(1)
	{		 
		
		if(initshow == 0)   //如果initshow为0，则进入初始化界面
		{
		  initshow = 1;
		  OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
			initShow();  //显示初始化界面
			OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
		}
		
		if(startshow == 1) //如果startshow为1，
		{
			OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
			if(page == 0)    //如果page为0，则进入
			{
				amp_fre();		//进入扫频函数
			}		
			else if(page == 1)			//输入电阻并且显示
			{
				Rin();
			}		
			else if(page == 2)		//算输出电阻并且显示
			{
				Rout();
			}
			else if(page == 3)		//算增益并且显示
			{
				gain();
			}
			else if(page == 4)		//示波埔约捌灯滓趋并且显示
			{
				spectrometer();
			}
			else if(page == 5)		//判断电路故障原因并且显示
			{
				estimate_device();
			}
			else if(page == 6)   //评估电路故障原因并显示
			{
				judgeC();
			}
			startshow = 0;
			LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
      LCD_ShowString(lcddev.width-24,38,200,16,16,"FRE");//显示刷新区域
			OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
	  }
		i++;
		if(i>50)
		{
		 i=0;
		 led5 = !led5; //通过led5引脚产生方波
		}
		delay_ms(20); //延时20ms
		OSSemPend(sem_start,0,&err); //开始请求信号量sem_start
		startshow = (u8)OSMboxPend(startshow_ECB,10,&err); //startshow申请邮箱得到数据
		page = (u8)OSMboxPend(page_ECB,10,&err); //page申请邮箱得到数据
	}
	
}		   

