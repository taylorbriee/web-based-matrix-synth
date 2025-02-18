/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class WebMatrixSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    WebMatrixSynthAudioProcessorEditor (WebMatrixSynthAudioProcessor&);
    ~WebMatrixSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void updateButtons();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    juce::Slider dial1, dial2, dial3, dial4, dial5, dial6, dial7, dial8, dial9, dial10, dial11, dial12, dial13, dial14, dial15, dial16, oDial1, oDial2, oDial3, oDial4;
    
    juce::ComboBox box1, box2, box3, box4;
    
    juce::Slider* dials[20] = {
        &dial1, &dial2, &dial3, &dial4,
        &dial5, &dial6, &dial7, &dial8,
        &dial9, &dial10, &dial11, &dial12,
        &dial13, &dial14, &dial15, &dial16,
        &oDial1, &oDial2, &oDial3, &oDial4
    };
    
    juce::ComboBox* boxes[4] = {
        &box1, &box2, &box3, &box4
    };
    

    juce::TextButton myButton;
    juce::Rectangle<int> topArea1;
    juce::Rectangle<int> smallerBox;
    
    
    juce::String selectedModules[4] = {"", "", "", ""};
    
    
    std::vector<juce::TextButton*> moduleButtons{nullptr, nullptr, nullptr, nullptr};




    juce::Rectangle<int> window;
    
    WebMatrixSynthAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WebMatrixSynthAudioProcessorEditor)
};
