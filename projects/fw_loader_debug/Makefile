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
#CC = gcc
CC = C:\MinGW\bin\gcc.exe

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
HAS_SERIAL_PORT=Y
HAS_X86_64=Y
#OPT += -DSORT_LIST
OPT += -DHAS_UTESTS
OPT += -DHAS_LOADER
OPT += -DX86_64
OPT += -DHAS_FW_UPDATE_DEBUG
#OPT += -DHAS_COM_PORT_DEBUG
#OPT += -DDEDUG_RX_CHAR

#files to link:
LFLAGS= -static -lm

EXECUTABLE=fw_loader_d

include $(PROJECT_ROOT)code_base.mk
#include $(PROJECT_ROOT)utils/win_utils/win_utils.mk
include $(PROJECT_ROOT)utils/utils.mk
include $(PROJECT_ROOT)utils/utils_files/utils_files.mk
#include $(PROJECT_ROOT)tests/tests.mk
SOURCES_C += main.c
#SOURCES_C += cli_wrappers.c
SOURCES_C += fw_update.c
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
