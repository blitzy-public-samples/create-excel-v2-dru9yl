#include "WhatIfAnalysis.h"
#include "DataStore.h"
#include "FormulaParser.h"
#include <algorithm>
#include <stdexcept>
#include <mutex>

WhatIfAnalysis::WhatIfAnalysis(DataStore* store, FormulaParser* parser)
    : dataStore(store), formulaParser(parser) {
    // Constructor implementation
}

bool WhatIfAnalysis::goalSeek(const std::string& targetCell, double targetValue, const std::string& variableCell) {
    std::lock_guard<std::mutex> lock(analysisMutex);

    // Validate input cells
    if (!dataStore->cellExists(targetCell) || !dataStore->cellExists(variableCell)) {
        throw std::runtime_error("Invalid cell reference");
    }

    // Implement binary search algorithm
    double low = 0.0, high = 1e6; // Assuming a reasonable range
    double epsilon = 1e-6; // Precision
    int maxIterations = 100;

    for (int i = 0; i < maxIterations; ++i) {
        double mid = (low + high) / 2;
        dataStore->setCellValue(variableCell, mid);
        double currentValue = evaluateFormula(targetCell);

        if (std::abs(currentValue - targetValue) < epsilon) {
            return true; // Goal seek successful
        }

        if (currentValue < targetValue) {
            low = mid;
        } else {
            high = mid;
        }
    }

    return false; // Goal seek failed to converge
}

bool WhatIfAnalysis::dataTable(const std::string& tableRange, const std::string& rowInputCell, const std::string& columnInputCell) {
    std::lock_guard<std::mutex> lock(analysisMutex);

    // Validate input range and cells
    if (!dataStore->rangeExists(tableRange) || 
        (!rowInputCell.empty() && !dataStore->cellExists(rowInputCell)) ||
        (!columnInputCell.empty() && !dataStore->cellExists(columnInputCell))) {
        throw std::runtime_error("Invalid range or cell reference");
    }

    // Get the range of cells for the data table
    auto [startRow, startCol, endRow, endCol] = dataStore->getRangeBounds(tableRange);

    // Iterate through the table range
    for (int row = startRow + 1; row <= endRow; ++row) {
        for (int col = startCol + 1; col <= endCol; ++col) {
            // Store original values
            double originalRowValue = dataStore->getCellValue(rowInputCell);
            double originalColValue = dataStore->getCellValue(columnInputCell);

            // Set input values
            if (!rowInputCell.empty()) {
                dataStore->setCellValue(rowInputCell, dataStore->getCellValue(dataStore->getCellReference(row, startCol)));
            }
            if (!columnInputCell.empty()) {
                dataStore->setCellValue(columnInputCell, dataStore->getCellValue(dataStore->getCellReference(startRow, col)));
            }

            // Calculate and set result
            std::string formulaCell = dataStore->getCellReference(startRow, startCol);
            double result = evaluateFormula(formulaCell);
            dataStore->setCellValue(dataStore->getCellReference(row, col), result);

            // Restore original values
            if (!rowInputCell.empty()) {
                dataStore->setCellValue(rowInputCell, originalRowValue);
            }
            if (!columnInputCell.empty()) {
                dataStore->setCellValue(columnInputCell, originalColValue);
            }
        }
    }

    return true;
}

bool WhatIfAnalysis::scenarioManager(const std::string& scenarioName, const std::vector<std::pair<std::string, double>>& changedCells) {
    std::lock_guard<std::mutex> lock(analysisMutex);

    // Validate scenario name and changed cells
    if (scenarioName.empty() || changedCells.empty()) {
        throw std::runtime_error("Invalid scenario name or empty changed cells");
    }

    // Store current cell values as a baseline
    std::vector<std::pair<std::string, double>> baseline;
    for (const auto& [cell, _] : changedCells) {
        baseline.emplace_back(cell, dataStore->getCellValue(cell));
    }

    // Apply changes to specified cells
    for (const auto& [cell, value] : changedCells) {
        dataStore->setCellValue(cell, value);
    }

    // Update all dependent cells
    dataStore->recalculateAll();

    // Store the scenario for future use
    // Note: This is a simplified implementation. In a real-world scenario,
    // we would need a more robust scenario storage mechanism.
    scenarios[scenarioName] = changedCells;

    return true;
}

double WhatIfAnalysis::evaluateFormula(const std::string& cellReference) {
    std::string cellValue = dataStore->getCellValue(cellReference);
    if (cellValue.empty() || cellValue[0] != '=') {
        return std::stod(cellValue);
    }
    return formulaParser->parseFormula(cellValue.substr(1), cellReference);
}

// Human tasks:
// TODO: Implement more sophisticated numerical methods for goal seeking
// TODO: Add support for multiple variable cells in goal seek
// TODO: Implement error handling for cases where goal seek is impossible
// TODO: Implement support for one-variable and two-variable data tables
// TODO: Add caching mechanism to improve performance for large data tables
// TODO: Implement parallel processing for data table calculations
// TODO: Implement scenario comparison functionality
// TODO: Add support for scenario merging
// TODO: Implement undo/redo functionality for scenario applications
// TODO: Implement caching mechanism for frequently evaluated formulas
// TODO: Add support for handling circular references in formulas