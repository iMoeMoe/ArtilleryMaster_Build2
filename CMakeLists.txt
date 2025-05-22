# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("ArtilleryMaster_Build2")

# Remove duplicate or unnecessary include directories and ensure correct order
include_directories(
    ${CMAKE_SOURCE_DIR}/item/SFML/include
    ${CMAKE_SOURCE_DIR}/item/imgui
    ${CMAKE_SOURCE_DIR}/item/imgui/imgui-sfml
    ${CMAKE_SOURCE_DIR}/item/imgui/libraries/include
)

# Ensure the linker can find the SFML and ImGui-SFML libraries
link_directories(
    ${CMAKE_SOURCE_DIR}/item/SFML/lib
    ${CMAKE_SOURCE_DIR}/item/imgui/libraries/lib
)

add_executable(ArtilleryMaster_Build2
    ArtilleryMaster_Build2/ArtilleryMaster_Build2.cpp
    # Add other source files here if needed
)

# Link against SFML and ImGui-SFML libraries (adjust names if needed)
target_link_libraries(ArtilleryMaster_Build2
    sfml-graphics
    sfml-window
    sfml-system
    ImGui-SFML
)

# Remove or correct the previous target_include_directories to avoid confusion
# and ensure all includes are set above
# target_include_directories(ArtilleryMaster_Build2 PRIVATE
#     ${CMAKE_SOURCE_DIR}/item/imgui-sfml/include
# )
cmake_minimum_required(VERSION 3.10)
project(ArtilleryMaster_Build2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Good practice

set(SFML_DIR "/path/to/sfml")

# --- SFML ---
# Ensure SFML is installed and findable by CMake.
# If it's not in a standard path, you might need to tell CMake where it is, e.g.,
# by setting CMAKE_PREFIX_PATH when you run cmake:
# cmake -DCMAKE_PREFIX_PATH=/actual/path/to/sfml ..

# --- ImGui ---
# This will build the imgui library from the submodule
# and create a target named 'imgui' (or 'ImGui' depending on its CMakeLists)
add_subdirectory(imgui)

# --- ImGui-SFML ---
# This will build the imgui-sfml library from the submodule
# and create a target, likely named 'ImGui-SFML' or 'imgui-sfml'
add_subdirectory(imgui-sfml) # <<< ADD THIS LINE

# --- Your Executable ---
add_executable(ArtilleryMaster_Build2 ArtilleryMaster_Build2.cpp)

# --- Include Directories ---
target_include_directories(ArtilleryMaster_Build2 PUBLIC
    ${SFML_INCLUDE_DIRS}  # Provided by find_package(SFML)
    imgui                 # For #include "imgui.h" (finds imgui/imgui.h)
    imgui-sfml            # For #include "imgui-SFML.h" (finds imgui-sfml/imgui-SFML.h)
                          # Note: Linking the ImGui-SFML target below might make this redundant
                          # as it often exports its include directories. But being explicit doesn't hurt.
)

# --- Link Libraries ---
target_link_libraries(ArtilleryMaster_Build2 PRIVATE
    # SFML libraries (e.g., sfml-graphics, sfml-window, sfml-system)
    ${SFML_LIBRARIES}

    # ImGui library target (usually 'imgui' or 'ImGui' - check imgui/CMakeLists.txt if unsure)
    imgui # Assuming the target defined in imgui/CMakeLists.txt is 'imgui'

    # ImGui-SFML library target (usually 'ImGui-SFML' - check imgui-sfml/CMakeLists.txt if unsure)
    ImGui-SFML # <<< ADD THIS LINE (or 'imgui-sfml' if that's the target name)
               # This is important. The official imgui-sfml CMakeLists.txt creates a target named 'ImGui-SFML'.
)

# If you are using the ImGui-SFML from https://github.com/eliasdaler/imgui-sfml
# its CMakeLists.txt should create an INTERFACE library called 'ImGui-SFML'.
# Linking against an INTERFACE library automatically handles include directories and dependencies.
# In that case, explicitly adding 'imgui-sfml' to target_include_directories might not be strictly
# necessary if 'ImGui-SFML' correctly propagates its include directory, but it's safer.
# The target 'imgui' from the official Dear ImGui submodule also correctly propagates its include dir.