`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2025/11/03 13:35:25
// Design Name: 
// Module Name: counter
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


module counter(
    input clock,
    input reset,
    input enable,
    output reg [3:0] count
    );
    
    always @(posedge clock or posedge reset) begin
        if(reset) begin
            count <= 0;
        end else if(enable) begin
            count <= count + 1;
        end
    end
endmodule
