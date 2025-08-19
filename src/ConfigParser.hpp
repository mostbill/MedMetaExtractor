#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class ConfigParser {
public:
    // Constructor that accepts path to JSON config file
    explicit ConfigParser(const std::string& configFilePath);
    
    // Getter methods for configuration settings
    std::string getOutputFormat() const;
    std::vector<std::string> getFields() const;
    bool getAnonymize() const;
    std::string getOutputFile() const;
    
private:
    // Configuration values with defaults
    std::string outputFormat_ = "csv";
    std::vector<std::string> fields_;
    bool anonymize_ = false;
    std::string outputFile_ = ""; // Empty means stdout
    
    // Helper method to load and parse JSON config
    void loadConfig(const std::string& configFilePath);
};