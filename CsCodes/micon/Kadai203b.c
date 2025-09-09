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

#include "lcd_lib4.h"
#include "iodefine.h"
#include"vect.h"
#include<machine.h>

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

volatile unsigned long time_1m_count;


void init_clk(void);
void init_CMT1(void);


void init_CMT1(void){
	SYSTEM.PRCR.WORD = 0x0A502;					//プロテクトの解除
	MSTP(CMT1)=0;								//モジュールストップ状態の解除
	SYSTEM.PRCR.WORD =  0x0A500;				//プロテクト設定
	CMT1.CMCR.WORD = 0x00;						//クロック設定 PCLK/8
	CMT1.CMCOR = 25000/8 - 1;                   //インターバルを10msecに設定
	CMT1.CMCR.WORD |= 0x00C0;					//コンペアマッチ割り込み許可
	IEN(CMT1,CMI1)=1;							//割込み要求の許可
	IPR(CMT1,CMI1)=1;							//割込み要求のプライオリティの設定

	CMT.CMSTR0.BIT.STR1 = 1;					//カウントスタート

}

void init_clk(void)
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

		init_clk();
		init_LCD();
		init_CMT1();
		setpsw_i();		//割込み許可


		//解答例
		lcd_puts("Timer");
		lcd_xy(1,2);
		lcd_puts("00:00:00");
		flush_lcd();


		while(1){};						//無限ループ
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
