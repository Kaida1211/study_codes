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


void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

void main(void)
{
	PORTC.PDR.BYTE = 0xFF;		//ポートCを出力を設定
	PORTH.PDR.BIT.B0 = 0;		//ポートHを入力に設定

	while(1)
	{
		PORTC.PODR.BYTE = 0x00;		//LEDをすべて消す

		if(PORTH.PIDR.BIT.B0 == 0)  PORTC.PODR.BYTE = 0xff;
	}
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
