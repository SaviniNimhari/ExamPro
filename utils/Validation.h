#ifndef VALIDATION_H
#define VALIDATION_H

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <limits>
#include <regex>

class Validation {
public:
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }

    static std::string toUpperCase(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
            return static_cast<char>(std::toupper(c));
        });
        return str;
    }

    static bool isValidMarks(double marks) {
        return marks >= 0.0 && marks <= 100.0;
    }

    static bool isValidDate(const std::string& date) {
        // Simple regex check for YYYY-MM-DD
        const std::regex datePattern(R"(\d{4}-\d{2}-\d{2})");
        if (!std::regex_match(date, datePattern)) return false;

        int year = std::stoi(date.substr(0, 4));
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));

        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        return true;
    }

    static std::string getValidString(const std::string& prompt, bool allowSpaces = true, size_t minLen = 1) {
        std::string input;
        while (true) {
            std::cout << prompt;
            if (!std::getline(std::cin, input)) {
                std::cin.clear();
                continue;
            }
            input = trim(input);
            if (input.length() >= minLen) {
                if (!allowSpaces && input.find(' ') != std::string::npos) {
                    std::cout << " [Error] Spaces are not allowed here. Please try again.\n";
                    continue;
                }
                return input;
            }
            std::cout << " [Error] Input cannot be empty (min " << minLen << " chars). Please try again.\n";
        }
    }

    static int getValidInt(const std::string& prompt, int minVal = std::numeric_limits<int>::min(), int maxVal = std::numeric_limits<int>::max()) {
        int value;
        std::string line;
        while (true) {
            std::cout << prompt;
            if (!std::getline(std::cin, line)) {
                std::cin.clear();
                continue;
            }
            line = trim(line);
            if (line.empty()) {
                std::cout << " [Error] Input cannot be empty. Please enter an integer.\n";
                continue;
            }
            try {
                size_t pos = 0;
                value = std::stoi(line, &pos);
                if (pos == line.length()) {
                    if (value >= minVal && value <= maxVal) {
                        return value;
                    }
                    std::cout << " [Error] Value must be between " << minVal << " and " << maxVal << ".\n";
                    continue;
                }
            } catch (...) {
                // Conversion failed
            }
            std::cout << " [Error] Invalid number. Please enter a valid integer.\n";
        }
    }

    static double getValidDouble(const std::string& prompt, double minVal = 0.0, double maxVal = 100.0) {
        double value;
        std::string line;
        while (true) {
            std::cout << prompt;
            if (!std::getline(std::cin, line)) {
                std::cin.clear();
                continue;
            }
            line = trim(line);
            if (line.empty()) {
                std::cout << " [Error] Input cannot be empty. Please enter a number.\n";
                continue;
            }
            try {
                size_t pos = 0;
                value = std::stod(line, &pos);
                if (pos == line.length()) {
                    if (value >= minVal && value <= maxVal) {
                        return value;
                    }
                    std::cout << " [Error] Marks must be between " << minVal << " and " << maxVal << ".\n";
                    continue;
                }
            } catch (...) {
                // Conversion failed
            }
            std::cout << " [Error] Invalid number. Please enter a valid decimal/number.\n";
        }
    }

    static std::string getValidDate(const std::string& prompt) {
        while (true) {
            std::string date = getValidString(prompt, false, 10);
            if (isValidDate(date)) {
                return date;
            }
            std::cout << " [Error] Invalid date format! Please use YYYY-MM-DD (e.g., 2026-06-15).\n";
        }
    }

    static bool confirmAction(const std::string& prompt) {
        while (true) {
            std::cout << prompt << " (y/n): ";
            std::string input;
            if (std::getline(std::cin, input)) {
                input = trim(input);
                if (input == "y" || input == "Y") return true;
                if (input == "n" || input == "N") return false;
            }
            std::cout << " [Error] Please enter 'y' for yes or 'n' for no.\n";
        }
    }

    static void pauseConsole() {
        std::cout << "\nPress Enter to continue...";
        std::string dummy;
        std::getline(std::cin, dummy);
    }
};

#endif // VALIDATION_H
