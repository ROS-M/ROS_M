MODULE_NAME := $(module)
MODULE_DIR := $(Board_DIR)

SUBDIRS := $(MODULE_DIR)/BSP/src/
SUBDIRS += $(MODULE_DIR)/Startup/src/
SUBDIRS += $(MODULE_DIR)/STM32F4xx_StdPeriph_Driver/src/
SUBDIRS += $(MODULE_DIR)/STM32F4x7_ETH_Driver/src/

include $(MAKE_FILE_DIR)/Rules.mk
