EXEC := test
debug ?= 1

# Directories
LIB_DIR := lib
BIN_DIR := bin
BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include

# Project Filenames
EXEC ?= test
OBJS := $(patsubst %.c,%.o,$(wildcard ${SRC_DIR}/*.c))
MAIN := main.o
HEADERS := $(wildcard ${INCLUDE_DIR}/*.h)
SO_FILE := lib$(EXEC).so

ifeq ($(strip $(OBJS)),)
	ERROR_MESSAGE := "No source file to compile"
endif

# Compiler Settings

# Compiler and Linker Flags
CC := gcc
# CFLAGS := -Wall -Werror -pedantic
LDFLAGS := -l$(EXEC)

ifeq ($(CC), clang-18)
	LINTER := clang-tidy-18
	FORMATTER := clang-format-18
	CLANG_PREPARE := format lint
endif

# Optimization Flags
ifeq ($(debug), 1)
	CFLAGS := $(CFLAGS) -g -O0
else
	CFLAGS := $(CFLAGS) -g -Os
endif

# Entry
all : $(EXEC)

# Install executable for whole system
install : $(CLANG_PREPARE) dir link
	cp $(abspath $(LIB_DIR)/$(SO_FILE)) /usr/local/lib
	chmod 0755 /usr/local/lib/$(SO_FILE)
	ldconfig
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(EXEC) $(SRC_DIR)/main.c $(LDFLAGS) -I$(INCLUDE_DIR)
	cp $(abspath $(BIN_DIR)/$(EXEC)) /usr/local/bin
	@echo "\n\033[32mDone.\033[0m"

uninstall :
	rm -rf /usr/local/lib/$(SO_FILE) /usr/local/bin/$(EXEC)
	ldconfig
	@echo "\n\033[32m$(EXEC) uninstalled.\033[0m"

$(EXEC) : $(CLANG_PREPARE) dir link
	@if [ -n "$(wildcard $(LIB_DIR)/*.so)" ]; then \
		$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $(BUILD_DIR)/$(MAIN) -L$(abspath $(LIB_DIR)) $(LDFLAGS) -I$(INCLUDE_DIR); \
		echo "$(BIN_DIR)/$@ created."; \
		echo "\n\033[33mRun command:\n\texport LD_LIBRARY_PATH=$(abspath $(LIB_DIR)):"'$$LD_LIBRARY_PATH'; \
		echo "\nOr add it to your .bashrc or .zshrc to make it available on bootup.\033[0m"; \
		echo "\n\033[32mDone.\033[0m"; \
	else \
		$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $(BUILD_DIR)/$(MAIN); \
		echo "$(BIN_DIR)/$@ created."; \
	fi

$(OBJS) : dir
	$(CC) $(CFLAGS) -fpic -o "$(BUILD_DIR)/$(@F)" -c "$(*).c" -I$(INCLUDE_DIR)
	@echo "$(BUILD_DIR)/$(@F) created."

link : dir $(OBJS)
	@if [ -z "$(OBJS)" ]; then echo $(ERROR_MESSAGE) ; exit 1; fi
	@if [ -n "$(HEADERS)" ]; then \
		$(CC) -shared -o $(LIB_DIR)/$(SO_FILE) $(filter-out $(BUILD_DIR)/$(MAIN), $(wildcard $(BUILD_DIR)/*.o)); \
		echo "$(LIB_DIR)/$(SO_FILE) created."; \
	fi

format :
	@if [ -n "$(HEADERS)" ]; then $(FORMATTER) -style=file -i $(SRC_DIR)/* $(INCLUDE_DIR)/*; fi

lint :
	@if [ -n "$(HEADERS)" ]; then $(LINTER) $(SRC_DIR)/*.c -- -I$(INCLUDE_DIR) $(CFLAGS); fi

dir :
	mkdir -p $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR)

clean :
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR)

