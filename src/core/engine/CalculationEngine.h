#ifndef CALCULATION_ENGINE_H
#define CALCULATION_ENGINE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <mutex>

class FormulaParser;
class CellManager;

class CalculationEngine {
public:
    // Constructor: Initializes the CalculationEngine with FormulaParser and CellManager instances
    CalculationEngine(FormulaParser* parser, CellManager* manager);

    // Recalculates the value of a specific cell
    void recalculateCell(const std::string& cellReference);

    // Recalculates all cells in the spreadsheet
    void recalculateAll();

    // Updates the dependency graph when a cell formula changes
    void updateDependencyGraph(const std::string& cellReference, const std::vector<std::string>& dependencies);

    // Checks if the engine is currently performing calculations
    bool isCurrentlyCalculating() const;

private:
    FormulaParser* formulaParser;
    CellManager* cellManager;
    std::unordered_map<std::string, std::unordered_set<std::string>> dependencyGraph;
    mutable std::mutex calculationMutex;
    bool isCalculating;

    // Add any additional private methods here
};

// Human tasks:
// TODO: Add comprehensive documentation for each method, including usage examples and edge cases
// TODO: Implement error handling mechanism for invalid cell references or circular dependencies
// TODO: Consider adding a method to cancel ongoing calculations
// TODO: Add support for registering calculation progress callbacks
// TODO: Implement a method to get the current calculation status (percentage complete, cells processed, etc.)
// TODO: Consider adding support for custom calculation modes (automatic, manual, etc.)

#endif // CALCULATION_ENGINE_H