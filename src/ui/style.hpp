#pragma once

namespace plot_genius {

class Style {
public:
    static void SetDarkTheme();
    static void SetLightTheme();
    static void SetCustomTheme(float* bgColor, float* textColor, float* accentColor);
}; 