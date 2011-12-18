VERSION 	= $$Id: Makefile,v 3.15 2004/12/18 15:52:30 fungos Exp $$

include Objects.in

TARGET = ../pk2sh

#
# Tools
#
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
GCC		= $(CROSS_COMPILE)g++
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip


#
# Compiler Flags and Parameters.
#
CFLAGS = -fPIC -Wall $(CDEFS)
CDEFS = -D GCC -D LINUX -D DEBUG -g3 -O0
#CDEFS = -D GCC -D LINUX -O3
LFLAGS = -lreadline -lboost_filesystem -lboost_program_options
CPPFLAGS = $(CFLAGS)

#
# AR Parameters
#
AR_PARAM = cru


#
# targets...
#

all: 	$(TARGET)


$(TARGET): $(OBJECTS)
	$(GCC) -o $(TARGET) $(LFLAGS) $(OBJECTS)
#	$(AR) $(AR_PARAM) $(TARGET) $(OBJECTS)


#
# Cleanup
#
clean:
	rm -f $(OBJECTS) $(MOBJECTS) $(TARGET) ../*~ ../core *~

