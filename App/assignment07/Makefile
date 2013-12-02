#
# Makefile for CG exercises for Linux and MacOS X
# On Windows Visual Studio can be used (you will have to build 
# a project file yourself).
#
CC     = cc
CPP    = c++
CPPFLAGS = -I../glfw/include -I../glloadgen -I../glm -pthread -O2 -Wall

GROUP = assignment

#
# OS detection
#
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
EXTRA_DEFINES  = -DLINUX
LIBS         = ../glfw/src/libglfw3.a
LFLAGS       = $(LIB_LINUX) -lXrandr -lGL -lXi -pthread -lm -lX11 -lXxf86vm
endif
ifeq ($(UNAME), Darwin)
EXTRA_DEFINES  = -DLION
LIBS         = ../glfw/src/libglfw3.a
LFLAGS       = $(LIB_MAC) -framework Cocoa -framework OpenGL -framework IOKit
endif

OBJECTFILES      = main.o $(GROUP).o vertexData.o teapot.o ../glloadgen/gl_core_32.o ../glloadgen/KHR_debug_emulator.o

assignment: $(OBJECTFILES) $(LIBS)
	$(CPP) $(OBJECTFILES) $(LIBS) $(LFLAGS) -o $(GROUP)

main.o: main.cpp
	$(CPP) $(CPPFLAGS) $(EXTRA_DEFINES) -c main.cpp -o main.o

../glloadgen/gl_core_32.o: ../glloadgen/gl_core_32.cc
	$(CPP) $(CPPFLAGS) -Wno-write-strings -c ../glloadgen/gl_core_32.cc -o ../glloadgen/gl_core_32.o

../glloadgen/KHR_debug_emulator.o: ../glloadgen/KHR_debug_emulator.cc
	$(CPP) $(CPPFLAGS) -Wno-write-strings -c ../glloadgen/KHR_debug_emulator.cc -o ../glloadgen/KHR_debug_emulator.o

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $<

../glfw/src/libglfw3.a:
	( cd ../glfw && cmake CMakeLists.txt && make )

clean:
	rm -f *.o
	rm -f $(GROUP)

mrproper:
	rm -f *.o
	rm -f $(GROUP)
	rm -f ../shared/tools.o
	rm -f ../glloadgen/gl_core_32.o
	rm -f ../glfw/lib/cocoa/libglfw.a
	rm -f ../glfw/lib/x11/libglfw.a



