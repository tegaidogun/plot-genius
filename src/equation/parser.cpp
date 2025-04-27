/**
 * Equation Parser Implementation
 * 
 * Implements a recursive descent parser for mathematical expressions.
 * Parses expressions into an abstract syntax tree (AST) and provides 
 * evaluation functionality for any x value.
 * 
 * Supports:
 * - Basic arithmetic operations (+, -, *, /)
 * - Mathematical functions (sin, cos, tan, sqrt, log, exp, abs, pow)
 * - Constants (pi, e)
 * - Parenthesized expressions
 * - Variable substitution (x)
 */

#include "parser.hpp"
#include <cmath>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <algorithm>

namespace plot_genius {

EquationParser::EquationParser() : m_root(nullptr) {
    InitializeConstants();
}

/**
 * Initializes mathematical constants used in equations
 * 
 * Currently supports:
 * - pi: The mathematical constant π (3.14159...)
 * - e: The base of natural logarithms (2.71828...)
 */
void EquationParser::InitializeConstants() {
    m_constants["pi"] = M_PI;
    m_constants["e"] = M_E;
}

/**
 * Validates that the equation is formatted correctly
 * 
 * Ensures the equation starts with 'y=' prefix as required
 * by the application's convention.
 * 
 * @param equation The equation string to validate
 * @return True if the format is valid, false otherwise with error message set
 */
bool EquationParser::ValidateEquationFormat(const ::std::string& equation) {
    // Check if equation starts with 'y='
    if (equation.length() < 2 || equation.substr(0, 2) != "y=") {
        m_lastError = "Equation must start with 'y='";
        return false;
    }
    return true;
}

/**
 * Parses a mathematical equation into an AST
 * 
 * Validates the equation format, removes the 'y=' prefix, and
 * parses the right-hand side expression.
 * 
 * @param equation The equation string to parse (should start with 'y=')
 * @return True if parsing succeeded, false otherwise with error message set
 */
bool EquationParser::Parse(const ::std::string& equation) {
    try {
        if (!ValidateEquationFormat(equation)) {
            return false;
        }

        // Remove 'y=' prefix and any whitespace
        ::std::string expr = equation.substr(2);
        expr.erase(::std::remove_if(expr.begin(), expr.end(), ::isspace), expr.end());

        m_root = ParseExpression(expr);
        return m_root != nullptr;
    } catch (const ::std::exception& e) {
        m_lastError = e.what();
        return false;
    }
}

/**
 * Evaluates the parsed equation for a specific x value
 * 
 * @param x The value to substitute for the variable x
 * @return The result of evaluating the equation
 * @throws std::runtime_error if no equation has been successfully parsed
 */
double EquationParser::Evaluate(double x) const {
    if (!m_root) {
        throw ::std::runtime_error("No equation has been parsed yet");
    }
    return m_root->Evaluate(x);
}

/**
 * Parses expressions with addition and subtraction operations
 * 
 * This handles the lowest precedence operators (+ and -).
 * 
 * @param expr The expression string to parse
 * @return Root node of the parsed expression subtree
 */
::std::unique_ptr<EquationParser::Node> EquationParser::ParseExpression(const ::std::string& expr) {
    auto node = ParseTerm(expr);
    if (!node) return nullptr;

    // Handle addition and subtraction
    ::std::size_t pos = 0;
    while (pos < expr.length()) {
        if (expr[pos] == '+' || expr[pos] == '-') {
            auto op = expr[pos];
            auto right = ParseTerm(expr.substr(pos + 1));
            if (!right) return nullptr;

            auto binaryNode = ::std::make_unique<BinaryOpNode>();
            binaryNode->left = ::std::move(node);
            binaryNode->right = ::std::move(right);
            binaryNode->op = (op == '+') ? 
                [](double a, double b) { return a + b; } :
                [](double a, double b) { return a - b; };
            
            node = ::std::move(binaryNode);
        }
        pos++;
    }
    return node;
}

/**
 * Parses terms with multiplication and division operations
 * 
 * This handles the medium precedence operators (* and /).
 * 
 * @param expr The expression string to parse
 * @return Root node of the parsed term subtree
 */
::std::unique_ptr<EquationParser::Node> EquationParser::ParseTerm(const ::std::string& expr) {
    auto node = ParseFactor(expr);
    if (!node) return nullptr;

    // Handle multiplication and division
    ::std::size_t pos = 0;
    while (pos < expr.length()) {
        if (expr[pos] == '*' || expr[pos] == '/') {
            auto op = expr[pos];
            auto right = ParseFactor(expr.substr(pos + 1));
            if (!right) return nullptr;

            auto binaryNode = ::std::make_unique<BinaryOpNode>();
            binaryNode->left = ::std::move(node);
            binaryNode->right = ::std::move(right);
            binaryNode->op = (op == '*') ? 
                [](double a, double b) { return a * b; } :
                [](double a, double b) { return a / b; };
            
            node = ::std::move(binaryNode);
        }
        pos++;
    }
    return node;
}

/**
 * Parses factors (highest precedence elements)
 * 
 * Handles parenthesized expressions, numbers, variables, 
 * constants, and function calls.
 * 
 * @param expr The expression string to parse
 * @return Root node of the parsed factor subtree
 * @throws std::runtime_error for syntax errors like unmatched parentheses
 */
::std::unique_ptr<EquationParser::Node> EquationParser::ParseFactor(const ::std::string& expr) {
    if (expr.empty()) return nullptr;

    // Handle parentheses
    if (expr[0] == '(') {
        ::std::size_t end = expr.find_last_of(')');
        if (end == ::std::string::npos) {
            throw ::std::runtime_error("Unmatched parentheses");
        }
        return ParseExpression(expr.substr(1, end - 1));
    }

    // Handle numbers
    if (::std::isdigit(expr[0]) || expr[0] == '.') {
        return ParseNumber(expr);
    }

    // Handle variable x
    if (expr[0] == 'x') {
        return ::std::make_unique<VariableNode>();
    }

    // Handle constants
    if (::std::isalpha(expr[0])) {
        auto constant = ParseConstant(expr);
        if (constant) return constant;
    }

    // Handle functions
    if (::std::isalpha(expr[0])) {
        return ParseFunction(expr);
    }

    return nullptr;
}

/**
 * Parses a numeric literal
 * 
 * @param expr The expression string starting with a numeric literal
 * @return NumberNode containing the parsed value
 */
::std::unique_ptr<EquationParser::Node> EquationParser::ParseNumber(const ::std::string& expr) {
    ::std::istringstream iss(expr);
    double value;
    if (iss >> value) {
        auto node = ::std::make_unique<NumberNode>();
        node->value = value;
        return node;
    }
    return nullptr;
}

/**
 * Parses a named constant (e.g., pi, e)
 * 
 * @param expr The expression string potentially starting with a constant name
 * @return ConstantNode if a valid constant is found, nullptr otherwise
 */
::std::unique_ptr<EquationParser::Node> EquationParser::ParseConstant(const ::std::string& expr) {
    for (const auto& [name, value] : m_constants) {
        if (expr.substr(0, name.length()) == name) {
            auto node = ::std::make_unique<ConstantNode>();
            node->value = value;
            return node;
        }
    }
    return nullptr;
}

/**
 * Parses a function call (e.g., sin(x), pow(x,2))
 * 
 * Supports various mathematical functions and handles
 * special cases like the two-argument pow function.
 * 
 * @param expr The expression string starting with a function name
 * @return UnaryOpNode (or BinaryOpNode for pow) representing the function
 * @throws std::runtime_error for invalid function syntax or unknown functions
 */
::std::unique_ptr<EquationParser::Node> EquationParser::ParseFunction(const ::std::string& expr) {
    ::std::size_t parenStart = expr.find('(');
    if (parenStart == ::std::string::npos) {
        throw ::std::runtime_error("Invalid function syntax");
    }

    ::std::string funcName = expr.substr(0, parenStart);
    ::std::string arg = expr.substr(parenStart + 1, expr.find_last_of(')') - parenStart - 1);

    auto argNode = ParseExpression(arg);
    if (!argNode) return nullptr;

    auto unaryNode = ::std::make_unique<UnaryOpNode>();
    unaryNode->operand = ::std::move(argNode);

    if (funcName == "sin") {
        unaryNode->op = [](double x) { return ::std::sin(x); };
    } else if (funcName == "cos") {
        unaryNode->op = [](double x) { return ::std::cos(x); };
    } else if (funcName == "tan") {
        unaryNode->op = [](double x) { return ::std::tan(x); };
    } else if (funcName == "sqrt") {
        unaryNode->op = [](double x) { return ::std::sqrt(x); };
    } else if (funcName == "log") {
        unaryNode->op = [](double x) { return ::std::log(x); };
    } else if (funcName == "exp") {
        unaryNode->op = [](double x) { return ::std::exp(x); };
    } else if (funcName == "abs") {
        unaryNode->op = [](double x) { return ::std::abs(x); };
    } else if (funcName == "pow") {
        // For pow, we need to parse two arguments
        ::std::size_t comma = arg.find(',');
        if (comma == ::std::string::npos) {
            throw ::std::runtime_error("pow function requires two arguments");
        }
        auto base = ParseExpression(arg.substr(0, comma));
        auto exponent = ParseExpression(arg.substr(comma + 1));
        if (!base || !exponent) {
            throw ::std::runtime_error("Invalid arguments for pow function");
        }
        auto binaryNode = ::std::make_unique<BinaryOpNode>();
        binaryNode->left = ::std::move(base);
        binaryNode->right = ::std::move(exponent);
        binaryNode->op = [](double a, double b) { return ::std::pow(a, b); };
        return binaryNode;
    } else {
        throw ::std::runtime_error("Unknown function: " + funcName);
    }

    return unaryNode;
}

} // namespace plot_genius 