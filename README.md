# Ojamajo Pacman
wakawakawaka

![](https://i.imgur.com/h5vgYYG.jpg)

*The image that inspired it all*

# Building
## Windows
- Install mingw-w64 (GCC 6.1.0, compiler version must match) from https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download (make sure it's in your PATH)
- Install CMake from https://cmake.org/download/
- Create a build directory with `mkdir build` and `cd build`
- Build with `cmake .. -G "MinGW Makefiles"` and `mingw32-make`
- Copy data (from /data) and .dlls (from /lib) to the /bin directory
## Mac
i dont kno

You can try following the Linux instructions after installing CMake and SFML with https://brew.sh
## Linux
- Install CMake and SFML (usually `cmake`, `sfml` or `libsfml-dev`)
- Create a build directory with `mkdir build && cd build`
- Build with `cmake .. && make && ln -s ../data ../bin/data`
