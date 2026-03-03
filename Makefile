include config.mk

HEADER_DIR = $(PREFIX)/include
TARGET_DIR = $(PREFIX)/lib

BUILD_DIR = build

HEADER = mcb
TARGET = libmcb.a

SUB_DIRS = src src/inst src/target src/target/gnu_asm
SRC = $(wildcard *.c $(addsuffix /*.c,$(SUB_DIRS)))
OBJ = $(addprefix $(BUILD_DIR)/,$(SRC:.c=.o))
OBJ_DIRS = $(BUILD_DIR) $(addprefix $(BUILD_DIR)/,$(SUB_DIRS))
OBJ_DEPS = $(addprefix $(BUILD_DIR)/,$(SRC:.c=.d))

CC_CMD = $(CC) $(CFLAGS) -g3 -c -o $@ $<

all: $(TARGET)

$(OBJ_DIRS):
	mkdir -p $@

$(BUILD_DIR)/%.o: %.c | $(OBJ_DIRS)
	@echo "  CC    " $@
	@$(CC_CMD) -MMD

$(TARGET): $(OBJ)
	@echo "  AR    " $@
	@$(AR) -rcs $@ $(OBJ) $(CLIBS)

clean:
	@echo "  CLEAN"
	@rm -f $(OBJ) $(TARGET)
	@rm -f test/main

install: $(TARGET)
	mkdir -p $(HEADER_DIR) $(TARGET_DIR)
	cp -rT include/$(HEADER) $(HEADER_DIR)/$(HEADER)
	cp -f $(TARGET) $(TARGET_DIR)/$(TARGET)

uninstall:
	rm -f $(HEADER_DIR)/$(HEADER) $(TARGET_DIR)/$(TARGET)

%.h:
	@:
ifeq (,$(filter clean,$(MAKECMDGOALS)))
-include $(OBJ_DEPS)
endif

.PHONY: all clean install uninstall
.PHONY: test

test: test/main
test/main: test/main.c $(TARGET)
	@echo "  CC    " $@
	@$(CC) $(CFLAGS) -g3 -o $@ $^
