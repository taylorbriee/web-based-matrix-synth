/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WebMatrixSynthAudioProcessorEditor::WebMatrixSynthAudioProcessorEditor (WebMatrixSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    dial1.setSliderStyle(juce::Slider::Rotary);
    
    myButton.setButtonText("OSC");  // Set text of the button
    addAndMakeVisible(myButton);  // Make the button visible in the UI
    
    for (auto* dial : dials)  // Assuming `dials` is an array or vector of sliders
    {
        dial->setSliderStyle(juce::Slider::Rotary);
        dial->setTextBoxStyle(juce::Slider::TextBoxRight, false, 30, 20);

        addAndMakeVisible(*dial);
    }
    
    
    for (auto* box : boxes)  // Assuming `dials` is an array or vector of sliders
    {

        
        
        box->addItem("None", 1);

        box->addSectionHeading("Generator Modules");

        box->addItem("OSC", 2);
        box->addItem("LFO", 3);
        box->addItem("VCO", 4);

        box->addSectionHeading("Outputs");

        box->addItem("1", 5);
        box->addItem("2", 6);
        box->addItem("3", 7);
        box->addItem("4", 8);

        box->setSelectedId(1, juce::dontSendNotification);
     
        
        addAndMakeVisible(box);

    }
    
    box1.onChange = [this]() {
        selectedModules[0] = box1.getText();
        DBG("Box1 selected text: " + box1.getText());
    };
    
    box2.onChange = [this]() {
        selectedModules[1] = box2.getText();
        DBG("Box2 selected text: " + box2.getText());
    };
    
    box3.onChange = [this]() {
        selectedModules[2] = box3.getText();
        DBG("Box3 selected text: " + box3.getText());

    };
    
    box4.onChange = [this]() {
        selectedModules[3] = box4.getText();
        DBG("Box4 selected text: " + box4.getText());
    };
}

WebMatrixSynthAudioProcessorEditor::~WebMatrixSynthAudioProcessorEditor()
{
}

//==============================================================================
void WebMatrixSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    
    // Draw a smaller rectangle inside the drawable area
    g.setColour(juce::Colours::grey);
    g.fillRect(smallerBox);
}

void WebMatrixSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    jassert (true);
    dial1.setBounds(getLocalBounds());
    


    
    juce::Grid grid;
    
    
    

    
    // Define a 4x4 grid (4 columns, 4 rows)
    grid.templateColumns = {
        juce::Grid::TrackInfo(juce::Grid::Fr(1)),
                             juce::Grid::TrackInfo(juce::Grid::Fr(1)),
                             juce::Grid::TrackInfo(juce::Grid::Fr(1)),
                             juce::Grid::TrackInfo(juce::Grid::Fr(1)),
                             juce::Grid::TrackInfo(juce::Grid::Fr(1)) };

    grid.templateRows = { juce::Grid::TrackInfo(juce::Grid::Fr(1)),
                          juce::Grid::TrackInfo(juce::Grid::Fr(1)),
                          juce::Grid::TrackInfo(juce::Grid::Fr(1)),
                          juce::Grid::TrackInfo(juce::Grid::Fr(1)),
                            juce::Grid::TrackInfo(juce::Grid::Fr(1))};
    

    
    
    // Add components to the grid
    grid.items = {
        
        juce::GridItem(box1), juce::GridItem(dial1), juce::GridItem(dial2), juce::GridItem(dial3), juce::GridItem(dial4),
        juce::GridItem(box2), juce::GridItem(dial5), juce::GridItem(dial6), juce::GridItem(dial7), juce::GridItem(dial8),
        juce::GridItem(box3), juce::GridItem(dial9), juce::GridItem(dial10), juce::GridItem(dial11), juce::GridItem(dial12),
        juce::GridItem(box4), juce::GridItem(dial13), juce::GridItem(dial14), juce::GridItem(dial15), juce::GridItem(dial16),
        {}, juce::GridItem(oDial1), juce::GridItem(oDial2), juce::GridItem(oDial3), juce::GridItem(oDial4),
    };
    
    window = getLocalBounds();
    
    topArea1 = window.removeFromTop(window.getHeight() * 0.70);
    
    auto gridArea = topArea1.removeFromBottom(topArea1.getHeight() * 0.80);
    
    grid.performLayout(gridArea);

    
    smallerBox = window.reduced(10); // Smaller margins

    
    int padding = 5;

    
    
    int buttonWidth = smallerBox.getWidth()/10;
    int buttonHeight = smallerBox.reduced(padding).getHeight();
    int buttonX = smallerBox.reduced(padding).getX();
    int buttonY = smallerBox.reduced(padding).getY();

    myButton.setBounds(buttonX, buttonY, buttonWidth, buttonHeight);
    

    repaint();

}
