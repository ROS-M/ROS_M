MODULE_NAME := $(module)
MODULE_DIR := $(Lwip_DIR)

SUBDIRS := $(MODULE_DIR)/src/api/
SUBDIRS += $(MODULE_DIR)/src/core/
SUBDIRS += $(MODULE_DIR)/src/core/ipv4/
SUBDIRS += $(MODULE_DIR)/portable/arch/
SUBDIRS += $(MODULE_DIR)/portable/netif/

include $(MAKE_FILE_DIR)/Rules.mk

