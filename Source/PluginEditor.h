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
    void updateOutputOptions();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    juce::Slider dial_1x1, dial_1x2, dial_1x3, dial_1x4, dial_2x1, dial_2x2, dial_2x3, dial_2x4, dial_3x1, dial_3x2, dial_3x3, dial_3x4, dial_4x1, dial_4x2, dial_4x3, dial_4x4, oDial1, oDial2, oDial3, oDial4;
    
    juce::Grid grid;

    
    juce::ComboBox inputBox1, inputBox2, inputBox3, inputBox4, outputBox1, outputBox2, outputBox3, outputBox4;
    
    std::unordered_map<juce::String, std::unordered_map<juce::String, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>>> sliderAttachments;
    
    juce::Slider* dials[20] = {
        &dial_1x1, &dial_2x1, &dial_3x1, &dial_4x1,
        &dial_1x2, &dial_2x2, &dial_3x2, &dial_4x2,
        &dial_1x3, &dial_2x3, &dial_3x3, &dial_4x3,
        &dial_1x4, &dial_2x4, &dial_3x4, &dial_4x4,
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
    
    juce::String curModule;
    
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


