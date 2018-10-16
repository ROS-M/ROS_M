# ===========================================
# Make Configuration File to be included
# 
# Using arm-none-eabi compiler
# 
# Author	: Justin. H Kang
# Date		: 2013. 12. 19
#
# ===========================================

# ========  TOOL CHAIN CONFIGURATION ========
ifeq ($(SYSTEM), CortexM4)
TCHAIN_PREFIX = arm-none-eabi-
endif

# Define programs and commands.
CC      	= $(TCHAIN_PREFIX)gcc
CPP     	= $(TCHAIN_PREFIX)g++
AR      	= $(TCHAIN_PREFIX)ar
LD		= $(TCHAIN_PREFIX)ld
OBJCOPY 	= $(TCHAIN_PREFIX)objcopy
OBJDUMP 	= $(TCHAIN_PREFIX)objdump
SIZE    	= $(TCHAIN_PREFIX)size
NM      	= $(TCHAIN_PREFIX)nm
# ============================================

# ======== FLAGS OF COMPILE ========

ifeq ($(SYSTEM), CortexM4)
# FLAGS depends on CPU (Arm-Cortex M4, STM32F407VGT6)
CPU_SPEC = -mcpu=cortex-m4 -mthumb -DUSE_STDPERIPH_DRIVER
#CPU_SPEC += -mfloat-abi=hard -mfpu=fpv4-sp-d16
FLAGS = -fno-builtin
endif

#DEBUG = -ggdb
#OPTIMIZE += -O$(OPT)
#WARNING = -Wall -W

CFLAGS = $(CPU_SPEC) $(DEBUG) $(OPTIMIZE) $(WARNING) $(FLAGS)
# ==================================


