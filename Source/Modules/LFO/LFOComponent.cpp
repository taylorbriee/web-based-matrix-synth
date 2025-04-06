/*
  ==============================================================================

    LFOComponent.cpp
    Created: 13 Mar 2025 4:48:22pm
    Author:  Taylot Brierley

  ==============================================================================
*/

#include "LFOComponent.h"


#include <JuceHeader.h>
#include "LFOComponent.h"


//==============================================================================
LFOComponent::LFOComponent(juce::AudioProcessorValueTreeState& apvts, juce::String slot)
    : apvts(apvts), slot(slot)
{


}

LFOComponent::~LFOComponent()
{
}

void LFOComponent::paint (juce::Graphics& g)
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

void LFOComponent::resized()
{
    

    window = getLocalBounds();
    
    titleArea = window.removeFromTop(60);
    
    addAndMakeVisible(freqDial);
    addAndMakeVisible(pulseWidthDial);
    
    

    
    auto topLeftArea = window.removeFromTop(window.getHeight() * 0.5);
    auto topLeftDialBounds = topLeftArea.removeFromLeft(window.getWidth() * 0.5);
    auto topRightArea = topLeftArea;
    
    auto freqLabelSpace = topLeftDialBounds.removeFromBottom(40);
    auto pulseWidthLabelSpace = topRightArea.removeFromBottom(40);
    
    freqLabel.setBounds(freqLabelSpace);
    pulseWidthLabel.setBounds(pulseWidthLabelSpace);
    
    freqLabel.setJustificationType(juce::Justification::centred);
    pulseWidthLabel.setJustificationType(juce::Justification::centred);

    
    topLeftDialBounds = topLeftDialBounds;
    
    freqDial.setSliderStyle(juce::Slider::Rotary);
    freqDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    freqDial.setBounds(topLeftDialBounds);
    
    freqDial.setSkewFactor(0.3);
    freqDial.setTextValueSuffix(" Hz");
    freqDial.setRange(0.0001, 1024, 0.0001);
    
    

    
    
    
    
    pulseWidthDial.setSliderStyle(juce::Slider::Rotary);
    pulseWidthDial.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    pulseWidthDial.setBounds(topRightArea);
    
    pulseWidthDial.setTextValueSuffix(" %");
    pulseWidthDial.setRange(0.1, 10.0, 0.1);

    
    auto windowBottomHalf = window;
    
    
    auto bottomSeg1 = windowBottomHalf.removeFromTop(windowBottomHalf.getHeight() / 4);

    auto bottomSeg4 = windowBottomHalf;
    
    addAndMakeVisible(waveTypeBox);
    waveTypeBox.addItem("Sine", 1);
    waveTypeBox.addItem("Saw", 2);
    waveTypeBox.addItem("Square", 3);
    waveTypeBox.addItem("Noise", 4);
    waveTypeBox.setSelectedId(1);
    
    waveTypeBox.setBounds(bottomSeg1);
    



    addAndMakeVisible(backButton);
    backButton.setButtonText("Back");
    backButton.setBounds(bottomSeg4);
    
    backButton.onClick = [this]() {
        this->setVisible(false);
    };

    addAndMakeVisible(freqLabel);
    addAndMakeVisible(pulseWidthLabel);
    
    freqLabel.setText("Frequency", juce::dontSendNotification);
    pulseWidthLabel.setText("Pulse Width", juce::dontSendNotification);

    
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    
    
    
    juce::String Freq_Param = slot+"_LFO_Freq";
    juce::String PW_Param = slot+"_LFO_PW";
    juce::String WF_Param = slot+"_LFO_WF";
    
    LFO_Freq_Attach = std::make_unique<SliderAttachment>(apvts, Freq_Param, freqDial);
    LFO_PW_Attach = std::make_unique<SliderAttachment>(apvts, PW_Param, pulseWidthDial);
    LFO_WF_Attach = std::make_unique<ComboBoxAttachment>(apvts, WF_Param, waveTypeBox);
 
}

void LFOComponent::loadState()
{
    if (vcoState.hasProperty("freq"))
        freqDial.setValue(vcoState.getProperty("freq"));
    
    if (vcoState.hasProperty("pulseWidth"))
        pulseWidthDial.setValue(vcoState.getProperty("pulseWidth"));
}

void LFOComponent::saveState()
{
    vcoState.setProperty("freq", freqDial.getValue(), nullptr);
    vcoState.setProperty("pulseWidth", pulseWidthDial.getValue(), nullptr);
}

