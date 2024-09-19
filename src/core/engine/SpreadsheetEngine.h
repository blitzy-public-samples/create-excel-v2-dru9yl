#ifndef SPREADSHEET_ENGINE_H
#define SPREADSHEET_ENGINE_H

#include <string>
#include <memory>
#include <mutex>

// Forward declarations
class CellManager;
class FormulaParser;
class CalculationEngine;
class MemoryManager;
class UndoRedoStack;
class DataValidation;
class FormattingEngine;
struct FormatOptions;
struct ValidationRule;

class SpreadsheetEngine {
public:
    // Constructor
    SpreadsheetEngine();

    // Destructor
    ~SpreadsheetEngine() = default;

    // Disable copy constructor and assignment operator
    SpreadsheetEngine(const SpreadsheetEngine&) = delete;
    SpreadsheetEngine& operator=(const SpreadsheetEngine&) = delete;

    /**
     * @brief Sets the value of a cell and triggers necessary updates
     * @param cellReference The reference of the cell to update
     * @param value The new value to set
     */
    void setCellValue(const std::string& cellReference, const std::string& value);

    /**
     * @brief Retrieves the value of a cell
     * @param cellReference The reference of the cell to retrieve
     * @return The value of the specified cell
     */
    std::string getCellValue(const std::string& cellReference) const;

    /**
     * @brief Triggers a full recalculation of the spreadsheet
     */
    void recalculateAll();

    /**
     * @brief Undoes the last action
     * @return True if undo was successful, false otherwise
     */
    bool undo();

    /**
     * @brief Redoes the last undone action
     * @return True if redo was successful, false otherwise
     */
    bool redo();

    /**
     * @brief Applies formatting to a cell or range of cells
     * @param cellRange The cell or range of cells to format
     * @param options The formatting options to apply
     */
    void applyFormatting(const std::string& cellRange, const FormatOptions& options);

    /**
     * @brief Validates data in a cell or range of cells
     * @param cellRange The cell or range of cells to validate
     * @param rule The validation rule to apply
     * @return True if all cells pass validation, false otherwise
     */
    bool validateData(const std::string& cellRange, const ValidationRule& rule);

private:
    std::unique_ptr<CellManager> cellManager;
    std::unique_ptr<FormulaParser> formulaParser;
    std::unique_ptr<CalculationEngine> calculationEngine;
    std::unique_ptr<MemoryManager> memoryManager;
    std::unique_ptr<UndoRedoStack> undoRedoStack;
    std::unique_ptr<DataValidation> dataValidation;
    std::unique_ptr<FormattingEngine> formattingEngine;
    mutable std::mutex engineMutex;
};

// Human tasks:
// TODO: Implement proper error handling mechanisms for all public methods
// TODO: Add documentation comments for all public methods and class members
// TODO: Consider adding a method for batch operations to improve performance for multiple cell updates
// TODO: Implement a mechanism for registering callbacks or observers for cell value changes
// TODO: Add methods for saving and loading spreadsheet state
// TODO: Consider implementing a thread-safe singleton pattern for global access to the SpreadsheetEngine instance

#endif // SPREADSHEET_ENGINE_H