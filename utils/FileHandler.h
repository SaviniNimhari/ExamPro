#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class FileHandler {
public:
    static bool ensureDirectory(const std::string& dirPath) {
        try {
            if (!fs::exists(dirPath)) {
                return fs::create_directories(dirPath);
            }
            return true;
        } catch (const std::exception& e) {
            std::cerr << " [FileHandler Error] Could not create directory: " << e.what() << "\n";
            return false;
        }
    }

    static bool fileExists(const std::string& filePath) {
        return fs::exists(filePath);
    }

    static std::vector<std::string> readAllLines(const std::string& filePath) {
        std::vector<std::string> lines;
        std::ifstream file(filePath);
        if (!file.is_open()) {
            return lines;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Trim carriage return if file has CRLF
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            // Skip purely empty lines
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        file.close();
        return lines;
    }

    static bool writeAllLines(const std::string& filePath, const std::vector<std::string>& lines) {
        try {
            fs::path p(filePath);
            if (p.has_parent_path()) {
                ensureDirectory(p.parent_path().string());
            }
            std::ofstream file(filePath, std::ios::trunc);
            if (!file.is_open()) {
                return false;
            }
            for (const auto& line : lines) {
                file << line << "\n";
            }
            file.close();
            return true;
        } catch (const std::exception& e) {
            std::cerr << " [FileHandler Error] Could not write file: " << e.what() << "\n";
            return false;
        }
    }

    static bool appendLine(const std::string& filePath, const std::string& line) {
        try {
            fs::path p(filePath);
            if (p.has_parent_path()) {
                ensureDirectory(p.parent_path().string());
            }
            std::ofstream file(filePath, std::ios::app);
            if (!file.is_open()) {
                return false;
            }
            file << line << "\n";
            file.close();
            return true;
        } catch (const std::exception& e) {
            std::cerr << " [FileHandler Error] Could not append to file: " << e.what() << "\n";
            return false;
        }
    }
};

#endif // FILEHANDLER_H
