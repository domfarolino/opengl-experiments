UNAME := $(shell uname)
CXX=g++
SRCDIR = ./

ifeq ($(UNAME), Linux)
  CFLAGS= -lSDL2 `sdl2-config --cflags` -lGLEW -g -lGL -std=c++0x
else
  CFLAGS= -I/Library/Frameworks/SDL2.Framework/headers -framework SDL2 -framework OpenGL -framework GLUT -L/usr/local/lib -lGLEW -Wl,-rpath,../ -g -std=c++0x
endif

main:
	$(CXX) 02-Drawing.cpp $(CFLAGS)
