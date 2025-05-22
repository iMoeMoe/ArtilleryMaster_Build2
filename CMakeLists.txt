cmake_minimum_required(VERSION 3.10) # Specifies the minimum CMake version
project(ArtilleryMaster_Build2 LANGUAGES CXX) # Defines the project name and language

# Set the C++ standard (e.g., C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Optional: disable compiler-specific extensions

# --- SFML ---
# Find the SFML package.
# This requires SFML to be installed in a location CMake can find.
# You might need to set CMAKE_PREFIX_PATH or SFML_DIR if it's not found automatically.
# Example: cmake -D SFML_DIR=/path/to/sfml/lib/cmake/SFML ..
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
# SFML_INCLUDE_DIRS and SFML_LIBRARIES will be populated by find_package

# --- Dear ImGui Submodule ---
# Add the imgui submodule directory. This will process its CMakeLists.txt.
# This assumes the 'imgui' directory is a submodule in your project root.
# It should create a target, typically named 'imgui'.
add_subdirectory(imgui)

# --- ImGui-SFML Submodule ---
# Add the imgui-sfml submodule directory. This will process its CMakeLists.txt.
# This assumes the 'imgui-sfml' directory is a submodule in your project root.
# It should create a target, typically named 'ImGui-SFML'.
add_subdirectory(imgui-sfml)

# --- Your Executable ---
# Define your main executable and list its source files.
# This assumes ArtilleryMaster_Build2.cpp is in the same directory as this CMakeLists.txt.
add_executable(ArtilleryMaster_Build2 ArtilleryMaster_Build2.cpp)

# --- Include Directories ---
# Specify include directories needed for your executable.
target_include_directories(ArtilleryMaster_Build2 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} # Include the current directory (optional, good practice)
    ${SFML_INCLUDE_DIRS}        # For SFML headers (e.g., <SFML/Graphics.hpp>)
    # The following might be automatically handled by linking against the imgui and ImGui-SFML
    # targets if their CMakeLists.txt are set up to export include directories (INTERFACE_INCLUDE_DIRECTORIES).
    # However, explicitly adding them can be safer or needed for older/different setups.
    imgui                       # Allows #include "imgui.h" (searches in 'imgui' folder)
    imgui-sfml                  # Allows #include "imgui-SFML.h" (searches in 'imgui-sfml' folder)
)

# --- Link Libraries ---
# Link your executable against the libraries it depends on.
target_link_libraries(ArtilleryMaster_Build2 PRIVATE
    # SFML libraries (e.g., sfml-graphics, sfml-window, sfml-system)
    # The specific target names like sfml-graphics are often wrapped by ${SFML_LIBRARIES}
    ${SFML_LIBRARIES}

    # ImGui library target (usually 'imgui', defined in imgui/CMakeLists.txt)
    imgui

    # ImGui-SFML library target (usually 'ImGui-SFML', defined in imgui-sfml/CMakeLists.txt)
    ImGui-SFML
)

# --- Optional: For Visual Studio, set this project as the default startup project ---
# (This usually happens automatically if there's only one executable)
# if(MSVC)
#     set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ArtilleryMaster_Build2)
# endif()