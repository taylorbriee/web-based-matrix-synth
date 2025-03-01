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
OSCComponent::OSCComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

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
    
    
    auto bottomSeg1 = windowBottomHalf.removeFromTop(windowBottomHalf.getHeight() / 3);
    auto bottomSeg2 = windowBottomHalf.removeFromTop(windowBottomHalf.getHeight() / 2);
    auto bottomSeg3 = windowBottomHalf;
    
    addAndMakeVisible(waveTypeBox);
    waveTypeBox.addItem("Sine", 1);
    waveTypeBox.addItem("Saw", 2);
    waveTypeBox.addItem("Square", 3);
    waveTypeBox.addItem("Noise", 4);
    waveTypeBox.setSelectedId(1);
    
    waveTypeBox.setBounds(bottomSeg1);
    
    addAndMakeVisible(inputModeBox);
    inputModeBox.addItem("Inc MIDI", 1);
    inputModeBox.addItem("Drone", 2);
    inputModeBox.setSelectedId(1);
    
    inputModeBox.setBounds(bottomSeg2);

    addAndMakeVisible(backButton);
    backButton.setButtonText("Back");
    backButton.setBounds(bottomSeg3);

    addAndMakeVisible(freqLabel);
    addAndMakeVisible(pulseWidthLabel);
    
    freqLabel.setText("Frequency", juce::dontSendNotification);
    pulseWidthLabel.setText("Pulse Width", juce::dontSendNotification);



    
    
    

    

}
