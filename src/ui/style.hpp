/**
 * UI Style Management Header
 * 
 * Defines a utility class for managing and applying application themes.
 */

#pragma once

namespace plot_genius {

/**
 * Style management class for UI theming
 */
class Style {
public:
    /**
     * Sets the application UI to dark theme
     */
    static void SetDarkTheme();
    
    /**
     * Sets the application UI to light theme
     */
    static void SetLightTheme();
    
    /**
     * Sets a custom theme using specified colors
     * 
     * @param bgColor Background color as RGBA float array
     * @param textColor Text color as RGBA float array
     * @param accentColor Accent color as RGBA float array
     */
    static void SetCustomTheme(float* bgColor, float* textColor, float* accentColor);
}; 