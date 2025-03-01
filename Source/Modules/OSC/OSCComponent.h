/*
  ==============================================================================

    OSCComponent.h
    Created: 25 Feb 2025 6:34:01pm
    Author:  Taylot Brierley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OSCComponent  : public juce::Component
{
public:
    OSCComponent();
    ~OSCComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider pulseWidthDial, freqDial;
    juce::ComboBox inputModeBox, waveTypeBox;
    
    juce::TextButton backButton;
    
    juce::Label freqLabel, pulseWidthLabel;
    
    juce::Rectangle<int> window, titleArea;

    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCComponent)
};
