MK_PATH:=$(dir $(realpath $(lastword $(MAKEFILE_LIST))))
#@echo $(error MK_PATH=$(MK_PATH))
PROJECT_ROOT:= $(MK_PATH)../../
#@echo $(error PROJECT_ROOT= $(PROJECT_ROOT))
INCDIR += -I$(PROJECT_ROOT)

BUILDDIR := $(PROJECT_ROOT)/Build

SRC_PATH :=  $(dir $(abspath $(dir $$PWD) ))
#@echo $(error SRC_PATH=$(SRC_PATH))

OBJDIR := $(SRC_PATH)obj
# the compiler to use
#CC = clang
#CC = C:\cygwin64\bin\gcc.exe
CC = C:\MinGW\bin\gcc.exe

HAS_TEST_CLI=Y
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS += -g
CFLAGS += -m32
CFLAGS += -std=c11
#CFLAGS += -Og
CFLAGS  += -O0
#CFLAGS  += -Wall
#CFLAGS  +=-pedantic
#CFLAGS += -ftime-report

HAS_X86_64=Y
#OPT += -DSORT_LIST
OPT += -DHAS_UTESTS
OPT += -DHAS_DEBUG
OPT += -DX86_64
OPT += -DHAS_SW_TESTS
OPT += -DHAS_TEST_CLI
OPT += -DHAS_RTCM3
OPT += -DHAS_TEST_SUIT_CALENDAR
OPT += -DHAS_TEST_SUIT_CONVERT
OPT += -DHAS_TEST_SUIT_CRC
OPT += -DHAS_TEST_SUIT_FIFO
OPT += -DHAS_TEST_SUIT_NMEA_PROTO
OPT += -DHAS_TEST_SUIT_RTCM3_PROTO
OPT += -DHAS_TEST_SUIT_STRING_UTILS
OPT += -DHAS_TEST_SUIT_TBFP
OPT += -DHAS_TEST_SUIT_TBFP_RTCM
OPT += -DHAS_TEST_SUIT_UTILS_GNSS
OPT += -DHAS_TEST_SUIT_UTILS
OPT += -DHAS_TEST_SUIT_UBLOX_PROTO

#files to link:
LFLAGS= -static -lm

EXECUTABLE=test_code_base

include $(PROJECT_ROOT)code_base.mk
include $(PROJECT_ROOT)common/common.mk
include $(PROJECT_ROOT)protocols/protocols.mk
include $(PROJECT_ROOT)utils/utils_files/utils_files.mk
include $(PROJECT_ROOT)utils/utils.mk
include $(PROJECT_ROOT)unit_tests/unit_test.mk
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
