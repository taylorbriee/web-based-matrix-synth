#pragma once
#include <JuceHeader.h>

class TextComponent : public juce::Component
{
public:
    TextComponent(const juce::String& text);
    void paint(juce::Graphics& g) override;

private:
    juce::String labelText;
};

