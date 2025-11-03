`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2025/11/03 13:02:21
// Design Name: 
// Module Name: chattering
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


module chattering #(parameter bitW = 17)(
    input clock,
    input in,
    input reset,
    output reg out
    );
    
    reg [bitW:0] count;
    reg in_reg;
    
    always @(posedge clock or posedge reset) begin
        if(reset) begin
            out <= 0;
            count <= 0;
            in_reg <= 0;
        end else begin
            if(in != in_reg) begin
                in_reg <= in;
            end else if(count < bitW + 1) begin
                count <= count + 1;
            end else begin
                out <= in_reg;
            end
        end
    end
endmodule
