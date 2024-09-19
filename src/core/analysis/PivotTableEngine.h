#ifndef PIVOT_TABLE_ENGINE_H
#define PIVOT_TABLE_ENGINE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include "src/core/data/DataStore.h"

// Structure to hold information about a pivot table
struct PivotTableInfo {
    std::string sourceRange;
    std::string destinationRange;
    std::vector<std::string> rowFields;
    std::vector<std::string> columnFields;
    std::vector<std::string> dataFields;
    std::string aggregateFunction;
};

class PivotTableEngine {
public:
    // Constructor: Initializes the PivotTableEngine with a reference to the DataStore
    PivotTableEngine(DataStore* store);

    // Creates a new pivot table based on the specified parameters
    bool createPivotTable(const std::string& sourceRange,
                          const std::string& destinationCell,
                          const std::vector<std::string>& rowFields,
                          const std::vector<std::string>& columnFields,
                          const std::vector<std::string>& dataFields,
                          const std::string& aggregateFunction);

    // Updates an existing pivot table with new data or settings
    bool updatePivotTable(const std::string& pivotTableRange);

    // Deletes an existing pivot table
    bool deletePivotTable(const std::string& pivotTableRange);

    // Retrieves information about an existing pivot table
    PivotTableInfo getPivotTableInfo(const std::string& pivotTableRange);

    // Refreshes the data in an existing pivot table
    bool refreshPivotTable(const std::string& pivotTableRange);

private:
    DataStore* dataStore;
    mutable std::mutex pivotMutex;
};

// Human tasks:
// - Add comprehensive documentation for each method, including usage examples and best practices
// - Implement error handling and validation for input parameters
// - Add support for multiple aggregate functions per data field
// - Implement advanced pivot table features like filtering, sorting, and grouping
// - Add support for calculated fields and items in pivot tables
// - Implement pivot cache for improved performance with large datasets
// - Add support for pivot charts
// - Implement data refresh mechanisms for pivot tables connected to external data sources
// - Add support for custom styles and formatting options for pivot tables

#endif // PIVOT_TABLE_ENGINE_H