.SILENT:
.EXPORT_ALL_VARIABLES:

# ===========================================================
# User should define the project top dir
# ===========================================================

#COMPILER_DIR = ../../../gcc-arm-none-eabi-5_4-2016q2/bin/
STLINK_DIR = ../../../stlink
BIN_DIR := ../../bin
SOURCE_DIR := ../../source
MAKE_FILE_DIR := ../../build/make


# --- Target Device Setting
TARGET := templete_node
#SYSTEM := LINUX
SYSTEM := CortexM4

include $(MAKE_FILE_DIR)/Compiler.mk
include $(MAKE_FILE_DIR)/Color.mk

# --- Define modules to be compiled
MODULE += App
MODULE += ROS_M

ifeq ($(SYSTEM), CortexM4)
MODULE += Board
MODULE += FreeRTOS
MODULE += Lwip
endif


# --- Define directories of source codes according to modeules
ifeq ($(SYSTEM), LINUX)
App_DIR		:= $(SOURCE_DIR)/App/linux
endif

ifeq ($(SYSTEM), CortexM4)
App_DIR		:= $(SOURCE_DIR)/App/cortexM4
Board_DIR 	:= $(SOURCE_DIR)/Board/cortexM4
FreeRTOS_DIR 	:= $(SOURCE_DIR)/FreeRTOS
Lwip_DIR	:= $(SOURCE_DIR)/Lwip
endif


ROS_M_DIR	:= $(SOURCE_DIR)/ROS_M
ifeq ($(SYSTEM), LINUX)
INCLUDE += -I$(ROS_M_DIR)/portable/Linux
endif

ifeq ($(SYSTEM), CortexM4)
INCLUDE += -I$(ROS_M_DIR)/portable/CortexM4
endif

# --- Set include directories
INCLUDE += -I$(App_DIR)/inc
INCLUDE += -I$(ROS_M_DIR)/core

ifeq ($(SYSTEM), CortexM4)
INCLUDE += -I$(Board_DIR)/BSP/inc
INCLUDE += -I$(Board_DIR)/Startup/inc
INCLUDE += -I$(Board_DIR)/Startup/CMSIS
INCLUDE += -I$(Board_DIR)/STM32F4xx_StdPeriph_Driver/inc
INCLUDE += -I$(Board_DIR)/STM32F4x7_ETH_Driver/inc
INCLUDE += -I$(FreeRTOS_DIR)/include
INCLUDE += -I$(FreeRTOS_DIR)/portable
INCLUDE += -I$(Lwip_DIR)/src/include
INCLUDE += -I$(Lwip_DIR)/src/include/ipv4
INCLUDE += -I$(Lwip_DIR)/portable
INCLUDE += -I$(Lwip_DIR)/portable/netif
endif


# --- Set complile flags for linker
ifeq ($(SYSTEM), LINUX)
LIBRARY := -lpthread
LDFLAGS := $(LIBRARY)
endif

ifeq ($(SYSTEM), CortexM4)
MAP := -Wl,-Map=$(BIN_DIR)/$(TARGET).map
LIBRARY := -lc -lm -lgcc
LINKSCRIPT := -T./STM32F407_512k_192k_flash.ld
LDFLAGS := -nostartfiles $(MAP) $(LIBRARY) $(LINKSCRIPT)
LDFLAGS += -specs=nosys.specs
endif


# --- General rules
all: clean
ifdef module
	@echo "$(YELLOW)## $(LIGHT_GREEN)MAKING $(LIGHT_PURPLE)BUILT-IN OBJECT $(WHITE)FOR $(ORANGE)[$(LIGHT_CYAN)$(module)$(ORANGE)]$(NOCOLOR)";
	make --makefile=./modules/$(module).mk built-in;
else
	@for module_name in $(MODULE); do \
		echo "$(YELLOW)## $(LIGHT_GREEN)MAKING $(LIGHT_PURPLE)BUILT-IN OBJECT $(WHITE)FOR $(ORANGE)[$(LIGHT_CYAN)$$module_name$(ORANGE)]$(NOCOLOR)"; \
		make module=$$module_name --makefile=./modules/$$module_name.mk built-in; \
	done

	@echo "$(YELLOW)## $(LIGHT_GREEN)MAKING $(LIGHT_PURPLE)ELF $(WHITE)FOR $(ORANGE)[$(LIGHT_CYAN)$(TARGET)$(ORANGE)]$(NOCOLOR)"
	$(COMPILER_DIR)$(CC) $(addsuffix -built-in.o, $(MODULE)) --output $(BIN_DIR)/$(TARGET).elf $(CFLAGS) $(LDFLAGS)


ifeq ($(SYSTEM), CortexM4)
	@echo "$(YELLOW)## $(LIGHT_GREEN)MAKING $(LIGHT_PURPLE)BINARY $(WHITE)FOR $(ORANGE)[$(LIGHT_CYAN)$(TARGET)$(ORANGE)]$(NOCOLOR)"
	$(OBJCOPY) -O binary $(BIN_DIR)/$(TARGET).elf $(BIN_DIR)/$(TARGET).bin
endif

endif


clean:
ifdef module
	@echo "$(YELLOW)## $(LIGHT_RED)CLEANING $(ORANGE)[$(LIGHT_CYAN)$(module)$(ORANGE)]"
	make --makefile=./modules/$(module).mk clean;
else
	@for module_name in $(MODULE); do \
		echo "$(YELLOW)## $(LIGHT_GREEN)MAKING $(LIGHT_PURPLE)BUILT-IN OBJECT $(WHITE)FOR $(ORANGE)[$(LIGHT_CYAN) module $(ORANGE)]$(NOCOLOR)"; \
		make module=$$module_name module_dir=$($(module)_DIR) --makefile=./modules/$$module_name.mk clean; \
	done
	@echo "$(YELLOW)## $(RED)Removing $(ORANGE)[$(CYAN)output files$(ORANGE)] $(NOCOLOR)"
	rm -rf *.o
	rm -rf $(BIN_DIR)/*.elf
	rm -rf $(BIN_DIR)/*.bin
	rm -rf $(BIN_DIR)/*.map
endif

# Flash the STM32F4
burn: 
	@echo "$(COMPILER_DIR)"
	sudo $(STLINK_DIR)/st-flash --reset write $(BIN_DIR)/$(TARGET).bin 0x8000000 
