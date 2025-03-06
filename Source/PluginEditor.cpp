/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "TextComponent.h"
#include <memory> // For std::unique_ptr
#include <juce_gui_basics/juce_gui_basics.h> // Include JUCE basics
#include <iostream>
#include <format>



//==============================================================================
PluginEditor::PluginEditor (WebMatrixSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    
    dial1.setSliderStyle(juce::Slider::Rotary);
    
    myButton.setButtonText("VCO");  // Set text of the button
    addAndMakeVisible(myButton);  // Make the button visible in the UI
    
    
    for (auto* dial : dials)  // Assuming `dials` is an array or vector of sliders
    {
        dial->setSliderStyle(juce::Slider::Rotary);
        dial->setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
        
        dial->setRange(0.0, 10.0, 0.1);

        addAndMakeVisible(*dial);
    }
    
    
    for (auto* box : inputBoxes)  // Assuming `dials` is an array or vector of sliders
    {
        
        box->addItem(" ", 1);

        box->addSectionHeading("Generator Modules");

        box->addItem("VCO", 2);
        box->addItem("LFO", 3);

        box->addSectionHeading("Outputs");

        box->addItem("1", 5);
        box->addItem("2", 6);
        box->addItem("3", 7);
        box->addItem("4", 8);

        box->setSelectedId(1, juce::dontSendNotification);
     
        
        addAndMakeVisible(box);
    }
    
    for (auto* box : outputBoxes)  // Assuming `dials` is an array or vector of sliders
    {
        addAndMakeVisible(box);
    }
    
    
    
    inputBox1.onChange = [this]() {
        selectedModules[0] = inputBox1.getText();
        updateButtons(0, inputBox1.getText());
        
        DBG("Box1 selected text: " + inputBox1.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }
    };
    
    inputBox2.onChange = [this]() {
        selectedModules[1] = inputBox2.getText();
        updateButtons(1, inputBox2.getText());
        
        DBG("Box2 selected text: " + inputBox2.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }
    };
    
    inputBox3.onChange = [this]() {
        selectedModules[2] = inputBox3.getText();
        updateButtons(2, inputBox3.getText());
        
        DBG("Box3 selected text: " + inputBox3.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }

    };
    
    inputBox4.onChange = [this]() {
        selectedModules[3] = inputBox4.getText();
        updateButtons(3, inputBox4.getText());
        
        DBG("Box4 selected text: " + inputBox4.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }
    };
    



    

    

    smallerBox = window.reduced(10); // Smaller margins
    repaint();

}


void PluginEditor::updateButtons(int index, juce::String updateTo)
{

    
    // Counters for numbering each module
    int vcoCounter = 1, lfoCounter = 1;
    
    int padding = 5;

    
    int buttonWidth = smallerBox.getWidth()/10;
    int buttonHeight = smallerBox.reduced(padding).getHeight();
    int buttonX  = smallerBox.reduced(padding).getX();
    int buttonY = smallerBox.reduced(padding).getY();
    
    for (auto* box : outputBoxes)
    {
        box->clear();
    }
    
    int outputBoxItemID=1;

    for (auto* box : outputBoxes)
    {
        box->addItem(" ", outputBoxItemID++);
        box->addItem("Main Output", outputBoxItemID++);

    }
    
// Create new buttons based on selectedModules

        
    juce::String module = selectedModules[index];

    if (module == "VCO")
    {
        
        moduleComponents[index] = std::make_unique<OSCComponent>();
        
        newButton = std::make_unique<juce::TextButton>("VCO " + juce::String(vcoCounter++));
        
        //HERE
        newButton->onClick = [this, index]() {
//                this->setVisible(false);

            addAndMakeVisible(*moduleComponents[index]);
            moduleComponents[index]->setBounds(getLocalBounds());
            
            
        };


        for (auto* box : outputBoxes)
        {
            box->addSectionHeading("VCO "+ std::to_string(vcoCounter));

            box->addItem("Frequency", outputBoxItemID++);
            box->addItem("Pulse Width", outputBoxItemID++);

        }
        
    }
    else if (module == "LFO")
    {
        for (auto* box : outputBoxes)
        {
            box->addSectionHeading("LFO "+ std::to_string(vcoCounter));

            box->addItem("Frequency", outputBoxItemID++);
            box->addItem("Pulse Width", outputBoxItemID++);

        }
        newButton = std::make_unique<juce::TextButton>("LFO " + juce::String(lfoCounter++));
    }

    if (newButton != nullptr)
    {
        
        addAndMakeVisible(*newButton);
        
        newButton->setBounds(buttonX, buttonY, buttonWidth, buttonHeight);
        
        buttonX+=65;
        
        moduleButtons[index].reset();
        moduleButtons[index]= std::move(newButton);

    }
    

    // Update layout
    resized();
    repaint();
}



//when time comes use extra parameter to input a function to be called when dropout menues have been clicked.
void PluginEditor::updateOutputBoxes(std::string modType, int modNum)
{


    
    if (modType == "VCO")
    {

    }
    else if (modType == "LFO")
    {
        for (auto* box : outputBoxes)
        {

        }

    }

    
    
}



PluginEditor::~PluginEditor()
{
}

//==============================================================================
void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    // Draw a smaller rectangle inside the drawable area
    g.setColour(juce::Colours::grey);
    g.fillRect(smallerBox);
}

void PluginEditor::resized()
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
                            juce::Grid::TrackInfo(juce::Grid::Fr(1)),
        juce::Grid::TrackInfo(juce::Grid::Fr(1))};
    
    myTextComponent = std::make_unique<TextComponent>("Output: ");
    addAndMakeVisible(*myTextComponent);

    
    // Add components to the grid
    grid.items = {
        juce::GridItem(inputBox1), juce::GridItem(dial1), juce::GridItem(dial2), juce::GridItem(dial3), juce::GridItem(dial4),
        juce::GridItem(inputBox2), juce::GridItem(dial5), juce::GridItem(dial6), juce::GridItem(dial7), juce::GridItem(dial8),
        juce::GridItem(inputBox3), juce::GridItem(dial9), juce::GridItem(dial10), juce::GridItem(dial11), juce::GridItem(dial12),
        juce::GridItem(inputBox4), juce::GridItem(dial13), juce::GridItem(dial14), juce::GridItem(dial15), juce::GridItem(dial16),
        juce::GridItem(*myTextComponent), juce::GridItem(oDial1), juce::GridItem(oDial2), juce::GridItem(oDial3), juce::GridItem(oDial4),
        juce::GridItem(), juce::GridItem(outputBox1), juce::GridItem(outputBox2), juce::GridItem(outputBox3), juce::GridItem(outputBox4)
    };
    
 
    window = getLocalBounds();
    
    
    topArea1 = window.removeFromTop(window.getHeight() * 0.70);
    
    auto gridArea = topArea1.removeFromBottom(topArea1.getHeight() * 0.95);
    
    grid.performLayout(gridArea);
    

    



    
    repaint();

}
