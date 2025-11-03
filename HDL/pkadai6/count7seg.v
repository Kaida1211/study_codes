`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2025/11/03 13:53:28
// Design Name: 
// Module Name: count7seg
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


module count7seg(
    input clock,
    input countup,
    input reset,
    output [7:0] seg,
    output [3:0] anode
    );
    
    wire ccount;
    
    wire [3:0] segcount;
    
    chattering chattering(
        .clock(clock),
        .reset(reset),
        .in(countup),
        .out(ccount)
    );
    
    counter counter(
        .clock(ccount),
        .reset(reset),
        .enable(ccount),
        .count(segcount)
    );
    
    sevenseg sevenseg(
        .in(segcount),
        .seg(seg),
        .anode(anode)
    );
    
endmodule
