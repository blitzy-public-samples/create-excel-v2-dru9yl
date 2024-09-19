#include "FileIO.h"
#include "DataStore.h"
#include <fstream>
#include <stdexcept>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>
#include <zlib.h>
#include <mutex>

FileIO::FileIO(DataStore* store) : dataStore(store) {}

bool FileIO::saveWorkbook(const std::string& filePath) {
    std::lock_guard<std::mutex> lock(ioMutex);

    try {
        // Create XML document representing the workbook
        rapidxml::xml_document<> doc = createXMLDocument();

        // Convert XML to string
        std::string xml_string;
        rapidxml::print(std::back_inserter(xml_string), doc);

        // Compress the XML data
        std::vector<unsigned char> compressed_data = compressData(xml_string);

        // Write the compressed data to the file
        std::ofstream file(filePath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Unable to open file for writing");
        }
        file.write(reinterpret_cast<const char*>(compressed_data.data()), compressed_data.size());

        return true;
    } catch (const std::exception& e) {
        // Log error message
        return false;
    }
}

bool FileIO::loadWorkbook(const std::string& filePath) {
    std::lock_guard<std::mutex> lock(ioMutex);

    try {
        // Read the compressed data from the file
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (!file) {
            throw std::runtime_error("Unable to open file for reading");
        }
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<unsigned char> compressed_data(size);
        if (!file.read(reinterpret_cast<char*>(compressed_data.data()), size)) {
            throw std::runtime_error("Error reading file");
        }

        // Decompress the data
        std::string xml_string = decompressData(compressed_data);

        // Parse the XML document
        rapidxml::xml_document<> doc;
        doc.parse<0>(&xml_string[0]);

        // Populate the DataStore with the parsed data
        parseXMLDocument(doc);

        return true;
    } catch (const std::exception& e) {
        // Log error message
        return false;
    }
}

rapidxml::xml_document<> FileIO::createXMLDocument() {
    rapidxml::xml_document<> doc;

    // Create root element for the workbook
    rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_element, "workbook");
    doc.append_node(root);

    // Iterate through all sheets in the DataStore
    for (const auto& sheetName : dataStore->getSheetNames()) {
        rapidxml::xml_node<>* sheet = doc.allocate_node(rapidxml::node_element, "sheet");
        sheet->append_attribute(doc.allocate_attribute("name", doc.allocate_string(sheetName.c_str())));
        root->append_node(sheet);

        // Iterate through all cells in the sheet
        for (int row = 0; row < dataStore->getRowCount(sheetName); ++row) {
            for (int col = 0; col < dataStore->getColumnCount(sheetName); ++col) {
                std::string cellValue = dataStore->getCellValue(sheetName, row, col);
                if (!cellValue.empty()) {
                    rapidxml::xml_node<>* cell = doc.allocate_node(rapidxml::node_element, "cell");
                    cell->append_attribute(doc.allocate_attribute("row", doc.allocate_string(std::to_string(row).c_str())));
                    cell->append_attribute(doc.allocate_attribute("col", doc.allocate_string(std::to_string(col).c_str())));
                    cell->append_attribute(doc.allocate_attribute("value", doc.allocate_string(cellValue.c_str())));
                    sheet->append_node(cell);
                }
            }
        }
    }

    return doc;
}

void FileIO::parseXMLDocument(const rapidxml::xml_document<>& doc) {
    // Get the root workbook element
    rapidxml::xml_node<>* root = doc.first_node("workbook");
    if (!root) {
        throw std::runtime_error("Invalid XML: missing workbook element");
    }

    // Iterate through sheet elements
    for (rapidxml::xml_node<>* sheet = root->first_node("sheet"); sheet; sheet = sheet->next_sibling("sheet")) {
        std::string sheetName = sheet->first_attribute("name")->value();
        dataStore->createSheet(sheetName);

        // Iterate through cell elements in the sheet
        for (rapidxml::xml_node<>* cell = sheet->first_node("cell"); cell; cell = cell->next_sibling("cell")) {
            int row = std::stoi(cell->first_attribute("row")->value());
            int col = std::stoi(cell->first_attribute("col")->value());
            std::string value = cell->first_attribute("value")->value();

            dataStore->setCellValue(sheetName, row, col, value);
        }
    }
}

std::vector<unsigned char> FileIO::compressData(const std::string& data) {
    uLongf compressedSize = compressBound(data.size());
    std::vector<unsigned char> compressedData(compressedSize);

    if (compress(compressedData.data(), &compressedSize, reinterpret_cast<const Bytef*>(data.data()), data.size()) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressedData.resize(compressedSize);
    return compressedData;
}

std::string FileIO::decompressData(const std::vector<unsigned char>& compressedData) {
    uLongf decompressedSize = compressedData.size() * 10; // Estimate decompressed size
    std::vector<unsigned char> decompressedData(decompressedSize);

    while (true) {
        int result = uncompress(decompressedData.data(), &decompressedSize, compressedData.data(), compressedData.size());
        if (result == Z_OK) {
            break;
        } else if (result == Z_BUF_ERROR) {
            decompressedSize *= 2;
            decompressedData.resize(decompressedSize);
        } else {
            throw std::runtime_error("Decompression failed");
        }
    }

    return std::string(decompressedData.begin(), decompressedData.begin() + decompressedSize);
}

// Human tasks:
// TODO: Implement error handling for file write failures
// TODO: Add support for different Excel file formats (e.g., XLSX, XLSM)
// TODO: Implement progress reporting for large file saves
// TODO: Implement error handling for file read failures and corrupt data
// TODO: Implement progress reporting for large file loads
// TODO: Optimize XML creation for large workbooks
// TODO: Add support for storing cell formulas and formatting in the XML
// TODO: Implement error handling for malformed XML
// TODO: Add support for parsing cell formulas and formatting