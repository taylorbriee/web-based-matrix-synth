/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "TextComponent.h"
#include "Modules/OSC/OSCComponent.h"
#include "Modules/LFO/LFOComponent.h"

#include "Modules/BaseComponent.h"


//==============================================================================
/**
*/
class PluginEditor  : public juce::AudioProcessorEditor
{
public:
    PluginEditor (WebMatrixSynthAudioProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void updateButtons(int buttonIndex, juce::String updateTo);
    void updateOutputBoxes(std::string, int);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    juce::Slider dial1, dial2, dial3, dial4, dial5, dial6, dial7, dial8, dial9, dial10, dial11, dial12, dial13, dial14, dial15, dial16, oDial1, oDial2, oDial3, oDial4;
    
    juce::Grid grid;

    
    juce::ComboBox inputBox1, inputBox2, inputBox3, inputBox4, outputBox1, outputBox2, outputBox3, outputBox4;
    
    std::unordered_map<juce::String, std::unordered_map<juce::String, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>>> sliderAttachments;
    
    juce::Slider* dials[20] = {
        &dial1, &dial2, &dial3, &dial4,
        &dial5, &dial6, &dial7, &dial8,
        &dial9, &dial10, &dial11, &dial12,
        &dial13, &dial14, &dial15, &dial16,
        &oDial1, &oDial2, &oDial3, &oDial4
    };
    
    juce::ComboBox* inputBoxes[4] = {
        &inputBox1, &inputBox2, &inputBox3, &inputBox4
    };
    
    juce::ComboBox* outputBoxes[4] = {
        &outputBox1, &outputBox2, &outputBox3, &outputBox4
    };
    std::unique_ptr<OSCComponent> vcoComponent;
    
    WebMatrixSynthAudioProcessor& audioProcessor;

    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
//    Slot1_VCO_Freq
    
    std::unique_ptr<SliderAttachment> Slot1_VCO_Freq;
    std::unique_ptr<SliderAttachment> Slot1_VCO_PW;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> Slot1_VCO_WF;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> Slot1_VCO_VM;

    

    juce::TextButton myButton;
    juce::Rectangle<int> topArea1;
    juce::Rectangle<int> smallerBox;
    
    
    int buttonWidth;
    int buttonHeight;
    int buttonX;
    int buttonY;
    
    int vcoCounter, lfoCounter;

    
    
    
    juce::String selectedModules[4] = {"", "", "", ""};
    
    
//    std::vector<std::unique_ptr<juce::TextButton>> moduleButtons;
    
    std::unique_ptr<juce::TextButton> moduleButtons[4];
    
    std::unique_ptr<BaseComponent> moduleComponents[4];


    juce::AudioProcessorValueTreeState& apvts;

    juce::Label outputText;

    std::unique_ptr<juce::TextButton> newButton; 

    std::unique_ptr<TextComponent> myTextComponent;
    
    
    
    bool firstButton;
    
    juce::Rectangle<int> window;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};


