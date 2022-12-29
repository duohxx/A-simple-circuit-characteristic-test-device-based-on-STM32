#ifndef _chart_H
#define _chart_H

#include "system.h"


extern const unsigned char gImage_sss[145280];
void WriFre1000(void);
void initShow(void);
void underpainting(void);
void spectrometer(void);
void DSP_FFT1024(void);
void Compare_Max(void);
void PowerMag(u16 FFT_NPT);
void Show_Picture(void);//œ‘ æ“ª’≈Õº∆
void Load_Drow_Dialog(void);	 	
void rtp_test(void); 					//µÁ◊Ë∆¡≤‚ ‘

#endif