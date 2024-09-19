#ifndef FORMULA_PARSER_H
#define FORMULA_PARSER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <mutex>

class FormulaParser {
public:
    // Constructor: Initializes the FormulaParser with standard operators and functions
    FormulaParser();

    // Parses and evaluates an Excel formula
    // @param formula: The Excel formula to parse and evaluate
    // @param cellValueProvider: A function that provides cell values given a cell reference
    // @return: The result of the formula evaluation
    double parseFormula(const std::string& formula, const std::function<std::string(const std::string&)>& cellValueProvider);

    // Registers a custom operator
    // @param op: The operator symbol (e.g., "+", "-", "*", "/")
    // @param func: A pointer to the function that implements the operator
    void registerOperator(const std::string& op, double (*func)(double, double));

    // Registers a custom function
    // @param name: The name of the function
    // @param func: A pointer to the function that implements the custom function
    void registerFunction(const std::string& name, double (*func)(const std::vector<double>&));

private:
    // Map of operators and their corresponding functions
    std::unordered_map<std::string, double (*)(double, double)> operators;

    // Map of functions and their corresponding implementations
    std::unordered_map<std::string, double (*)(const std::vector<double>&)> functions;

    // Mutex for thread-safe operations
    mutable std::mutex parserMutex;
};

// Human tasks:
// TODO: Add comprehensive documentation for each method, including usage examples
// TODO: Implement error handling mechanism for invalid formulas or operations
// TODO: Consider adding a method to clear or reset all custom operators and functions
// TODO: Add support for handling circular references in formulas
// TODO: Implement a mechanism to parse and evaluate array formulas
// TODO: Consider adding support for volatile functions (functions that need to be recalculated on every sheet change)

#endif // FORMULA_PARSER_H