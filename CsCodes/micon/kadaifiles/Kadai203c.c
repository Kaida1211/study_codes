/***********************************************************************/
/*                                                                     */
/*  FILE        : Main.c                                   */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

#include "iodefine.h"
#include "lcd_lib4.h"
#include <machine.h>
#include "vect.h"

volatile unsigned long time_10m_count;

volatile char disp_txt[] = "Polytech College Niigata";
volatile char disp_txt16[17];
volatile unsigned char str_index;
volatile unsigned char str_count;

void init_CMT0(void)
{
	SYSTEM.PRCR.WORD = 0x0A502;
	MSTP(CMT0)=0;
	SYSTEM.PRCR.WORD = 0x0A500;
	CMT0.CMCR.WORD = 0x0000;
	CMT0.CMCOR = 250000/8-1;
	CMT0.CMCR.WORD|=0x00C0;
	IEN(CMT0,CMI0)=1;
	IPR(CMT0,CMI0)=1;

	CMT.CMSTR0.BIT.STR0 = 1;
}

void init_CLK(void)
{
	int i;

	SYSTEM.PRCR.WORD = 0xA50F;
	SYSTEM.VRCR = 0x00;
	SYSTEM.SOSCCR.BIT.SOSTP = 1;
	while(SYSTEM.SOSCCR.BIT.SOSTP !=1);
	RTC.RCR3.BYTE = 0x0C;
	while(RTC.RCR3.BIT.RTCEN !=0);
	SYSTEM.MOFCR.BYTE = 0x30;
	SYSTEM.MOSCWTCR.BYTE = 0x0D;
	SYSTEM.MOSCCR.BIT.MOSTP = 0;
	while(SYSTEM.MOSCCR.BIT.MOSTP != 0);
	for(i = 0; i < 100; i++);
	SYSTEM.PLLCR.WORD = 0x0901;
	SYSTEM.PLLWTCR.BYTE = 0x09;
	SYSTEM.PLLCR2.BYTE = 0x00;
	for(i = 0;i < 100;i++);
	SYSTEM.OPCCR.BYTE = (0x00);
	while (0 != SYSTEM.OPCCR.BIT.OPCMTSF);
	 SYSTEM.SCKCR.LONG = 0x21821211;

	while(SYSTEM.SCKCR.LONG != 0x21821211);
	SYSTEM.SCKCR3.WORD = 0x400;
	while(SYSTEM.SCKCR3.WORD != 0x400);
	SYSTEM.PRCR.WORD = 0xA500;
}
void main(void)
{
volatile char *p = disp_txt;
init_CLK();
init_CMT0();
init_LCD();

str_index = 0;
str_count = 0;
while(*p++)
	str_count++;
disp_txt16[16] = 0;

while(1){

}

}





#ifdef __cplusplus
void abort(void)
{

}
#endif


































