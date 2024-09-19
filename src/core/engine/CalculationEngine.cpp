#include "CalculationEngine.h"
#include "FormulaParser.h"
#include "CellManager.h"
#include <queue>
#include <unordered_set>
#include <stdexcept>
#include <thread>
#include <future>
#include <mutex>

// Constructor implementation
CalculationEngine::CalculationEngine(FormulaParser* parser, CellManager* manager)
    : formulaParser(parser), cellManager(manager), isCalculating(false) {
    // Initialize dependencyGraph as an empty map
    dependencyGraph = std::unordered_map<std::string, std::unordered_set<std::string>>();
}

// Recalculate a specific cell
void CalculationEngine::recalculateCell(const std::string& cellReference) {
    std::lock_guard<std::mutex> lock(calculationMutex);

    // Get the current cell value from CellManager
    std::string cellValue = cellManager->getCellValue(cellReference);

    // If the cell contains a formula, parse and evaluate it
    if (cellValue.length() > 0 && cellValue[0] == '=') {
        try {
            double result = formulaParser->parseFormula(cellValue.substr(1));
            cellManager->setCellValue(cellReference, std::to_string(result));
        } catch (const std::runtime_error& e) {
            // Handle formula parsing or evaluation errors
            cellManager->setCellValue(cellReference, "#ERROR!");
        }
    }
}

// Recalculate all cells in the spreadsheet
void CalculationEngine::recalculateAll() {
    std::lock_guard<std::mutex> lock(calculationMutex);
    isCalculating = true;

    // Get all cell references from CellManager
    std::vector<std::string> allCells = cellManager->getAllCellReferences();

    // Create a queue of cells to recalculate
    std::queue<std::string> cellQueue;
    for (const auto& cell : allCells) {
        cellQueue.push(cell);
    }

    // Process the queue
    while (!cellQueue.empty()) {
        std::string currentCell = cellQueue.front();
        cellQueue.pop();

        recalculateCell(currentCell);

        // Add dependent cells to the queue
        if (dependencyGraph.find(currentCell) != dependencyGraph.end()) {
            for (const auto& dependentCell : dependencyGraph[currentCell]) {
                cellQueue.push(dependentCell);
            }
        }
    }

    isCalculating = false;
}

// Update the dependency graph when a cell formula changes
void CalculationEngine::updateDependencyGraph(const std::string& cellReference, const std::vector<std::string>& dependencies) {
    std::lock_guard<std::mutex> lock(calculationMutex);

    // Remove old dependencies
    if (dependencyGraph.find(cellReference) != dependencyGraph.end()) {
        dependencyGraph.erase(cellReference);
    }

    // Add new dependencies
    for (const auto& dep : dependencies) {
        dependencyGraph[dep].insert(cellReference);
    }
}

// Human tasks (commented):
/*
TODO: Implement circular reference detection and handling in recalculateCell
TODO: Add support for array formulas in recalculateCell
TODO: Implement caching mechanism for frequently accessed cell values in recalculateCell
TODO: Implement parallel processing for faster recalculation of independent cells in recalculateAll
TODO: Add progress reporting mechanism for long-running calculations in recalculateAll
TODO: Implement smart recalculation to minimize unnecessary calculations in recalculateAll
TODO: Implement efficient graph traversal for large spreadsheets in updateDependencyGraph
TODO: Add support for named ranges in dependency tracking in updateDependencyGraph
TODO: Implement visualization of the dependency graph for debugging purposes
*/