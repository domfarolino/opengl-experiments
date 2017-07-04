# opengl-experiments
Repository accompanying me as I learn more about OpenGL, SDL, and GLEW

This repository will document my experience learning about building OpenGL programs in C++. For starters, I'm following [this](https://open.gl)
wonderful tutorial to help me get started. I'm following the guide on the path of using the Simple DirectMedia Library 2 (SDL2) to create nice
windows and respond to events from the OS, and GLEW to ensure the most modern OpenGL functions available at runtime are being used.

# Installation & Configuration

The process of installing the aforementioned dependencies (SDL2 and GLEW) vary on each platform. At the time of writing I'm using macOS Sierra
10.12.5, but given you have all necessary libraries installed properly the code (modulo the Makefile) should work just fine.

## Installing SDL2 (macOS)

There are two valid ways to install SDL2 on macOS. The first is seamless and requires no compiling. Go to the
[SDL download page](https://www.libsdl.org/download-2.0.php) and downloading the `.dmg` file. Run the `.dmg` file and follow the very simple
accompanying `ReadMe.txt`.

You can also build it "the Unix way" by following [these](https://wiki.libsdl.org/Installation#Mac_OS_X) instructions but probably not necessary.

## Installing GLEW (macOS)

Apparently GLEW isn't even necessary on macOS
(see [this](https://stackoverflow.com/a/22116623/3947332) and [this](https://stackoverflow.com/questions/12229714) source) but I know
I'll probably need to do this on linux anyways so I'm going to roll with it for now.

This was as simple as:

 - Downloading the latest version of the library via `curl -LOk https://github.com/nigels-com/glew/releases/download/glew-2.0.0/glew-2.0.0.zip`
 - Unzipping it `unzip glew-2.0.0.zip`
 - Following build instructions `cd glew-2.0.0 && make && sudo make install && make clean`

**As the tutorial states in the "Context creation" page, ensure the GL/glew.h header appears before OpenGL headers or the library used for window creation (SDL)**
