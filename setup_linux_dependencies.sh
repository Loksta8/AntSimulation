#!/bin/bash
set -e # Exit immediately if a command exits with a non-zero status.

# AntSimulation - An SFML simulation of an ant colony.
# Copyright (C) 2025 Logan Herrera <jherre36@live.nmhu.edu>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the
# Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

echo "This script will attempt to install dependencies for AntSimulation."
echo "It supports Debian/Ubuntu (apt) and Fedora (dnf)."

if command -v apt-get &> /dev/null; then
    echo "Detected apt (Debian/Ubuntu-based system)."
    sudo apt-get update
    sudo apt-get install -y build-essential cmake git \
        libgl1-mesa-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev \
        libopenal-dev libflac-dev libvorbis-dev libfreetype6-dev \
        libsfml-dev \
        mesa-common-dev # Added for comprehensive X11/OpenGL dev headers
elif command -v dnf &> /dev/null; then
    echo "Detected dnf (Fedora-based system)."
    sudo dnf install -y gcc-c++ cmake git \
        mesa-libGL-devel libXrandr-devel libXcursor-devel libXi-devel \
        systemd-devel openal-soft-devel libflac-devel \
        libvorbis-devel freetype-devel \
        SFML-devel

else
    echo "Could not detect a supported package manager (apt or dnf)."
    echo "Please install dependencies manually as per the README.md."
    echo "Essential dependencies usually include: a C++ compiler, CMake, Git, and SFML development files (or its individual dependencies like OpenGL, OpenAL, FreeType, X11 libs, etc.)."
    exit 1
fi

if [ $? -eq 0 ]; then
    echo "Dependency installation attempted successfully."
    echo "Please check for any specific error messages above if the build still fails."
else
    echo "Dependency installation failed or was aborted. Please check the output for errors."
fi