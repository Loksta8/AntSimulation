# AntSimulation - An SFML simulation of an ant colony.
# Copyright (C) 2025 Logan Herrera <jherre36@live.nmhu.edu>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.28)

project(AntSimulation LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# ---------------------------
# Copy Resource Files to Output
# ---------------------------

# Define all resource files that the executable needs at runtime
set(RESOURCE_FILES
    "Fonts/Vertiky.ttf"
    "verification.txt"
)

# Custom target to copy all resources to the output directory after a build
add_custom_target(copy_resources ALL)

foreach(resource ${RESOURCE_FILES})
    # Use generator expressions for cross-platform compatibility
    # Get just the filename part
    get_filename_component(filename ${resource} NAME)
    
    if(CMAKE_CONFIGURATION_TYPES)
        # Multi-config generators (Visual Studio, Xcode)
        # Copies to bin/Debug/ or bin/Release/
        add_custom_command(
            TARGET copy_resources POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/${resource}
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/${filename}
            COMMENT "Copying resource ${resource} to output directory"
        )
    else()
        # Single-config generators (Make, Ninja)
        # Copies to bin/
        add_custom_command(
            TARGET copy_resources POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/${resource}
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${filename}
            COMMENT "Copying resource ${resource} to output directory"
        )
    endif()
endforeach()

# ---------------------------
# Source Files
# ---------------------------

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.hpp)

add_executable(main ${SRC_FILES})

# ---------------------------
# OpenSSL Configuration
# ---------------------------

# Platform-specific OpenSSL configuration
if(WIN32)
    set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    set(OPENSSL_LIBRARIES "${OPENSSL_ROOT_DIR}/lib")
endif()

find_package(OpenSSL REQUIRED)
target_link_libraries(main PRIVATE sfml-graphics OpenSSL::SSL OpenSSL::Crypto)

# Make sure fonts are copied before building the app
add_dependencies(main copy_resources)

# ---------------------------
# Platform-specific post-build steps
# ---------------------------

if(WIN32)
    # Copy OpenAL DLL on Windows
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll
            $<TARGET_FILE_DIR:main>
        VERBATIM
    )
elseif(UNIX AND NOT APPLE)
    # Linux-specific configurations
    # Find threads package (often required on Linux)
    find_package(Threads REQUIRED)
    target_link_libraries(main PRIVATE Threads::Threads)
    
    # Add RPATH for better library handling
    set_target_properties(main PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endif()

# ---------------------------
# Installation rules (optional)
# ---------------------------

install(TARGETS main
    RUNTIME DESTINATION bin
)

install(FILES ${RESOURCE_FILES}
    DESTINATION bin
)
