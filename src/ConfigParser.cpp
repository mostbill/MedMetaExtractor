#include "ConfigParser.hpp"
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

void ConfigParser::loadConfig(const std::string& configFilePath) {
    try {
        std::ifstream configFile(configFilePath);
        if (!configFile.is_open()) {
            std::cerr << "Warning: Could not open config file '" << configFilePath 
                      << "'. Using default values." << std::endl;
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
                std::cerr << "Warning: Invalid output_format '" << format 
                          << "'. Using default 'csv'." << std::endl;
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
        
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Warning: JSON parsing error in config file '" << configFilePath 
                  << "': " << e.what() << ". Using default values." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Warning: Error reading config file '" << configFilePath 
                  << "': " << e.what() << ". Using default values." << std::endl;
    }
}