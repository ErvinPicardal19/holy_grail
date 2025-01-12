EXEC := doubly_linked_list
MAKE_SHARED_LIBRARY := 0
EXEC ?= test
MAKE_SHARED_LIBRARY ?= 0
DEBUG ?= 1

# Project Dirs
LIB_DIR := lib
BIN_DIR := bin
BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include

# Files
MAIN := ${EXEC}.o
SRC := $(wildcard ${SRC_DIR}/*.c)
OBJS := $(patsubst ${SRC_DIR}/%.c,${BUILD_DIR}/%.o,${SRC}) 

# Flags
CC ?= gcc
CFLAGS ?= -Wall -Werror -pedantic
LDFLAGS +=

ifeq ($(CC), clang-18)
	LINTER := clang-tidy-18
	FORMATTER := clang-format-18
	CLANG_PREPARE := format lint
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0
else
	CFLAGS += -g -Os
endif

ifeq (${MAKE_SHARED_LIBRARY}, 1)
	CFLAGS += -fPIC
	TARGET := ${BIN_DIR}/lib${EXEC}.so
	BIN_INSTALL_PATH := /usr/local/lib
else
	TARGET := ${BIN_DIR}/${EXEC}
	BIN_INSTALL_PATH := /usr/local/bin
endif

# Entry
all : dir ${TARGET}

install : dir ${TARGET}
	@cp -v $(abspath ${TARGET}) ${BIN_INSTALL_PATH}
	@mkdir -pv /usr/local/include/${EXEC}
	@if [ -n "$(wildcard ${INCLUDE_DIR}/*.h)" ]; then \
		cp -v ${INCLUDE_DIR}/* /usr/local/include/; \
	fi
	chmod 755 ${BIN_INSTALL_PATH}/$(notdir ${TARGET})
	ldconfig
	@echo "\n\033[32mDone.\033[0m"

uninstall :
	@rm -rvf ${BIN_INSTALL_PATH}/$(notdir ${TARGET})
	@rm -rvf /usr/local/include/${EXEC}
	ldconfig
	@echo "\n\033[32mDone.\033[0m"

${BIN_DIR}/${EXEC} : ${OBJS}
	${CC} -o $@ ${OBJS} -L${LIB_DIR}/ ${LDFLAGS}
	@echo "\n\033[32mDone.\033[0m"
	@if [ -n "${LDFLAGS}" ]; then \
		echo "\n\033[33mRun command:\n\texport LD_LIBRARY_PATH=$(abspath $(LIB_DIR)):"'$$LD_LIBRARY_PATH'; \
		echo "\nAdd it to your .bashrc or .zshrc to make it available on bootup.\033[0m"; \
	fi
${BIN_DIR}/lib${EXEC}.so : ${OBJS}
	${CC} -shared -o $@ ${OBJS} -L${LIB_DIR}/ ${LDFLAGS}

${BUILD_DIR}/%.o : ${SRC_DIR}/%.c
	${CC} ${CFLAGS} -o $@ -c $^ -I${INCLUDE_DIR}

dir :
	@mkdir -pv ${BIN_DIR} ${BUILD_DIR}

clean :
	@rm -rvf ${BIN_DIR} ${BUILD_DIR}
