﻿# CMakeList.txt : CMake project for ArtilleryMaster_Build2, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (ArtilleryMaster_Build2 "ArtilleryMaster_Build2.cpp" "ArtilleryMaster_Build2.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ArtilleryMaster_Build2 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
