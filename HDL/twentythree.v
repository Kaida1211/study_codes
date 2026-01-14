`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2026/01/14 10:31:36
// Design Name: 
// Module Name: twentythree
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
module counter1sec(
        input clock, reset,
        output reg  count1s
    );
        parameter countN = 100000000/2;
        parameter bitW = $clog2(countN + 1);
        reg [bitW-1:0] count; 
    
        always @(posedge clock or posedge reset) begin
        if(reset) begin
            count1s <= 0;
            count <= 0;
        end else begin
            if(count == countN) begin
                count1s <= ~count1s;
                count <= 0;
            end else begin
                count <= count + 1;
            end
        end
    end
endmodule

module counterkhz(
        input clock, reset,
        output reg  countkhz
    );
        parameter countN = 1000;
        parameter bitW = $clog2(countN + 1);
        reg [bitW-1:0] count; 
    
        always @(posedge clock or posedge reset) begin
        if(reset) begin
            countkhz <= 0;
            count <= 0;
        end else begin
            if(count == countN) begin
                countkhz <= ~countkhz;
                count <= 0;
            end else begin
                count <= count + 1;
            end
        end
    end
endmodule 

module counter10(
        input clock, reset,
        output reg [3:0] count,
        output en
    );
        always@(posedge clock or posedge reset) begin
            if(reset) begin
                count <= 0;
            end else begin
               if(count == 9) begin
                    count <= 0;
               end else begin
                    count <= count + 1;
               end
            end
        end
        assign en = (count == 9) ? 0:1; 
endmodule

module counter6(
        input clock, reset, en,
        output reg [3:0] count
    );
        always@(posedge clock or posedge reset) begin
            if(reset) begin
                count <= 0;
            end else if(!en) begin
               if(count == 5)begin
                    count <= 0;
               end else begin
                    count <= count + 1;
               end
            end
        end
endmodule

module reset_mod(
    input  [3:0] count10s, count1s,
    input        clock,
    input        ext_reset,   
    output reg   reset_out
);

    always @(posedge clock or posedge ext_reset) begin
        if (ext_reset)
            reset_out <= 1;
        else if (count10s == 2 && count1s == 3)  
            reset_out <= 1;      
        else
            reset_out <= 0;
    end

endmodule

module counter24(
    input clock,
    input reset,   
    output [3:0] count1s,
    output [3:0] count10s
);

    wire en;
    wire [3:0] ocount1s, ocount10s;
    wire local_reset;

    counter10 counter10(
        .clock(clock),
        .reset(local_reset),
        .count(ocount1s),
        .en(en)
    );

    counter6 counter6(
        .clock(clock),
        .reset(local_reset),
        .en(en),
        .count(ocount10s)
    );
    
    reset_mod reset_mod(
        .count10s(ocount10s),
        .count1s(ocount1s),
        .clock(clock),
        .ext_reset(reset),
        .reset_out(local_reset)
    );

    assign count10s = ocount10s;
    assign count1s  = ocount1s;

endmodule

module selector(
        input clock,
        input [3:0] bcd1s, bcd10s,
        output [3:0] bcd,
        output [3:0] K
    );
        function [3:0] selectbcd;
            input clock;
            begin
                selectbcd = (clock==0)?bcd1s:bcd10s;
            end
        endfunction
        
        function [1:0] selectK;
            input clock;
            begin
               selectK = (clock==0)?2'b10:2'b01;
            end
        endfunction
        
        assign bcd = selectbcd(clock);
        assign K = {2'b11,selectK(clock)};
endmodule

module sevenseg(
        input [3:0] bcd,
        output [7:0] Y
    );
    
        function [7:0] bcd7;
            input [3:0] bcd;
            begin
                case(bcd)
                    4'h0: bcd7 = 8'b11111100;
                    4'h1: bcd7 = 8'b01100000;
                    4'h2: bcd7 = 8'b11011010;
                    4'h3: bcd7 = 8'b11110010;
                    4'h4: bcd7 = 8'b01100110;
                    4'h5: bcd7 = 8'b10110110;
                    4'h6: bcd7 = 8'b10111110;
                    4'h7: bcd7 = 8'b11100100;
                    4'h8: bcd7 = 8'b11111110;
                    4'h9: bcd7 = 8'b11110110;
                    default: bcd7 = 8'b11111100;
                endcase
            end
        endfunction
        
        assign Y = ~bcd7(bcd);
endmodule


module twentythree(
        input clock, reset,
        output [7:0] Y,
        output [3:0] K
    );
    
    wire count1s,countkhz;
    wire [3:0] count1s_24,count10,bcd;
    
    counterkhz counterkhz(
            .clock(clock),
            .reset(reset),
            .countkhz(countkhz)
    );
    
    counter1sec counter1sec(
            .clock(clock),
            .reset(reset),
            .count1s(count1s)
    );
    
     counter24 counter24(
             .clock(count1s),
             .reset(reset),   
             .count1s(count1s_24),
             .count10s(count10s)
    );
    
    selector selector(
            .clock(countkhz),
            .bcd1s(count1s_24),
            .bcd10s(count10s),
            .bcd(bcd),
            .K(K)
    );
    
    sevenseg sevenseg(
            .bcd(bcd),
            .Y(Y)
    );
endmodule

