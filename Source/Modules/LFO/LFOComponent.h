/*
  ==============================================================================

    LFOComponent.h
    Created: 13 Mar 2025 4:48:22pm
    Author:  Taylot Brierley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../BaseComponent.h"


//==============================================================================

class LFOComponent : public BaseComponent
{
public:
    LFOComponent();
    ~LFOComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void loadState() override;
    void saveState() override;
    
private:
    juce::Slider pulseWidthDial, freqDial;
    juce::ComboBox waveTypeBox;
    
    juce::TextButton backButton;
    
    juce::Label freqLabel, pulseWidthLabel;
    
    juce::Rectangle<int> window, titleArea;
    
    juce::ValueTree vcoState { "VCO" };


    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};
