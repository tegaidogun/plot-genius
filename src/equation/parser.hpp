#pragma once

#include <string>
#include <memory>
#include <functional>
#include <cmath>
#include <stdexcept>

namespace plot_genius {

class EquationParser {
public:
    EquationParser();
    ~EquationParser() = default;

    // Parse an equation string and validate its syntax
    bool Parse(const ::std::string& equation);

    // Evaluate the equation at a specific x value
    double Evaluate(double x) const;

    // Get the last error message if parsing failed
    const ::std::string& GetLastError() const { return m_lastError; }

private:
    // Internal node types for the expression tree
    struct Node {
        virtual ~Node() = default;
        virtual double Evaluate(double x) const = 0;
    };

    struct NumberNode : public Node {
        double value;
        double Evaluate(double x) const override { return value; }
    };

    struct VariableNode : public Node {
        double Evaluate(double x) const override { return x; }
    };

    struct BinaryOpNode : public Node {
        ::std::unique_ptr<Node> left;
        ::std::unique_ptr<Node> right;
        ::std::function<double(double, double)> op;
        double Evaluate(double x) const override {
            return op(left->Evaluate(x), right->Evaluate(x));
        }
    };

    struct UnaryOpNode : public Node {
        ::std::unique_ptr<Node> operand;
        ::std::function<double(double)> op;
        double Evaluate(double x) const override {
            return op(operand->Evaluate(x));
        }
    };

    ::std::unique_ptr<Node> m_root;
    ::std::string m_lastError;

    // Helper functions for parsing
    ::std::unique_ptr<Node> ParseExpression(const ::std::string& expr);
    ::std::unique_ptr<Node> ParseTerm(const ::std::string& expr);
    ::std::unique_ptr<Node> ParseFactor(const ::std::string& expr);
    ::std::unique_ptr<Node> ParseNumber(const ::std::string& expr);
    ::std::unique_ptr<Node> ParseFunction(const ::std::string& expr);
};

} // namespace plot_genius 