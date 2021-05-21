`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/05/11 10:54:47
// Design Name: 
// Module Name: count10
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


module count10(
input clk,input clr, output[3:0] cnt
    );
    reg[3:0] cnt;
    
    always @(posedge clk or posedge clr)
    begin
        if(clr)
            cnt <= 4'b0000;
        else
            if(cnt==4'b1001)
                cnt <= 4'b0000;
            else
                cnt<= cnt+1;
    end
//    assign q=cnt;
endmodule
