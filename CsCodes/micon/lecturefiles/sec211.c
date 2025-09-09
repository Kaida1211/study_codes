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

//10msecタイマのカウント用
volatile unsigned long time_10m_count;

unsigned char mode;			//現在モード記憶変数
unsigned char pre_mode;		//前回モード記憶変数
unsigned char cur_sw;		//現在モードスイッチ変数
unsigned char pre_sw;		//前回モードスイッチ変数

//---------------------------------------------
// システムクロック設定
//---------------------------------------------
// システムクロック 25MHz に設定
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

//---------------------------------------------
// タイマ割り込み関連
//---------------------------------------------
//CMT0 10msec 毎に割り込み発生
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


void main(void)
{
	unsigned long tc;

	//モード変更スイッチの接続ポート
	PORTH.PDR.BIT.B0 = 0;

	init_CLK();
	init_CMT0();
	init_LCD();
	setpsw_i();

	time_10m_count = 0;  //10msタイマカウントのリセット
	mode = 1;            //現在のモードの初期化(モード1から始める)
	pre_mode = 0;		//

	pre_sw = PORTH.PIDR.BIT.B0;

	while(1)
	{
		cur_sw = PORTH.PIDR.BIT.B0;
		if((pre_sw == 0)&&(cur_sw == 1))  //0 離したタイミング
		{
			mode++;			//この行をコメントアウトして↓
			if(mode > 3)   //if(++mode > 3) でも可能
			{
				mode = 1;
			}
			pre_sw = cur_sw;
		}
			switch(mode)	//モード別処理
			{
				case 1:		//モード1
					if(pre_mode != mode)
					{
						time_10m_count = 0;
						while(time_10m_count < 10);   //0.1秒待ち チャタリング防止
						lcd_clear();
						lcd_xy(1,1);
						lcd_puts("0.01 timer");
						flush_lcd();
					}
				break;

				case 2:		//モード2
					if(pre_mode != mode)
					{
						time_10m_count = 0;
						while(time_10m_count < 10);   //0.1秒待ち チャタリング防止
						lcd_clear();
						lcd_xy(1,1);
						lcd_puts("0.01 timer");
						flush_lcd();
					}

					tc = time_10m_count;
					lcd_xy(8,2);
					lcd_dataout(tc % 10);
					tc /= 10;
					lcd_xy(7,2);
					lcd_dataout(tc % 6);
					tc /= 6;
					lcd_xy(5,2);
					lcd_dataout(tc % 10);
					lcd_put(':');
					tc /= 10;
					lcd_xy(4,2);
					lcd_dataout(tc % 6);
					lcd_xy(2,2);
					lcd_dataout(tc % 10);
					tc /= 10;
					lcd_xy(1,2);
					lcd_dataout(tc % 10);

					flush_lcd();

				break;

				case 3:		//モード3
					if(pre_mode != mode)
					{
						time_10m_count = 0;
						while(time_10m_count < 10);   //0.1秒待ち チャタリング防止
						lcd_clear();
						lcd_xy(1,1);
						lcd_puts("1 timer");
						flush_lcd();
					}

					tc = time_10m_count/100;
					lcd_xy(8,2);
					lcd_dataout(tc % 10);
					tc /= 10;
					lcd_xy(7,2);
					lcd_dataout(tc % 6);
					tc /= 6;
					lcd_xy(5,2);
					lcd_dataout(tc % 10);
					lcd_put(':');
					tc /= 10;
					lcd_xy(4,2);
					lcd_dataout(tc % 6);
					lcd_xy(2,2);
					lcd_dataout(tc % 10);
					tc /= 10;
					lcd_xy(1,2);
					lcd_dataout(tc % 2);

					flush_lcd();

				break;

			}

	}
}

