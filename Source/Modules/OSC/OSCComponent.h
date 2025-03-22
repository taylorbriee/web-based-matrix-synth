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
    OSCComponent(juce::AudioProcessorValueTreeState& apvts, juce::String slot);
    ~OSCComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void loadState() override;
    void saveState() override;
    
    juce::Slider pulseWidthDial, freqDial;
    juce::ComboBox inputModeBox, waveTypeBox, oscVoices;
    
private:

    juce::String slot;
    juce::AudioProcessorValueTreeState& apvts;
    
    juce::Label freqLabel, pulseWidthLabel;
    
    juce::TextButton backButton;
    
    juce::Rectangle<int> window, titleArea;
    
    juce::ValueTree vcoState { "VCO" };


    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCComponent)
};
