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
