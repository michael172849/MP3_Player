################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/Program Files (x86)/CCS/ccsv5/tools/compiler/msp430_4.2.1

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./dr_sdcard/mmc.obj" \
"./dr_sdcard/ff.obj" \
"./dr_sdcard/HAL_SDCard.obj" \
"./dr_usbmsc/usbEventHandling.obj" \
"./dr_usbmsc/usb.obj" \
"./dr_usbmsc/dma.obj" \
"./dr_usbmsc/descriptors.obj" \
"./dr_usbmsc/UsbMscStateMachine.obj" \
"./dr_usbmsc/UsbMscScsi.obj" \
"./dr_usbmsc/UsbMscReq.obj" \
"./dr_usbmsc/UsbIsr.obj" \
"./dr_usbmsc/HAL_UCS.obj" \
"./dr_usbmsc/HAL_TLV.obj" \
"./dr_usbmsc/HAL_PMM.obj" \
"./dr_usbmsc/HAL_PMAP.obj" \
"./dr_usbmsc/HAL_FLASH.obj" \
"./main.obj" \
"./fw_queue.obj" \
"./dr_usbmsc.obj" \
"./dr_tft2.obj" \
"./dr_tft.obj" \
"./dr_audio.obj" \
"../lnk_msp430f6638.cmd" \
"../msp430USB.cmd" \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include dr_usbmsc/subdir_vars.mk
-include dr_sdcard/subdir_vars.mk
-include subdir_rules.mk
-include dr_usbmsc/subdir_rules.mk
-include dr_sdcard/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Pro01a_sdcard_audio.out

# Tool invocations
Pro01a_sdcard_audio.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"D:/Program Files (x86)/CCS/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx --abi=coffabi -g --advice:power="all" --define=__MSP430F6638__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=nofloat -z --stack_size=0x400 -m"Pro01a_sdcard_audio.map" --heap_size=0x400 --use_hw_mpy=F5 -i"D:/Program Files (x86)/CCS/ccsv5/ccs_base/msp430/include" -i"D:/Program Files (x86)/CCS/ccsv5/tools/compiler/msp430_4.2.1/lib" -i"D:/Program Files (x86)/CCS/ccsv5/tools/compiler/msp430_4.2.1/include" --reread_libs --warn_sections --xml_link_info="Pro01a_sdcard_audio_linkInfo.xml" --rom_model -o "Pro01a_sdcard_audio.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(MSP430_EXECUTABLE_OUTPUTS__QUOTED) "Pro01a_sdcard_audio.out"
	-$(RM) "dr_audio.pp" "dr_tft.pp" "dr_tft2.pp" "dr_usbmsc.pp" "fw_queue.pp" "main.pp" "dr_usbmsc\HAL_FLASH.pp" "dr_usbmsc\HAL_PMAP.pp" "dr_usbmsc\HAL_PMM.pp" "dr_usbmsc\HAL_TLV.pp" "dr_usbmsc\HAL_UCS.pp" "dr_usbmsc\UsbIsr.pp" "dr_usbmsc\UsbMscReq.pp" "dr_usbmsc\UsbMscScsi.pp" "dr_usbmsc\UsbMscStateMachine.pp" "dr_usbmsc\descriptors.pp" "dr_usbmsc\dma.pp" "dr_usbmsc\usb.pp" "dr_usbmsc\usbEventHandling.pp" "dr_sdcard\HAL_SDCard.pp" "dr_sdcard\ff.pp" "dr_sdcard\mmc.pp" 
	-$(RM) "dr_audio.obj" "dr_tft.obj" "dr_tft2.obj" "dr_usbmsc.obj" "fw_queue.obj" "main.obj" "dr_usbmsc\HAL_FLASH.obj" "dr_usbmsc\HAL_PMAP.obj" "dr_usbmsc\HAL_PMM.obj" "dr_usbmsc\HAL_TLV.obj" "dr_usbmsc\HAL_UCS.obj" "dr_usbmsc\UsbIsr.obj" "dr_usbmsc\UsbMscReq.obj" "dr_usbmsc\UsbMscScsi.obj" "dr_usbmsc\UsbMscStateMachine.obj" "dr_usbmsc\descriptors.obj" "dr_usbmsc\dma.obj" "dr_usbmsc\usb.obj" "dr_usbmsc\usbEventHandling.obj" "dr_sdcard\HAL_SDCard.obj" "dr_sdcard\ff.obj" "dr_sdcard\mmc.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

