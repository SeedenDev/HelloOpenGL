# HelloOpenGL 
Learning OpenGL from several sources on the web, but highly following the tutorials from [LearnOpenGL.com]((https://learnopengl.com/)) as for the beginning.
My first project written in C++ too, so I'm also learning it here.

I intend to archive this repository as soon as I'm done with the TODOs to start from scratch another engine but this time with a better code architecture.
I've already learnt a lot from this project (and since I've started learning including other stuff like browsing opensource code) and there are some design problems 
I don't want to waste time on here to fix/refactor, it'll be better starting off anew to do cleaner code and consolidate my knowledge.

## Features
WIP

## TODOs
- More rendering stuff, but in the first place finish at least [LearnOpenGL.com](https://learnopengl.com/) tutorials.
- Optimise models loading (very long atm)
- Test LTO & SIMD flags on Linux
- Change how asm output and preprocessed output is handled (custom target?)
- Fix cross-compilation Linux -> Windows (libcurl issue)
- More CMake stuff (cross-compilation to more targets and w/ more options, more Windows setup choices)
- Once MinGW compiler is working for Windows: test the -mwindows linker flag is working (windowed app = no console)
- Test on Windows using Visual Studio 2026
- Make the project works on Windows ARM64 and Linux Aarch64
- The README
- Check gitignored models licence
- Fix gitmodules

## Requirements
- CMake 3.21 or higher
- An OpenGL 4.5+ capable GPU driver (Warning: may evolve in the future in order to test newer features)
- Linux x64 or Windows x64.
- Any C++20 compilers should work. Tested using:
  - **Linux**: GCC 16.1 or CLang 22.1
  - **Windows (to-be-tested)**: MSVC (using Visual Studio 2022 or 2026) *or [TODO] GCC/CLang (using Ninja generator)*
  - **Cross-compile Linux x64 -> Windows x64**: x86_64-w64-mingw32-g++ (using gcc 16.1) // BROKEN!!! bc of libcurl
  - **Cross-compile Windows x64 -> Linux x64**: WIP

### About MacOS support
MacOS Intel and Apple Silicon will never have a support. The current codebase uses a bunch of 4.3+ OpenGL functions and it would be very annoying
to remove some of them for me currently. Feel free to do your own modifications if you want to try making it run on Mac.

## Setup / Build / Run / Cross-compile
WIP but simply put for now: CMake (should work on CLion without much trouble as I'm using it).

More CLion info: don't forget to set the WORKING_DIR in your run configuration. An example config file can be found in `cmake/ide-templates/clion/runConfigurations`. You should be able to use the `$ProjectFileDir$` variable to automatically target this root folder.
for this program is lic
Visual Studio 22 info: I've never tried opening the CMakeLists.txt with the IDE in order to generate the build files so I won't guarantee
it works this way. Prefer using `cmake --preset windows-x64-msvc-vs2022` (or vs2026, untested) or running `cmake/Setup.bat` and then 
you can open the .sln file with Visual Studio in the `build` folder.

## Third-party libraries
- GLAD 2.0.8 (https://github.com/Dav1dde/glad)
- GLFW 3.5.1 (https://github.com/glfw/glfw)
- GLM 1.0.3 (https://github.com/g-truc/glm)
- stb_image 2.30 (https://github.com/nothings/stb)
- Dear ImGui 1.92.2b (https://github.com/ocornut/imgui)
- Assimp 6.0.5 (https://github.com/assimp/assimp)
- Libcurl 8.21.0 (https://github.com/curl/curl)

## Licenses
The code for this program is licensed under the "Unlicense" license. See `LICENSE.txt` file for more information.

Every third party license is available in their corresponding vendor subfolders.