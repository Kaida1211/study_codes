#include"iodefine.h"
#include<stdint.h>

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

void init_MTU_PWM(void)
{
    SYSTEM.PRCR.WORD = 0xA502;      // プロテクト解除
    MSTP(MTU0) = 0;                 // MTU0スタンバイ解除
    MSTP(MTU1) = 0;					// MTU1スタンバイ解除
    MSTP(MTU2) = 0;					// MTU2スタンバイ解除
    SYSTEM.PRCR.WORD = 0xA500;      // プロテクト再設定

    MTU.TSTR.BIT.CST0 = 0;          // MTU0カウント停止

    // ==== クロック・モード設定 ====
    // ---- MTU0 ----
    MTU0.TCR.BIT.TPSC = 0;          // クロック選択 PCLK/1
    MTU0.TCR.BIT.CKEG = 0;          // 立ち上がりエッジでカウント
    MTU0.TCR.BIT.CCLR = 1;          // TGRAコンペアマッチでカウンタクリア
    MTU0.TMDR.BIT.MD = 2;           // PWMモード1 *
    // ---- MTU1 ----
    MTU1.TCR.BIT.TPSC = 0;          // クロック選択 PCLK/1
    MTU1.TCR.BIT.CKEG = 0;          // 立ち上がりエッジでカウント
    MTU1.TCR.BIT.CCLR = 1;          // TGRAコンペアマッチでカウンタクリア
    MTU1.TMDR.BIT.MD = 2;           // PWMモード1 *
    // ---- MTU2 ----
    MTU2.TCR.BIT.TPSC = 0;          // クロック選択 PCLK/1
    MTU2.TCR.BIT.CKEG = 0;          // 立ち上がりエッジでカウント
    MTU2.TCR.BIT.CCLR = 1;          // TGRAコンペアマッチでカウンタクリア
    MTU2.TMDR.BIT.MD = 2;           // PWMモード1 *

    // ==== 周期・デューティ設定 ====
    // ---- MTU0 ----
    MTU0.TGRA = 2499;               // 周期(20kHz)
    MTU0.TGRB = 1250;               // Duty 50%
    MTU0.TIORH.BIT.IOA = 2;     	// MTIOC0A: toggle High→Low U HO
    MTU0.TIORH.BIT.IOB = 2;     	// MTIOC0B: toggle High→Low U LO
    // ---- MTU1 ----
    MTU1.TGRA = 2499;               // 周期(20kHz)
    MTU1.TGRB = 1250;               // Duty 50%
    MTU1.TIOR.BIT.IOA = 2;     		// MTIOC1A: toggle High→Low V HO
    MTU1.TIOR.BIT.IOB = 2;     		// MTIOC1B: toggle High→Low U LO
    // ---- MTU2 ----
    MTU2.TGRA = 2499;               // 周期(20kHz)
    MTU2.TGRB = 1250;               // Duty 50%
    MTU2.TIOR.BIT.IOA = 2;     		// MTIOC2A: toggle High→Low W HO
    MTU2.TIOR.BIT.IOB = 2;     		// MTIOC2B: toggle High→Low U LO


    // ==== 同期スタート設定　====
    MTU.TSYR.BIT.SYNC0 = 0;
    MTU.TSYR.BIT.SYNC1 = 1;
    MTU.TSYR.BIT.SYNC2 = 1;

    // ==== 位相調整 ====
    MTU1.TCNT = 833;            // 約120°遅れ		2499/3
    MTU2.TCNT = 1666;           // 約240°遅れ		2499*2/3

    // ==== ピン設定 ====
    PORTm.PMR.BIT.Bn = 1;           // Pmnを周辺機能ピンに
    MPC.PmnPFS.BYTE = 0;         	// MTIOC0Aに割り当て

    PORTm.PMR.BIT.Bn = 1;           // Pmnを周辺機能ピンに
    MPC.PmnPFS.BYTE = 0;         	// MTIOC0Bに割り当て

    PORTm.PMR.BIT.Bn = 1;           // Pmnを周辺機能ピンに
    MPC.PmnPFS.BYTE = 0;         	// MTIOC1Aに割り当て

    PORTm.PMR.BIT.Bn = 1;           // Pmnを周辺機能ピンに
    MPC.PmnPFS.BYTE = 0;         	// MTIOC1Bに割り当て

    PORTm.PMR.BIT.Bn = 1;           // Pmnを周辺機能ピンに
    MPC.PmnPFS.BYTE = 0;         	// MTIOC2Aに割り当て

    PORTm.PMR.BIT.Bn = 1;           // Pmnを周辺機能ピンに
    MPC.PmnPFS.BYTE = 0;         	// MTIOC2Bに割り当て

    // ==== カウンタ初期化 ====
    MTU0.TCNT = 0;
    MTU1.TCNT = 0;
    MTU2.TCNT = 0;

    // ==== 同期スタート ====
    MTU.TSTR.BIT.CST0 = 1;          // カウントスタート（PWM開始）
}

void init_IRQ_HS(void)
{
    /* ==== ポート設定（P10, P11, P12 → IRQ0, IRQ1, IRQ2） ==== */
    SYSTEM.PRCR.WORD = 0xA502;		//プロテクト解除

    // IRQ0 → Pmn
    PORTm.PMR.BIT.Bn = 0;          // 周辺機能ではなく入力ピン
    PORTm.PDR.BIT.Bn = 0;          // 入力に設定
    MPC.PmnPFS.BIT.ISEL = 1;       // 割り込み入力として使用

    // IRQ1 → Pmn
    PORTm.PMR.BIT.Bn = 0;          // 周辺機能ではなく入力ピン
    PORTm.PDR.BIT.Bn = 0;          // 入力に設定
    MPC.PmnPFS.BIT.ISEL = 1;       // 割り込み入力として使用

    // IRQ2 → Pmn
    PORTm.PMR.BIT.Bn = 0;          // 周辺機能ではなく入力ピン
    PORTm.PDR.BIT.Bn = 0;          // 入力に設定
    MPC.PmnPFS.BIT.ISEL = 1;       // 割り込み入力として使用

    SYSTEM.PRCR.WORD = 0xA500;		//プロテクト再定義

    /* ==== 割り込みエッジ設定 ==== */
    ICU.IRQCR[0].BIT.IRQMD = 2;    // 立ち上がりエッジ検出（ホールセンサ出力が0→1）
    ICU.IRQCR[1].BIT.IRQMD = 2;
    ICU.IRQCR[2].BIT.IRQMD = 2;

    /* ==== 割り込み優先度設定 ==== */
    IPR(ICU, IRQ0) = 3;            // 優先度3（0〜15で設定）
    IPR(ICU, IRQ1) = 3;
    IPR(ICU, IRQ2) = 3;

    /* ==== 割り込み許可 ==== */
    IR(ICU, IRQ0) = 0;             // フラグクリア・・・初期化時は0
    IR(ICU, IRQ1) = 0;
    IR(ICU, IRQ2) = 0;
    IEN(ICU, IRQ0) = 1;            // 割り込み許可
    IEN(ICU, IRQ1) = 1;
    IEN(ICU, IRQ2) = 1;
}

volatile uint8_t hall_state = 0;
volatile uint8_t step_index = 0;

void update_motor_step(void)
{
    // ホールセンサ状態を3ビット値に変換
    uint8_t h1 = PORTm.PIDR.BIT.Bn;  // IRQ0
    uint8_t h2 = PORTm.PIDR.BIT.Bn;  // IRQ1
    uint8_t h3 = PORTm.PIDR.BIT.Bn;  // IRQ2
    hall_state = (h3 << n) | (h2 << n) | h1;

    // ホールパターンに対応するステップ切替
    switch(hall_state)
    {
        case 0x: step_index = 0; break;     // 0b001
        case 0x: step_index = 1; break;	   	// 0b101
        case 0x: step_index = 2; break;	   	// 0b100
        case 0x: step_index = 3; break;		// 0b110
        case 0x: step_index = 4; break;		// 0b010
        case 0x: step_index = 5; break;		// 0b011
        default:    step_index = 0xFF; break;   // 異常
    }

    // ここで PWM 出力のON/OFFまたはDuty変更を行う関数を呼ぶ
    set_pwm_pattern(step_index);
}

void set_pwm_pattern(uint8_t step)
{
    // === 初期化 ===
    MTU0.TIORH.BIT.IOA = 0; // U+
    MTU0.TIORH.BIT.IOB = 0; // U-
    MTU1.TIOR.BIT.IOA  = 0; // V+
    MTU1.TIOR.BIT.IOB  = 0; // V-
    MTU2.TIOR.BIT.IOA  = 0; // W+
    MTU2.TIOR.BIT.IOB  = 0; // W-

    // ===
    switch(step)
    {
        case 0: // H3H2H1 = 001
            // U+: PWM, V-: ON
            MTU0.TIORH.BIT.IOA = 2;  // U+
            MTU1.TIOR.BIT.IOB  = 1;  // V-
            break;

        case 1: // 101
            // U+: PWM, W-: ON
            MTU0.TIORH.BIT.IOA = 2;  // U+
            MTU2.TIOR.BIT.IOB  = 1;  // W-
            break;

        case 2: // 100
            // V+: PWM, W-: ON
            MTU1.TIOR.BIT.IOA  = 2;  // V+
            MTU2.TIOR.BIT.IOB  = 1;  // W-
            break;

        case 3: // 110
            // V+: PWM, U-: ON
            MTU1.TIOR.BIT.IOA  = 2;  // V+
            MTU0.TIORH.BIT.IOB = 1;  // U-
            break;

        case 4: // 010
            // W+: PWM, U-: ON
            MTU2.TIOR.BIT.IOA  = 2;  // W+
            MTU0.TIORH.BIT.IOB = 1;  // U-
            break;

        case 5: // 011
            // W+: PWM, V-: ON
            MTU2.TIOR.BIT.IOA  = 2;  // W+
            MTU1.TIOR.BIT.IOB  = 1;  // V-
            break;

        default:
            // 非常停止
            MTU0.TIORH.BIT.IOA = 0;
            MTU0.TIORH.BIT.IOB = 0;
            MTU1.TIOR.BIT.IOA  = 0;
            MTU1.TIOR.BIT.IOB  = 0;
            MTU2.TIOR.BIT.IOA  = 0;
            MTU2.TIOR.BIT.IOB  = 0;
            break;
    }
}

void main(void)
{
	init_CLK();
	init_MTU_PWM();
	init_IRQ_HS();

	while(1);

}
