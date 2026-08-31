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
- Find a better Sponza model (like, proper file names for textures i've seen one like this, perhaps also as a .obj)
- Confirm Windows native compilation works using VS22
- Fix cross-compilation Linux -> Windows (libcurl issue)
- More CMake stuff (cross-compilation to more targets and w/ more options, more Windows setup choices)
- Make the project works on Mac (dependencies & CMake setup)
- The README
- Check gitignored models licence

## Requirements
- CMake 3.21 or higher
- An OpenGL 4.1+ capable GPU driver (Warning: may evolve in the future in order to test newer features)
- Linux x64 (tested) or Windows x64 (to be tested). MacOS Intel and Apple Silicon may need some adjustments on your own to be able to run.
- Any C++20 compilers should work. Tested using:
  - **Linux**: GCC 16.1 or CLang 22.1
  - **Windows (to-be-tested)**: MSVC (using Visual Studio 2022 or 2026) *or [TODO] GCC/CLang (using Ninja generator)*
  - **Cross-compile Linux x64 -> Windows x64**: x86_64-w64-mingw32-g++ (using gcc 16.1) // BROKEN!!! bc of libcurl
  - **Cross-compile Windows x64 -> Linux x64**: WIP

## Setup / Build / Run / Cross-compile
WIP but simply put for now: CMake (should work on CLion without much trouble as I'm using it).

More CLion info: don't forget to set the WORKING_DIR in your run configuration. An example config file can be found in `cmake/ide-templates/clion/runConfigurations`. You should be able to use the `$ProjectFileDir$` variable to automatically target this root folder.

## Third-party libraries
- GLAD 2.0.8 (https://github.com/Dav1dde/glad)
- GLFW 3.5.1 (https://github.com/glfw/glfw)
- GLM 1.0.3 (https://github.com/g-truc/glm)
- stb_image 2.30 (https://github.com/nothings/stb)
- Dear ImGui 1.92.2b (https://github.com/ocornut/imgui)
- Assimp 6.0.5 (https://github.com/assimp/assimp)
- Libcurl 8.21.0 (https://github.com/curl/curl)

## Licence
WIP

Every third party licence is available in their corresponding vendor subfolders.