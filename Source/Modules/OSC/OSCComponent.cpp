/*
  ==============================================================================

    OSCComponent.cpp
    Created: 25 Feb 2025 6:34:01pm
    Author:  Taylot Brierley

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OSCComponent.h"


//==============================================================================
OSCComponent::OSCComponent(juce::AudioProcessorValueTreeState& apvts, juce::String slot)
: apvts(apvts), slot(slot)
{
    
}

OSCComponent::~OSCComponent()
{
}

void OSCComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (20.0f));
    g.drawText ("Oscillator", titleArea,
                juce::Justification::centred, true);   // draw some placeholder text
}

void OSCComponent::resized()
{
    
    window = getLocalBounds();
    
    titleArea = window.removeFromTop(60);
    
    addAndMakeVisible(freqDial);
    addAndMakeVisible(freqModDial);
    

    
    auto topLeftArea = window.removeFromTop(window.getHeight() * 0.5);
    auto topLeftDialBounds = topLeftArea.removeFromLeft(window.getWidth() * 0.5);
    auto topRightArea = topLeftArea;
    
    auto freqLabelSpace = topLeftDialBounds.removeFromBottom(40);
    auto FreqModLabelSpace = topRightArea.removeFromBottom(40);
    
    freqLabel.setBounds(freqLabelSpace);
    freqModLabel.setBounds(FreqModLabelSpace);
    
    freqLabel.setJustificationType(juce::Justification::centred);
    freqModLabel.setJustificationType(juce::Justification::centred);

    
    topLeftDialBounds = topLeftDialBounds;
    
    freqModDial.setSliderStyle(juce::Slider::Rotary);
    freqModDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    freqModDial.setBounds(topRightArea);
    
    freqModDial.setRange(-100.0, 100.0, 5.0);
    
    freqDial.setSliderStyle(juce::Slider::Rotary);
    freqDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    freqDial.setBounds(topLeftDialBounds);
    
    freqDial.setSkewFactor(0.3);
    freqDial.setTextValueSuffix(" Hz");
    freqDial.setRange(0.0001, 1024, 0.0001);
    
    
    auto windowBottomHalf = window;
    
    
    auto bottomSeg1 = windowBottomHalf.removeFromTop(windowBottomHalf.getHeight() / 4);
    auto bottomSeg2 = windowBottomHalf.removeFromTop(windowBottomHalf.getHeight() / 3);
    auto bottomSeg3 = windowBottomHalf.removeFromTop(windowBottomHalf.getHeight() / 2);

    auto bottomSeg4 = windowBottomHalf;
    
    
    addAndMakeVisible(waveTypeBox);
    waveTypeBox.addItem("Sine", 1);
    waveTypeBox.addItem("Saw", 2);
    waveTypeBox.addItem("Square", 3);
    waveTypeBox.addItem("Noise", 4);
    waveTypeBox.setSelectedId(1);
    
    waveTypeBox.setBounds(bottomSeg1);
    
    
    addAndMakeVisible(perfMode);
    perfMode.addItem("Drone", 1);
    perfMode.addItem("MIDI In", 2);
    perfMode.setSelectedId(1);
    perfMode.setBounds(bottomSeg2);
    
    
    
    
    addAndMakeVisible(oscVoices);
    oscVoices.addItem("Monophonic", 1);
    oscVoices.addItem("Polyphonic", 2);
    oscVoices.setSelectedId(1);
    oscVoices.setBounds(bottomSeg3);


    addAndMakeVisible(backButton);
    backButton.setButtonText("Back");
    backButton.setBounds(bottomSeg4);
    
    backButton.onClick = [this]() {
        this->setVisible(false);
    };

    addAndMakeVisible(freqLabel);
    addAndMakeVisible(freqModLabel);
    
    freqLabel.setText("Frequency", juce::dontSendNotification);
    freqModLabel.setText("Frequency Modulation", juce::dontSendNotification);
    
    
    
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;


    
    juce::String WF_Param = slot+"_VCO_WF";
    juce::String Inputs_Param = slot+"_VCO_Inputs";
    juce::String VM_Param = slot+"_VCO_VM";
    VCO_WF_Attach = std::make_unique<ComboBoxAttachment>(apvts, WF_Param, waveTypeBox);
    VCO_VM_Attach = std::make_unique<ComboBoxAttachment>(apvts, VM_Param, oscVoices);
    
    
    
    juce::String Freq_Param = slot+"_VCO_Freq";
    juce::String Freq_Mod_Param = slot+"_VCO_Freq_Mod";
    VCO_Freq_Attach = std::make_unique<SliderAttachment>(apvts, Freq_Param, freqDial);
    VCO_Freq_Mod_Attach = std::make_unique<SliderAttachment>(apvts, Freq_Mod_Param, freqModDial);
    
    
 
}

void OSCComponent::loadState()
{
    if (vcoState.hasProperty("freq"))
        freqDial.setValue(vcoState.getProperty("freq"));
    
    if (vcoState.hasProperty("pulseWidth"))
        freqModDial.setValue(vcoState.getProperty("pulseWidth"));
}

void OSCComponent::saveState()
{
    vcoState.setProperty("freq", freqDial.getValue(), nullptr);
    vcoState.setProperty("pulseWidth", freqModDial.getValue(), nullptr);
}
