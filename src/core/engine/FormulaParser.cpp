#include "FormulaParser.h"
#include <stack>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <mutex>

// Constructor implementation
FormulaParser::FormulaParser() {
    // Initialize the operators map with basic arithmetic operations
    operators["+"] = [](double a, double b) { return a + b; };
    operators["-"] = [](double a, double b) { return a - b; };
    operators["*"] = [](double a, double b) { return a * b; };
    operators["/"] = [](double a, double b) { return a / b; };
    operators["^"] = [](double a, double b) { return std::pow(a, b); };

    // Initialize the functions map with common Excel functions
    functions["SUM"] = [](const std::vector<double>& args) {
        return std::accumulate(args.begin(), args.end(), 0.0);
    };
    functions["AVERAGE"] = [](const std::vector<double>& args) {
        return std::accumulate(args.begin(), args.end(), 0.0) / args.size();
    };
    functions["MIN"] = [](const std::vector<double>& args) {
        return *std::min_element(args.begin(), args.end());
    };
    functions["MAX"] = [](const std::vector<double>& args) {
        return *std::max_element(args.begin(), args.end());
    };
}

// Parse and evaluate the formula
double FormulaParser::parseFormula(const std::string& formula, const std::function<std::string(const std::string&)>& cellValueProvider) {
    std::lock_guard<std::mutex> lock(parserMutex);

    // Tokenize the input formula
    std::vector<std::string> tokens = tokenize(formula);

    // Convert the tokens to Reverse Polish Notation (RPN)
    std::vector<std::string> rpnTokens;
    std::stack<std::string> operatorStack;

    for (const auto& token : tokens) {
        if (std::isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
            rpnTokens.push_back(token);
        } else if (std::isalpha(token[0])) {
            if (functions.find(token) != functions.end()) {
                operatorStack.push(token);
            } else {
                rpnTokens.push_back(token); // Cell reference
            }
        } else if (token == "(") {
            operatorStack.push(token);
        } else if (token == ")") {
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                rpnTokens.push_back(operatorStack.top());
                operatorStack.pop();
            }
            if (!operatorStack.empty() && operatorStack.top() == "(") {
                operatorStack.pop();
            }
            if (!operatorStack.empty() && functions.find(operatorStack.top()) != functions.end()) {
                rpnTokens.push_back(operatorStack.top());
                operatorStack.pop();
            }
        } else {
            while (!operatorStack.empty() && operatorStack.top() != "(" &&
                   operators.find(token) != operators.end() &&
                   operators.find(operatorStack.top()) != operators.end()) {
                rpnTokens.push_back(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.push(token);
        }
    }

    while (!operatorStack.empty()) {
        rpnTokens.push_back(operatorStack.top());
        operatorStack.pop();
    }

    // Evaluate the RPN expression
    return evaluateRPN(rpnTokens, cellValueProvider);
}

// Tokenize the formula string
std::vector<std::string> FormulaParser::tokenize(const std::string& formula) {
    std::vector<std::string> tokens;
    std::string token;
    bool inQuotes = false;

    for (size_t i = 0; i < formula.length(); ++i) {
        char c = formula[i];

        if (c == '"') {
            inQuotes = !inQuotes;
            token += c;
        } else if (inQuotes) {
            token += c;
        } else if (std::isspace(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else if (c == '(' || c == ')' || c == ',' || c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(std::string(1, c));
        } else {
            token += c;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}

// Evaluate the RPN expression
double FormulaParser::evaluateRPN(const std::vector<std::string>& rpnTokens, const std::function<std::string(const std::string&)>& cellValueProvider) {
    std::stack<double> operandStack;

    for (const auto& token : rpnTokens) {
        if (std::isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
            operandStack.push(std::stod(token));
        } else if (std::isalpha(token[0])) {
            if (functions.find(token) != functions.end()) {
                // Function call
                std::vector<double> args;
                while (!operandStack.empty()) {
                    args.push_back(operandStack.top());
                    operandStack.pop();
                }
                std::reverse(args.begin(), args.end());
                operandStack.push(functions[token](args));
            } else {
                // Cell reference
                std::string cellValue = cellValueProvider(token);
                operandStack.push(std::stod(cellValue));
            }
        } else if (operators.find(token) != operators.end()) {
            double b = operandStack.top();
            operandStack.pop();
            double a = operandStack.top();
            operandStack.pop();
            operandStack.push(operators[token](a, b));
        }
    }

    if (operandStack.size() != 1) {
        throw std::runtime_error("Invalid formula: unexpected number of operands");
    }

    return operandStack.top();
}

// Human tasks:
// TODO: Implement error handling for invalid formulas
// TODO: Add support for more complex Excel functions
// TODO: Optimize the parsing algorithm for better performance with large formulas
// TODO: Improve handling of string literals in formulas
// TODO: Add support for array formulas
// TODO: Implement proper error handling for mismatched parentheses
// TODO: Implement proper error handling for stack underflow
// TODO: Add support for more Excel functions
// TODO: Optimize memory usage for large formulas