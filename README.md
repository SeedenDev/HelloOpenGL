# HelloOpenGL 
Learning OpenGL from several sources on the web, but highly following the tutorials from [LearnOpenGL.com]((https://learnopengl.com/)) as for the beginning.
My first project written in C++ too, so I'm also learning it here.

I intend to archive this repository as soon as I'm done with the TODOs to start from scratch another engine but this time with a better code architecture.
I've already learnt a lot from this project (and since I've started learning including other stuff like browsing opensource code) and there are some design problems 
I don't want to waste time on here to fix/refactor, it'll be better starting off anew to do cleaner code and consolidate my knowledge.

Disclaimer: as I kinda said before, I know this code is ahh, please be kind to me in your head if you read it lol.

## Features
- WIP

## TODOs
- More rendering stuff, but in the first place finish at least [LearnOpenGL.com](https://learnopengl.com/) tutorials.
  - Optimise models loading (very long atm)
          - Test on Linux: asm output and preprocessed output working? (with the new fixed cmakelists.txt)
- Change how asm output and preprocessed output is handled (custom target?)
- More CMake stuff (cross-compilation to more targets and w/ more options, more Windows setup choices)
- Once MinGW GCC/Clang build presets (with ninja) are there for native Windows: test that the -mwindows linker flag is working (windowed app = no console)
- Test on Windows using Visual Studio 2026
- Make the project works on Windows ARM64 and Linux Aarch64 eventually
- The README
- Check gitignored models' licenses
- Fix gitmodules

## Requirements
- CMake 3.21 or higher
- An OpenGL 4.5+ capable GPU driver (Warning: may evolve in the future in order to test 4.6 features)
- Linux x64 or Windows x64.
- Any C++20 compilers should work. Tested using:
  - **Linux**: GCC 16.1 or Clang 22.1
  - **Windows (to-be-tested)**: MSVC (using Visual Studio 2022 or 2026) *or [TODO] GCC/Clang (using Ninja generator)*
  - **Cross-compile Linux x64 -> Windows x64**: x86_64-w64-mingw32-g++ (using gcc 16.1) // BROKEN!!! bc of libcurl
  - **Cross-compile Windows x64 -> Linux x64**: WIP

### About macOS support
MacOS Intel and Apple Silicon support is not my priority right now. The current codebase uses a bunch of 4.3+ OpenGL functions and atm it would be very annoying for me
to deal with it by either removing some of them and using extensions or either heavily editing the codebase to handle older OpenGL versions with runtime checks to
enable or not some features. I will definitely need this knowledge in the future, for sure, but now is not the time I should focus on achieving a proper rendering engine first.
Nonetheless, feel free to do your own modifications if you want to try making it run on Mac (and make a PR :D).

## Setup / Build / Run / Cross-compile
Simply put: basic CMake project. Setup and build with the preconfigured presets. You also have cross-compilation toolchain files (WIP).
Or you can use your favorite IDE (tested with Microsoft Visual Studio 2022 on Windows x64 and CLion on Linux x64).

- CLion info: don't forget to set the WORKING_DIR in your run configuration. An example config file can be found in `cmake/ide-templates/clion/runConfigurations`.
  You should be able to use the `$ProjectFileDir$` variable to automatically target this root folder.

- Visual Studio 22 info, two ways of doing it: either way after the CMake project has been setup, you can just launch any preset with or without debugging as usual.
  - Simply open this root directory (where the `CMakeLists.txt` file is) with Visual Studio, it should automatically setup the CMake project for you with the right stuff in the Solution Explorer.
  - Or, you can also use `cmake --preset windows-x64-msvc-vs2022` (or vs2026, untested) or run `cmake/Setup.bat` and then open the .sln or .vcxproj file (inside the `build/` folder) with Visual Studio.
    However, you will need to enter the folder view ("Switch between solution and available views" button in the top bar of the Solution Explorer) in order to see all the right files and folders from the root directory.

Btw, don't forget to include the `assets` folder alongside the executable if you want to send it to your friends!

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