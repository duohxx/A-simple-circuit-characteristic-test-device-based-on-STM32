#ifndef _KEYBOARD_H
#define _KEYBOARD_H         
#include "system.h"
void KeyBoard_Init(void);
u8 Read_KeyValue(void);
#define KEY_GPIO GPIOF
#define RCC_APB2Periph_KEY_GPIO RCC_APB2Periph_GPIOF

#endif
