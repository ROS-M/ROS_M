include $(MAKE_FILE_DIR)/Color.mk

SOURCES += $(wildcard $(addsuffix *.c, $(SUBDIRS)))
SOURCES += $(wildcard $(addsuffix *.s, $(SUBDIRS)))

OBJECTS := $(patsubst %.c, %.o, $(SOURCES))
OBJECTS := $(patsubst %.s, %.o, $(OBJECTS))

%.o : %.c
	@echo "$(YELLOW)## $(GREEN)Compiling $(ORANGE)[$(BLUE)$<$(ORANGE)] $(NOCOLOR)to $(ORANGE)[$(CYAN)$@$(ORANGE)] $(NOCOLOR)"
	$(COMPILER_DIR)$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

%.o : %.s
	@echo "$(YELLOW)## $(GREEN)Compiling $(ORANGE)[$(BLUE)$<$(ORANGE)] $(NOCOLOR)to $(ORANGE)[$(CYAN)$@$(ORANGE)] $(NOCOLOR)"
	$(COMPILER_DIR)$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(MODULE_NAME)-built-in.o : $(OBJECTS)
	$(COMPILER_DIR)$(LD) -r $(OBJECTS) -o $(MODULE_NAME)-built-in.o

built-in : $(MODULE_NAME)-built-in.o 
	@echo "$(YELLOW)## $(GREEN)Linking $(ORANGE)[$(BLUE)OBJ Files$(ORANGE)] $(NOCOLOR)to $(ORANGE)[$(CYAN)$(MODULE_NAME)-built-in.o$(ORANGE)]$(NOCOLOR)"

clean:
	@echo "$(YELLOW)## $(RED)Removing $(ORANGE)[$(CYAN)objective files : $(OBJECTS)$(ORANGE)] $(NOCOLOR)"
	rm -rf $(OBJECTS);
	@echo "$(YELLOW)## $(RED)Removing $(ORANGE)[$(CYAN)$(MODULE_NAME)-built-in.o$(ORANGE)] $(NOCOLOR)"
	rm -rf $(MODULE_NAME)-built-in.o;
