#!/bin/bash

# Exit on any error
set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${GREEN}Checking dependencies...${NC}"

# Function to check if a package is installed
check_package() {
    if ! pacman -Qi "$1" &>/dev/null; then
        echo -e "${YELLOW}Package $1 is not installed. Installing...${NC}"
        sudo pacman -S --noconfirm "$1"
    else
        echo -e "${GREEN}Package $1 is installed${NC}"
    fi
}

# Required packages
packages=(
    "cmake"
    "glfw"
    "glm"
    "nlohmann-json"
    "gcc"
    "make"
)

# Check and install packages
for package in "${packages[@]}"; do
    check_package "$package"
done

# Check for GLAD
if [ ! -d "libs/glad" ]; then
    echo -e "${YELLOW}GLAD not found. Cloning...${NC}"
    mkdir -p libs
    git clone https://github.com/Dav1dde/glad.git libs/glad
    cd libs/glad
    python3 -m pip install --user glad
    python3 -m glad --generator=c --out-path=.
    cd ../..
fi

echo -e "${GREEN}All dependencies are installed!${NC}" 