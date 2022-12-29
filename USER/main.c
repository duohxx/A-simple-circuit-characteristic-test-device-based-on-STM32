/*************************************
��·���Բ���ϵͳ

������������ϵͳ��STM32F103ZET6Ϊ���Ĵ�������ͨ������AD9959�������Ҳ�����
��ILI9341���������̵�����VCA821�̿طŴ���ʵ�ֵ�·���Բ��Թ��ܡ�
��ϵͳ�ܲ�����·��������衢������衢���桢��Ƶ�������ߡ����ж�
��·����ԭ�򣬻��ܶ��źŽ���Ƶ�׷������˲���
��ϵͳʵʱ�Ըߡ��ȶ������˼ʻ��������Ѻã���ͨ�������Ͱ������п�
�á�

���ߣ�����
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

//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				64
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);	

//����������
//�����������ȼ�
#define TOUCH_TASK_PRIO       		 	6
//���������ջ��С
#define TOUCH_STK_SIZE  				128
//�����ջ	
OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//������
void touch_task(void *pdata);

//����ɨ������
//�����������ȼ�
#define KEY_TASK_PRIO       			5 
//���������ջ��С
#define KEY_STK_SIZE  					128
//�����ջ	
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//������
void key_task(void *pdata);

//������
//�����������ȼ�
#define MAIN_TASK_PRIO       			4 
//���������ջ��С
#define MAIN_STK_SIZE  					256
//�����ջ	
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//������
void main_task(void *pdata);

OS_EVENT * startshow_ECB;			//���������¼���	  
OS_EVENT * page_ECB;			//���������¼���	  
OS_EVENT * initshow_ECB;			//���������¼���
OS_EVENT * sem_start;		//�����ź���ָ��	 

int main()
{

	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��	
	Init_AD9959();				//DDS��ʼ��
	tp_dev.init();				//��������ʼ��
	LED_Init();					//LED��ʼ��
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
  LCD_Init();			  //��ʼ��Һ�� 
	KEY_Init();	 		 //������ʼ��
	ADCx_Init();			//ADC��·��ʼ��
	LCD_Display_Dir(1);       //����
	LCD->LCD_RAM = BLACK;			//���û�����ɫ
	LCD_Clear(BLUE);        //����
	led2 = 0;
	led5 = 0;	
	Show_Picture();    //��ʾ��ʼͼƬ
	while(1)     //��ĻУ׼
	{
		tp_dev.scan(1);//ɨ����������
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)//����������һ��(��ʱ�����ɿ���.)
		{tp_dev.sta&=~(1<<6);//��ǰ����Ѿ����������.
			break;
		}
	}
  delay_ms(500); 
	TP_Adjust();  	   //��ĻУ׼  
	OSInit();  	 				//��ʼ��UCOSII
  OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();      //UCOSII��ʼ����
	while(1);
}

void start_task(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
	u8 err;	    	    
	pdata = pdata; 	
  startshow_ECB = OSMboxCreate((void*)0);	//�������䣨startshow��
	page_ECB = OSMboxCreate((void*)0);	//�������䣨page��
	sem_start = OSSemCreate(0);  //�����ź���
	OSStatInit();						//��ʼ��ͳ������.�������ʱ1��������	
 	OS_ENTER_CRITICAL();				//�����ٽ���(�޷����жϴ��)    			   
 	OSTaskCreate(touch_task,(void *)0,(OS_STK*)&TOUCH_TASK_STK[TOUCH_STK_SIZE-1],TOUCH_TASK_PRIO);	 //������������				    				   
 	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);	 		//����������		    				   
 	OSTaskCreate(key_task,(void *)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);	 	//������������			   
 	OSTaskSuspend(START_TASK_PRIO);		//������ʼ����.
	OS_EXIT_CRITICAL();					//�˳��ٽ���(���Ա��жϴ��)
}

void key_task(void *pdata)  //��������
{	
	u8 key_read=0;
  u8 startshow=0;
	u8 page=0;
  POINT_COLOR=BLACK;	//���ñ�����ɫΪ��ɫ
	while(1)   //����ѭ��
	{
		key_read = KEY_Scan(1);   //����ɨ��
		if(key_read != 0)      //���а������µ�ʱ��
		{
			if(key_read == 5)    //��key5������
			{
				page = 0;    //����ɨƵģʽ
				startshow = 1;  
			}
			else if(key_read == 1)    //��key1������
			{
				page ++;  //����һҳ
				if(page > 6)   //�������һҳʱ
				{
					page = 0;  //ת���һҳ
				}
				startshow = 1;
				delay_ms(500);
			}
			else if(key_read == 2)   
			{
				startshow = 1;     //���²�
			}
			else if(key_read == 4)    
			{
			}
			else if(key_read == 6)   //��key3������
			{
				page --; //ҳ��������һҳ
				if(page < 0) //��ת����һҳʱ
				{
					page = 6;  //ת�����һҳ
				}
				startshow = 1;
				delay_ms(200); //��ʱ200����ȥ��
			}	
			OSMboxPost(page_ECB,(void*)page);//������Ϣ
			OSMboxPost(startshow_ECB,(void*)startshow);//������Ϣ
			OSSemPost(sem_start);  //���а�������ʱ���ź�����ʼ
		}
 		delay_ms(20);
	}
}

void touch_task(void *pdata) //��������
{	  	

  u8 startshow=0;	
	u8 page=0;
	while(1)  //���봥������ѭ��
		{
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//������������
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
			  OSMboxPost(startshow_ECB,(void*)startshow);//������Ϣ
				}
				if(tp_dev.x[0]>20 && tp_dev.x[0]<120 && tp_dev.y[0]>114 && tp_dev.y[0]<138)
				{
					page = 1;  startshow = 1;
					OSMboxPost(page_ECB,(void*)page);//������Ϣ
			  OSMboxPost(startshow_ECB,(void*)startshow);//������Ϣ
				}	
				if(tp_dev.x[0]>20 && tp_dev.x[0]<120 && tp_dev.y[0]>142 && tp_dev.y[0]<166)
				{
					page = 2; startshow = 1;
					OSMboxPost(page_ECB,(void*)page);//������Ϣ
			  OSMboxPost(startshow_ECB,(void*)startshow);//������Ϣ
				}	
				if(tp_dev.x[0]>20 && tp_dev.x[0]<120 && tp_dev.y[0]>174 && tp_dev.y[0]<202)
				{
					page = 3; startshow = 1;
					OSMboxPost(page_ECB,(void*)page);//������Ϣ
			  OSMboxPost(startshow_ECB,(void*)startshow);//������Ϣ
				}	
				if(tp_dev.x[0]>20 && tp_dev.x[0]<120 && tp_dev.y[0]>204 && tp_dev.y[0]<234)
				{
					page = 0;  startshow = 1;
					OSMboxPost(page_ECB,(void*)page);//������Ϣ
			  OSMboxPost(startshow_ECB,(void*)startshow);//������Ϣ
				}	
				if(tp_dev.x[0]>160 && tp_dev.x[0]<260 && tp_dev.y[0]>114 && tp_dev.y[0]<138)
				{
					page = 6; startshow = 1;
					OSMboxPost(page_ECB,(void*)page);//������Ϣ
			  OSMboxPost(startshow_ECB,(void*)startshow);//������Ϣ
				}	
				if(tp_dev.x[0]>160 && tp_dev.x[0]<260 && tp_dev.y[0]>142 && tp_dev.y[0]<166)
				{
					page = 5;  startshow = 1;
					OSMboxPost(page_ECB,(void*)page);//������Ϣ
			  OSMboxPost(startshow_ECB,(void*)startshow);//������Ϣ
				}	
				if(tp_dev.x[0]>160 && tp_dev.x[0]<260 && tp_dev.y[0]>174 && tp_dev.y[0]<202)
			  {
					page = 4; startshow = 1;
					OSMboxPost(page_ECB,(void*)page);//������Ϣ
			  OSMboxPost(startshow_ECB,(void*)startshow);//������Ϣ
				}			
				OSSemPost(sem_start);
				delay_ms(100);		  
			}
		}
	}
}  

//������
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
		
		if(initshow == 0)   //���initshowΪ0��������ʼ������
		{
		  initshow = 1;
		  OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
			initShow();  //��ʾ��ʼ������
			OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
		}
		
		if(startshow == 1) //���startshowΪ1��
		{
			OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
			if(page == 0)    //���pageΪ0�������
			{
				amp_fre();		//����ɨƵ����
			}		
			else if(page == 1)			//������貢����ʾ
			{
				Rin();
			}		
			else if(page == 2)		//��������貢����ʾ
			{
				Rout();
			}
			else if(page == 3)		//�����沢����ʾ
			{
				gain();
			}
			else if(page == 4)		//ʾ����Լ�Ƶ�����������ʾ
			{
				spectrometer();
			}
			else if(page == 5)		//�жϵ�·����ԭ������ʾ
			{
				estimate_device();
			}
			else if(page == 6)   //������·����ԭ����ʾ
			{
				judgeC();
			}
			startshow = 0;
			LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//��ʾ��������
      LCD_ShowString(lcddev.width-24,38,200,16,16,"FRE");//��ʾˢ������
			OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
	  }
		i++;
		if(i>50)
		{
		 i=0;
		 led5 = !led5; //ͨ��led5���Ų�������
		}
		delay_ms(20); //��ʱ20ms
		OSSemPend(sem_start,0,&err); //��ʼ�����ź���sem_start
		startshow = (u8)OSMboxPend(startshow_ECB,10,&err); //startshow��������õ�����
		page = (u8)OSMboxPend(page_ECB,10,&err); //page��������õ�����
	}
	
}		   

