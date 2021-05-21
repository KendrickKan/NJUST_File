`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/04/19 08:18:47
// Design Name: 
// Module Name: project83
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


module project83(
input [7:0] in,
output [2:0] out
    );
    reg [2:0] out;
    always@(in)
    begin
        if(in[0]) out = 3'b000;
        else if (in[1]) out = 3'b001;
        else if (in[2]) out = 3'b010;
        else if (in[3]) out = 3'b011;
        else if (in[4]) out = 3'b100;
        else if (in[5]) out = 3'b101;
        else if (in[6]) out = 3'b110;
        else if (in[7]) out = 3'b111;
        else out = 3'bxxx;
    end
endmodule
