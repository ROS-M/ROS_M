MODULE_NAME := $(module)
MODULE_DIR := $(ROS_M_DIR)

SUBDIRS += $(MODULE_DIR)/core/

ifeq ($(SYSTEM), LINUX)
SUBDIRS += $(MODULE_DIR)/portable/Linux/
endif

ifeq ($(SYSTEM), CortexM4)
SUBDIRS += $(MODULE_DIR)/portable/CortexM4/
endif

include $(MAKE_FILE_DIR)/Rules.mk
