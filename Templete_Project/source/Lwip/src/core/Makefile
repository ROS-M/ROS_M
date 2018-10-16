ifndef TOPDIR
TOPDIR := ../../..
endif

include $(TOPDIR)/Config.mk

TARGET	:= built-in.o
SUBDIRS := 
	
OBJS 	:= $(patsubst %.s, %.o, $(wildcard *.s))
OBJS	+= $(patsubst %.c, %.o, $(wildcard *.c))

all : $(OBJS)
	$(LD) -r $(addsuffix /$(TARGET), $(SUBDIRS)) $(OBJS) -o $(TARGET)

include $(TOPDIR)/Rules.mk
