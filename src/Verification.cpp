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


std::string generateVerificationHash() {
    std::string original_identifier = "Logan_Ant_Simulation_V1";
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(original_identifier.c_str()), original_identifier.size(), hash);

    std::ostringstream hash_str;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        hash_str << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    // Write the hash to a file
    std::ofstream file("verification.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open verification.txt for writing!" << std::endl;
        return ""; // Return an empty string to indicate failure
    }

    file << hash_str.str();
    file.close();

    return hash_str.str();
}

std::string getStoredVerificationHash() {
    std::ifstream file("verification.txt");
    std::string hash;

    if (file) {
        file >> hash;
    }
    else {
        // Generate a new hash and create verification.txt
        hash = generateVerificationHash();
        std::ofstream newFile("verification.txt");
        if (newFile.is_open()) {
            newFile << hash;
            newFile.close();
        }
    }
    return hash;
}
