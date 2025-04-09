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
    
    juce::Slider freqModDial, freqDial;
    juce::ComboBox waveTypeBox, oscVoices;
    
private:

    juce::AudioProcessorValueTreeState& apvts;
    
    juce::String slot;    
    juce::Label freqLabel, freqModLabel;
    
    juce::TextButton backButton;
    
    juce::Rectangle<int> window, titleArea;
    
    juce::ValueTree vcoState { "VCO" };
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> VCO_Freq_Attach, VCO_Freq_Mod_Attach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> VCO_WF_Attach, VCO_VM_Attach;
    
    //                Slot1_VCO_Freq
    //                Slot1_VCO_PW
    //                Slot1_VCO_WF
    //                Slot1_VCO_Inputs
    //                Slot1_VCO_VM
    
//    std::unique_ptr<>
    
    
//    params.push_back(std::make_unique<juce::AudioParameterChoice> (juce::ParameterID(paramID, 1), slot+" VCO Waveform", juce::StringArray {"Sine","Saw","Square","Noise"}, 0));
//    
//    paramID = slot+"_VCO_Inputs";
//    
//    params.push_back(std::make_unique<juce::AudioParameterChoice> (juce::ParameterID(paramID, 1), slot+" VCO Inputs", juce::StringArray {"inc MIDI", "Drone"}, 0));
//    
//    
//    paramID = slot+"_VCO_VM";
//    
//    params.push_back(std::make_unique<juce::AudioParameterChoice> (juce::ParameterID(paramID, 1), slot+" VCO Voice Mode", juce::StringArray {"monophonic", "polyphonic"}, 0));

    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCComponent)
};
