# JUCE Mini Template

A cross-platform JUCE project template using CMake and C++20.

## Features

- Cross-platform support (Windows, macOS, Linux)
- C++20 standard
- CMake build system
- Git submodules for dependency management

## Third-Party Libraries

- **JUCE** - Audio/GUI application framework
- **nlohmann/json** - Modern C++ JSON library
- **Catch2** - Unit testing framework
- **Easylogging++** - Single header C++ logging library
- **SQLite** - Embedded SQL database engine

## Directory Structure

```
juce-mini/
├── CMakeLists.txt          # Root CMake configuration
├── src/
│   ├── SampleProject/      # Sample JUCE application
│   ├── third_party/        # Git submodules
│   └── script/             # Build scripts
├── tests/                  # Unit tests
└── util/
    └── bin/                # Utility binaries
```

## Building

### Prerequisites

- CMake 3.22 or later
- C++20 compatible compiler
- Platform-specific dependencies:
  - **Linux**: ALSA, FreeType2, libcurl
  - **macOS**: Xcode command line tools
  - **Windows**: Visual Studio 2019 or later

### Clone with Submodules

```bash
git clone --recursive <repository-url>
cd juce-mini
```

If you already cloned without `--recursive`:

```bash
git submodule update --init --recursive
```

### Build Commands

```bash
# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build .

# Run tests
ctest --output-on-failure
```

### Platform-Specific Notes

#### Linux

Install dependencies:

```bash
# Ubuntu/Debian
sudo apt-get install libasound2-dev libfreetype6-dev libcurl4-openssl-dev

# Fedora
sudo dnf install alsa-lib-devel freetype-devel libcurl-devel
```

#### macOS

```bash
# Universal binary (x86_64 + arm64)
cmake -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" ..
```

#### Windows

```bash
# Visual Studio generator
cmake -G "Visual Studio 17 2022" -A x64 ..
```

## License

See individual library licenses in their respective directories.
