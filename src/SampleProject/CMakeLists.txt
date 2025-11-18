# SampleProject CMake configuration

juce_add_gui_app(SampleProject
    PRODUCT_NAME "Sample Project"
    COMPANY_NAME "JUCE Mini"
    BUNDLE_ID "com.jucemini.sampleproject"
    VERSION "1.0.0"
)

target_sources(SampleProject PRIVATE
    Main.cpp
    MainComponent.cpp
    MainComponent.h
)

target_compile_definitions(SampleProject PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:SampleProject,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:SampleProject,JUCE_VERSION>"
)

target_link_libraries(SampleProject PRIVATE
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    nlohmann_json::nlohmann_json
    easyloggingpp
    sqlite3
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
