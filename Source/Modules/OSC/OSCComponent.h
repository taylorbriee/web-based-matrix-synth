/*
  ==============================================================================

    OSCComponent.h
    Created: 25 Feb 2025 6:34:01pm
    Author:  Taylot Brierley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../BaseComponent.h"


//==============================================================================

class OSCComponent : public BaseComponent
{
public:
    OSCComponent();
    ~OSCComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void loadState() override;
    void saveState() override;
    
private:
    juce::Slider pulseWidthDial, freqDial;
    juce::ComboBox inputModeBox, waveTypeBox;
    
    juce::TextButton backButton;
    
    juce::Label freqLabel, pulseWidthLabel;
    
    juce::Rectangle<int> window, titleArea;
    
    juce::ValueTree vcoState { "VCO" };


    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCComponent)
};
