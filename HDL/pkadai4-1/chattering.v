`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2025/10/19 16:24:22
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


module chattering(
    input clock,
    input reset,
    input in,
    output reg out
    );
    
    reg [17:0] count;
    reg in_reg;
    
    always @(posedge clock or posedge reset)
        if(reset) begin
            count <= 0;
            in_reg <= 0;
            out <= 0;
         end else begin
            // 入力が前回と異なる場合、カウンタをリセット
            if (in != in_reg) begin
                in_reg <= in;
                count <= 0;
            end else if (count < 18'h3FFFF) begin
                count <= count + 1;
            end else begin
                // 一定時間（カウントが上限まで到達）経過で出力確定
                out <= in_reg;
            end
        end
    
endmodule
