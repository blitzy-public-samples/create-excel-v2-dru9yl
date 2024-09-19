#include "DataStore.h"
#include <stdexcept>
#include <algorithm>
#include <mutex>

// Constructor implementation
DataStore::DataStore() {
    // Initialize the sheets map (empty by default)
}

// Set the value of a cell in a specific sheet
void DataStore::setCellValue(const std::string& sheetName, const std::string& cellReference, const CellData& value) {
    std::lock_guard<std::mutex> lock(dataMutex);

    // Check if the sheet exists, create it if not
    if (sheets.find(sheetName) == sheets.end()) {
        sheets[sheetName] = std::unordered_map<std::string, CellData>();
    }

    // Set the cell value in the appropriate sheet
    sheets[sheetName][cellReference] = value;
}

// Retrieve the value of a cell from a specific sheet
CellData DataStore::getCellValue(const std::string& sheetName, const std::string& cellReference) {
    std::lock_guard<std::mutex> lock(dataMutex);

    // Check if the sheet exists
    auto sheetIt = sheets.find(sheetName);
    if (sheetIt != sheets.end()) {
        // Check if the cell exists in the sheet
        auto cellIt = sheetIt->second.find(cellReference);
        if (cellIt != sheetIt->second.end()) {
            return cellIt->second;
        }
    }

    // Return a default CellData if not found
    return CellData();
}

// Clear all data from a specific sheet
void DataStore::clearSheet(const std::string& sheetName) {
    std::lock_guard<std::mutex> lock(dataMutex);

    // Check if the sheet exists
    auto sheetIt = sheets.find(sheetName);
    if (sheetIt != sheets.end()) {
        // Clear all cell data from the sheet
        sheetIt->second.clear();
    }
}

// Delete a specific sheet from the data store
bool DataStore::deleteSheet(const std::string& sheetName) {
    std::lock_guard<std::mutex> lock(dataMutex);

    // Check if the sheet exists and remove it
    auto sheetIt = sheets.find(sheetName);
    if (sheetIt != sheets.end()) {
        sheets.erase(sheetIt);
        return true;
    }

    return false;
}

// Human tasks (commented):
/*
TODO: Implement data validation before setting cell value
TODO: Add support for undo/redo operations
TODO: Implement notification mechanism for cell value changes
TODO: Implement caching mechanism for frequently accessed cell values
TODO: Add support for retrieving ranges of cell values efficiently
TODO: Implement undo/redo support for sheet clearing
TODO: Add option to clear specific data types (e.g., values, formats, formulas)
TODO: Implement safeguards against accidental sheet deletion
TODO: Add support for archiving deleted sheets for potential recovery
*/