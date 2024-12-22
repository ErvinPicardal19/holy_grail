EXEC := test
MAIN := ${EXEC}.o
MAKE_SHARED_LIBRARY := 0
DEBUG ?= 1

# Project Dirs
LIB_DIR := lib
BIN_DIR := bin
BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include

# Files
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
else
	TARGET := ${BIN_DIR}/${EXEC}
endif

# Entry
all : dir ${OBJS} ${TARGET}

${BIN_DIR}/${EXEC} :
	${CC} -o $@ ${OBJS} -L${LIB_DIR}/ ${LDFLAGS}
	@echo "Done."

${BIN_DIR}/lib${EXEC}.so :
	${CC} -shared -o $@ ${OBJS} -L${LIB_DIR}/ ${LDFLAGS}

${BUILD_DIR}/%.o : ${SRC_DIR}/%.c
	${CC} ${CFLAGS} -o $@ -c $^ -I${INCLUDE_DIR}

dir :
	@mkdir -pv ${BIN_DIR} ${BUILD_DIR}

clean :
	@rm -rvf ${BIN_DIR} ${BUILD_DIR}
