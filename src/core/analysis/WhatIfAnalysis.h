#ifndef WHAT_IF_ANALYSIS_H
#define WHAT_IF_ANALYSIS_H

#include <string>
#include <vector>
#include <utility>
#include <mutex>

// Forward declarations
class DataStore;
class FormulaParser;

class WhatIfAnalysis {
public:
    // Constructor
    WhatIfAnalysis(DataStore* store, FormulaParser* parser);

    // Goal seek analysis
    bool goalSeek(const std::string& targetCell, double targetValue, const std::string& variableCell);

    // Data table creation
    bool dataTable(const std::string& tableRange, const std::string& rowInputCell, const std::string& columnInputCell);

    // Scenario management
    bool scenarioManager(const std::string& scenarioName, const std::vector<std::pair<std::string, double>>& changedCells);
    bool createScenario(const std::string& scenarioName, const std::vector<std::pair<std::string, double>>& changedCells);
    bool deleteScenario(const std::string& scenarioName);
    std::vector<std::string> listScenarios();

private:
    DataStore* dataStore;
    FormulaParser* formulaParser;
    mutable std::mutex analysisMutex;
};

// Human tasks:
// - Add comprehensive documentation for each method, including usage examples and best practices
// - Implement error handling and input validation for all public methods
// - Add support for sensitivity analysis functionality
// - Implement Monte Carlo simulation capabilities for risk analysis
// - Add support for optimization problems (e.g., linear programming, nonlinear optimization)
// - Implement a mechanism to visualize what-if analysis results (e.g., charts, graphs)
// - Add support for saving and loading what-if analysis configurations
// - Implement a user-friendly interface for creating and managing scenarios
// - Add support for collaborative what-if analysis in shared workbooks

#endif // WHAT_IF_ANALYSIS_H