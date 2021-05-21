`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/04/19 09:33:58
// Design Name: 
// Module Name: selector_tb
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


module project_2_tb(

    );
reg [5:0]in;
wire out;
project_2 u0(.in(in),.out(out));

always begin
    in = 6'b101000;#20;
    in = 6'b101001;#20;
    in = 6'b101010;#20;
    in = 6'b101011;#20;
  end
endmodule
