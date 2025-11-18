#pragma once

#include <JuceHeader.h>
#include <nlohmann/json.hpp>

class MainComponent : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::TextButton infoButton;
    juce::Label titleLabel;
    juce::Label infoLabel;

    void showProjectInfo();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
