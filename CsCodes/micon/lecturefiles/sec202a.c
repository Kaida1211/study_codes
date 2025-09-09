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
#include "lcd_lib4.h"
#include<machine.h>

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

void main(void)
{
	int i = 256;

	init_LCD();

	while(1){
		lcd_puts("-- LCD test --"); //1列1行目からの表示

		lcd_xy(1,2);	//1列, 2行に移動
		lcd_put('a');	//'a'をセット
		lcd_put('q');	//'q'をセット
		lcd_put(0x04);	//'q'をセット
		lcd_puts("$$$$");	//文字列をセット
		flush_lcd();	//セットした文字列を表示
		wait50us(20000);	//1sの表示間隔
		lcd_xy(5,2);	//カーソルを3文字分戻す
		lcd_puts(" ");	//空白をセット
		lcd_xy(1,2);	//1列,2行に移動
		wait50us(20000);	//1sの表示間隔
		lcd_puts("i = ");	//文字列をセット
		lcd_dataout(i--);	//iの値をセット
		flush_lcd();	//セットした文字列を表示
		wait50us(20000);	//1sの表示間隔
		lcd_puts("???");	//?のセット
		lcd_clear();	//lcdの表示クリア
		wait50us(20000);	//1sの表示間隔
	};
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
