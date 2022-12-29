#ifndef _led_H
#define _led_H

#include "system.h"

/*  LED时钟端口、引脚定义 */
#define LED_PORT 			GPIOC   
#define LED_PIN 			(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_9)
#define LED_PORT_RCC		RCC_APB2Periph_GPIOC


#define led1 PGout(0)
#define led2 PCout(3)
#define led3 PCout(2)
#define led4 PBout(3)
#define led5 PCout(9)
#define led6 PFout(5)
#define led7 PFout(6)
#define led8 PFout(7)
#define LED1					PBout(2)
#define LED2					PAout(11)
void LED_Init(void);


#endif
