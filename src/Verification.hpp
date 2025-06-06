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

#ifndef VERIFICATION_HPP
#define VERIFICATION_HPP

#include <string>
#include <vector>

/**
 * @class ValidationLogger
 * @brief Handles logging of verification operations with log rotation
 */
class ValidationLogger {
public:
    /**
     * @brief Get the singleton instance of ValidationLogger
     * @return Reference to the ValidationLogger instance
     */
    static ValidationLogger& getInstance();
    
    /**
     * @brief Log a validation event
     * @param filename The name of the file being validated
     * @param hash The hash value
     * @param isValid Whether the validation was successful
     */
    void logValidation(const std::string& filename, const std::string& hash, bool isValid);
    
    // Delete copy constructor and assignment operator
    ValidationLogger(const ValidationLogger&) = delete;
    ValidationLogger& operator=(const ValidationLogger&) = delete;

private:
    // Private constructor for singleton pattern
    ValidationLogger();
    
    // Check if log rotation is needed and perform it if necessary
    void rotateLogIfNeeded();
    
    const std::string logFilename = "validation_history_log.txt";
    const size_t maxLogSizeBytes = 5 * 1024 * 1024; // 5 MB max size
    const int maxBackupFiles = 5; // Keep 5 backup files
};

/**
 * @class Verification
 * @brief Handles verification operations for the application
 * 
 * This class works with the git hook system that automatically updates
 * the verification.txt file with a new hash on each commit.
 */
class Verification {
public:
    /**
     * @brief Get the singleton instance of Verification
     * @return Reference to the Verification instance
     */
    static Verification& getInstance();
    
    /**
     * @brief Get the stored verification hash from verification.txt
     * @return The stored hash or "UNKNOWN" if file is missing
     * 
     * This hash is automatically updated by git hooks on each commit.
     */
    std::string getStoredVerificationHash();
    
    /**
     * @brief Verify the application integrity using the stored hash
     * @return True if verification succeeds, false otherwise
     */
    bool verifyApplicationIntegrity();
    
    // Delete copy constructor and assignment operator
    Verification(const Verification&) = delete;
    Verification& operator=(const Verification&) = delete;

private:
    // Private constructor for singleton pattern
    Verification();
    
    // Reference to the validation logger
    ValidationLogger& logger;
    
    // The verification file path
    const std::string verificationFile = "verification.txt";
};



#endif
