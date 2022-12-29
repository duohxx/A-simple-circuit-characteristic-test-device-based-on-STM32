#include "stm32f10x.h"
#include "SysTick.h"
#include "KeyBoard.h"
#include "led.h"
#include "oled.h"

void KeyBoard_Init(void) 
{ 
GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_KEY_GPIO,ENABLE);    
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
		GPIO_Init(KEY_GPIO, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  
		GPIO_Init(KEY_GPIO, &GPIO_InitStructure);  
							
		GPIO_SetBits(KEY_GPIO,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
		GPIO_ResetBits(KEY_GPIO,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
     
}
u8 Read_KeyValue(void)
{         
		u8 KeyValue = 0;
		if(GPIO_ReadInputData(KEY_GPIO)&0xf0)
		{
				delay_ms(10);
				if(GPIO_ReadInputData(KEY_GPIO)&0xf0)
				{
						GPIO_SetBits(KEY_GPIO,GPIO_Pin_0);
						GPIO_ResetBits(KEY_GPIO,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
						switch(GPIO_ReadInputData(KEY_GPIO)&0xff)
						{
							case 0x11:KeyValue = 13; break;
							case 0x21:KeyValue = 12; break;
							case 0x41:KeyValue = 11; break;
							case 0x81:KeyValue = 10; break; 
							
				    }
						GPIO_SetBits(KEY_GPIO,GPIO_Pin_1);
						GPIO_ResetBits(KEY_GPIO,GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3);
						switch(GPIO_ReadInputData(KEY_GPIO)&0xff)
						{
							case 0x12:KeyValue = 15; break;    //#  15
							case 0x22:KeyValue = 9; break;
							case 0x42:KeyValue = 6; break;
							case 0x82:KeyValue = 3; break;        
						}
						GPIO_SetBits(KEY_GPIO,GPIO_Pin_2);
						GPIO_ResetBits(KEY_GPIO,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3);
						switch(GPIO_ReadInputData(KEY_GPIO)&0xff)
						{
							case 0x14:KeyValue = 0; break;
							case 0x24:KeyValue = 8; break;
							case 0x44:KeyValue = 5; break;
							case 0x84:KeyValue = 2; break;        
						}
						GPIO_SetBits(KEY_GPIO,GPIO_Pin_3);
						GPIO_ResetBits(KEY_GPIO,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
						switch(GPIO_ReadInputData(KEY_GPIO)&0xff)
						{
							case 0x18:KeyValue = 14; break;   //*14
							case 0x28:KeyValue = 7; break;
							case 0x48:KeyValue = 4; break;
							case 0x88:KeyValue = 1; break;        
						}
						
						while(GPIO_ReadInputData(KEY_GPIO)&0xf0);
						GPIO_SetBits(KEY_GPIO,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
			    	GPIO_ResetBits(KEY_GPIO,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
						return KeyValue;
				}		
		}
		return 0xff;
}


 void GetKeyVal(void) 
{
    int num;
	  num = Read_KeyValue();
	  switch(num)
	  { 
        case 0: {OLED_ShowString(0,40," 0",16); }break;					  				      
				case 1: OLED_ShowString(0,40," 1",16); break;					  				       
				case 2: OLED_ShowString(0,40," 2",16); break;					  				     
				case 3: OLED_ShowString(0,40," 3",16); break;					  				     
				case 4: OLED_ShowString(0,40," 4",16); break;				 	       
				case 5: OLED_ShowString(0,40," 5",16); break;					  				      
				case 6: OLED_ShowString(0,40," 6",16); break;					  				       
				case 7: OLED_ShowString(0,40," 7",16); break;					 			       
				case 8: OLED_ShowString(0,40," 8",16); break;								 		       
				case 9: OLED_ShowString(0,40," 9",16); break;							 				     	
			  case 10: OLED_ShowString(0,40," A",16); break;						 				      		
				case 11: OLED_ShowString(0,40," B",16); break;					 				      
				case 12: OLED_ShowString(0,40," C",16); break;							 				      	
			  case 13: OLED_ShowString(0,40," D",16); break;							 				       	
				case 14: OLED_ShowString(0,40," E",16); break;					 				      
				case 15: OLED_ShowString(0,40," F",16); break;	
		//	default:	OLED_ShowString(0,40," lll",16);break;			
    }
}