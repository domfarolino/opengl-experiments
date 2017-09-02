UNAME := $(shell uname)
CPP=g++
SRCDIR = ./

ifeq ($(UNAME), Linux)
  CFLAGS= -lSDL2 `sdl2-config --cflags` -lGLEW -g -lGL -std=c++0x
else
  CFLAGS= -I/Library/Frameworks/SDL2.Framework/headers -framework SDL2 -L/usr/local/lib -Wl,-rpath,../ -g -std=c++0x
endif

main:
	g++ 02-Drawing.cpp $(CFLAGS)
