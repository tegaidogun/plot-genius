#pragma once

#include <string>
#include <functional>
#include <vector>

namespace plot_genius {

struct Equation {
    std::string expression;
    bool isActive{true};
    int id;
};

class EquationPanel {
public:
    EquationPanel();
    ~EquationPanel() = default;

    void Render();
    void SetEquationCallback(std::function<void(const std::string&)> callback);
    void SetRemoveCallback(std::function<void(int)> callback);
    void SetCurrentEquation(const std::string& equation);
    void DrawEquationInput();

private:
    void DrawEquationsList();
    void DrawHistory();
    bool ValidateEquation(const std::string& equation);
    void AddEquation(const std::string& equation);
    void RemoveEquation(int id);
    int GetNextEquationId();

    std::string m_currentEquation;
    std::vector<Equation> m_equations;
    std::vector<std::string> m_history;
    std::function<void(const std::string&)> m_equationCallback;
    std::function<void(int)> m_removeCallback;
    bool m_hasError{false};
    std::string m_errorMessage;
    int m_nextEquationId{0};
    char m_inputBuffer[256]{};
};

} // namespace plot_genius 