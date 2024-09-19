#ifndef DATASTORE_H
#define DATASTORE_H

#include <string>
#include <unordered_map>
#include <mutex>
#include <variant>
#include <vector>
#include "src/core/engine/FormattingEngine.h"

// Represents the data stored in a single cell
struct CellData {
    std::variant<std::string, double, bool> value;
    std::string formula;
    CellFormat format;
};

// Manages in-memory data storage and retrieval for Excel spreadsheets
class DataStore {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, CellData>> sheets;
    mutable std::mutex dataMutex;

public:
    // Initializes the DataStore
    DataStore() = default;

    // Sets the value of a cell in a specific sheet
    void setCellValue(const std::string& sheetName, const std::string& cellReference, const CellData& value);

    // Retrieves the value of a cell from a specific sheet
    CellData getCellValue(const std::string& sheetName, const std::string& cellReference);

    // Clears all data from a specific sheet
    void clearSheet(const std::string& sheetName);

    // Deletes a specific sheet from the data store
    bool deleteSheet(const std::string& sheetName);

    // Retrieves a list of all sheet names in the data store
    std::vector<std::string> getSheetNames();

    // Retrieves a range of cell values from a specific sheet
    std::vector<std::vector<CellData>> getCellRange(const std::string& sheetName, const std::string& startCell, const std::string& endCell);
};

// Human tasks:
// - Add comprehensive documentation for each method, including usage examples and best practices
// - Implement a mechanism for data change notifications to update dependent cells and UI
// - Add support for transactions to allow atomic updates of multiple cells
// - Implement a caching mechanism for frequently accessed data
// - Add support for lazy loading of sheet data to improve memory usage
// - Implement data validation hooks to ensure data integrity
// - Add support for concurrent access optimizations (e.g., read-write locks for sheets)
// - Implement a mechanism for efficient bulk updates of cell data

#endif // DATASTORE_H