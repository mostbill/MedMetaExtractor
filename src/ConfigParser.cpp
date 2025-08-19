#include "ConfigParser.hpp"
#include "Logger.hpp"
#include <fstream>
#include <iostream>

ConfigParser::ConfigParser(const std::string& configFilePath) {
    loadConfig(configFilePath);
}

std::string ConfigParser::getOutputFormat() const {
    return outputFormat_;
}

std::vector<std::string> ConfigParser::getFields() const {
    return fields_;
}

bool ConfigParser::getAnonymize() const {
    return anonymize_;
}

std::string ConfigParser::getOutputFile() const {
    return outputFile_;
}

void ConfigParser::loadConfig(const std::string& configFilePath) {
    try {
        std::ifstream configFile(configFilePath);
        if (!configFile.is_open()) {
            Logger::warn("Could not open config file '" + configFilePath + "'. Using default values.");
            return;
        }
        
        nlohmann::json config;
        configFile >> config;
        
        // Parse output_format with validation and default fallback
        if (config.contains("output_format") && config["output_format"].is_string()) {
            std::string format = config["output_format"];
            if (format == "csv" || format == "json") {
                outputFormat_ = format;
            } else {
                Logger::warn("Invalid output_format '" + format + "'. Using default 'csv'.");
            }
        }
        
        // Parse fields array with default fallback
        if (config.contains("fields") && config["fields"].is_array()) {
            fields_.clear();
            for (const auto& field : config["fields"]) {
                if (field.is_string()) {
                    fields_.push_back(field);
                }
            }
        }
        
        // Parse anonymize boolean with default fallback
        if (config.contains("anonymize") && config["anonymize"].is_boolean()) {
            anonymize_ = config["anonymize"];
        }
        
        // Parse output_file string (optional)
        if (config.contains("output_file") && config["output_file"].is_string()) {
            outputFile_ = config["output_file"];
        }
        
    } catch (const nlohmann::json::exception& e) {
        Logger::warn("JSON parsing error in config file '" + configFilePath + "': " + e.what() + ". Using default values.");
    } catch (const std::exception& e) {
        Logger::warn("Error reading config file '" + configFilePath + "': " + e.what() + ". Using default values.");
    }
}