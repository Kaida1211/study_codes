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

volatile unsigned long time_1m_count;
volatile unsigned long btime;
volatile unsigned char sw;

void init_CLK(){

	unsigned int i;

	SYSTEM.PRCR.WORD=0xA50F;
	SYSTEM.VRCR=0x00;
	SYSTEM.SOSCCR.BIT.SOSTP = 1;
	while(SYSTEM.SOSCCR.BIT.SOSTP != 1);

	RTC.RCR3.BYTE = 0x0C;
	while(RTC.RCR3.BIT.RTCEN != 0);

	SYSTEM.MOFCR.BYTE=(0x0D);
	SYSTEM.MOSCWTCR.BYTE=(0x0D);
	SYSTEM.MOSCCR.BIT.MOSTP=0x00;
	while(0x00 != SYSTEM.MOSCCR.BIT.MOSTP != 0);

	for(i=0;i<100;i++)
		nop();

	SYSTEM.PLLCR.WORD=(0x0901);
	SYSTEM.PLLWTCR.BYTE=(0x09);
	SYSTEM.PLLCR2.BYTE=0x00;

	for(i=0;i<100;i++)
		nop();

	SYSTEM.OPCCR.BYTE=(0x00);
	while(0 != SYSTEM.OPCCR.BIT.OPCMTSF);

	SYSTEM.SCKCR.LONG = 0x21821211;
	while(0x21821211 != SYSTEM.SCKCR.LONG);

	SYSTEM.SCKCR3.WORD = (0x0400);
	while((0x0400) != SYSTEM.SCKCR3.WORD);

	SYSTEM.PRCR.WORD = 0xA500;
}


void init_CMT1(void){

	//タイマ回路に電源を接続
	SYSTEM.PRCR.WORD=0x0A502; //ロックの解除
	MSTP(CMT1)=0;			  //MTU1 モジュールスタンバイの解除
	SYSTEM.PRCR.WORD=0x0A500; //ロック状態に戻す

	SYSTEM.PRCR.WORD = 0x0000;
	CMT1.CMCOR = 25000/8 - 1;
	CMT1.CMCR.WORD |= 0x00C0;
	IEN(CMT1, CMI1)=1;
	IPR(CMT1, CMI1)=1;

	CMT.CMSTR0.BIT.STR1 = 1;

}

void init_IRQ0(void)
{
	IEN(ICU, IRQ0) = 0;                //IRQ0を禁止
	ICU.IRQFLTE0.BIT.FLTEN0 = 0;    //デジタルフィルタ停止
	ICU.IRQFLTC0.BIT.FCLKSEL0 = 3; //デジタルフィルタをPCLK/64にする
	PORTH.PDR.BIT.B1 = 0;          //PH1を入力に設定
	PORTH.PMR.BIT.B1 = 1;          //PH1のモードを変更
	MPC.PWPR.BIT.B0WI = 0;         //設定変更を可にする
	MPC.PWPR.BIT.PFSWE = 1;        //設定変更レジスタの書き込み許可
	MPC.PH1PFS.BIT.ISEL = 1;		//PH1をIRQ０として使用する
	ICU.IRQCR[0].BIT.IRQMD = 1;  //割り込みは立下りエッジで発生
	ICU.IRQFLTE0.BIT.FLTEN0 = 1; //デジタルフィルタ有効
	IR(ICU, IRQ0) = 0;            //割り込みのフラグを下げる
	IEN(ICU, IRQ0) = 1;          //IRQ0の割り込み許可
	IPR(ICU, IRQ0) = 1;			//割り込みの優先順位を最大にする
}

void init_IRQ1(void)
{
	IEN(ICU, IRQ1) = 0;                //IRQ0を禁止
	ICU.IRQFLTE0.BIT.FLTEN1 = 0;    //デジタルフィルタ停止
	ICU.IRQFLTC0.BIT.FCLKSEL1 = 3; //デジタルフィルタをPCLK/64にする
	PORTH.PDR.BIT.B2 = 0;          //PH1を入力に設定
	PORTH.PMR.BIT.B2 = 1;          //PH1のモードを変更
	MPC.PWPR.BIT.B0WI = 0;         //設定変更を可にする
	MPC.PWPR.BIT.PFSWE = 1;        //設定変更レジスタの書き込み許可
	MPC.PH2PFS.BIT.ISEL = 1;		//PH1をIRQ０として使用する
	ICU.IRQCR[1].BIT.IRQMD = 1;  //割り込みは立下りエッジで発生
	ICU.IRQFLTE0.BIT.FLTEN1 = 1; //デジタルフィルタ有効
	IR(ICU, IRQ1) = 0;            //割り込みのフラグを下げる
	IEN(ICU, IRQ1) = 1;          //IRQ0の割り込み許可
	IPR(ICU, IRQ1) = 1;			//割り込みの優先順位を最大にする
}

void init_BUZZER(){

	//タイマ回路に電源を接続
	SYSTEM.PRCR.WORD=0x0A502; //ロックの解除
	MSTP(MTU0)=0;			  //MTU0 モジュールスタンバイの解除
	SYSTEM.PRCR.WORD=0x0A500; //ロック状態に戻す

	//タイマ機能を利用する端子の設定
	PORT3.PDR.BIT.B4=1;          //P34を出力に設定
	PORT3.PMR.BIT.B4=1;          //P34を汎用入出力ポートに設定
	MPC.PWPR.BIT.B0WI=0;         //PFSWEを書き込み可に設定
	MPC.PWPR.BIT.PFSWE=1;		 //PFSレジスタへの書き込みを許可
	MPC.P34PFS.BIT.PSEL=1;		 //P34をMTIOC0Aとして使用する
	MPC.PWPR.BIT.PFSWE=0;        //PFSレジスタへの書き込みを

	//MTUタイマの設定
	//cycle=PCLK*1000UL/1000UL/2UL; //タイマのカウント数
	MTU.TSTR.BIT.CST0=0x00;       //MTUカウントを停止
	MTU0.TCR.BIT.TPSC=0x01;		  //CLKはPCLK/4でカウント
	MTU0.TCR.BIT.CCLR=0x01;		  //TCNT0はTRGBのコンペアマッチでクリア
	MTU0.TMDR.BIT.MD=0x02;		  //PWMモード

	//出力波形の設定
	MTU0.TIORH.BIT.IOA=0x06;	  //MTIOC０Aの初期出力はLow出力、 コンペアマッチでトグル出力
	MTU0.TIORH.BIT.IOB=0x05;      //MTIOC0Bは出力禁止
	MTU0.TCNT=0;				  //カウントクリア
}


void beep(unsigned int tone, unsigned int interval)
{
	if(tone)
	{
		MTU.TSTR.BIT.CST0 = 0;
		MTU0.TGRA = tone;
		MTU0.TGRB = tone/2;
		MTU.TSTR.BIT.CST0 = 1;
	}else{
		MTU.TSTR.BIT.CST0 = 0;
	}

	btime = interval;

}

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

void main(void)
{
	long i = 0;
	unsigned char pre_sw8, cur_sw8;

	PORTH.PDR.BIT.B3 = 0;
	init_CLK();
	init_LCD();

	init_BUZZER();

	init_IRQ0();
	init_IRQ1();
	init_CMT1();
	setpsw_i();

	lcd_puts("STOP WATCH");
	flush_lcd();

	while(1)
	{
		i++;
		if(i == 10)
		{
			cur_sw8 = PORTH.PIDR.BIT.B3;

			//if(PORTH.PIDR.BIT.B3 == 0) sw = 1;
			if((cur_sw8 == 1) && (pre_sw8 == 0))
			{
				sw = 1;
				beep(DO1, 500);
			}
			i = 0;

			pre_sw8 = cur_sw8;
		}
	}

}

#ifdef __cplusplus
void abort(void)
{

}
#endif
