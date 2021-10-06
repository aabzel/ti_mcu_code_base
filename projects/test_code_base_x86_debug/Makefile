MK_PATH:=$(dir $(realpath $(lastword $(MAKEFILE_LIST))))
#@echo $(error MK_PATH= $(MK_PATH))
PROJECT_ROOT:= $(MK_PATH)../../
#@echo $(error PROJECT_ROOT= $(PROJECT_ROOT))
INCDIR += -I$(PROJECT_ROOT)

BUILDDIR := $(PROJECT_ROOT)/Build

SRC_PATH :=  $(dir $(abspath $(dir $$PWD) ))
#@echo $(error SRC_PATH=$(SRC_PATH))

OBJDIR := $(SRC_PATH)obj
# the compiler to use
#CC = clang
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  += -g
#CFLAGS += -Og
CFLAGS  += -O0
#CFLAGS  += -Wall
#CFLAGS  +=-pedantic
#CFLAGS += -ftime-report

#OPT += -DSORT_LIST
OPT += -DHAS_UTESTS
OPT += -DX86_64
OPT += -DHAS_SW_TESTS
OPT += -DHAS_TEST_SUIT_FIFO
OPT += -DHAS_TEST_SUIT_UTILS
OPT += -DHAS_TEST_SUIT_CONVERT
OPT += -DHAS_TEST_SUIT_CRC
OPT += -DHAS_TEST_SUIT_UBLOX_PROTO
OPT += -DHAS_TEST_SUIT_NMEA_PROTO

#files to link:
LFLAGS= -static

EXECUTABLE=test_code_base

include $(PROJECT_ROOT)common/common.mk
include $(PROJECT_ROOT)protocols/protocols.mk
include $(PROJECT_ROOT)utils/utils_file/utils_file.mk
include $(PROJECT_ROOT)utils/io_utils/io_utils.mk
include $(PROJECT_ROOT)utils/utils.mk
include $(PROJECT_ROOT)unit_tests/unit_test.mk
include $(PROJECT_ROOT)utils/utils_string/string_utils.mk
#include $(PROJECT_ROOT)tests/tests.mk
SOURCES_C += main.c
# the name to use for both the target source file, and the output file:
#include $(PROJECT_ROOT)system_explore/system_explore.mk


#@echo $(error SOURCES_C= $(SOURCES_C))
INCDIR := $(subst /cygdrive/c/,C:/, $(INCDIR))
#@echo $(error INCDIR= $(INCDIR))
OBJ := $(patsubst %.c, %.o, $(SOURCES_C))
OBJ := $(subst /cygdrive/c/,C:/, $(OBJ))
#@echo $(error OBJ= $(OBJ))

.PHONY:all

all:$(OBJ) $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) $(INCDIR) $(OPT) -c $< -o $@ 
	#mv *.o $(SRC_PATH)

$(EXECUTABLE): $(OBJ)
	$(CC) $(OPT) $(CFLAGS) $(INCDIR) $(OBJ) $(LFLAGS) -o $(EXECUTABLE).exe 

clean:
	rm -r $(EXECUTABLE) $(OBJ) 
