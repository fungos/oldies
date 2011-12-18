VERSION = $Id: $
PROJECT=pk2tool

include Objects.in

TARGET = bin/$(PROJECT)

# Flags/Compilers
GCC = g++
CFLAGS = -Wall $(CDEFS)
CDEFS = -fPIC -g  -D LINUX -D _DEBUG -D _SECURITY
LFLAGS = -L/bin -lboost_filesystem -lboost_program_options -lboost_system
CPPFLAGS = $(CFLAGS)
LIBS =

# targets...

all:	$(TARGET)

$(TARGET): $(OBJECTS)
	$(GCC) -o $(TARGET) $(OBJECTS) $(LFLAGS) $(LIBS)


release:
	$(GCC) -o $(TARGET) $(OBJECTS) $(LFLAGS) $(LIBS) -o3
	strip $(TARGET)

valgrind:
	valgrind --leak-check=full $(TARGET)

run:
	$(TARGET)

#
# Cleanup
#
clean:
	rm -f $(OBJECTS) $(TARGET) *~ src/*~ core
