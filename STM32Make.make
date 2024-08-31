##########################################################################################################################
# File automatically-generated by STM32forVSCode
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

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

######################################
# source
######################################
# C sources
C_SOURCES =  \
Core/Src/L9963E_utils.c \
Core/Src/adc.c \
Core/Src/app_db.c \
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
Core/Src/syscalls.c \
Core/Src/sysmem.c \
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
Lib/SCan/SC22EVO/artifacts/HVCB/c_source/hvcb.c \
Lib/SCan/SC22EVO/artifacts/MCB/c_source/mcb.c \
Lib/SCan/SC24/artifacts/HVCB/c_source/hvcb.c \
Lib/SCan/SC24/artifacts/IFCB/c_source/ifcb.c \
Lib/SCan/SC24/artifacts/IRCB/c_source/ircb.c \
Lib/SCan/SC24/artifacts/MCB/c_source/mcb.c \
Lib/SCan/SC24/artifacts/MCB_og/c_source/mcb_og.c \
Lib/stmlibs/error_utils/error_utils.c \
Lib/stmlibs/logger/logger.c \
Lib/stmlibs/timebase/timebase.c \
Lib/stmlibs/timer_utils/timer_utils.c


CPP_SOURCES = \


# ASM sources
ASM_SOURCES =  \
startup_stm32f446xx.s



#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
POSTFIX = "
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
GCC_PATH="/usr/bin
ifdef GCC_PATH
CXX = $(GCC_PATH)/$(PREFIX)g++$(POSTFIX)
CC = $(GCC_PATH)/$(PREFIX)gcc$(POSTFIX)
AS = $(GCC_PATH)/$(PREFIX)gcc$(POSTFIX) -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy$(POSTFIX)
SZ = $(GCC_PATH)/$(PREFIX)size$(POSTFIX)
else
CXX = $(PREFIX)g++
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

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
-DSTM32F446xx \
-DUSE_HAL_DRIVER


# CXX defines
CXX_DEFS =  \
-DSTM32F446xx \
-DUSE_HAL_DRIVER


# AS includes
AS_INCLUDES = \

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
-ILib/SCan/SC22EVO/artifacts/HVCB/c_source \
-ILib/SCan/SC22EVO/artifacts/MCB/c_source \
-ILib/SCan/SC24/artifacts/HVCB/c_source \
-ILib/SCan/SC24/artifacts/IFCB/c_source \
-ILib/SCan/SC24/artifacts/IRCB/c_source \
-ILib/SCan/SC24/artifacts/MCB/c_source \
-ILib/SCan/SC24/artifacts/MCB_og/c_source \
-ILib/stmlibs \
-ILib/stmlibs/circular_buffer \
-ILib/stmlibs/critical_section \
-ILib/stmlibs/digital_filters/iir_filter \
-ILib/stmlibs/digital_filters/median_filter \
-ILib/stmlibs/error_utils \
-ILib/stmlibs/fsm \
-ILib/stmlibs/lock \
-ILib/stmlibs/logger \
-ILib/stmlibs/longcounter \
-ILib/stmlibs/pwm \
-ILib/stmlibs/timebase \
-ILib/stmlibs/timer_utils



# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CXXFLAGS = $(MCU) $(CXX_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -feliminate-unused-debug-types

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf -ggdb
CXXFLAGS += -g -gdwarf -ggdb
endif

# Add additional flags
CFLAGS += -Wall -fdata-sections -ffunction-sections 
ASFLAGS += -Wall -fdata-sections -ffunction-sections 
CXXFLAGS += 

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
CXXFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

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
# list of cpp program objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

# list of C objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# list of ASM program objects
UPPER_CASE_ASM_SOURCES = $(filter %.S,$(ASM_SOURCES))
LOWER_CASE_ASM_SOURCES = $(filter %.s,$(ASM_SOURCES))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(UPPER_CASE_ASM_SOURCES:.S=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(LOWER_CASE_ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.cpp STM32Make.make | $(BUILD_DIR) 
	$(CXX) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cxx STM32Make.make | $(BUILD_DIR) 
	$(CXX) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cxx=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.c STM32Make.make | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s STM32Make.make | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.S STM32Make.make | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) STM32Make.make
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# flash
#######################################
flash: $(BUILD_DIR)/$(TARGET).elf
	"/usr/bin/openocd" -f ./openocd.cfg -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

#######################################
# erase
#######################################
erase: $(BUILD_DIR)/$(TARGET).elf
	"/usr/bin/openocd" -f ./openocd.cfg -c "init; reset halt; stm32f4x mass_erase 0; exit"

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# custom makefile rules
#######################################




#######################################
# can_build
#######################################
can_build: $(BUILD_DIR)/$(TARGET)_shifted.sx
	
      


#######################################
# $(BUILD_DIR)/$(TARGET)_shifted.sx
#######################################
$(BUILD_DIR)/$(TARGET)_shifted.sx: $(BUILD_DIR)/$(TARGET)_shifted.bin
	bin2srec -a $$(grep 'FLASH (rx)      : ORIGIN =' STM32F446RETx_FLASH_shifted.ld | awk '{print $$6}' | sed 's/.$$//') -i $(BUILD_DIR)/$(TARGET)_shifted.bin -o $(BUILD_DIR)/$(TARGET)_shifted.sx
      


#######################################
# $(BUILD_DIR)/$(TARGET)_shifted.elf
#######################################
$(BUILD_DIR)/$(TARGET)_shifted.elf: $(OBJECTS) STM32Make.make
	$(CC) $(OBJECTS) $(MCU) $(ADDITIONALLDFLAGS) -TSTM32F446RETx_FLASH_shifted.ld $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -o $@
      


#######################################
# $(BUILD_DIR)/$(TARGET)_shifted.bin
#######################################
$(BUILD_DIR)/$(TARGET)_shifted.bin: $(BUILD_DIR)/$(TARGET)_shifted.elf | $(BUILD_DIR)
	$(BIN) $< $@
      


#######################################
# can_flash
#######################################
can_flash: $(BUILD_DIR)/$(TARGET)_shifted.sx
	bootcommander -t=xcp_can -d=can0 -b=1000000 -tid=7h -rid=181h $(BUILD_DIR)/$(TARGET)_shifted.sx
      
	
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***