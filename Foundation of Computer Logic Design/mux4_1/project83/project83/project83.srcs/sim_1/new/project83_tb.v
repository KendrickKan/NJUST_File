`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/04/19 08:24:14
// Design Name: 
// Module Name: project83_tb
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


module project83_tb();
reg[7:0] in;
wire[2:0] out;
project83 u0 (.in(in),.out(out));
always begin
    in = 8'b00000001;#20;
    in = 8'b00000010;#20;
    in = 8'b00000100;#20;
    in = 8'b00001000;#20;
    in = 8'b00010000;#20;
    in = 8'b00100000;#20;
    in = 8'b01000000;#20;
    in = 8'b10000000;#20;
end
endmodule
