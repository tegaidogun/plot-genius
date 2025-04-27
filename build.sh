#!/bin/bash

# Function to check if git submodules exist
check_submodules() {
    if [ ! -f "thirdparty/glfw/CMakeLists.txt" ] || [ ! -d "thirdparty/glm/glm" ] || [ ! -d "thirdparty/imgui" ]; then
        echo "Initializing git submodules..."
        git submodule update --init --recursive
    fi
}

# Parse command line arguments
USE_SYSTEM_DEPS=false
WITH_WAYLAND=true   # Default to Wayland
WITHOUT_X11=true    # Default to no X11
BUILD_TYPE="Debug"

for arg in "$@"; do
    case $arg in
        --system-deps)
            USE_SYSTEM_DEPS=true
            shift
            ;;
        --with-x11)
            WITHOUT_X11=false
            shift
            ;;
        --without-wayland)
            WITH_WAYLAND=false
            WITHOUT_X11=false  # If disabling Wayland, enable X11
            shift
            ;;
        --release)
            BUILD_TYPE="Release"
            shift
            ;;
    esac
done

# Check and initialize submodules if needed
check_submodules

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to build directory
cd build

# Configure CMake with appropriate options
CMAKE_OPTIONS="-DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=${BUILD_TYPE}"

if [ "$USE_SYSTEM_DEPS" = true ]; then
    CMAKE_OPTIONS="$CMAKE_OPTIONS -DUSE_SYSTEM_PACKAGES=ON"
    echo "Using system-installed dependencies..."
else
    CMAKE_OPTIONS="$CMAKE_OPTIONS -DUSE_SYSTEM_PACKAGES=OFF"
    echo "Using bundled dependencies from thirdparty directory..."
fi

# Set Wayland/X11 options
CMAKE_OPTIONS="$CMAKE_OPTIONS -DWITH_WAYLAND=${WITH_WAYLAND}"
CMAKE_OPTIONS="$CMAKE_OPTIONS -DWITHOUT_X11=${WITHOUT_X11}"

if [ "$WITH_WAYLAND" = true ]; then
    echo "Building with Wayland support (default)..."
else
    echo "Building without Wayland support..."
fi

if [ "$WITHOUT_X11" = true ]; then
    echo "Building without X11 support (default)..."
else
    echo "Including X11 support..."
fi

echo "CMake options: $CMAKE_OPTIONS"
cmake $CMAKE_OPTIONS ..

# Build the project
make -j$(nproc)

# Create symlink to compile_commands.json in the root directory for IDE support
if [ -f "compile_commands.json" ]; then
    echo "Creating compile_commands.json symlink for IDE integration..."
    cd ..
    ln -sf build/compile_commands.json .
    cd build
fi

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "You can run the application with: ./plot_genius"
else
    echo "Build failed!"
fi 