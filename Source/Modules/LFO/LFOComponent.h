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
    LFOComponent(juce::AudioProcessorValueTreeState& apvts, juce::String slot);
    ~LFOComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void loadState() override;
    void saveState() override;
    
    juce::Slider pulseWidthDial, freqDial;
    juce::ComboBox waveTypeBox;
    
private:
    
    juce::String slot;
    juce::AudioProcessorValueTreeState& apvts;
    

    juce::Label freqLabel, pulseWidthLabel;
    juce::TextButton backButton;
    
    juce::Rectangle<int> window, titleArea;
    
    juce::ValueTree vcoState { "VCO" };


    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOComponent)
};
