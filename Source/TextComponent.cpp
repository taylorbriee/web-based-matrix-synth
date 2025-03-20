#include "TextComponent.h"

TextComponent::TextComponent(const juce::String& text) : labelText(text) {}

void TextComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    g.setFont(16.0f);
    g.drawText(labelText, getLocalBounds(), juce::Justification::centred, true);

//    DBG("TextComponent paint() called");
}

