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

#endif

#include "iodefine.h"
#include <machine.h>
#include "vect.h"
#include "lcd_lib4.h"
#include "onkai.h"

const unsigned int GAKUHU[][2] =
{
		{MI1,400},{MI1,200},{RE1,200},{DO1,400},{DO1,400},{RE1,400},{RE1,400},{MI1,200},
		{RE1,200},{DO1,400},{SO1,400},{SO1,200},{FA1,200},{MI1,400},{MI1,400},{RE1,200},
		{DO1,200},{RE1,200},{MI1,200},{DO1,400},{KU,400},{MI1,400},{MI1,200},{FA1,200},
		{SO1,400},{SO1,400},{RA1,400},{RA1,400},{SO1,200},{FA1,200},{MI1,400},{MI1,400},
		{MI1,200},{FA1,200},{SO1,400},{SO1,400},{RA1,400},{RA1,400},{SO1,400},{KU,400},
};

#define GAKUHU_LENGTH (sizeof(GAKUHU)/(sizeof(unsigned int)*2))

volatile unsigned long btime;
volatile unsigned char sw;

void init_CLK(void)
{
    int i;
	SYSTEM.PRCR.WORD = 0xA50F;
	SYSTEM.VRCR = 0x00;
	SYSTEM.SOSCCR.BIT.SOSTP = 1;
	while(SYSTEM.SOSCCR.BIT.SOSTP != 1);
	RTC.RCR3.BYTE = 0x0C;
	while(RTC.RCR3.BIT.RTCEN != 0);
	SYSTEM.MOFCR.BYTE = 0x30;
	SYSTEM.MOSCWTCR.BYTE = 0x0D;
	SYSTEM.MOSCCR.BIT.MOSTP = 0;
	while(SYSTEM.MOSCCR.BIT.MOSTP != 0);
    for(i = 0; i < 100; i++);
	SYSTEM.PLLCR.WORD = 0x0901;
	SYSTEM.PLLWTCR.BYTE = 0x09;
	SYSTEM.PLLCR2.BYTE = 0x00;
	for(i = 0; i < 100; i++);
	SYSTEM.OPCCR.BYTE = (0x00);
	while(0 != SYSTEM.OPCCR.BIT.OPCMTSF);
	SYSTEM.SCKCR.LONG = 0x21821211;
	while(SYSTEM.SCKCR.LONG != 0x21821211);
	SYSTEM.SCKCR3.WORD = 0x0400;
	while(SYSTEM.SCKCR3.WORD != 0x0400);
	SYSTEM.PRCR.WORD = 0xA500;
}

void init_CMT0(void)
{
	SYSTEM.PRCR.WORD = 0x0A502;
	MSTP(CMT0) = 0;
	SYSTEM.PRCR.WORD = 0x0A500;
	CMT0.CMCR.WORD = 0x0000;
	CMT0.CMCOR = 25000/8-1;
	CMT0.CMCR.WORD |=0x00C0;
	IEN(CMT0,CMI0) = 1;
	IPR(CMT0,CMI0) = 1;

	CMT.CMSTR0.BIT.STR0 = 1;
}

void init_IRQ(void){   //IRQ0とIRQ1の両方を設定している。
	IEN(ICU,IRQ0) = 0;
	IEN(ICU,IRQ1) = 0;
	ICU.IRQFLTE0.BIT.FLTEN0 = 0;
	ICU.IRQFLTE0.BIT.FLTEN1 = 0;
	ICU.IRQFLTC0.BIT.FCLKSEL0 = 3;
	ICU.IRQFLTC0.BIT.FCLKSEL1 = 3;
	PORTH.PDR.BIT.B1 = 0;
	PORTH.PDR.BIT.B2 = 0;
	PORTH.PMR.BIT.B1 = 1;
	PORTH.PMR.BIT.B2 = 1;
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.PH1PFS.BIT.ISEL = 1;
	MPC.PH2PFS.BIT.ISEL = 1;
	ICU.IRQCR[0].BIT.IRQMD = 1;
	ICU.IRQCR[1].BIT.IRQMD = 1;
	ICU.IRQFLTE0.BIT.FLTEN0 = 1;
	ICU.IRQFLTE0.BIT.FLTEN1 = 1;
	IR(ICU,IRQ0) = 0;
	IR(ICU,IRQ1) = 1;
	IEN(ICU,IRQ0) = 1;
	IEN(ICU,IRQ1) = 1;
	IPR(ICU,IRQ0) = 1;
	IPR(ICU,IRQ1) = 1;
}

void init_BUZZER(void)
{
	SYSTEM.PRCR.WORD = 0x0A502;
	MSTP(MTU0) = 0;
	SYSTEM.PRCR.WORD =0x0A500;

	PORT3.PDR.BIT.B4 = 1;
	PORT3.PMR.BIT.B4 = 1;
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.P34PFS.BIT.PSEL = 1;
	MPC.PWPR.BIT.PFSWE = 0;

	MTU.TSTR.BIT.CST0 = 0x00;
	MTU0.TCR.BIT.TPSC = 0x01;
	MTU0.TCR.BIT.CCLR = 0x01;
	MTU0.TMDR.BIT.MD = 0x02;
	MTU0.TIORH.BIT.IOA = 0x06;
	MTU0.TIORH.BIT.IOB = 0x05;
	MTU0.TCNT = 0;
}

void beep(unsigned int tone, unsigned int interval)
{
	if(tone)
	{
		MTU.TSTR.BIT.CST0 = 0;
		MTU0.TGRA=tone;
		MTU0.TGRB=tone/2;
		MTU.TSTR.BIT.CST0 = 1;
	}
	else
	{
		MTU.TSTR.BIT.CST0 = 0;
	}
	btime=interval;
}

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif


void main(void)
{
	int i;

	sw = 0;

	PORTH.PDR.BIT.B3;

	init_CLK();
	init_BUZZER();
	//init_IRQ();
	init_CMT0();
	setpsw_i();

	while(1)
	{
		i = GAKUHU_LENGTH;

		if(PORTH.PDR.BIT.B1 == 0) sw = 1;
		if(sw)
		{
			while(i)
			{
				beep(GAKUHU[GAKUHU_LENGTH-i][0],GAKUHU[GAKUHU_LENGTH-i][1]);

				while(btime)
				{
				}
				i--;

			}

			sw = 0;
		}
	}
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
