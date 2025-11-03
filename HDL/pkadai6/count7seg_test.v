`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2025/11/03 14:38:21
// Design Name: 
// Module Name: count7seg_test
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module count7seg_test;

    reg clock;
    reg countup;
    reg reset;
    wire [7:0] seg;
    wire [3:0] anode;
    
    defparam test.chattering.bitW = 1;

    // 被テストモジュールのインスタンス化
    // チャタリング除去をバイパスして趣味レーション用に動作
    count7seg test (
        .clock(clock),
        .countup(countup),
        .reset(reset),
        .seg(seg),
        .anode(anode)
    );

    // クロック生成（100MHz → 10ns周期）
    initial begin
        clock = 0;
        forever #5 clock = ~clock;
    end

    // テストシーケンス
    initial begin
        $display("=== count7seg simulation start ===");
        reset = 1;
        countup = 0;
        #50;          // リセット期間
        reset = 0;

        // 0〜15まで順にカウントアップ
        repeat (16) begin
            countup = 1;
            #10;        // 1クロックパルス
            countup = 0;
            #10;        // 次カウントの間隔
        end

        #100;
        $display("=== simulation finished ===");
        $finish;
    end

    // 7セグ表示をコンソールに出力（1クロックごとに更新）
    always @(posedge clock) begin
        $display("time=%0t ns | seg=%b | anode=%b", $time, seg, anode);

        // 7セグの形を文字で表示
        if (seg[6]) $display(" --"); else $display("   ");   // a
        $display("%s  %s", seg[5] ? "|" : " ", seg[1] ? "|" : " "); // f,b
        if (seg[0]) $display(" --"); else $display("   ");   // g
        $display("%s  %s", seg[4] ? "|" : " ", seg[2] ? "|" : " "); // e,c
        if (seg[3]) $display(" --\n"); else $display("   \n"); // d
    end
endmodule
