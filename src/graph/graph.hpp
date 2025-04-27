/**
 * Graph Component Header
 * 
 * Defines graph-related structures and classes for mathematical function plotting.
 */

#pragma once

#include <vector>
#include <memory>
#include "../equation/parser.hpp"

namespace plot_genius {

/**
 * Represents a single point in a 2D coordinate system
 */
struct Point {
    double x;  ///< X coordinate
    double y;  ///< Y coordinate
};

/**
 * Graph class for representing and evaluating mathematical functions
 * 
 * Handles equation parsing, evaluation, and point generation for plotting.
 */
class Graph {
public:
    /**
     * Default constructor
     */
    Graph();
    
    /**
     * Destructor
     */
    ~Graph();

    /**
     * Sets the equation to be plotted
     * 
     * @param equation Mathematical expression as a string
     * @return True if parsing was successful, false otherwise
     */
    bool SetEquation(const std::string& equation);

    /**
     * Evaluates the equation at a specific x value
     * 
     * @param x Input value
     * @return Result of the expression evaluated at x
     */
    double Evaluate(double x) const;

    /**
     * Generates a series of points for plotting within a specified range
     * 
     * @param xMin Minimum x value
     * @param xMax Maximum x value
     * @param numPoints Number of points to generate (default: 100)
     * @return Vector of points representing the function
     */
    std::vector<Point> GeneratePoints(double xMin, double xMax, int numPoints = 100) const;

    /**
     * Gets the last error message from the equation parser
     * 
     * @return Error message string
     */
    const std::string& GetLastError() const;

private:
    std::unique_ptr<EquationParser> m_parser;  ///< Equation parser instance
};

} // namespace plot_genius 