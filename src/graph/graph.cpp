#include "graph.hpp"
#include "../core/logger.hpp"
#include "../equation/parser.hpp"

namespace plot_genius {

Graph::Graph() : m_parser(::std::make_unique<EquationParser>()) {}

Graph::~Graph() = default;

bool Graph::SetEquation(const ::std::string& equation) {
    return m_parser->Parse(equation);
}

double Graph::Evaluate(double x) const {
    return m_parser->Evaluate(x);
}

::std::vector<Point> Graph::GeneratePoints(double xMin, double xMax, int numPoints) const {
    ::std::vector<Point> points;
    points.reserve(numPoints);

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

const ::std::string& Graph::GetLastError() const {
    return m_parser->GetLastError();
}

} // namespace plot_genius 