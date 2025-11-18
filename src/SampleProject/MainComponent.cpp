#include "MainComponent.h"
#include "easylogging++.h"
#include <sqlite3.h>

INITIALIZE_EASYLOGGINGPP

MainComponent::MainComponent()
{
    // Initialize logging
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime %level %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);

    LOG(INFO) << "MainComponent initialized";

    // Setup title label
    titleLabel.setText("JUCE Mini Template", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);

    // Setup info label
    infoLabel.setText("A cross-platform JUCE project template with CMake", juce::dontSendNotification);
    infoLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(infoLabel);

    // Setup button
    infoButton.setButtonText("Show Project Info");
    infoButton.onClick = [this] { showProjectInfo(); };
    addAndMakeVisible(infoButton);

    setSize(600, 400);
}

MainComponent::~MainComponent()
{
    LOG(INFO) << "MainComponent destroyed";
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(20);

    titleLabel.setBounds(area.removeFromTop(40));
    area.removeFromTop(10);
    infoLabel.setBounds(area.removeFromTop(30));
    area.removeFromTop(20);

    auto buttonArea = area.removeFromTop(40);
    infoButton.setBounds(buttonArea.withSizeKeepingCentre(200, 35));
}

void MainComponent::showProjectInfo()
{
    // Demonstrate nlohmann JSON usage
    nlohmann::json projectInfo = {
        {"name", "JUCE Mini Template"},
        {"version", "1.0.0"},
        {"cpp_standard", "C++20"},
        {"platforms", {"Windows", "macOS", "Linux"}},
        {"libraries", {
            {"JUCE", "Audio/GUI Framework"},
            {"nlohmann/json", "JSON Library"},
            {"Catch2", "Unit Testing"},
            {"easylogging++", "Logging"},
            {"SQLite", "Database"}
        }}
    };

    // Get SQLite version to demonstrate SQLite integration
    juce::String sqliteVersion = sqlite3_libversion();

    juce::String infoText;
    infoText << "Project: " << projectInfo["name"].get<std::string>() << "\n";
    infoText << "Version: " << projectInfo["version"].get<std::string>() << "\n";
    infoText << "C++ Standard: " << projectInfo["cpp_standard"].get<std::string>() << "\n";
    infoText << "SQLite Version: " << sqliteVersion << "\n\n";
    infoText << "Third-party Libraries:\n";

    for (const auto& [name, desc] : projectInfo["libraries"].items())
    {
        infoText << "  - " << name << ": " << desc.get<std::string>() << "\n";
    }

    LOG(INFO) << "Showing project info dialog";

    juce::AlertWindow::showMessageBoxAsync(
        juce::AlertWindow::InfoIcon,
        "Project Information",
        infoText,
        "OK"
    );
}
