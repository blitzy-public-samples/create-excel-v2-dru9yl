#ifndef DATA_VALIDATION_H
#define DATA_VALIDATION_H

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

// Represents a data validation rule for Excel cells
class ValidationRule {
public:
    enum class ValidationType {
        // Add validation types here (e.g., NUMBER, TEXT, DATE, etc.)
    };

    ValidationType type;
    std::vector<std::string> parameters;
    std::string errorMessage;

    // Checks if the given data is valid according to the rule
    bool isValid(const std::string& data) const {
        // Implement validation logic here
        return true; // Placeholder
    }
};

// Manages data validation rules and performs validation checks on cell data
class DataValidation {
private:
    std::unordered_map<std::string, ValidationRule> validationRules;
    mutable std::mutex validationMutex;

public:
    // Initializes the DataValidation object
    DataValidation() = default;

    // Sets a validation rule for a specific cell or range of cells
    void setValidationRule(const std::string& cellRange, const ValidationRule& rule) {
        std::lock_guard<std::mutex> lock(validationMutex);
        validationRules[cellRange] = rule;
    }

    // Removes a validation rule for a specific cell or range of cells
    bool removeValidationRule(const std::string& cellRange) {
        std::lock_guard<std::mutex> lock(validationMutex);
        return validationRules.erase(cellRange) > 0;
    }

    // Validates data against the set rules for a specific cell
    bool validateData(const std::string& cellReference, const std::string& data) const {
        std::lock_guard<std::mutex> lock(validationMutex);
        auto it = validationRules.find(cellReference);
        if (it != validationRules.end()) {
            return it->second.isValid(data);
        }
        return true; // No rule set, consider valid
    }

    // Retrieves the validation rule for a specific cell
    const ValidationRule* getValidationRule(const std::string& cellReference) const {
        std::lock_guard<std::mutex> lock(validationMutex);
        auto it = validationRules.find(cellReference);
        if (it != validationRules.end()) {
            return &(it->second);
        }
        return nullptr;
    }
};

// Human tasks:
// - Add comprehensive documentation for each method, including usage examples and best practices
// - Implement support for custom validation functions
// - Add a method to bulk set validation rules for multiple cell ranges
// - Implement a mechanism to export and import validation rules
// - Consider adding support for conditional validation rules
// - Implement a notification system for validation failures
// - Add support for localization of error messages

#endif // DATA_VALIDATION_H