#include "OutputFormatter.hpp"
#include <nlohmann/json.hpp>
#include <sstream>

OutputFormatter::OutputFormatter(const std::vector<std::map<std::string, std::string>>& data,
                               const std::vector<std::string>& fieldNames)
    : data_(data), fieldNames_(fieldNames) {
}

void OutputFormatter::toCSV(std::ostream& out) {
    // Write header row
    for (size_t i = 0; i < fieldNames_.size(); ++i) {
        if (i > 0) {
            out << ",";
        }
        out << escapeCSVField(fieldNames_[i]);
    }
    out << "\n";

    // Write data rows
    for (const auto& record : data_) {
        for (size_t i = 0; i < fieldNames_.size(); ++i) {
            if (i > 0) {
                out << ",";
            }
            
            auto it = record.find(fieldNames_[i]);
            std::string value = (it != record.end()) ? it->second : "";
            out << escapeCSVField(value);
        }
        out << "\n";
    }
}

void OutputFormatter::toJSON(std::ostream& out) {
    nlohmann::json jsonArray = nlohmann::json::array();

    for (const auto& record : data_) {
        nlohmann::json jsonObject;
        
        for (const auto& fieldName : fieldNames_) {
            auto it = record.find(fieldName);
            std::string value = (it != record.end()) ? it->second : "";
            jsonObject[fieldName] = value;
        }
        
        jsonArray.push_back(jsonObject);
    }

    out << jsonArray.dump(2); // Pretty print with 2-space indentation
}

std::string OutputFormatter::escapeCSVField(const std::string& value) {
    // Check if the field contains comma, quote, or newline
    bool needsQuoting = value.find(',') != std::string::npos ||
                       value.find('"') != std::string::npos ||
                       value.find('\n') != std::string::npos ||
                       value.find('\r') != std::string::npos;

    if (!needsQuoting) {
        return value;
    }

    // Escape quotes by doubling them and wrap in quotes
    std::string escaped = "\"";
    for (char c : value) {
        if (c == '"') {
            escaped += "\"\"";
        } else {
            escaped += c;
        }
    }
    escaped += "\"";
    
    return escaped;
}