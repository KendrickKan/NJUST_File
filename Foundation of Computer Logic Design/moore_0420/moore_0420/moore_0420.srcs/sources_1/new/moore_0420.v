`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/05/18 11:15:48
// Design Name: 
// Module Name: moore_0420
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


module moore_0420(
    input clk, input clr, input din, output dout
    );
    reg dout;
    reg [2:0] cs,ns;
    parameter s0=3'b000,s1=3'b001,s2=3'b010,s3=3'b011,s4=3'b100,s5=3'b101;
    always @(posedge clk or posedge clr)
    begin
        if(clr==1) cs<=s0;
        else cs<=ns;
    end
    always @(cs or din)
    begin
        case(cs)
            s0:if(din==1) ns<=s1; else ns<=s0;
            s1:if(din==0) ns<=s2; else ns<=s1;
            s2:if(din==1) ns<=s3; else ns<=s0;
            s3:if(din==0) ns<=s4; else ns<=s1;
            s4:if(din==1) ns<=s5; else ns<=s0;
            s5:if(din==0) ns<=s0; else ns<=s1;
            default: ns<=s0;
        endcase
    end
    always @(cs)
    begin
        if(cs==s5)
            dout<=1'b1;
        else 
            dout<=1'b0;
    end
endmodule
