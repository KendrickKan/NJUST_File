@echo off
set xv_path=C:\\Xilinx\\Vivado\\2017.1\\bin
call %xv_path%/xsim moore_0420_tb_behav -key {Behavioral:sim_1:Functional:moore_0420_tb} -tclbatch moore_0420_tb.tcl -view D:/moore_0420/moore_0420/moore_0420_tb_behav.wcfg -log simulate.log
if "%errorlevel%"=="0" goto SUCCESS
if "%errorlevel%"=="1" goto END
:END
exit 1
:SUCCESS
exit 0
