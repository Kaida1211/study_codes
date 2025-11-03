`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2025/11/03 13:42:57
// Design Name: 
// Module Name: sevenseg
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


module sevenseg(
    input [3:0] in,
    output[7:0] seg,
    output [3:0] anode
    );
    
    assign anode = 4'b1110;
    
    assign seg[6:0] = ~bcd7segdec(in);
    assign seg[7] = 1;
    
    function [6:0] bcd7segdec;
        input [3:0] in;
        begin
            case(in)
                4'h0: bcd7segdec = 7'b1111110; // 0
                4'h1: bcd7segdec = 7'b0110000; // 1
                4'h2: bcd7segdec = 7'b1101101; // 2
                4'h3: bcd7segdec = 7'b1111001; // 3
                4'h4: bcd7segdec = 7'b0110011; // 4
                4'h5: bcd7segdec = 7'b1011011; // 5
                4'h6: bcd7segdec = 7'b1011111; // 6
                4'h7: bcd7segdec = 7'b1110000; // 7
                4'h8: bcd7segdec = 7'b1111111; // 8
                4'h9: bcd7segdec = 7'b1111011; // 9
                4'hA: bcd7segdec = 7'b1110111; // A
                4'hB: bcd7segdec = 7'b0011111; // b
                4'hC: bcd7segdec = 7'b1001110; // C
                4'hD: bcd7segdec = 7'b0111101; // d
                4'hE: bcd7segdec = 7'b1001111; // E
                4'hF: bcd7segdec = 7'b1000111; // F
                default: bcd7segdec = 7'b0000000;
            endcase
        end
   endfunction
    
endmodule
