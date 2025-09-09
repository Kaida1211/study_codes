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

#include"iodefine.h"
#include"lcd_lib4.h"


void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

int cnt;
void init_CLK(void);

void init_CLK(void)
{
	int i;

	//システムクロック設定
	//PLL50MHz システムクロック25MHz
	SYSTEM.PRCR.WORD = 0xA50F;					//プロテクト解除
	SYSTEM.VRCR = 0x00;							//システムクロック変更のためのVRCR設定
	SYSTEM.SOSCCR.BIT.SOSTP = 1;				//サブクロック発振器停止
	while(SYSTEM.SOSCCR.BIT.SOSTP != 1);		//レジスタ書き込み終了待ち
	RTC.RCR3.BYTE = 0x0C;						//RTCへのサブクロック使用停止
	while(RTC.RCR3.BIT.RTCEN != 0);				//PCR3 レジスタ書き込み終了待ち
	SYSTEM.MOFCR.BYTE = 0x03;					//メインクロック外部10MHz
	SYSTEM.MOSCWTCR.BYTE = 0x0D;				//メインクロックウェイト131072サイクル
	SYSTEM.MOSCCR.BIT.MOSTP = 0;				//メインクロック発振器終了
	while(SYSTEM.MOSCCR.BIT.MOSTP != 0);		//レジスタ書き込み時間待ち
	for(i = 0;i < 100;i++);						//PLL安定動作まで時間待ち
	SYSTEM.PLLCR.WORD = 0x0901;					//PLL1/2分周 5逓倍設定
	SYSTEM.PLLWTCR.BYTE = 0x09;					//PLLクロックウェイト 131072
	SYSTEM.PLLCR2.BYTE = 0x00;					//PLL動作設定
	for(i = 0;i < 100;i++);						//PLL安定動作待ち
	SYSTEM.OPCCR.BYTE = (0x00);					//高速動作モード
	while( 0 != SYSTEM.OPCCR.BIT.OPCMTSF);		//レジスタ書き込み終了待ち
	SYSTEM.SCKCR.LONG = 0x21821211;				//FCLK1/4分周、ICLK1/2分周、BLCK1/4分周
														//PLCKB1/4分周、PCLKD1/2分周
    while(SYSTEM.SCKCR.LONG != 0x21821211);		//レジスタ書き込み終了待ち
	SYSTEM.SCKCR3.WORD = 0x0400;				//クロックソースPLL設定
	while(SYSTEM.SCKCR3.WORD != 0x0400);		//レジスタ書き込み終了待ち
	SYSTEM.PRCR.WORD = 0xA500;					//プロテクト設定

}

void main(void)
{
	long i;
	unsigned char cur_sw5;
	unsigned char pre_sw5;
	unsigned char cur_sw6;
	unsigned char pre_sw6;

	init_CLK();
	init_LCD();

	PORTH.PDR.BIT.B0 = 0;
	PORTH.PDR.BIT.B1 = 0;

	cnt = 0;
	pre_sw5 = PORTH.PIDR.BIT.B0;
	pre_sw6 = PORTH.PIDR.BIT.B1;
	lcd_puts("switch count");
	flush_lcd();

	while(1)
	{
		i++;

		if(i == 8000)
		{
			cur_sw5 = PORTH.PIDR.BIT.B0;
			cur_sw6 = PORTH.PIDR.BIT.B1;

			if((cur_sw5 == 0) && (pre_sw5 == 1))
			{
				cnt++;
				lcd_xy(1,2);
				lcd_puts(" ");
				lcd_xy(1,2);
				if(cnt < 0)
				{
					lcd_put('-');
					lcd_dataout(-cnt);
				}
				else
				{
					lcd_dataout(cnt);
					lcd_put(' ');
				}
				flush_lcd();
			}

			/*
			if((cur_sw5 == 0) && (pre_sw5 == 1))
			{
				cnt++;
				lcd_xy(1,2);
				lcd_puts(" ");
				lcd_xy(1,2);
				lcd_dataout(cnt);
				flush_lcd();
			}
			*/

			if((cur_sw6 == 0) && (pre_sw6 == 1))
			{
				cnt--;
				lcd_xy(1,2);
				lcd_puts(" ");
				lcd_xy(1,2);
				if(cnt < 0)
				{
					lcd_put('-');
					lcd_dataout((-1)*cnt);
				}
				else lcd_dataout(cnt);
				flush_lcd();
			}
			/*
			if((cur_sw6 == 0) && (pre_sw6 == 1))
			{
				lcd_xy(1,2);
				lcd_puts(" ");
				lcd_xy(1,2);
				cnt--;
				if(cnt <= 0)
				{
				    lcd_put('-');
					lcd_dataout((-1)*cnt);
				}
				else
				{
					lcd_dataout(cnt);
				}
				flush_lcd();
			}
			*/
			pre_sw5 = cur_sw5;
			pre_sw6 = cur_sw6;
			i = 0;
		}

	}

}

#ifdef __cplusplus
void abort(void)
{

}
#endif
