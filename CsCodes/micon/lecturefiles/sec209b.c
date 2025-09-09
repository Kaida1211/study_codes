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

//---------------------------------------------
// 温度測定関連
//---------------------------------------------
//A/D変換初期化
void init_AD0(void)
{
	SYSTEM.PRCR.WORD = 0xA502;				//プロテクト解除
	MSTP(S12AD) = 0;						//A/Dコンバータ モジュールスタンバイ
	SYSTEM.PRCR.WORD = 0xA500;				//プロテクト設定
	PORT4.PMR.BIT.B0 = 1;					//P40 を周辺機器として使用
	S12AD.ADCSR.BIT.ADIE = 1;				//割り込みを使う
	S12AD.ADANSA.BIT.ANSA0 = 1;				//アナログ入力チャネル選択 AN000
	S12AD.ADCSR.BIT.ADCS = 0;				//シングルスキャンモード
	MPC.PWPR.BIT.B0WI = 0;					//PFSWE 書き込み可
	MPC.PWPR.BIT.PFSWE = 1;					//PFSレジスタへの書き込み
	MPC.P40PFS.BIT.ASEL = 1;				//P40をAN000として使用
	MPC.PWPR.BIT.PFSWE = 0;					//PFSレジスタへの書き込み禁止
	IEN(S12AD,S12ADI0) = 1;					//割り込み要求を許可
	IPR(S12AD,S12ADI0) = 1;					//割り込み要求のプライオリティの設定
}


//温度測定
void MEAS_TEMP(void)
{
	S12AD.ADCSR.BIT.ADST = 1;				//A\D変換スタート

}
/*
	long temp;


	while(S12AD.ADCSR.BIT.ADST == 1);		//変換終了を待つ

	temp = S12AD.ADDR0*4700/4096/2-600;

	lcd_xy(1,2);
	lcd_puts("     ");
	lcd_xy(1,2);
	lcd_dataout(temp/10);
	lcd_put('.');
	lcd_dataout(temp%10);
	lcd_put(0xdf);							//LCD表示 右上半濁音
	lcd_put(0x43);							//LCD表示 C
	flush_lcd();
}
*/



void main(void)
{
	init_CLK();
	init_CMT0();
	init_AD0();
	init_LCD();
	setpsw_i();

	lcd_puts("temperature");
	while(1);
}
