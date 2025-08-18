#include <iostream>
#include "ConfigParser.hpp"

int main() {
    std::cout << "MedMetaExtractor initialized" << std::endl;
    
    // Test ConfigParser with sample config
    try {
        ConfigParser config("config.json");
        
        std::cout << "\nConfiguration loaded:" << std::endl;
        std::cout << "Output format: " << config.getOutputFormat() << std::endl;
        std::cout << "Anonymize: " << (config.getAnonymize() ? "true" : "false") << std::endl;
        
        std::cout << "Fields: ";
        const auto& fields = config.getFields();
        if (fields.empty()) {
            std::cout << "(none)";
        } else {
            for (size_t i = 0; i < fields.size(); ++i) {
                std::cout << fields[i];
                if (i < fields.size() - 1) std::cout << ", ";
            }
        }
        std::cout << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}