#include "DicomReader.hpp"
#include "Logger.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

// DCMTK includes
#ifdef DCMTK_AVAILABLE
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#endif

// Simple SHA-256 implementation (for demonstration)
#include <cstring>

DicomReader::DicomReader(const std::string& filePath) 
    : m_filePath(filePath), m_isValid(false), m_dataset(nullptr) {
    m_isValid = loadFile();
}

DicomReader::~DicomReader() {
#ifdef DCMTK_AVAILABLE
    if (m_dataset) {
        delete static_cast<DcmDataset*>(m_dataset);
        m_dataset = nullptr;
    }
#endif
}

bool DicomReader::loadFile() {
#ifdef DCMTK_AVAILABLE
    try {
        DcmFileFormat fileFormat;
        OFCondition status = fileFormat.loadFile(m_filePath.c_str());
        
        if (status.good()) {
            DcmDataset* dataset = fileFormat.getDataset();
            if (dataset) {
                // Create a copy of the dataset
                m_dataset = new DcmDataset(*dataset);
                return true;
            }
        }
        
        Logger::error("Error loading DICOM file: " + std::string(status.text()));
        return false;
    } catch (const std::exception& e) {
        Logger::error("Exception loading DICOM file: " + std::string(e.what()));
        return false;
    } catch (...) {
        Logger::error("Unknown error loading DICOM file");
        return false;
    }
#else
    Logger::error("DCMTK not available. Cannot load DICOM file.");
    return false;
#endif
}

bool DicomReader::isValid() const {
    return m_isValid;
}

std::map<std::string, std::string> DicomReader::extractFields(
    const std::vector<std::string>& fields, 
    bool anonymize) {
    
    std::map<std::string, std::string> result;
    
    if (!m_isValid) {
        // Return N/A for all fields if file is invalid
        for (const auto& field : fields) {
            result[field] = "N/A";
        }
        return result;
    }
    
    for (const auto& field : fields) {
        std::string value = getFieldValue(field);
        
        // Apply anonymization if requested
        if (anonymize && field == "PatientID" && value != "N/A") {
            value = generateSHA256(value);
        }
        
        result[field] = value;
    }
    
    return result;
}

std::string DicomReader::getFieldValue(const std::string& fieldName) const {
#ifdef DCMTK_AVAILABLE
    if (!m_dataset) {
        return "N/A";
    }
    
    try {
        DcmDataset* dataset = static_cast<DcmDataset*>(m_dataset);
        auto tagPair = getTagForField(fieldName);
        DcmTag tag(tagPair.first, tagPair.second);
        
        OFString value;
        OFCondition status = dataset->findAndGetOFString(tag, value);
        
        if (status.good()) {
            return std::string(value.c_str());
        }
        
        return "N/A";
    } catch (const std::exception& e) {
        Logger::error("Error extracting field " + fieldName + ": " + e.what());
        return "N/A";
    } catch (...) {
        Logger::error("Unknown error extracting field " + fieldName);
        return "N/A";
    }
#else
    return "N/A";
#endif
}

std::pair<unsigned short, unsigned short> DicomReader::getTagForField(const std::string& fieldName) const {
    // Map common field names to DICOM tags
    if (fieldName == "PatientID") return {0x0010, 0x0020};
    if (fieldName == "PatientName") return {0x0010, 0x0010};
    if (fieldName == "StudyDate") return {0x0008, 0x0020};
    if (fieldName == "StudyTime") return {0x0008, 0x0030};
    if (fieldName == "Modality") return {0x0008, 0x0060};
    if (fieldName == "StudyDescription") return {0x0008, 0x1030};
    if (fieldName == "SeriesDescription") return {0x0008, 0x103E};
    if (fieldName == "InstitutionName") return {0x0008, 0x0080};
    if (fieldName == "ManufacturerModelName") return {0x0008, 0x1090};
    if (fieldName == "SliceThickness") return {0x0018, 0x0050};
    if (fieldName == "ImageType") return {0x0008, 0x0008};
    if (fieldName == "AccessionNumber") return {0x0008, 0x0050};
    
    // Default to unknown tag if not found
    return {0x0000, 0x0000};
}

std::string DicomReader::generateSHA256(const std::string& input) const {
    // Simple hash function for demonstration (not cryptographically secure)
    // In production, use a proper SHA-256 implementation like OpenSSL
    std::hash<std::string> hasher;
    size_t hashValue = hasher(input);
    
    std::stringstream ss;
    ss << std::hex << hashValue;
    return "HASH_" + ss.str();
}