# Hunting Wumpus
'Hunting Wumpus' is a crossplatforms fan-made game adapted from the classic Hunt the Wumpus.<br>


This project is inspired by an exercise from PPP3 by Bjarne Stroustrup.


This project is licensed under the [MIT License](LICENSE.txt).<br>
You are free to use, modify, merge, distribute, and even use the source code for commercial purposes.<br>
When you distribute copies or derivative works, you must retain the original copyright notice and the MIT license text.<br>
The original author provides no warranty and assumes no liability for any issues arising from its use.<br>

## Game Background
There was a foul-smelling monster called the Wumpus that once plagued mankind.<br>
After a human manhunt, it was grievously wounded and fled into a cave.<br>
The king called for brave warriors to venture into the cave and slay it.

## How to play?
You have 5 arrows first. And you can only do two actions: 'move' and 'shoot'.
### command
Move action: input m[room_id]  (e.g. input m1 to move Room1 )<br>
Shoot action: input s[room_id-room_id-room_id] (e.g. input s1-2-3 or s1-2 or s1)  you can not input more than three room_id.<br>
If the room_id is not through, the arrow will shoot randomly, it will shoot yourself too!<br>
When you met bat, you will take to somewhere else.<br>
When you met hole, you will fall into the hole and die.<br>
When you met Wumpus, you will die too.<br>
You can shoot to kill Wumpus.


When Wumpus dead, you win the game.<br>
When you dead or your arrows are used before the Wumpus dead, you lose the game.<br>

## third-party library
This project use two libraries:<br>
graph-v3: <https://github.com/stdgraph/graph-v3><br>
pegtl: <https://github.com/taocpp/PEGTL>

## Contributing
### Requirements
C++23 compiler [GCC 13+, LLVM Clang 17+, MSVC 18.12+] (MSVC is better) <br>
Cmake 4.5+<br>
Ninja (recommended generator; installed by default on most Linux distros)
### Build
#### 1. Get the source code
```bash
git clone https://github.com/zkr4113-svg/Hunting_Wumpus.git
cd Hunting_Wumpus
```
PEGTL is vendored in `third_party/PEGTL`, so it does not need to be fetched.<br>
graph-v3 is downloaded by CMake (`FetchContent`) during the configure step, so the first configure requires network access and `git`.

#### 2. Configure the project
On Windows, run the following in a **Developer PowerShell / Developer Command Prompt for Visual Studio** so that `cl.exe` and `VSINSTALLDIR` are available (the presets rely on both):
```bat
cmake --preset x64-release
```
Available presets are `x64-debug`, `x64-release`, `x86-debug` and `x86-release`; they all use the Ninja generator and write to `out/build/<preset-name>`.

To configure manually (for example on Linux/macOS with GCC or Clang):
```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
```

#### 3. Build
```bash
cmake --build out/build/x64-release --target Hunting_Wumpus
```
When using a custom binary directory, build that directory instead:
```bash
cmake --build build --target Hunting_Wumpus
```
Building the `Hunting_Wumpus` target explicitly is recommended: `FetchContent` also brings in graph-v3's own test suites, which may fail to compile on some systems (for example MSVC on a non-UTF-8 code page treats C4819 as an error). Omit `--target Hunting_Wumpus` only if you also want to build those third-party targets.

#### 4. Run
```bat
.\out\build\x64-release\Hunting_Wumpus.exe
```
For a custom binary directory the executable is located at `build/Hunting_Wumpus.exe`.

#### Notes
- The project uses C++23 named modules (`FILE_SET CXX_MODULES`), so the compiler and CMake must support C++20 module scanning; MSVC is the recommended toolchain.
- On Windows, CMake presets and `cl.exe` require the environment set up by the Visual Studio Developer Command Prompt; running `cmake --build` from a plain terminal fails with errors such as `cannot open include file: 'vector'`.
- To build without network access, point `FetchContent` at a local graph-v3 clone: `cmake --preset x64-release -DFETCHCONTENT_SOURCE_DIR_GRAPH3=<path-to-graph-v3>`.
- Visual Studio can open the repository folder directly: the CMake presets are detected automatically, and the `Hunting_Wumpus` target can be built from Solution Explorer.

## Cmakelists.txt
```cmake
cmake_minimum_required (VERSION 4.5)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Hunting_Wumpus")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)
FetchContent_Declare(
  graph3
  GIT_REPOSITORY https://github.com/stdgraph/graph-v3.git
  GIT_TAG        main
  SYSTEM
)
FetchContent_MakeAvailable(graph3)

add_subdirectory(third_party/PEGTL)

# PEGTL is a third-party header-only library: mark its headers as external (system)
# so warnings raised inside its headers (e.g. C6294 in rep_min_max.hpp) are not
# reported for our target, which inherits /W4 /WX from graph3's INTERFACE options.
get_target_property(PEGTL_INCLUDE_DIRS pegtl INTERFACE_INCLUDE_DIRECTORIES)
set_target_properties(pegtl PROPERTIES
  INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${PEGTL_INCLUDE_DIRS}")

add_executable (Hunting_Wumpus "src/main.cpp"  "src/RoomGen.cpp"  "src/GameManager.cpp")

#supprot utf-8
target_compile_options(Hunting_Wumpus PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

target_sources(
Hunting_Wumpus
PRIVATE
FILE_SET CXX_MODULES
FILES
"src/vertex.cppm" "src/room_generator.cppm" "src/command.cppm" "src/game_manager.cppm")

target_link_libraries(Hunting_Wumpus PRIVATE graph::graph3)
target_link_libraries(Hunting_Wumpus PRIVATE taocpp::pegtl)

```