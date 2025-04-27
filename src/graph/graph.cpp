/**
 * Graph Component Implementation
 * 
 * Implementation of the Graph class for equation parsing, evaluation,
 * and point generation for plotting mathematical functions.
 */

#include "graph.hpp"
#include "../core/logger.hpp"
#include "../equation/parser.hpp"

namespace plot_genius {

Graph::Graph() : m_parser(::std::make_unique<EquationParser>()) {}

Graph::~Graph() = default;

/**
 * Sets the equation to be plotted
 * 
 * Forwards the equation string to the parser for processing
 * 
 * @param equation Mathematical expression as a string
 * @return True if parsing was successful, false otherwise
 */
bool Graph::SetEquation(const ::std::string& equation) {
    return m_parser->Parse(equation);
}

/**
 * Evaluates the equation at a specific x value
 * 
 * @param x Input value
 * @return Result of the expression evaluated at x
 */
double Graph::Evaluate(double x) const {
    return m_parser->Evaluate(x);
}

/**
 * Generates a series of points for plotting within a specified range
 * 
 * Divides the x-range into equal intervals and evaluates the function at each point.
 * Handles evaluation errors by logging and skipping invalid points.
 * 
 * @param xMin Minimum x value
 * @param xMax Maximum x value
 * @param numPoints Number of points to generate
 * @return Vector of points representing the function
 */
::std::vector<Point> Graph::GeneratePoints(double xMin, double xMax, int numPoints) const {
    ::std::vector<Point> points;
    points.reserve(numPoints);

    // Calculate step size for even distribution of points
    double step = (xMax - xMin) / (numPoints - 1);
    for (int i = 0; i < numPoints; ++i) {
        double x = xMin + i * step;
        try {
            double y = Evaluate(x);
            points.push_back({x, y});
        } catch (const ::std::exception& e) {
            core::Logger::GetInstance().Error("Failed to evaluate point at x = {}: {}", x, e.what());
            // Skip invalid points
            continue;
        }
    }

    return points;
}

/**
 * Gets the last error message from the equation parser
 * 
 * @return Error message string
 */
const ::std::string& Graph::GetLastError() const {
    return m_parser->GetLastError();
}

} // namespace plot_genius 