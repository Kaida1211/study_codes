`timescale 1ns / 1ps

module countchattering(
    input clock,
    input countup,
    input reset,
    output reg [3:0] count
    );
    
    //チャタリング除去後の信号を受ける線
    wire clean_countup;
    
    chattering u_chattering (
        .clock(clock),
        .reset(reset),
        .in(countup),
        .out(clean_countup)
    );
    
    counter u_counter(
        .clock(out),
        .reset(reset),
        .in(clean_countup),
        .out(count)
    );
    
 endmodule