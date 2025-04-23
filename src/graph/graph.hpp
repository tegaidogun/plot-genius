#pragma once

#include <vector>
#include <memory>
#include "../equation/parser.hpp"

namespace plot_genius {

struct Point {
    double x;
    double y;
};

class Graph {
public:
    Graph();
    ~Graph();

    // Set the equation to plot
    bool SetEquation(const std::string& equation);

    // Evaluate the equation at a given x value
    double Evaluate(double x) const;

    // Generate points for plotting
    std::vector<Point> GeneratePoints(double xMin, double xMax, int numPoints = 100) const;

    // Get the last error message
    const std::string& GetLastError() const;

private:
    std::unique_ptr<EquationParser> m_parser;
};

} // namespace plot_genius 