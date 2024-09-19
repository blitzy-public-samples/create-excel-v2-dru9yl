#ifndef CONDITIONAL_FORMATTING_H
#define CONDITIONAL_FORMATTING_H

#include <string>
#include <vector>
#include <mutex>
#include "../data/DataStore.h"

// Enum class representing different types of conditional formatting
enum class FormatType {
    CELL_VALUE,
    FORMULA,
    COLOR_SCALE,
    DATA_BAR,
    ICON_SET
};

// Class representing a conditional formatting rule
class ConditionalFormattingRule {
public:
    std::string id;
    FormatType type;
    std::string range;
    std::string condition;
    CellFormat format;
};

// Manages conditional formatting rules and applies them to cells in Excel
class ConditionalFormatting {
private:
    DataStore* dataStore;
    std::vector<ConditionalFormattingRule> rules;
    mutable std::mutex formatMutex;

public:
    // Constructor: Initializes the ConditionalFormatting with a reference to the DataStore
    ConditionalFormatting(DataStore* store);

    // Adds a new conditional formatting rule
    std::string addRule(const ConditionalFormattingRule& rule);

    // Removes an existing conditional formatting rule
    bool removeRule(const std::string& ruleId);

    // Applies conditional formatting to the specified range
    void applyFormatting(const std::string& range);

    // Retrieves all conditional formatting rules
    std::vector<ConditionalFormattingRule> getRules();

    // Clears all conditional formatting from the specified range
    void clearFormatting(const std::string& range);
};

// Human tasks:
// - Add comprehensive documentation for each method, including usage examples and best practices
// - Implement error handling and input validation for all public methods
// - Add support for more complex conditional formatting rules (e.g., multi-criteria rules)
// - Implement a mechanism to handle rule priorities and conflicts
// - Add support for custom user-defined functions in conditional formatting rules
// - Implement performance optimizations for applying formatting to large ranges
// - Add support for exporting and importing conditional formatting rules
// - Implement a user-friendly interface for creating and editing conditional formatting rules
// - Add support for conditional formatting based on values in other sheets or workbooks
// - Implement accessibility features for conditionally formatted cells (e.g., alternative text descriptions)

#endif // CONDITIONAL_FORMATTING_H