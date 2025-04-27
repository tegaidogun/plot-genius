#!/bin/bash

# Installation script for Plot Genius dependencies

# Detect package manager
if command -v apt &> /dev/null; then
    echo "Detected apt package manager (Debian/Ubuntu)"
    PACKAGE_MANAGER="apt"
    INSTALL_CMD="sudo apt update && sudo apt install -y"
    
    REQUIRED_PACKAGES="build-essential cmake libgl1-mesa-dev"
    WAYLAND_PACKAGES="libwayland-dev wayland-protocols libxkbcommon-dev libdecor-0-dev"
    X11_PACKAGES="xorg-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev"
    
elif command -v dnf &> /dev/null; then
    echo "Detected dnf package manager (Fedora/RHEL)"
    PACKAGE_MANAGER="dnf"
    INSTALL_CMD="sudo dnf install -y"
    
    REQUIRED_PACKAGES="gcc gcc-c++ cmake make mesa-libGL-devel"
    WAYLAND_PACKAGES="wayland-devel wayland-protocols-devel libxkbcommon-devel libdecor-devel"
    X11_PACKAGES="libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel"
    
elif command -v pacman &> /dev/null; then
    echo "Detected pacman package manager (Arch Linux)"
    PACKAGE_MANAGER="pacman"
    INSTALL_CMD="sudo pacman -Sy --needed"
    
    REQUIRED_PACKAGES="base-devel cmake mesa"
    WAYLAND_PACKAGES="wayland wayland-protocols libxkbcommon libdecor"
    X11_PACKAGES="libx11 libxrandr libxinerama libxcursor libxi"
    
else
    echo "Could not detect package manager. Please install the following dependencies manually:"
    echo "- C/C++ compiler (GCC or Clang)"
    echo "- CMake (version 3.10 or newer)"
    echo "- OpenGL development libraries"
    echo "- For Wayland support: wayland-dev, wayland-protocols, libxkbcommon-dev"
    echo "- Optional but recommended: libdecor-dev (for Wayland window decorations)"
    echo "- For X11 support: libx11-dev, libxrandr-dev, libxinerama-dev, libxcursor-dev, libxi-dev"
    exit 1
fi

# Install required packages
echo "Installing required packages: $REQUIRED_PACKAGES"
$INSTALL_CMD $REQUIRED_PACKAGES

# Ask for Wayland support
read -p "Do you want to install Wayland support? (y/n) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Installing Wayland packages: $WAYLAND_PACKAGES"
    $INSTALL_CMD $WAYLAND_PACKAGES
    
    echo "Note: libdecor is recommended but optional for proper Wayland window decorations."
    echo "If window decorations are missing, make sure libdecor is installed."
fi

# Ask for X11 support
read -p "Do you want to install X11 support? (y/n) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Installing X11 packages: $X11_PACKAGES"
    $INSTALL_CMD $X11_PACKAGES
fi

echo "All dependencies installed successfully."
echo ""
echo "You can now build the project with:"
echo "  ./build.sh" 