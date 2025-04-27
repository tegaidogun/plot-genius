# Plot Genius

A mathematical function plotting application with a modern GUI.

## Dependencies

All third-party libraries are now included in the thirdparty directory and will be built automatically:
- GLM (included)
- ImGui (included)
- GLAD (included)
- GLFW (included)

The only system dependencies are:
- OpenGL
- Wayland development libraries
- X11 development libraries

### Installing System Dependencies [Only for Linux]

We provide a script to install the necessary system dependencies:

```bash
# Run the dependency installation script for linux
./install_dependencies_linux.sh
```

This script detects your package manager (apt, dnf, or pacman) and installs the required packages.

## Building

```bash
# Clone the repository with submodules
git clone --recursive https://github.com/yourusername/plot-genius.git
cd plot-genius

# Install system dependencies for linux (if not already installed)
./install_dependencies_linux.sh

# Option 1: Build using the script (recommended)
./build.sh

# Option 2: Manual build
mkdir -p build && cd build
cmake ..
make -j$(nproc)
```

### Using System Libraries

If you prefer to use system-installed libraries instead of the bundled ones:

```bash
# Using the build script
./build.sh --system-deps

# Or manually
mkdir -p build && cd build
cmake -DUSE_SYSTEM_PACKAGES=ON ..
make -j$(nproc)
```

In this case, you'll need to have the following packages installed:
- GLFW3
- GLM

## Features

- Interactive equation editor with syntax highlighting
- Real-time graph visualization
- Support for multiple equations simultaneously
- Customizable graph appearance

## Build Options

The following options can be passed to `./build.sh`:

- `--system-deps`: Use system-provided libraries instead of bundled ones
- `--with-x11`: Include X11 support (Wayland is used by default)
- `--without-wayland`: Disable Wayland support and use X11 instead
- `--release`: Build in Release mode instead of Debug

By default, the application is built with **Wayland support only**. X11 support is only
included when explicitly requested with `--with-x11`.

Example:
```bash
# Default build with Wayland only
./build.sh

# Build with both Wayland and X11 support
./build.sh --with-x11

# Build with X11 only (no Wayland)
./build.sh --without-wayland
```

## Running the Application

After building, you can run the application with:

```bash
cd build
./plot_genius
```

## Planned Features

- Real-time equation parsing and plotting
- Interactive pan and zoom
- Configurable axes and grid
- Theme presets (light/dark)
- Export to PNG
- Fixing the X11 support

## Project Structure

```
plot-genius/
├── src/          # Source code
├── thirdparty/   # Third-party dependencies
└── docs/         # Documentation
```

## License

MIT License 

## Prerequisites

- C++ compiler with C++17 support (GCC 8+ or Clang 6+)
- CMake 3.10 or higher
- OpenGL development libraries

### For Wayland support:
- libwayland-dev
- wayland-protocols
- libxkbcommon-dev

### For X11 support:
- libx11-dev
- libxrandr-dev
- libxinerama-dev
- libxcursor-dev
- libxi-dev

## Building from Source

### Automatic setup

The easiest way to build Plot Genius is to use our provided scripts:

```bash
# Install dependencies for Linux
./install_dependencies_linux.sh

# Build the project
./build.sh
```

The install script will detect your package manager and install the required dependencies.

### Building manually

If you prefer to build manually:

```bash
# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build
make -j$(nproc)
```

### Build options

The following options can be passed to `./build.sh` or directly to CMake:

- `--system-deps`: Use system-provided libraries instead of bundled ones
- `--with-wayland`: Build with Wayland support (default)
- `--without-wayland`: Disable Wayland support
- `--without-x11`: Disable X11 support
- `--force-x11`: Force X11 backend even when running under Wayland
- `--release`: Build in Release mode instead of Debug

Example:
```bash
./build.sh --with-wayland --system-deps
```

## Notes on Wayland Support

The application uses Wayland by default on systems that support it. 