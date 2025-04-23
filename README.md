# Plot Genius

A mathematical function plotting application with a modern GUI.

## Dependencies

- GLM (included)
- ImGui (included)
- GLAD (included)
- OpenGL (will be pre-included at a later date)
- GLFW (will be pre-included at a later date)

## Building

```bash
# Clone the repository with submodules
git clone --recursive https://github.com/yourusername/plot-genius.git
cd plot-genius

# Create build directory and build
mkdir build && cd build
cmake ..
make
```

## Features

- Real-time equation parsing and plotting
- Interactive pan and zoom
- Configurable axes and grid
- Theme presets (light/dark)
- Export to PNG

## Project Structure

```
plot-genius/
├── src/          # Source code
├── thirdparty/   # Third-party dependencies
└── docs/         # Documentation
```

## License

MIT License 