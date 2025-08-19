#ifndef DICOMREADER_HPP
#define DICOMREADER_HPP

#include <string>
#include <vector>
#include <map>

class DicomReader {
public:
    /**
     * Constructor that accepts a DICOM file path
     * @param filePath Path to the DICOM file
     */
    explicit DicomReader(const std::string& filePath);

    /**
     * Destructor
     */
    ~DicomReader();

    /**
     * Extract specified DICOM fields from the file
     * @param fields Vector of field names to extract
     * @param anonymize If true, anonymize sensitive fields like PatientID
     * @return Map of field name to field value, "N/A" if field is missing
     */
    std::map<std::string, std::string> extractFields(
        const std::vector<std::string>& fields, 
        bool anonymize = false
    );

    /**
     * Check if the DICOM file was loaded successfully
     * @return true if file is valid and readable
     */
    bool isValid() const;

private:
    std::string m_filePath;
    bool m_isValid;
    void* m_dataset; // Forward declaration for DCMTK dataset

    /**
     * Load the DICOM file and initialize the dataset
     * @return true if successful
     */
    bool loadFile();

    /**
     * Get DICOM tag value by field name
     * @param fieldName Name of the DICOM field
     * @return Field value or "N/A" if not found
     */
    std::string getFieldValue(const std::string& fieldName) const;

    /**
     * Generate SHA-256 hash for anonymization
     * @param input Input string to hash
     * @return SHA-256 hash as hex string
     */
    std::string generateSHA256(const std::string& input) const;

    /**
     * Map field names to DICOM tag identifiers
     * @param fieldName Human-readable field name
     * @return DICOM tag identifier
     */
    std::pair<unsigned short, unsigned short> getTagForField(const std::string& fieldName) const;
};

#endif // DICOMREADER_HPP