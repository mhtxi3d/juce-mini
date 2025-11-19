# JUCE LookAndFeel Libraries

This folder contains third-party JUCE LookAndFeel libraries and modules for custom GUI theming.

## Libraries Included

### 1. nt_lookandfeel
- **Source**: https://github.com/jonathonracz/nt_lookandfeel
- **Description**: A JUCE module which aims to be a total replacement of the default LookAndFeel classes. Follows modern UI trends with flat, dark background and light text design.
- **Features**:
  - Platform-native styling when available
  - Modern flat UI aesthetic
  - Dark theme with light text
  - Platform-agnostic design
- **Usage**: Add as a JUCE module via CMake or Projucer

### 2. foleys_gui_magic
- **Source**: https://github.com/ffAudio/foleys_gui_magic
- **Description**: A GUI builder module for JUCE with runtime editing capabilities and advanced styling options.
- **License**: BSD V2 3-clause (permissive, can be used in commercial products)
- **Features**:
  - Runtime GUI editing
  - Custom LookAndFeel registration
  - Signal visualizers (levels, spectra)
  - No extra coding for visualizations
- **Documentation**: https://foleysfinest.com/developer/pluginguimagic/
- **Usage**: Add the `modules/foleys_gui_magic` module to your project

### 3. Juce-look-and-feel-examples
- **Source**: https://github.com/audioplastic/Juce-look-and-feel-examples
- **Description**: Example implementations showing how to create custom LookAndFeel classes in JUCE.
- **Features**:
  - Custom color schemes for sliders, buttons, scroll bars
  - Example code for various widget customizations
  - Good learning resource for LookAndFeel development
- **Usage**: Reference code for creating your own custom LookAndFeel

## Integration

### CMake Integration

To add these modules to your CMake project:

```cmake
# For nt_lookandfeel
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/JUCE_LookAndFeel/nt_lookandfeel/nt_lookandfeel)

# For foleys_gui_magic
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/JUCE_LookAndFeel/foleys_gui_magic/modules/foleys_gui_magic)
```

### Basic Usage Example

```cpp
#include <nt_lookandfeel/nt_lookandfeel.h>

// In your application
nt::LookAndFeel lookAndFeel;
LookAndFeel::setDefaultLookAndFeel(&lookAndFeel);
```

## Additional Resources

- [JUCE LookAndFeel Tutorial](https://docs.juce.com/master/tutorial_look_and_feel_customisation.html)
- [LookAndFeel_V4 Documentation](https://docs.juce.com/master/classLookAndFeel__V4.html)
