#include "Logger.hpp"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

// ANSI color codes
const std::string Logger::RESET = "\033[0m";
const std::string Logger::GREEN = "\033[32m";
const std::string Logger::YELLOW = "\033[33m";
const std::string Logger::RED = "\033[31m";

void Logger::info(const std::string& message) {
    if (shouldUseColors()) {
        std::cerr << GREEN << message << RESET << std::endl;
    } else {
        std::cerr << message << std::endl;
    }
}

void Logger::warn(const std::string& message) {
    if (shouldUseColors()) {
        std::cerr << YELLOW << message << RESET << std::endl;
    } else {
        std::cerr << "Warning: " << message << std::endl;
    }
}

void Logger::error(const std::string& message) {
    if (shouldUseColors()) {
        std::cerr << RED << message << RESET << std::endl;
    } else {
        std::cerr << "Error: " << message << std::endl;
    }
}

bool Logger::shouldUseColors() {
#ifdef _WIN32
    // Enable ANSI colors on Windows 10+
    HANDLE hOut = GetStdHandle(STD_ERROR_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return false;
    
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return false;
    
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    return SetConsoleMode(hOut, dwMode);
#else
    // Check if stderr is a terminal
    return isatty(STDERR_FILENO);
#endif
}