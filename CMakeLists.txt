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

# ---------------------------
# SFML via FetchContent
# ---------------------------
# This will download and build SFML directly within your project's build tree.
include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x # Or a specific commit/tag if you prefer.
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
	# NEW: Pass C++ standard and compiler flags explicitly to SFML's build
    CMAKE_ARGS
        -DCMAKE_CXX_STANDARD=17
        -DCMAKE_CXX_STANDARD_REQUIRED=ON
        -DCMAKE_CXX_EXTENSIONS=OFF
        # For Visual Studio 2022 (v143 toolset)
        -DCMAKE_GENERATOR_TOOLSET=v143
	)
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
    # Get just the filename part (e.g., "Vertiky.ttf" from "Fonts/Vertiky.ttf")
    get_filename_component(resource_name ${resource} NAME)

    # Destination directory is ALWAYS CMAKE_CURRENT_BINARY_DIR.
    # This ensures resources are in the project's top-level build folder (e.g., build/) on all platforms,
    # matching the common working directory for tests and debugger launches.
    set(destination_base_dir "${CMAKE_CURRENT_BINARY_DIR}") # This is always the project's build/ folder
    
    add_custom_command(
        TARGET copy_resources POST_BUILD
        # No 'make_directory' for 'resource_dir_part' as we don't want the subfolder (e.g., 'Fonts/') in the build/ folder
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/${resource}" # Source is original path (e.g., "Fonts/Vertiky.ttf")
            "${destination_base_dir}/${resource_name}" # Destination is base dir + just the filename (e.g., "build/Vertiky.ttf")
        COMMENT "Copying resource ${resource} to working directory"
    )
endforeach()

# ---------------------------
# Source Files
# ---------------------------

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.hpp)

add_executable(main ${SRC_FILES})

# ---------------------------
# Dependency Configurations
# ---------------------------

# OpenSSL Configuration (STILL VIA VCPKG, as it works)
find_package(OpenSSL REQUIRED)

# SFML Configuration (Now via FetchContent - Direct Linking)
# When FetchContent_MakeAvailable(SFML) is used, SFML is built into SFML_BINARY_DIR.
# We will manually add include directories and link libraries.

# Add SFML's include directories so the compiler can find headers like SFML/Graphics.hpp
target_include_directories(main PRIVATE
    "${SFML_SOURCE_DIR}/include"      # For original SFML source headers
    "${SFML_BINARY_DIR}/include"      # For any generated headers (less common for SFML but good practice)
)

# Add SFML's library directories to the linker search path
# SFML's compiled libraries will be in these locations relative to its build directory.
link_directories(
    "${SFML_BINARY_DIR}/lib"
    "${SFML_BINARY_DIR}/bin" # For DLLs on Windows (Release/Debug variants if multi-config)
)

# Link the executable against SFML's component libraries by their direct names.
# These names (e.g., sfml-graphics) correspond to the .lib files in the directories above.
target_link_libraries(main PRIVATE
    sfml-graphics   # For SFML/Graphics.hpp
    sfml-window     # For windowing functionality
    sfml-system     # For core system utilities
    # Add other SFML modules if the project uses them (e.g., sfml-audio, sfml-network)
    
    # OpenSSL components provided by find_package(OpenSSL) from vcpkg
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Make sure resources are copied before building the app
add_dependencies(main copy_resources)

# ---------------------------
# Platform-specific post-build steps
# ---------------------------

if(WIN32)
    # Copy OpenAL DLL on Windows (FIXED PATHING for FetchContent)
    # sfml_SOURCE_DIR is the variable typically set by FetchContent_MakeAvailable.
    set(SFML_ACTUAL_SOURCE_DIR "${sfml_SOURCE_DIR}")

    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy
            "${SFML_ACTUAL_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll"
            "$<TARGET_FILE_DIR:main>"
        VERBATIM
    )

    # Build Verification Hash on Windows
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Generating build verification hash on Windows..."
        COMMAND powershell -Command " \
            \$hash = (CertUtil -hashfile '$<TARGET_FILE:main>' SHA256)[1]; \
            \$hash = \$hash.Replace(' ',''); \
            Set-Content -Path '${CMAKE_CURRENT_SOURCE_DIR}/build_verification.txt' -Value \$hash \
        "
        COMMENT "Writing SHA256 hash of main executable to build_verification.txt"
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

    # Build Verification Hash on Unix (Linux/macOS)
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Generating build verification hash on Unix..."
        COMMAND sha256sum $<TARGET_FILE:main> | cut -d ' ' -f 1 > ${CMAKE_CURRENT_SOURCE_DIR}/build_verification.txt
        COMMENT "Writing SHA256 hash of main executable to build_verification.txt"
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

# Install the build verification file as well, if desired
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/build_verification.txt"
    DESTINATION bin # Or wherever you want to install it
    OPTIONAL # Don't fail if the file doesn't exist (e.g., if build fails before hash is generated)
)