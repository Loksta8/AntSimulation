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
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

// ValidationLogger implementation
ValidationLogger& ValidationLogger::getInstance() {
    static ValidationLogger instance;
    return instance;
}

ValidationLogger::ValidationLogger() {
    // Constructor is private for singleton pattern
}

void ValidationLogger::rotateLogIfNeeded() {
    // Check if the log file exists and exceeds max size
    if (fs::exists(logFilename) && fs::file_size(logFilename) > maxLogSizeBytes) {
        // Get list of existing backup files
        std::vector<std::string> backupFiles;
        for (int i = 1; i <= maxBackupFiles; i++) {
            std::string backupName = logFilename + "." + std::to_string(i);
            if (fs::exists(backupName)) {
                backupFiles.push_back(backupName);
            }
        }
        
        // Sort backup files by number (descending)
        std::sort(backupFiles.begin(), backupFiles.end(), std::greater<std::string>());
        
        // Remove oldest backup if we have reached max number
        if (backupFiles.size() >= maxBackupFiles && !backupFiles.empty()) {
            fs::remove(backupFiles.back());
        }
        
        // Shift all other backups
        for (size_t i = 0; i < backupFiles.size(); i++) {
            std::string currentName = backupFiles[i];
            size_t dotPos = currentName.find_last_of('.');
            if (dotPos != std::string::npos) {
                int currentNum = std::stoi(currentName.substr(dotPos + 1));
                std::string newName = logFilename + "." + std::to_string(currentNum + 1);
                fs::rename(currentName, newName);
            }
        }
        
        // Rename current log to .1
        fs::rename(logFilename, logFilename + ".1");
    }
}

void ValidationLogger::logValidation(const std::string& filename, const std::string& hash, bool isValid) {
    // Check if rotation is needed
    rotateLogIfNeeded();
    
    // Open the log file in append mode
    std::ofstream logFile(logFilename, std::ios::app);
    
    if (!logFile.is_open()) {
        std::cerr << "Error: Could not open log file: " << logFilename << std::endl;
        return;
    }
    
    // Get current time
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    
    // Format timestamp
    std::ostringstream timestamp;
    timestamp << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    
    // Write log entry
    logFile << timestamp.str() << " | "
            << "File: " << filename << " | "
            << "Hash: " << hash << " | "
            << "Status: " << (isValid ? "VALID" : "INVALID") << std::endl;
    
    logFile.close();
}

// Verification implementation
Verification& Verification::getInstance() {
    static Verification instance;
    return instance;
}

Verification::Verification() : logger(ValidationLogger::getInstance()) {
    // Constructor is private for singleton pattern
}

std::string Verification::getStoredVerificationHash() {
    std::ifstream file(verificationFile);
    std::string hash;
    bool isValid = false;
    
    if (file) {
        file >> hash;
        isValid = true;
    }
    else {
        std::cerr << "Warning: verification.txt missing! Commit required to update hash." << std::endl;
        hash = "UNKNOWN";
        isValid = false;
    }
    
    // Log the validation result
    logger.logValidation(verificationFile, hash, isValid);
    
    return hash;
}

bool Verification::verifyApplicationIntegrity() {
    std::string hash = getStoredVerificationHash();
    
    // Since the hash is updated by git hooks, we just need to check if it exists
    // and is not "UNKNOWN"
    bool isValid = (hash != "UNKNOWN");
    
    // Log the overall application integrity check
    logger.logValidation("Application Integrity", hash, isValid);
    
    return isValid;
}

