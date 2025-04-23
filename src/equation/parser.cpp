#include "parser.hpp"
#include <cmath>
#include <sstream>
#include <cctype>
#include <stdexcept>

namespace plot_genius {

EquationParser::EquationParser() : m_root(nullptr) {}

bool EquationParser::Parse(const ::std::string& equation) {
    try {
        m_root = ParseExpression(equation);
        return m_root != nullptr;
    } catch (const ::std::exception& e) {
        m_lastError = e.what();
        return false;
    }
}

double EquationParser::Evaluate(double x) const {
    if (!m_root) {
        throw ::std::runtime_error("No equation has been parsed yet");
    }
    return m_root->Evaluate(x);
}

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

    // Handle functions
    if (::std::isalpha(expr[0])) {
        return ParseFunction(expr);
    }

    return nullptr;
}

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
    } else {
        throw ::std::runtime_error("Unknown function: " + funcName);
    }

    return unaryNode;
}

} // namespace plot_genius 