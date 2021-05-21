`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/04/19 08:38:27
// Design Name: 
// Module Name: project_1
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


module project_2( 
    input[5:0]in,
    output out );
reg out;

always @(in)
begin
    if( in[0] && in[1]) out = in[2];
    else if(in[0] && !in[1]) out = in[3];
    else if(!in[0] && in[1]) out = in[4];
    else if(!in[0] && !in[1]) out = in[5];
end
endmodule
