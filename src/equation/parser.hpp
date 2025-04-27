/**
 * Equation Parser Header
 * 
 * Defines the EquationParser class that parses and evaluates mathematical expressions.
 * Uses a recursive descent parsing approach to build an abstract syntax tree (AST).
 */

#pragma once

#include <string>
#include <memory>
#include <functional>
#include <cmath>
#include <stdexcept>
#include <map>

namespace plot_genius {

/**
 * Class for parsing and evaluating mathematical expressions
 * 
 * Implements a recursive descent parser that constructs an abstract syntax tree (AST)
 * from the input expression, then evaluates the tree for specific x values.
 */
class EquationParser {
public:
    /**
     * Constructor that initializes the parser with common mathematical constants
     */
    EquationParser();
    
    /**
     * Default destructor
     */
    ~EquationParser() = default;

    /**
     * Parses a mathematical expression string into an AST
     * 
     * @param equation The mathematical expression to parse
     * @return True if parsing was successful, false otherwise
     */
    bool Parse(const ::std::string& equation);

    /**
     * Evaluates the parsed expression at a specific x value
     * 
     * @param x The value to substitute for the variable x
     * @return The result of evaluating the expression
     * @throws std::runtime_error if the expression is invalid or empty
     */
    double Evaluate(double x) const;

    /**
     * Returns the error message from the last parsing operation
     * 
     * @return Error message string
     */
    const ::std::string& GetLastError() const { return m_lastError; }

private:
    /**
     * Base abstract node class for the expression tree
     */
    struct Node {
        virtual ~Node() = default;
        
        /**
         * Evaluates this node with the given x value
         * 
         * @param x The value to substitute for variables
         * @return The computed result
         */
        virtual double Evaluate(double x) const = 0;
    };

    /**
     * Node representing a numeric literal
     */
    struct NumberNode : public Node {
        double value;  ///< The numeric value
        
        /**
         * Returns the stored numeric value
         * 
         * @param x Unused parameter (required by interface)
         * @return The stored numeric value
         */
        double Evaluate([[maybe_unused]] double x) const override { return value; }
    };

    /**
     * Node representing a variable (x)
     */
    struct VariableNode : public Node {
        /**
         * Returns the value of x
         * 
         * @param x The variable value
         * @return The value of x
         */
        double Evaluate(double x) const override { return x; }
    };

    /**
     * Node representing a binary operation (e.g., addition, multiplication)
     */
    struct BinaryOpNode : public Node {
        ::std::unique_ptr<Node> left;   ///< Left operand
        ::std::unique_ptr<Node> right;  ///< Right operand
        ::std::function<double(double, double)> op;  ///< Operation function
        
        /**
         * Evaluates both operands and applies the operation
         * 
         * @param x The value to substitute for variables
         * @return The result of the operation
         */
        double Evaluate(double x) const override {
            return op(left->Evaluate(x), right->Evaluate(x));
        }
    };

    /**
     * Node representing a unary operation (e.g., negation, functions)
     */
    struct UnaryOpNode : public Node {
        ::std::unique_ptr<Node> operand;  ///< The operand
        ::std::function<double(double)> op;  ///< Operation function
        
        /**
         * Evaluates the operand and applies the operation
         * 
         * @param x The value to substitute for variables
         * @return The result of the operation
         */
        double Evaluate(double x) const override {
            return op(operand->Evaluate(x));
        }
    };

    /**
     * Node representing a mathematical constant (e.g., π, e)
     */
    struct ConstantNode : public Node {
        double value;  ///< The constant value
        
        /**
         * Returns the stored constant value
         * 
         * @param x Unused parameter (required by interface)
         * @return The constant value
         */
        double Evaluate([[maybe_unused]] double x) const override { return value; }
    };

    ::std::unique_ptr<Node> m_root;  ///< Root node of the expression tree
    ::std::string m_lastError;  ///< Last parsing error message
    ::std::map<::std::string, double> m_constants;  ///< Map of named constants

    /**
     * Parses a complete expression (lowest precedence: addition/subtraction)
     * 
     * @param expr The expression string
     * @return Pointer to the root node of the parsed expression
     */
    ::std::unique_ptr<Node> ParseExpression(const ::std::string& expr);
    
    /**
     * Parses a term (medium precedence: multiplication/division)
     * 
     * @param expr The expression string
     * @return Pointer to the root node of the parsed term
     */
    ::std::unique_ptr<Node> ParseTerm(const ::std::string& expr);
    
    /**
     * Parses a factor (highest precedence: exponentiation, functions, parentheses)
     * 
     * @param expr The expression string
     * @return Pointer to the root node of the parsed factor
     */
    ::std::unique_ptr<Node> ParseFactor(const ::std::string& expr);
    
    /**
     * Parses a numeric literal
     * 
     * @param expr The expression string
     * @return Pointer to a NumberNode representing the parsed value
     */
    ::std::unique_ptr<Node> ParseNumber(const ::std::string& expr);
    
    /**
     * Parses a function call (e.g., sin, cos)
     * 
     * @param expr The expression string
     * @return Pointer to a UnaryOpNode representing the function
     */
    ::std::unique_ptr<Node> ParseFunction(const ::std::string& expr);
    
    /**
     * Parses a named constant (e.g., pi, e)
     * 
     * @param expr The expression string
     * @return Pointer to a ConstantNode with the corresponding value
     */
    ::std::unique_ptr<Node> ParseConstant(const ::std::string& expr);
    
    /**
     * Validates the overall format of the equation
     * 
     * @param equation The equation string to validate
     * @return True if the format is valid, false otherwise
     */
    bool ValidateEquationFormat(const ::std::string& equation);
    
    /**
     * Initializes the map of mathematical constants
     */
    void InitializeConstants();
};

} // namespace plot_genius 