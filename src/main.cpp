#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <fstream>
#include "ConfigParser.hpp"
#include "DicomReader.hpp"
#include "OutputFormatter.hpp"
#include "Logger.hpp"

void printUsage(const std::string& programName) {
    Logger::info("Usage: " + programName + " --input <dicom_file_or_directory> --config <config_file>");
    Logger::info("  --input   Path to DICOM file or directory containing .dcm files");
    Logger::info("  --config  Path to JSON configuration file");
}

bool hasDicomExtension(const std::string& filename) {
    if (filename.length() < 4) return false;
    std::string ext = filename.substr(filename.length() - 4);
    return ext == ".dcm" || ext == ".DCM";
}

std::vector<std::string> findDicomFiles(const std::string& inputPath) {
    std::vector<std::string> dicomFiles;
    
    if (std::filesystem::is_regular_file(inputPath)) {
        // Single file - check if it's a .dcm file
        if (hasDicomExtension(inputPath)) {
            dicomFiles.push_back(inputPath);
        } else {
            // Assume it's a DICOM file even without .dcm extension
            dicomFiles.push_back(inputPath);
        }
    } else if (std::filesystem::is_directory(inputPath)) {
        // Directory - recursively find all .dcm files
        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(inputPath)) {
                if (entry.is_regular_file()) {
                    const auto& path = entry.path().string();
                    if (hasDicomExtension(path)) {
                        dicomFiles.push_back(path);
                    }
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            Logger::error("Error accessing directory: " + std::string(e.what()));
        }
        
        // Sort files for consistent output order
        std::sort(dicomFiles.begin(), dicomFiles.end());
    }
    
    return dicomFiles;
}

int main(int argc, char* argv[]) {
    std::string inputFile;
    std::string configFile;
    
    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--input" && i + 1 < argc) {
            inputFile = argv[++i];
        } else if (arg == "--config" && i + 1 < argc) {
            configFile = argv[++i];
        } else if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        } else {
            Logger::error("Unknown argument '" + arg + "'");
            printUsage(argv[0]);
            return 1;
        }
    }
    
    // Validate required arguments
    if (inputFile.empty() || configFile.empty()) {
        Logger::error("Both --input and --config arguments are required");
        printUsage(argv[0]);
        return 1;
    }
    
    // Check if files exist
    if (!std::filesystem::exists(inputFile)) {
        Logger::error("Input file does not exist: " + inputFile);
        return 1;
    }

    if (!std::filesystem::exists(configFile)) {
        Logger::error("Config file does not exist: " + configFile);
        return 1;
    }
    
    try {
        // Load configuration
        ConfigParser config(configFile);
        
        // Find all DICOM files to process
        auto dicomFiles = findDicomFiles(inputFile);
        
        if (dicomFiles.empty()) {
            Logger::error("No DICOM files found in: " + inputFile);
            return 1;
        }
        
        Logger::info("Found " + std::to_string(dicomFiles.size()) + " DICOM file(s) to process");
        
        // Process each DICOM file and collect metadata
        std::vector<std::map<std::string, std::string>> allResults;
        int successCount = 0;
        int failureCount = 0;
        
        for (const auto& dicomFile : dicomFiles) {
            try {
                DicomReader reader(dicomFile);
                
                if (reader.isValid()) {
                    // Extract requested fields
                    auto extractedData = reader.extractFields(config.getFields(), config.getAnonymize());
                    
                    // Add filename to the extracted data for reference
                    extractedData["FileName"] = std::filesystem::path(dicomFile).filename().string();
                    
                    allResults.push_back(extractedData);
                    successCount++;
                } else {
                    Logger::warn("Failed to load DICOM file: " + dicomFile);
                    failureCount++;
                }
            } catch (const std::exception& e) {
                Logger::warn("Error processing " + dicomFile + ": " + e.what());
                failureCount++;
            }
        }
        
        if (allResults.empty()) {
            Logger::error("No DICOM files could be processed successfully");
            return 1;
        }
        
        std::string statusMsg = "Successfully processed " + std::to_string(successCount) + " file(s)";
        if (failureCount > 0) {
            statusMsg += ", failed to process " + std::to_string(failureCount) + " file(s)";
        }
        Logger::info(statusMsg);
        
        // Create field list including FileName
        auto fieldList = config.getFields();
        fieldList.insert(fieldList.begin(), "FileName");
        
        // Format and output all results
        OutputFormatter formatter(allResults, fieldList);
        
        // Determine output destination
        std::string outputFile = config.getOutputFile();
        if (!outputFile.empty()) {
            // Write to file
            std::ofstream outFile(outputFile);
            if (!outFile.is_open()) {
                Logger::error("Cannot open output file: " + outputFile);
                return 1;
            }
            
            if (config.getOutputFormat() == "csv") {
                formatter.toCSV(outFile);
            } else if (config.getOutputFormat() == "json") {
                formatter.toJSON(outFile);
            } else {
                Logger::error("Unsupported output format: " + config.getOutputFormat());
                return 1;
            }
            
            Logger::info("Results written to: " + outputFile);
        } else {
            // Write to stdout
            if (config.getOutputFormat() == "csv") {
                formatter.toCSV(std::cout);
            } else if (config.getOutputFormat() == "json") {
                formatter.toJSON(std::cout);
            } else {
                Logger::error("Unsupported output format: " + config.getOutputFormat());
                return 1;
            }
        }
        
    } catch (const std::exception& e) {
        Logger::error(e.what());
        return 1;
    }
    
    return 0;
}