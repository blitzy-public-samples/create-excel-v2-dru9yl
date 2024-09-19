#include "CellManager.h"
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <mutex>

// Constructor implementation
CellManager::CellManager() {
    // Initialize the cells unordered_map and cellMutex
    // (No explicit initialization needed for std::unordered_map and std::mutex)
}

void CellManager::setCellValue(const std::string& cellReference, const std::string& value) {
    // Validate the cell reference
    if (!validateCellReference(cellReference)) {
        throw std::runtime_error("Invalid cell reference: " + cellReference);
    }

    // Acquire lock on cellMutex
    std::lock_guard<std::mutex> lock(cellMutex);

    // Create a new Cell object if it doesn't exist
    auto& cell = cells[cellReference];

    // Set the value of the Cell object
    cell.setValue(value);

    // Lock is automatically released when lock_guard goes out of scope
}

std::string CellManager::getCellValue(const std::string& cellReference) {
    // Validate the cell reference
    if (!validateCellReference(cellReference)) {
        throw std::runtime_error("Invalid cell reference: " + cellReference);
    }

    // Acquire lock on cellMutex
    std::lock_guard<std::mutex> lock(cellMutex);

    // Retrieve the Cell object from the cells map
    auto it = cells.find(cellReference);
    if (it == cells.end()) {
        return ""; // Return empty string for non-existent cells
    }

    // Get the value from the Cell object
    std::string value = it->second.getValue();

    // Lock is automatically released when lock_guard goes out of scope

    // Return the cell value
    return value;
}

bool CellManager::validateCellReference(const std::string& cellReference) {
    // Check if the cell reference is empty
    if (cellReference.empty()) {
        return false;
    }

    // Extract the column letters and row number
    std::string columnLetters;
    int rowNumber = 0;
    std::stringstream ss(cellReference);

    // Read column letters
    while (ss.peek() >= 'A' && ss.peek() <= 'Z') {
        columnLetters += static_cast<char>(ss.get());
    }

    // Read row number
    ss >> rowNumber;

    // Validate the column letters (A-ZZ)
    if (columnLetters.empty() || columnLetters.length() > 2) {
        return false;
    }

    for (char c : columnLetters) {
        if (c < 'A' || c > 'Z') {
            return false;
        }
    }

    // Validate the row number (1-1048576)
    if (rowNumber < 1 || rowNumber > 1048576) {
        return false;
    }

    // Return true if both column and row are valid
    return true;
}

// Human tasks:
// TODO: Implement proper error handling for invalid cell references
// TODO: Add support for different data types (numbers, dates, etc.)
// TODO: Implement cell value change notification mechanism
// TODO: Implement caching mechanism for frequently accessed cell values
// TODO: Add support for retrieving formatted cell values
// TODO: Optimize the validation algorithm for better performance
// TODO: Add support for validating cell ranges (e.g., A1:B10)