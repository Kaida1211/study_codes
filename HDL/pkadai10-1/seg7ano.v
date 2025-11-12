`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2025/11/09 20:29:38
// Design Name: 
// Module Name: seg7ano
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

module decoder(
        input [1:0] s,
        output [3:0] d
    );
        
        assign d = deco(s);
        
        function [3:0] deco;
            input [1:0] s;
            begin
                deco = (s==0) ? 4'b1110:
                       (s==1) ? 4'b1101:
                       (s==2) ? 4'b1011:
                       (s==3) ? 4'b0111:4'b1111;
            end
        endfunction

endmodule

module seg7ano(
        input [1:0] a,
        output [3:0] d
    );
    
    decoder decoder(
        .a(a),
        .d(d)
    );
    
endmodule
