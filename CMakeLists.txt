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


target_sources(
Hunting_Wumpus
PRIVATE
FILE_SET CXX_MODULES
FILES
"src/vertex.cppm" "src/room_generator.cppm" "src/command.cppm" "src/game_manager.cppm")

target_link_libraries(Hunting_Wumpus PRIVATE graph::graph3)
target_link_libraries(Hunting_Wumpus PRIVATE taocpp::pegtl)
