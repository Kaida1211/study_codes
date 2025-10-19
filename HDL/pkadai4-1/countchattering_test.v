`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2025/10/19 20:20:39
// Design Name: 
// Module Name: countchattering_test
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


module countchattering_test;

    reg clock,countup,reset;
    wire [3:0] count;
    
    countchattering test(
        .clock(clock),
        .countup(countup),
        .reset(reset),
        .count(count)
    );
    
    initial begin
        clock = 0;
        forever #10 clock = ~clock;
    end
    
    
    initial begin
        $monitor($time,"clock = %b | reset = %b | countup = %b | count = %b"
                 ,clock ,reset, countup, count);
        
        //初期設定
        reset = 1;
        countup = 0;
        
        reset = 0;
        #20
        reset = 1;
        #50
        
        //countup パルス発生
        countup = 1;  #50
        countup = 0; #200
        
        //もう一度押す
        countup = 1; #50
        countup = 0; #200
        
        $finish;
    end
    
endmodule
