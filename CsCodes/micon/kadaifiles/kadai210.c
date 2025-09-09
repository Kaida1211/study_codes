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

#include<stdio.h>

#include "iodefine.h"
#include"lcd_lib4.h"
#include<machine.h>
#include"vect.h"
#include "onkai.h"

const unsigned int GAKUHU[][2] = {
		{MI1,400},{MI1,200},{RE1,200},{DO1,400},{DO1,400},{RE1,400},{RE1,400},{MI1,200},
		{RE1,200},{DO1,400},{SO1,400},{SO1,200},{FA1,200},{MI1,400},{MI1,400},{RE1,200},
		{DO1,200},{RE1,200},{MI1,200},{DO1,400},{KU,400},{MI1,400},{MI1,200},{FA1,200},
		{SO1,400},{SO1,400},{RA1,400},{RA1,400},{SO1,200},{FA1,200},{MI1,400},{MI1,400},
		{MI1,200},{FA1,200},{SO1,400},{SO1,400},{RA1,400},{RA1,400},{SO1,400},{KU,400},
};

const unsigned int GAKUHU2[][2] = {
		{RE1,400},{DO1,400},{RE1,400},{MI1,400},{SO1,400},{MI1,400},{RE1,400},{MI1,400},
		{SO1,400},{RA1,400},{SO1,400},{RA1,400},{RE1,400},{SI1,400},{RA1,400},{SO1,400},
		{MI1,400},{SO1,400},{RA1,400},{RE1,400},{DO1,400},{RE1,400},{MI1,400},{SO1,400},
		{RA1,400},{SO1,400},{MI1,400},{SO1,400},{RE1,400},{RA1,400},{DO1,400},{RE1,400},
		{DO1,400},{RE1,400},{RA1,400},{SO1,400},{RA1,400},{SO1,400},{MI1,400},{RE1,400},
};

#define GAKUHU_LENGTH (sizeof(GAKUHU)/(sizeof(unsigned int)*2))
#define GAKUHU_LENGTH2 (sizeof(GAKUHU2)/(sizeof(unsigned int)*2))

#define p 5000

volatile unsigned long time_10m_count;
volatile unsigned long btime;
volatile unsigned int roi;

volatile unsigned char sw_case1;		//ストップウォッチ用
volatile unsigned char sw_case2;
volatile unsigned char sw_case3;
volatile unsigned char sw_case4;

volatile unsigned long tc;
volatile long temp_total;

unsigned char mode;
unsigned char pre_mode;
unsigned char cur_sw;
unsigned char pre_sw;


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

//音楽演奏用
void init_CMT1(void)
{
	SYSTEM.PRCR.WORD = 0x0A502;
	MSTP(CMT1)=0;
	SYSTEM.PRCR.WORD = 0x0A500;
	CMT1.CMCR.WORD = 0x0000;
	CMT1.CMCOR = 25000/8-1;
	CMT1.CMCR.WORD|=0x00C0;
	IEN(CMT1,CMI1)=1;
	IPR(CMT1,CMI1)=1;

	CMT.CMSTR0.BIT.STR1 = 1;
}

//SW6 割り込み
void init_IRQ0(void)
{
	IEN(ICU,IRQ0) = 0;
	ICU.IRQFLTE0.BIT.FLTEN0 = 0;
	ICU.IRQFLTC0.BIT.FCLKSEL0 = 3;
	PORTH.PDR.BIT.B1 = 0;
	PORTH.PMR.BIT.B1 = 0;
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.PH1PFS.BIT.ISEL = 1;
	ICU.IRQCR[0].BIT.IRQMD = 1;
	ICU.IRQFLTE0.BIT.FLTEN0 = 1;
	IR(ICU,IRQ0) = 0;
	IEN(ICU,IRQ0) = 1;
	IPR(ICU,IRQ0) = 1;
}

//SW7 割り込み
void init_IRQ1(void)
{
	IEN(ICU,IRQ1) = 0;
	ICU.IRQFLTE0.BIT.FLTEN1 = 0;
	ICU.IRQFLTC0.BIT.FCLKSEL0 = 3;
	PORTH.PDR.BIT.B2 = 0;
	PORTH.PMR.BIT.B2 = 0;
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.PH2PFS.BIT.ISEL = 1;
	ICU.IRQCR[0].BIT.IRQMD = 1;
	ICU.IRQFLTE0.BIT.FLTEN1 = 1;
	IR(ICU,IRQ1) = 0;
	IEN(ICU,IRQ1) = 1;
	IPR(ICU,IRQ1) = 1;
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

void init_AD0(void)
{
	SYSTEM.PRCR.WORD = 0xA502;				//プロテクト解除
	MSTP(S12AD) = 0;						//A/Dコンバータ モジュールスタンバイ
	SYSTEM.PRCR.WORD = 0xA500;				//プロテクト設定
	PORT4.PMR.BIT.B0 = 1;					//P40 を周辺機器として使用
	S12AD.ADCSR.BIT.ADIE = 0;				//割り込みは使わない
	S12AD.ADANSA.BIT.ANSA0 = 1;				//アナログ入力チャネル選択 AN000
	S12AD.ADCSR.BIT.ADCS = 0;				//シングルスキャンモード
	MPC.PWPR.BIT.B0WI = 0;					//PFSWE 書き込み可
	MPC.PWPR.BIT.PFSWE = 1;					//PFSレジスタへの書き込み
	MPC.P40PFS.BIT.ASEL = 1;				//P40をAN000として使用
	MPC.PWPR.BIT.PFSWE = 0;					//PFSレジスタへの書き込み禁止
}

//温度測定
void MEAS_TEMP(void)
{
	long temp_ave;

	S12AD.ADCSR.BIT.ADST = 1;				//A\D変換スタート
	while(S12AD.ADCSR.BIT.ADST == 1);		//変換終了を待つ

	temp_total += S12AD.ADDR0*4700/4096/2-600;

	if(time_10m_count == 50)
	{
		temp_ave = temp_total / 50;
		lcd_xy(1,2);
		lcd_puts("       ");
		lcd_xy(1,2);
		lcd_dataout(temp_ave/10);
		lcd_put('.');
		lcd_dataout(temp_ave%10);
		lcd_put(0xdf);							//LCD表示 右上半濁音
		lcd_put(0x43);							//LCD表示 C
		flush_lcd();

		time_10m_count = 0;
		temp_total = 0;
	}
}

void main(void)
{

	int i;	//音楽演奏 楽譜用変数
	int sound = 0; //楽譜切り替え用変数
	unsigned int roi_pre,roj;

	PORTH.PDR.BIT.B0 = 0;  //状態遷移sw

	PORTH.PDR.BIT.B3 = 0;  //音楽演奏  押し判定用sw

	init_CLK();
	init_BUZZER();
	init_CMT0();
	init_CMT1();	//音楽演奏用
	init_MTU1();
	init_IRQ0();
	init_IRQ1();
	init_AD0();
	init_LCD();

	setpsw_i();

	time_10m_count = 0;
	mode = 1;
	pre_mode = 0;
	pre_sw = PORTH.PIDR.BIT.B0;

	while(1)
	{
		cur_sw = PORTH.PIDR.BIT.B0;
		if((pre_sw == 0) && (cur_sw == 1))
		{
			++mode;
			if(mode > 4) mode = 1;
		}
		pre_sw = cur_sw;


		switch(mode)
		{
			// ストップウォッチ
			case 1:
				sw_case4 = 0;
				if(pre_mode != mode)
				{
					time_10m_count = 0;
					while(time_10m_count < 10);
					lcd_clear();
					lcd_xy(1,1);
					lcd_puts("STOP WATCH");
					lcd_xy(1,2);
					lcd_puts("00:00:00");
					time_10m_count = 0;   //チャタリング除去後の初期化
					flush_lcd();
				}

				if(PORTH.PIDR.BIT.B3 == 0)
				{
					sw_case1 = 1;
					time_10m_count = 0;
				}


		     break;

		    // 音楽演奏
			case 2:
				sw_case1 = 0;
				if(pre_mode != mode)
				{
					time_10m_count = 0;
					while(time_10m_count < 10);
					lcd_clear();
					lcd_xy(1,1);
					lcd_puts("Music");
					time_10m_count = 0;
					flush_lcd();
				}

				if(PORTH.PIDR.BIT.B3 == 0)
				{

					sound++;
					if(sound > 3) sound = 0;
					if(sound == 1) i = GAKUHU_LENGTH;
					if(sound == 2) i = GAKUHU_LENGTH2;

					while(i)
					{
						beep(GAKUHU[GAKUHU_LENGTH-i][0],GAKUHU[GAKUHU_LENGTH-i][1]);

						while(btime){}
						i--;
					}
				}

			break;

			// 温度計
			case 3:
				sw_case2 = 0;
				sw_case3 = 1;
				if(pre_mode != mode)
				{
					time_10m_count = 0;
					while(time_10m_count < 10);
					lcd_clear();
					lcd_xy(1,1);
					lcd_puts("Temperature");
					time_10m_count = 0;
					flush_lcd();
				}

			break;

			// ラーメンタイマ
			case 4:
				sw_case3 = 0;

				if(pre_mode != mode)
				{
					time_10m_count = 0;
					while(time_10m_count < 10);
					lcd_clear();
					lcd_xy(1,1);
					lcd_puts("RAMEN TIMER");
					lcd_xy(1,2);
					lcd_puts("00:00");
					flush_lcd();
				}

				if(PORTH.PIDR.BIT.B3 == 0) sw_case4 = 1;

				if(roi != roi_pre)
				{
					roi_pre = roi;

					lcd_xy(1,2);
					lcd_puts("           ");

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

			break;

			default:
			break;
		}
		pre_mode = mode;
	}
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
