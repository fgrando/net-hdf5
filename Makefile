
TARGET_EXEC := nethdf5

ifeq ($(OS),Windows_NT)
BIN_EXTENSION=exe
CC=gcc
LFLAGS :=
else
CC=gcc
BIN_EXTENSION=elf
LFLAGS :=
endif


CFLAGS :=
CFLAGS += -Wall
CFLAGS += -Wpedantic
CFLAGS += -Werror
CFLAGS += -g

DIRS := ./include
DIRS += ./third-party/HDF5/include
DIRS += ./source
DIRS += ./source/utils

BUILD_DIR := build

INCLUDE_FLAGS := $(foreach dir, $(DIRS), -I$(dir))
INCLUDE_FILES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.h))
C_FILES       := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
C_NAMES       := $(notdir $(C_FILES))
OBJS          := $(addprefix $(BUILD_DIR)/,$(C_NAMES:%.c=%.o))

LFLAGS += -L./third-party/HDF5/lib
LFLAGS += -lhdf5

VPATH = $(DIRS)

$(info $$DIRS = $(DIRS))
$(info $$OBJS = $(OBJS))

$(BUILD_DIR)/%.o: %.c
	echo 'cc: $<'
#	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c "$<" -o "$@" $(LFLAGS)

$(TARGET_EXEC): $(OBJS)
	echo 'linking $@'
#	$(CC) $(CFLAGS) -s -shared -o $@.$(LIB_EXTENSION) $(OBJS) -Wl,--subsystem,windows -L $(MINGW_LIB_DIR) $(MLIBS)
# check dll with: dumpbin /exports osl.dll
	$(CC) $(CFLAGS) -o $@.$(BIN_EXTENSION) $(OBJS) $(LFLAGS)
	cp ./third-party/HDF5/bin/libhdf5.dll .
clean:
	rm -f $(BUILD_DIR)/*.*
	rm -f *.$(BIN_EXTENSION)

format:
	clang-format -i $(C_FILES) $(foreach dir, ./include, $(wildcard $(dir)/*.h))

all: $(TARGET_EXEC)

.PHONY: clean all
