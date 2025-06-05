// AntSimulation - An SFML simulation of an ant colony.
// Copyright (C) 2025 Logan Herrera <jherre36@live.nmhu.edu>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "Verification.hpp"
#include <iostream>
#include <string>
#include <openssl/sha.h>  // Requires OpenSSL for hashing
#include <sstream>  // For std::ostringstream
#include <iomanip>  // For std::setw and std::setfill
#include <fstream>


std::string getStoredVerificationHash() {
    std::ifstream file("verification.txt");
    std::string hash;
    if (file) {
        file >> hash;
    }
    else {
        std::cerr << "Warning: verification.txt missing! Commit required to update hash." << std::endl;
        hash = "UNKNOWN";
    }
    return hash;
}