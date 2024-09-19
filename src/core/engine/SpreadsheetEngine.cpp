#include "SpreadsheetEngine.h"
#include "CellManager.h"
#include "FormulaParser.h"
#include "CalculationEngine.h"
#include "MemoryManager.h"
#include "UndoRedoStack.h"
#include "DataValidation.h"
#include "FormattingEngine.h"
#include <stdexcept>
#include <algorithm>
#include <thread>
#include <mutex>

SpreadsheetEngine::SpreadsheetEngine()
    : cellManager(nullptr),
      formulaParser(nullptr),
      calculationEngine(nullptr),
      memoryManager(nullptr),
      undoRedoStack(nullptr),
      dataValidation(nullptr),
      formattingEngine(nullptr) {
    
    // Create instances of all required components
    cellManager = new CellManager();
    formulaParser = new FormulaParser();
    calculationEngine = new CalculationEngine();
    memoryManager = new MemoryManager();
    undoRedoStack = new UndoRedoStack();
    dataValidation = new DataValidation();
    formattingEngine = new FormattingEngine();

    // Set up any necessary connections between components
    calculationEngine->setFormulaParser(formulaParser);
    calculationEngine->setCellManager(cellManager);
}

void SpreadsheetEngine::setCellValue(const std::string& cellReference, const std::string& value) {
    std::lock_guard<std::mutex> lock(engineMutex);

    try {
        // Call CellManager to set the cell value
        cellManager->setCellValue(cellReference, value);

        // Trigger CalculationEngine to recalculate dependent cells
        calculationEngine->recalculate(cellReference);

        // Push the action to UndoRedoStack
        undoRedoStack->pushAction(cellReference, value);
    } catch (const std::exception& e) {
        // Handle any exceptions (e.g., invalid cell reference)
        throw std::runtime_error("Error setting cell value: " + std::string(e.what()));
    }
}

std::string SpreadsheetEngine::getCellValue(const std::string& cellReference) {
    std::lock_guard<std::mutex> lock(engineMutex);

    try {
        // Call CellManager to get the cell value
        return cellManager->getCellValue(cellReference);
    } catch (const std::exception& e) {
        // Handle any exceptions (e.g., invalid cell reference)
        throw std::runtime_error("Error getting cell value: " + std::string(e.what()));
    }
}

void SpreadsheetEngine::recalculateAll() {
    std::lock_guard<std::mutex> lock(engineMutex);

    // Call CalculationEngine to recalculate all cells
    calculationEngine->recalculateAll();
}

// Human tasks:
// TODO: Implement proper error handling for invalid cell references
// TODO: Optimize locking mechanism to reduce contention
// TODO: Implement batch update functionality for setting multiple cell values efficiently
// TODO: Implement caching mechanism for frequently accessed cell values
// TODO: Add support for retrieving formatted cell values
// TODO: Implement progress reporting for long-running recalculations
// TODO: Add support for partial recalculation of specific ranges