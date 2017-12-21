UNAME := $(shell uname)
CXX=g++
SRCDIR = ./

ifeq ($(UNAME), Linux)
  CFLAGS= -lSDL2 `sdl2-config --cflags` -lGLEW -g
else
  CFLAGS= -I/Library/Frameworks/SDL2.Framework/headers -framework SDL2 -L/usr/local/lib -lGLEW -Wl,-rpath,../ -g
endif

main:
	$(CXX) 01-OpenGL-Context.cpp $(CFLAGS)
