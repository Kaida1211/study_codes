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
#include <machine.h>
#include "vect.h"
#include "lcd_lib4.h"
#include "onkai.h"

const unsigned int GAKUHU[][2] =
{
		{MI1,400}
		/*,{MI1,200},{RE1,200},{DO1,400},{DO1,400},{RE1,400},{RE1,400},{MI1,200},
		{RE1,200},{DO1,400},{SO1,400},{SO1,200},{FA1,200},{MI1,400},{MI1,400},{RE1,200},
		{DO1,200},{RE1,200},{MI1,200},{DO1,400},{KU,400},{MI1,400},{MI1,200},{FA1,200},
		{SO1,400},{SO1,400},{RA1,400},{RA1,400},{SO1,200},{FA1,200},{MI1,400},{MI1,400},
		{MI1,200},{FA1,200},{SO1,400},{SO1,400},{RA1,400},{RA1,400},{SO1,400},{KU,400},*/
};

#define GAKUHU_LENGTH (sizeof(GAKUHU)/(sizeof(unsigned int)*2))

volatile unsigned long btime;

#define p 5000

volatile unsigned long time_10m_count;
volatile unsigned long time_1ms_count;
volatile unsigned int roi;

volatile char sw;

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
	CMT0.CMCOR = 250000/8-1;
	CMT0.CMCR.WORD |=0x00C0;
	IEN(CMT0,CMI0) = 1;
	IPR(CMT0,CMI0) = 1;

	CMT.CMSTR0.BIT.STR0 = 1;
}

void init_CMT1(void)
{
	SYSTEM.PRCR.WORD = 0x0A502;
	MSTP(CMT1) = 0;
	SYSTEM.PRCR.WORD = 0x0A500;
	CMT1.CMCR.WORD = 0x0000;
	CMT1.CMCOR = 25000/8-1;
	CMT1.CMCR.WORD |=0x00C0;
	IEN(CMT1,CMI1) = 1;
	IPR(CMT1,CMI1) = 1;

	CMT.CMSTR0.BIT.STR1 = 1;
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

void init_MTU1()
{
	SYSTEM.PRCR.WORD =0x0A502;
	MSTP(MTU1) = 0;
	SYSTEM.PRCR.WORD =0x0A500;

	PORT2.PMR.BIT.B4 = 1;
	PORT2.PMR.BIT.B5 = 1;
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.P24PFS.BIT.PSEL = 2;
	MPC.P25PFS.BIT.PSEL = 2;
	MPC.PWPR.BIT.PFSWE = 0;

	MTU1.TMDR.BIT.MD = 4;
	MTU1.TCNT = 0;
	MTU.TSTR.BIT.CST1 = 1;
}

void main(void)
{
	unsigned int roi_pre,roj;

	time_10m_count = 0;

	PORTH.PDR.BIT.B3 = 0;

	init_CLK();
	init_CMT0();
	init_CMT1();
	init_MTU1();
	init_LCD();
	init_BUZZER();
	setpsw_i();

	lcd_puts("RAMEN TIMER");
	lcd_xy(1,2);
	lcd_puts("00:00");
	flush_lcd();

//	roi = MTU1.TCNT / 4 ;

	while(1)
	{

		if(PORTH.PIDR.BIT.B3 == 0) sw = 1;

		if(roi != roi_pre)
		{
			roi_pre = roi;

			lcd_xy(1,2);
			lcd_puts("           ");


//			lcd_xy(1,2);
//			lcd_dataout(roi);
//			flush_lcd();


			//min
			lcd_xy(1,2);
			lcd_dataout((roi/600)%10);
			lcd_dataout((roi/60)%10);
			lcd_put(':');
			//sec
			lcd_dataout((roi/10)%6);
			lcd_dataout(roi%10);
			flush_lcd();

		}


		for(roj = 0; roj < p; roj++);

	}

}

#ifdef __cplusplus
void abort(void)
{

}
#endif
