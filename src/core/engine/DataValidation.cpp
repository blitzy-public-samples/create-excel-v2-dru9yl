#include "DataValidation.h"
#include <stdexcept>
#include <regex>
#include <algorithm>
#include <mutex>

// Constructor
DataValidation::DataValidation() {
    // Initialize the validationRules map
    // (Empty initialization as rules will be added later)
}

// Set a validation rule for a specific cell or range of cells
void DataValidation::setValidationRule(const std::string& cellRange, const ValidationRule& rule) {
    std::lock_guard<std::mutex> lock(validationMutex);
    // Add or update the validation rule for the specified cell range
    validationRules[cellRange] = rule;
}

// Remove a validation rule for a specific cell or range of cells
bool DataValidation::removeValidationRule(const std::string& cellRange) {
    std::lock_guard<std::mutex> lock(validationMutex);
    // Remove the validation rule for the specified cell range if it exists
    auto it = validationRules.find(cellRange);
    if (it != validationRules.end()) {
        validationRules.erase(it);
        return true;
    }
    return false;
}

// Validate data against the set rules for a specific cell
bool DataValidation::validateData(const std::string& cellReference, const std::string& data) {
    std::lock_guard<std::mutex> lock(validationMutex);
    
    // Find the matching validation rule for the cell
    ValidationRule* rule = findMatchingRule(cellReference);
    
    // If a rule is found, validate the data against the rule
    if (rule) {
        // Implement the validation logic based on the rule type
        // This is a simplified example and should be expanded based on actual rule types
        switch (rule->getType()) {
            case ValidationRule::Type::NUMBER:
                return std::regex_match(data, std::regex("^-?[0-9]*\\.?[0-9]+$"));
            case ValidationRule::Type::TEXT:
                return true; // All text is valid
            case ValidationRule::Type::DATE:
                // Implement date validation logic
                return false; // Placeholder
            case ValidationRule::Type::LIST:
                // Implement list validation logic
                return false; // Placeholder
            default:
                throw std::runtime_error("Unknown validation rule type");
        }
    }
    
    // If no rule is found, the data is considered valid
    return true;
}

// Find the validation rule that applies to a specific cell
ValidationRule* DataValidation::findMatchingRule(const std::string& cellReference) {
    // Iterate through the validationRules map
    auto it = std::find_if(validationRules.begin(), validationRules.end(),
        [&cellReference](const auto& pair) {
            // Check if the cell reference falls within the rule's range
            // This is a simplified check and should be improved for actual cell ranges
            return pair.first == cellReference;
        });
    
    // Return a pointer to the first matching rule found
    if (it != validationRules.end()) {
        return &(it->second);
    }
    
    // If no matching rule is found, return nullptr
    return nullptr;
}

// Human tasks:
// TODO: Implement support for overlapping cell ranges
// TODO: Add validation for the cell range format
// TODO: Implement cleanup of partially overlapping cell ranges
// TODO: Implement more complex validation rules (e.g., custom formulas)
// TODO: Add support for localized error messages
// TODO: Optimize the search algorithm for large numbers of rules
// TODO: Implement support for rule priorities in case of overlapping ranges