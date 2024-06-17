##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.19.2] date: [Sun Jun 25 21:20:20 CEST 2023] 
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# FLASH VIA CANBUS CAN-ids of device
######################################
CANID_XCP_HOST_TO_TARGET__HEX=007
CANID_XCP_TARGET_TO_HOST__HEX=181

######################################
# target
######################################
TARGET = BMS_LV


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build
# Release path
RELEASE_DIR = release
# Bootloader path
BOOTLOADER_DIR = openblt_f446re

######################################
# source
######################################
# C sources
C_SOURCES =  \
Core/Src/L9963E_utils.c \
Core/Src/adc.c \
Core/Src/can.c \
Core/Src/can_send_timebase.c \
Core/Src/can_utils.c \
Core/Src/data_reading_timebase.c \
Core/Src/dma.c \
Core/Src/eeprom/eeprom-config.c \
Core/Src/eeprom/m95256/m95256.c \
Core/Src/errors.c \
Core/Src/gpio.c \
Core/Src/lem.c \
Core/Src/logger_wrapper.c \
Core/Src/main.c \
Core/Src/ntc.c \
Core/Src/spi.c \
Core/Src/stm32_if.c \
Core/Src/stm32f4xx_hal_msp.c \
Core/Src/stm32f4xx_it.c \
Core/Src/system_stm32f4xx.c \
Core/Src/tim.c \
Core/Src/usart.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.c \
Lib/L9963E_lib/src/L9963E.c \
Lib/L9963E_lib/src/L9963E_drv.c \
Lib/SCan/SC22EVO/artifacts/c_source/mcb.c \
Lib/stmlibs/error_utils/error_utils.c \
Lib/stmlibs/logger/logger.c \
Lib/stmlibs/timebase/timebase.c \
Lib/stmlibs/timer_utils/timer_utils.c

# ASM sources
ASM_SOURCES =  \
startup_stm32f446xx.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
SREC = $(CP) -O srec -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F446xx


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-ICore/Inc \
-ICore/Src/eeprom \
-ICore/Src/eeprom/m95256 \
-IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
-IDrivers/CMSIS/Include \
-IDrivers/STM32F4xx_HAL_Driver/Inc \
-IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
-ILib/L9963E_lib/inc \
-ILib/L9963E_lib/interface_example \
-ILib/SCan/SC22EVO/artifacts/c_source \
-ILib/stmlibs \
-ILib/stmlibs/circular_buffer \
-ILib/stmlibs/critical_section \
-ILib/stmlibs/error_utils \
-ILib/stmlibs/fsm \
-ILib/stmlibs/lock \
-ILib/stmlibs/logger \
-ILib/stmlibs/longcounter \
-ILib/stmlibs/pwm \
-ILib/stmlibs/timebase \
-ILib/stmlibs/timer_utils



# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT)

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT)

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2 -ggdb
endif

# Add additional flags
CFLAGS += -Wall -fdata-sections -ffunction-sections -fstack-usage
ASFLAGS += -Wall -fdata-sections -ffunction-sections -fstack-usage

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F446RETx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = \


# Additional LD Flags from config file
ADDITIONALLDFLAGS = -specs=nano.specs -u_printf_float

LDFLAGS = $(MCU) $(ADDITIONALLDFLAGS) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of C objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# custom makefile rules
#######################################

# The openocd bin path can be either defined in make command via OPENOCD_PATH variable (> make OPENOCD_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef OPENOCD_PATH
OPENOCD = "$(OPENOCD_PATH)/openocd"
else
OPENOCD = "openocd"
endif

#######################################
# flash
#######################################
flash: openocd.cfg $(BUILD_DIR)/$(TARGET).elf
	$(OPENOCD) -f ./openocd.cfg -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

#######################################
# erase
#######################################
erase: openocd.cfg $(BUILD_DIR)/$(TARGET).elf
	$(OPENOCD) -f ./openocd.cfg -c "init; reset halt; stm32f4x mass_erase 0; exit"

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR) $(RELEASE_DIR)


#######################################
# can_build
#######################################
can_build: $(BUILD_DIR)/$(TARGET)_shifted.sx
	

#######################################
# $(BUILD_DIR)/$(TARGET)_shifted.elf
#######################################
$(BUILD_DIR)/$(TARGET)_shifted.elf: $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(OBJECTS) $(MCU) $(ADDITIONALLDFLAGS) -TSTM32F446RETx_FLASH_shifted.ld $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -o $@


#######################################
# $(BUILD_DIR)/$(TARGET)_shifted.bin
#######################################
$(BUILD_DIR)/$(TARGET)_shifted.bin: $(BUILD_DIR)/$(TARGET)_shifted.elf | $(BUILD_DIR)
	$(BIN) $< $@

#######################################
# $(BUILD_DIR)/$(TARGET)_shifted.sx (SREC format used for flashing via can)
#######################################
$(BUILD_DIR)/$(TARGET)_shifted.sx: $(BUILD_DIR)/$(TARGET)_shifted.elf | $(BUILD_DIR)
	$(SREC) $< $@	
	$(SZ) $@

# The openocd bin path can be either defined in make command via BOOTCOMMANDER_PATH variable (> make BOOTCOMMANDER_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef BOOTCOMMANDER_PATH
BOOTCOMMANDER = "$(BOOTCOMMANDER_PATH)/BootCommander"
else
BOOTCOMMANDER = "BootCommander"
endif

#######################################
# can_flash
#######################################
can_flash: $(BUILD_DIR)/$(TARGET)_shifted.sx
	$(BOOTCOMMANDER) -t=xcp_can -d=can0 -b=1000000 -tid="$(CANID_XCP_HOST_TO_TARGET__HEX)h" -rid="$(CANID_XCP_TARGET_TO_HOST__HEX)h" $(BUILD_DIR)/$(TARGET)_shifted.sx

#######################################
# release 
#######################################
$(BOOTLOADER_DIR)/release/* : 
	$(MAKE) -C $(BOOTLOADER_DIR) release

release: $(BUILD_DIR)/$(TARGET)_shifted.sx $(BUILD_DIR)/$(TARGET).elf $(BOOTLOADER_DIR)/release/*
	mkdir -p $(RELEASE_DIR) && cp $^ $(RELEASE_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
