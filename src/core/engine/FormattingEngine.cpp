#include "FormattingEngine.h"
#include <stdexcept>
#include <algorithm>
#include <mutex>

// Constructor implementation
FormattingEngine::FormattingEngine() {
    // Initialize the cellFormats map
    // No initialization needed as the map is empty by default
}

// Set the format for a specific cell or range of cells
void FormattingEngine::setCellFormat(const std::string& cellRange, const CellFormat& format) {
    std::lock_guard<std::mutex> lock(formatMutex);
    
    // Expand the cell range into individual cell references
    std::vector<std::string> cells = expandCellRange(cellRange);
    
    // For each cell reference, set or update the format in cellFormats
    for (const auto& cell : cells) {
        cellFormats[cell] = format;
    }
}

// Retrieve the format for a specific cell
CellFormat FormattingEngine::getCellFormat(const std::string& cellReference) {
    std::lock_guard<std::mutex> lock(formatMutex);
    
    // Look up the cell reference in cellFormats
    auto it = cellFormats.find(cellReference);
    
    // If found, return the associated CellFormat
    if (it != cellFormats.end()) {
        return it->second;
    }
    
    // If not found, return a default CellFormat
    return CellFormat();
}

// Clear the format for a specific cell or range of cells
void FormattingEngine::clearCellFormat(const std::string& cellRange) {
    std::lock_guard<std::mutex> lock(formatMutex);
    
    // Expand the cell range into individual cell references
    std::vector<std::string> cells = expandCellRange(cellRange);
    
    // For each cell reference, remove the format from cellFormats if it exists
    for (const auto& cell : cells) {
        cellFormats.erase(cell);
    }
}

// Expand a cell range string into individual cell references
std::vector<std::string> FormattingEngine::expandCellRange(const std::string& cellRange) {
    std::vector<std::string> result;
    
    // Parse the cell range string (e.g., 'A1:B3')
    size_t colonPos = cellRange.find(':');
    if (colonPos == std::string::npos) {
        // Single cell reference
        result.push_back(cellRange);
        return result;
    }
    
    std::string startCell = cellRange.substr(0, colonPos);
    std::string endCell = cellRange.substr(colonPos + 1);
    
    // Determine the start and end columns and rows
    char startCol = startCell[0];
    int startRow = std::stoi(startCell.substr(1));
    char endCol = endCell[0];
    int endRow = std::stoi(endCell.substr(1));
    
    // Generate all cell references within the range
    for (char col = startCol; col <= endCol; ++col) {
        for (int row = startRow; row <= endRow; ++row) {
            result.push_back(std::string(1, col) + std::to_string(row));
        }
    }
    
    return result;
}

// Human tasks:
// TODO: Implement efficient storage for shared formats across multiple cells
// TODO: Add support for conditional formatting
// TODO: Implement caching mechanism for frequently accessed cell formats
// TODO: Add support for inheriting formats from column or row styles
// TODO: Implement bulk clear operation for improved performance
// TODO: Add support for clearing specific format properties while retaining others
// TODO: Optimize the expansion algorithm for large ranges
// TODO: Add support for non-contiguous ranges (e.g., 'A1:B3,D5:E7')