MODULE_NAME := $(module)
MODULE_DIR := $(FreeRTOS_DIR)

SUBDIRS := $(MODULE_DIR)/
SUBDIRS += $(MODULE_DIR)/portable/
SUBDIRS += $(MODULE_DIR)/portable/MemMang/

include $(MAKE_FILE_DIR)/Rules.mk
