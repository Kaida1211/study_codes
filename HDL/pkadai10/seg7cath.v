`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2025/11/09 16:51:42
// Design Name: 
// Module Name: seg7cath
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

module selector(
    input [3:0] digit0, digit1, digit2, digit3,
    input [1:0] sel,
    output [3:0] digitout
);  
    
    assign digitout = select(sel);
    
    function [3:0] select;
        input [1:0] s;
        begin
            select = (s==0) ? digit0:
                     (s==1) ? digit1:
                     (s==2) ? digit2:
                     (s==3) ? digit3: 4'b1111;
        end
    endfunction 
endmodule


module sevenseg(
    input [3:0] bcd,
    output [7:0] seg
);

    assign  seg = seg7(bcd);
     
    function [7:0] seg7;
        input [3:0] bcd;
        begin
            case(bcd)
                4'h0: seg7 = 8'b11111100;
                4'h1: seg7 = 8'b01100000;
                4'h2: seg7 = 8'b11011010;
                4'h3: seg7 = 8'b11110010;
                4'h4: seg7 = 8'b01100110;
                4'h5: seg7 = 8'b10110110;
                4'h6: seg7 = 8'b10111110;
                4'h7: seg7 = 8'b11100000;
                4'h8: seg7 = 8'b11111110;
                4'h9: seg7 = 8'b11110110;
                4'hA: seg7 = 8'b11101110;
                4'hB: seg7 = 8'b00111110;
                4'hC: seg7 = 8'b10011100;
                4'hD: seg7 = 8'b01111010;
                4'hE: seg7 = 8'b10011110;
                4'hF: seg7 = 8'b10001110;
            endcase
        end
    endfunction
endmodule


module seg7cath(
        input [3:0] digit0, digit1, digit2, digit3,
        input [1:0] sel,
        output [6:0] y
    );
    
    wire [3:0] digitout;
    
    selector selector(
        .digit0(digit0),
        .digit1(digit1),
        .digit2(digit2),
        .digit3(digit3),
        .sel(sel),
        .digitout(digitout)
    );
    
    sevenseg sevenseg(
        .bcd(digitout),
        .seg(y)
    );
    
endmodule
