`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/05/18 11:16:35
// Design Name: 
// Module Name: moore_0420_tb
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


module moore_0420_tb(

    );
    reg clk,clr,din;
    wire dout;
    moore_0420 u0 (.clk(clk),.clr(clr),.din(din),.dout(dout));
    initial begin
        clk=0;#10;
        clr=1;#10;
        din=0;#10;
    end
    always begin
        clk=0;#10;
        clk=1;#10;
    end
    always begin
        clr=1;#20;
        clr=0;#10000;
    end
    always begin
        din=1;#20;
        din=0;#20;
        din=1;#20;
        din=0;#20;
        din=1;#20;
    end
endmodule
