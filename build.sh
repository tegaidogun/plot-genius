#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to build directory
cd build

# Configure CMake
cmake ..

# Build the project
make

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "You can run the application with: ./build/plot_genius"
else
    echo "Build failed!"
fi 