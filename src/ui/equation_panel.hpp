#pragma once

#include <string>
#include <vector>
#include <functional>

namespace plot_genius {

class EquationPanel {
public:
    EquationPanel();
    ~EquationPanel() = default;

    void Render();
    void SetEquationCallback(std::function<void(const std::string&)> callback);

private:
    std::string m_currentEquation;
    std::vector<std::string> m_history;
    std::function<void(const std::string&)> m_equationCallback;

    void DrawEquationInput();
    void DrawHistory();
};
} 