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

#define p 5000

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
	int roi,roi_pre,roj;

	init_CLK();
	init_MTU1();
	init_LCD();
	setpsw_i();

	lcd_puts("rotary count");
	lcd_xy(1,2);
	lcd_dataout(roi);
	flush_lcd();

	while(1)
	{
		roi = MTU1.TCNT / 4;

		if(roi != roi_pre)
		{
			roi_pre = roi;

			lcd_xy(1,2);
			lcd_puts("     ");
			lcd_xy(1,2);
			lcd_dataout(roi);
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
