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

#ifndef RANDOM_UTILS_HPP
#define RANDOM_UTILS_HPP

#include <random>

class RandomUtils {
public:
    static std::mt19937& getGenerator() {
        static std::random_device rd;
        static std::mt19937 gen(rd()); // Ensures one instance across all files
        return gen;
    }
};

#endif // RANDOM_UTILS_HPP