`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2025/11/09 20:42:55
// Design Name: 
// Module Name: dynamic4sel
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


module dynamic4sel(
        input clock,reset,
        input [3:0] digit0, digit1, digit2, digit3,
        output [6:0] y,
        output [3:0] k
    );
    
    wire [1:0] sel, d;
    
    counter counter(
        .clock(clock),
        .reset(reset),
        .d(d)
    );
    
    seg7cath seg7cath(
        .digit0(digit0),
        .digit1(digit1),
        .digit2(digit2),
        .digit3(digit3),
        .sel(d),
        .y(y)
    );
    
    seg7ano seg7ano(
        .s(d),
        .d(k)
    );
    
endmodule
