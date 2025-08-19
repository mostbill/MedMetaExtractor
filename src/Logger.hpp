#pragma once

#include <string>

class Logger {
public:
    // Static methods for colored logging
    static void info(const std::string& message);
    static void warn(const std::string& message);
    static void error(const std::string& message);
    
private:
    // ANSI color codes
    static const std::string RESET;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string RED;
    
    // Helper method to check if colors should be used
    static bool shouldUseColors();
};