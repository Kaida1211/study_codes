`timescale 1ns / 1ps

module counter(
    input clock,
    input reset,
    input in,
    output reg [3:0] out
    );
    
    always @(posedge clock or negedge reset) begin
        if(!reset) begin
            out <= 0;
        end else begin
            out <= out + 1;
        end
    end
    
endmodule
