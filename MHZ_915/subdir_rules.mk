################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="D:/faraday_git/faradayio-firmware" --include_path="D:/faraday_git/faradayio-firmware/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

uart.obj: ../uart.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="D:/faraday_git/faradayio-firmware" --include_path="D:/faraday_git/faradayio-firmware/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="none" --define=DEPRECATED --define=__CC430F6137__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU15 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="uart.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


