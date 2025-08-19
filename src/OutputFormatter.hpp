#ifndef OUTPUTFORMATTER_HPP
#define OUTPUTFORMATTER_HPP

#include <vector>
#include <map>
#include <string>
#include <ostream>

class OutputFormatter {
public:
    /**
     * Constructor
     * @param data Collection of extracted metadata from multiple files
     * @param fieldNames List of field names (column order)
     */
    OutputFormatter(const std::vector<std::map<std::string, std::string>>& data,
                   const std::vector<std::string>& fieldNames);

    /**
     * Write data as CSV format
     * @param out Output stream to write CSV data
     */
    void toCSV(std::ostream& out);

    /**
     * Write data as JSON format
     * @param out Output stream to write JSON data
     */
    void toJSON(std::ostream& out);

private:
    std::vector<std::map<std::string, std::string>> data_;
    std::vector<std::string> fieldNames_;

    /**
     * Escape CSV field value (handle commas, quotes, newlines)
     * @param value The value to escape
     * @return Escaped value suitable for CSV
     */
    std::string escapeCSVField(const std::string& value);
};

#endif // OUTPUTFORMATTER_HPP