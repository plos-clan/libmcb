include config.mk

SILENT =

ifeq ($(strip $(SILENT)),)
	E = @echo
	Q = @
else
	E = @\#
	Q =
endif
export E Q

HEADER_DIR = $(PREFIX)/include
TARGET_DIR = $(PREFIX)/lib

BUILD_DIR = build

HEADER = mcb
TARGET = libmcb.a

SUB_DIRS = src src/amd64
SRC = $(wildcard *.c $(addsuffix /*.c,$(SUB_DIRS)))
OBJ = $(addprefix $(BUILD_DIR)/,$(SRC:.c=.o))
OBJ_DIRS = $(BUILD_DIR) $(addprefix $(BUILD_DIR)/,$(SUB_DIRS))
OBJ_DEPS = $(addprefix $(BUILD_DIR)/,$(SRC:.c=.d))

CC_CMD = $(CC) $(CFLAGS) -g3 -c -o $@ $<

all: $(TARGET)

include tool.mk

$(OBJ_DIRS):
	$(Q) mkdir -p $@

$(BUILD_DIR)/%.o: %.c | $(OBJ_DIRS)
	$(E) "  CC    " $@
	$(Q) $(CC_CMD) -MMD

$(TARGET): $(OBJ)
	$(E) "  AR    " $@
	$(Q) $(AR) -rcs $@ $(OBJ) $(CLIBS)

clean: clean-gen
	$(E) "  CLEAN"
	$(Q) rm -f $(OBJ) $(TARGET) test/main

clean-all: clean clean-tool

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

include gen.mk

test: test/main
test/main: test/main.c $(TARGET)
	$(E) "  CC    " $@
	$(Q) $(CC) $(CFLAGS) -g3 -o $@ $^
