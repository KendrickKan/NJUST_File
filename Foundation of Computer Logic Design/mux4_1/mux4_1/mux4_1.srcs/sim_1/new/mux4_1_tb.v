`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/04/19 09:34:39
// Design Name: 
// Module Name: mux4_1_tb
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

module mux4_1_tb;
reg i0,i1,i2,i3;
reg s1,s0;
wire out;
mux4_1 unit(
    .i0(i0),
    .i1(i1),
    .i2(i2),
    .i3(i3),
    .s1(s1),
    .s0(s0),
    .out(out)
);
initial
    begin
    i0=2'b00;
    i1=2'b01;
    i2=2'b10;
    i3=2'b11;
    s1=1'b0;s0=1'b0;
    #20
    s1=1'b0;s0=1'b1;
    #20
    s1=1'b1;s0=1'b0;
    #20
    s1=1'b1;s0=1'b1;
    #20;
    end
endmodule
