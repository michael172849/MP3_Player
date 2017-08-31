################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
dr_sdcard/HAL_SDCard.obj: ../dr_sdcard/HAL_SDCard.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files (x86)/CCS/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx --abi=coffabi -g --include_path="D:/Program Files (x86)/CCS/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files (x86)/CCS/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power="all" --define=__MSP430F6638__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=nofloat --preproc_with_compile --preproc_dependency="dr_sdcard/HAL_SDCard.pp" --obj_directory="dr_sdcard" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dr_sdcard/ff.obj: ../dr_sdcard/ff.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files (x86)/CCS/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx --abi=coffabi -g --include_path="D:/Program Files (x86)/CCS/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files (x86)/CCS/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power="all" --define=__MSP430F6638__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=nofloat --preproc_with_compile --preproc_dependency="dr_sdcard/ff.pp" --obj_directory="dr_sdcard" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

dr_sdcard/mmc.obj: ../dr_sdcard/mmc.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Program Files (x86)/CCS/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx --abi=coffabi -g --include_path="D:/Program Files (x86)/CCS/ccsv5/ccs_base/msp430/include" --include_path="D:/Program Files (x86)/CCS/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power="all" --define=__MSP430F6638__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=nofloat --preproc_with_compile --preproc_dependency="dr_sdcard/mmc.pp" --obj_directory="dr_sdcard" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


