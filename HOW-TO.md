# HOW-TO Guide

This guide provides step-by-step instructions for common development tasks in the JUCE Mini Template project.

## Table of Contents

1. [Creating a New Project](#1-creating-a-new-project)
2. [Adding a New C++ Library](#2-adding-a-new-c-library)
3. [Writing Unit Tests](#3-writing-unit-tests)
4. [Adding Source Files to Existing Projects](#4-adding-source-files-to-existing-projects)
5. [Building the Project](#5-building-the-project)
6. [Running Tests](#6-running-tests)
7. [Using JUCE Modules](#7-using-juce-modules)
8. [Configuring Compile Definitions](#8-configuring-compile-definitions)
9. [Debugging](#9-debugging)
10. [Common Issues and Solutions](#10-common-issues-and-solutions)

---

## 1. Creating a New Project

To create a new JUCE application under the `src/` folder:

### Step 1: Create Project Directory

```bash
mkdir -p src/MyNewProject
```

### Step 2: Create CMakeLists.txt

Create `src/MyNewProject/CMakeLists.txt`:

```cmake
# Define the JUCE application
juce_add_gui_app(MyNewProject
    PRODUCT_NAME "My New Project"
    COMPANY_NAME "Your Company"
    BUNDLE_ID "com.yourcompany.mynewproject"
    VERSION "1.0.0"
)

# Add source files
target_sources(MyNewProject
    PRIVATE
        Main.cpp
        MainComponent.cpp
        MainComponent.h
)

# Link required libraries
target_link_libraries(MyNewProject
    PRIVATE
        # JUCE modules
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags

        # Third-party libraries (optional)
        # nlohmann_json::nlohmann_json
        # easyloggingpp
        # sqlite3
)

# Compile definitions
target_compile_definitions(MyNewProject
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:MyNewProject,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:MyNewProject,JUCE_VERSION>"
)
```

### Step 3: Create Main Application Files

Create `src/MyNewProject/Main.cpp`:

```cpp
#include <JuceHeader.h>
#include "MainComponent.h"

class MyNewProjectApplication : public juce::JUCEApplication
{
public:
    MyNewProjectApplication() {}

    const juce::String getApplicationName() override    { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override          { return true; }

    void initialise(const juce::String& commandLine) override
    {
        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name,
                             juce::Desktop::getInstance().getDefaultLookAndFeel()
                                 .findColour(juce::ResizableWindow::backgroundColourId),
                             DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);

            #if JUCE_IOS || JUCE_ANDROID
                setFullScreen(true);
            #else
                setResizable(true, true);
                centreWithSize(getWidth(), getHeight());
            #endif

            setVisible(true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(MyNewProjectApplication)
```

Create `src/MyNewProject/MainComponent.h`:

```cpp
#pragma once

#include <JuceHeader.h>

class MainComponent : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
```

Create `src/MyNewProject/MainComponent.cpp`:

```cpp
#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize(800, 600);
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);
    g.drawText("Hello, World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // Layout child components here
}
```

### Step 4: Register Project in Root CMakeLists.txt

Add the following line to the root `CMakeLists.txt` after the existing `add_subdirectory` calls:

```cmake
add_subdirectory(src/MyNewProject)
```

---

## 2. Adding a New C++ Library

### Option A: Adding as Git Submodule (Recommended)

This is the preferred method for external dependencies.

#### Step 1: Add the Submodule

```bash
cd src/third_party
git submodule add https://github.com/owner/library.git library_name
cd ../..
git submodule update --init --recursive
```

#### Step 2: Configure in third_party CMakeLists.txt

Edit `src/third_party/CMakeLists.txt` and add configuration based on library type:

**For Header-Only Libraries:**

```cmake
# Library Name Configuration
set(LIBRARY_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(library_name)
```

**For Libraries Requiring Compilation:**

```cmake
# Library Name - Static Library
add_library(library_name STATIC
    library_name/src/library.cpp
)

target_include_directories(library_name PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/library_name/include
)

# Optional: Add compile definitions
target_compile_definitions(library_name PRIVATE
    LIBRARY_SOME_OPTION=1
)
```

#### Step 3: Link to Your Project

In your project's `CMakeLists.txt`:

```cmake
target_link_libraries(MyProject
    PRIVATE
        library_name  # or library_name::library_name for namespaced targets
)
```

### Option B: Adding a Local Library

For custom libraries within the project:

#### Step 1: Create Library Directory

```bash
mkdir -p src/MyLibrary
```

#### Step 2: Create Library CMakeLists.txt

Create `src/MyLibrary/CMakeLists.txt`:

```cmake
add_library(MyLibrary STATIC
    src/MyLibrary.cpp
    include/MyLibrary.h
)

target_include_directories(MyLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set C++ standard
target_compile_features(MyLibrary PUBLIC cxx_std_20)
```

#### Step 3: Register and Link

Add to root `CMakeLists.txt`:

```cmake
add_subdirectory(src/MyLibrary)
```

Link in your project:

```cmake
target_link_libraries(MyProject PRIVATE MyLibrary)
```

---

## 3. Writing Unit Tests

This project uses [Catch2](https://github.com/catchorg/Catch2) for unit testing.

### Step 1: Create Test File

Create a new test file in the `src/tests/` directory:

```bash
touch src/tests/test_myfeature.cpp
```

### Step 2: Write Test Cases

Example `src/tests/test_myfeature.cpp`:

```cpp
#include <catch2/catch_all.hpp>

// Include headers for code you're testing
// #include "MyLibrary.h"

TEST_CASE("MyFeature basic functionality", "[myfeature]")
{
    SECTION("Test case 1")
    {
        int result = 2 + 2;
        REQUIRE(result == 4);
    }

    SECTION("Test case 2")
    {
        std::string str = "hello";
        REQUIRE(str.length() == 5);
        REQUIRE(str == "hello");
    }
}

TEST_CASE("MyFeature edge cases", "[myfeature][edge]")
{
    SECTION("Empty input")
    {
        std::vector<int> empty;
        REQUIRE(empty.empty());
    }

    SECTION("Boundary values")
    {
        int max_val = std::numeric_limits<int>::max();
        REQUIRE(max_val > 0);
    }
}

TEST_CASE("MyFeature with fixtures", "[myfeature]")
{
    // Setup
    std::vector<int> data = {1, 2, 3, 4, 5};

    SECTION("Sum calculation")
    {
        int sum = 0;
        for (int val : data) sum += val;
        REQUIRE(sum == 15);
    }

    SECTION("Find element")
    {
        auto it = std::find(data.begin(), data.end(), 3);
        REQUIRE(it != data.end());
        REQUIRE(*it == 3);
    }
}
```

### Step 3: Register Test File

Edit `src/tests/CMakeLists.txt` to add your test file:

```cmake
add_executable(SampleProjectTests
    test_main.cpp
    test_sample.cpp
    test_myfeature.cpp  # Add your new test file here
)
```

### Step 4: Link Required Libraries

If your tests need additional libraries, add them to `src/tests/CMakeLists.txt`:

```cmake
target_link_libraries(SampleProjectTests
    PRIVATE
        Catch2::Catch2
        nlohmann_json::nlohmann_json
        easyloggingpp
        sqlite3
        MyLibrary  # Add your library here
)
```

### Step 5: Run Tests

```bash
# Build and run all tests
cd build
cmake --build .
ctest --output-on-failure

# Run specific test tags
./bin/SampleProjectTests "[myfeature]"

# Run with verbose output
./bin/SampleProjectTests -v

# List all tests
./bin/SampleProjectTests --list-tests
```

### Catch2 Common Assertions

```cpp
REQUIRE(expression);           // Must be true, fails test if false
CHECK(expression);             // Checks but continues on failure
REQUIRE_FALSE(expression);     // Must be false
REQUIRE_THROWS(expression);    // Must throw an exception
REQUIRE_NOTHROW(expression);   // Must not throw
REQUIRE_THAT(value, matcher);  // Use Catch2 matchers
```

---

## 4. Adding Source Files to Existing Projects

### Step 1: Create Your Files

```bash
touch src/SampleProject/NewFeature.h
touch src/SampleProject/NewFeature.cpp
```

### Step 2: Update CMakeLists.txt

Edit `src/SampleProject/CMakeLists.txt`:

```cmake
target_sources(SampleProject
    PRIVATE
        Main.cpp
        MainComponent.cpp
        MainComponent.h
        NewFeature.cpp    # Add new files
        NewFeature.h
)
```

### Step 3: Include in Your Code

```cpp
#include "NewFeature.h"
```

---

## 5. Building the Project

### Initial Setup

```bash
# Clone with submodules
git clone --recursive <repository-url>
cd juce-mini

# Or if already cloned
git submodule update --init --recursive
```

### Build Commands

**Linux/macOS:**

```bash
mkdir build && cd build
cmake ..
cmake --build . --parallel
```

**macOS Universal Binary:**

```bash
mkdir build && cd build
cmake .. -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"
cmake --build . --parallel
```

**Windows (Visual Studio):**

```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### Build Types

```bash
# Debug build (default)
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Release build
cmake .. -DCMAKE_BUILD_TYPE=Release

# Release with debug info
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
```

### Build Output

- Executables: `build/bin/`
- Libraries: `build/lib/`

---

## 6. Running Tests

### Using CTest

```bash
cd build
cmake --build .
ctest --output-on-failure

# Verbose output
ctest -V

# Run specific test by name
ctest -R "test_name"

# Run tests in parallel
ctest -j4
```

### Running Test Executable Directly

```bash
# Run all tests
./build/bin/SampleProjectTests

# Run tests by tag
./build/bin/SampleProjectTests "[json]"
./build/bin/SampleProjectTests "[sqlite]"

# Exclude tags
./build/bin/SampleProjectTests "~[slow]"

# Combine tags (AND)
./build/bin/SampleProjectTests "[feature][unit]"

# List available tests
./build/bin/SampleProjectTests --list-tests

# List available tags
./build/bin/SampleProjectTests --list-tags
```

---

## 7. Using JUCE Modules

### Available JUCE Modules

Common modules and their purposes:

| Module | Purpose |
|--------|---------|
| `juce_core` | Core utilities, strings, files, threads |
| `juce_events` | Message loop, timers, async callbacks |
| `juce_graphics` | 2D rendering, fonts, images |
| `juce_gui_basics` | UI components, layout |
| `juce_gui_extra` | Additional UI (code editor, web browser) |
| `juce_audio_basics` | Audio buffers, MIDI |
| `juce_audio_devices` | Audio hardware I/O |
| `juce_audio_formats` | Audio file reading/writing |
| `juce_audio_processors` | Plugin hosting/development |
| `juce_audio_utils` | Audio UI components |
| `juce_dsp` | DSP algorithms |

### Adding Modules to Your Project

In your project's `CMakeLists.txt`:

```cmake
target_link_libraries(MyProject
    PRIVATE
        juce::juce_core
        juce::juce_dsp              # Add DSP module
        juce::juce_opengl           # Add OpenGL module
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
```

### Including JUCE Headers

```cpp
#include <JuceHeader.h>  // Includes all linked modules
// or
#include <juce_core/juce_core.h>  // Specific module
```

---

## 8. Configuring Compile Definitions

### Project-Specific Definitions

In your project's `CMakeLists.txt`:

```cmake
target_compile_definitions(MyProject
    PRIVATE
        # JUCE configuration
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_MODAL_LOOPS_PERMITTED=1

        # Custom definitions
        MY_CUSTOM_FLAG=1
        VERSION_STRING="1.0.0"

        # Debug-only definitions
        $<$<CONFIG:Debug>:DEBUG_MODE=1>

        # Release-only definitions
        $<$<CONFIG:Release>:NDEBUG>
)
```

### Global Definitions

In root `CMakeLists.txt`:

```cmake
add_compile_definitions(
    PROJECT_NAME="JUCEMiniTemplate"
)
```

---

## 9. Debugging

### Debug Build

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

### Using GDB (Linux)

```bash
gdb ./build/bin/SampleProject
(gdb) run
(gdb) bt  # Backtrace on crash
```

### Using LLDB (macOS)

```bash
lldb ./build/bin/SampleProject.app/Contents/MacOS/SampleProject
(lldb) run
(lldb) bt  # Backtrace on crash
```

### Using Visual Studio (Windows)

Open the generated `.sln` file in `build/` directory and use the integrated debugger.

### JUCE Debugging Tips

```cpp
// Debug output
DBG("Debug message: " << variable);

// Assertions
jassert(condition);  // Debug-only assertion

// Break into debugger
JUCE_BREAK_IN_DEBUGGER;
```

---

## 10. Common Issues and Solutions

### Issue: Submodules Not Initialized

**Symptoms:** Build fails with missing headers or CMake errors about missing directories.

**Solution:**
```bash
git submodule update --init --recursive
```

### Issue: Linux Dependencies Missing

**Symptoms:** CMake fails to find ALSA, FreeType, or other system libraries.

**Solution:**

Ubuntu/Debian:
```bash
sudo apt-get install libasound2-dev libfreetype6-dev libcurl4-openssl-dev \
    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev \
    mesa-common-dev libopengl-dev libxcomposite-dev
```

Fedora:
```bash
sudo dnf install alsa-lib-devel freetype-devel libcurl-devel \
    libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel \
    mesa-libGL-devel libXcomposite-devel
```

### Issue: CMake Version Too Old

**Symptoms:** CMake errors about unsupported features.

**Solution:** Install CMake 3.22 or newer:
```bash
# Ubuntu
sudo apt-get install cmake

# Or download from cmake.org
```

### Issue: C++20 Features Not Available

**Symptoms:** Compilation errors for C++20 syntax.

**Solution:** Ensure you have a C++20 compatible compiler:
- GCC 10+
- Clang 10+
- MSVC 2019 16.8+

### Issue: Linker Errors for Third-Party Libraries

**Symptoms:** Undefined reference errors.

**Solution:** Verify library is:
1. Built before your project (check `add_subdirectory` order)
2. Linked in `target_link_libraries`
3. Using correct target name

### Issue: Tests Not Discovered

**Symptoms:** CTest reports no tests found.

**Solution:**
1. Ensure `catch_discover_tests()` is called in `src/tests/CMakeLists.txt`
2. Rebuild the project
3. Check that Catch2 is properly linked

---

## Quick Reference

### Create New Project Checklist

- [ ] Create directory under `src/`
- [ ] Create `CMakeLists.txt` with `juce_add_gui_app`
- [ ] Create `Main.cpp` with application class
- [ ] Create `MainComponent.h` and `MainComponent.cpp`
- [ ] Add `add_subdirectory()` to root `CMakeLists.txt`

### Add Library Checklist

- [ ] Add as git submodule or create local library
- [ ] Configure in `src/third_party/CMakeLists.txt`
- [ ] Link in project's `target_link_libraries`

### Add Tests Checklist

- [ ] Create test file in `src/tests/`
- [ ] Write test cases with `TEST_CASE` and `SECTION`
- [ ] Add file to `src/tests/CMakeLists.txt`
- [ ] Link required libraries
- [ ] Run with `ctest` or test executable
