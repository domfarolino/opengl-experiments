UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LDFLAGS=
FLAGS= -lSDL2 `sdl2-config --cflags` -lGLEW -g -std=c++0x
else
LDFLAGS="-Wl,-rpath,../"
FLAGS= -I/Library/Frameworks/SDL2.Framework/headers -framework SDL2 -L/usr/local/lib -lGLEW -g -std=c++0x
endif

main:
	g++ 02-Drawing.cpp $(LDFLAGS) $(FLAGS)
