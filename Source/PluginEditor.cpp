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
    : AudioProcessorEditor (p), audioProcessor (p), apvts(audioProcessor.apvts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    
//    apvts = audioProcessor.apvts;

    

    
    
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
        updateButtons(0, inputBox1.getText());
        
//        DBG("Box1 selected text: " + inputBox1.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }
    };
    
    inputBox2.onChange = [this]() {
        updateButtons(1, inputBox2.getText());
        
//        DBG("Box2 selected text: " + inputBox2.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }
    };
    
    inputBox3.onChange = [this]() {
        updateButtons(2, inputBox3.getText());
        
//        DBG("Box3 selected text: " + inputBox3.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }

    };
    
    inputBox4.onChange = [this]() {
        updateButtons(3, inputBox4.getText());
        
//        DBG("Box4 selected text: " + inputBox4.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }
    };
    





    
    int padding = 5;

    smallerBox = window.reduced(10); // Smaller margins
    
    buttonWidth = smallerBox.getWidth()/10;
    buttonHeight = smallerBox.reduced(padding).getHeight();
    buttonY = smallerBox.reduced(padding).getY();
    buttonX = smallerBox.reduced(5).getX();
    
    firstButton=true;


    repaint();

}


void PluginEditor::updateButtons(int index, juce::String updateTo)
{
    
    selectedModules[index]=updateTo;


    //OUTPUT BOXES
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
    if (updateTo == "VCO")
    {

        
//            juce::Slider pulseWidthDial, freqDial;
//            juce::ComboBox inputModeBox, waveTypeBox, oscVoices;
//            do all parameter creation here
//
//
//
//                Slot1_VCO_Freq
//                Slot1_VCO_PW
//                Slot1_VCO_WF
//                Slot1_VCO_Inputs
//                Slot1_VCO_VM
            
//        audioProcessor.addModule("VCO", index);
        
        moduleComponents[index] = std::make_unique<OSCComponent>(apvts, juce::String("Slot"+juce::String(index+1)));
        if (auto* oscComponent = dynamic_cast<OSCComponent*>(moduleComponents[index].get()))
        {
             

//
//                oscComponent->pulseWidthDial;
//                oscComponent->inputModeBox;
//                oscComponent->waveTypeBox;
//                oscComponent->oscVoices;
        }
        else
        {
            DBG("Failed to cast to OSCComponent");
        }
        

    
        //possibly create a new array storing strings of chosen modules

        
//            newButton = std::make_unique<juce::TextButton>("VCO " + juce::String(++vcoCounter));
        newButton = std::make_unique<juce::TextButton>("VCO Slot: " + juce::String(index+1));
        addAndMakeVisible(*newButton);
        
        newButton->onClick = [this, index]() {

            addAndMakeVisible(*moduleComponents[index]);
            moduleComponents[index]->setBounds(getLocalBounds());
        };
        
        moduleButtons[index]= std::move(newButton);



        for (auto* box : outputBoxes)
        {
            box->addSectionHeading("VCO ");

            box->addItem("Frequency", outputBoxItemID++);
            box->addItem("Pulse Width", outputBoxItemID++);

        }
    }
    
    else if (updateTo == "LFO")
    {
        
        
        moduleComponents[index] = std::make_unique<LFOComponent>(apvts, juce::String("Slot"+juce::String(index+1)));
        
        audioProcessor.addModule("LFO", index);
        newButton = std::make_unique<juce::TextButton>("LFO Slot: " + juce::String(index+1));
        addAndMakeVisible(*newButton);

        
        newButton->onClick = [this, index]() {
            
            addAndMakeVisible(*moduleComponents[index]);
            moduleComponents[index]->setBounds(getLocalBounds());
        };
        
        moduleButtons[index]= std::move(newButton);

        
        for (auto* box : outputBoxes)
        {
            
            box->addSectionHeading("LFO ");

            box->addItem("Frequency", outputBoxItemID++);
            box->addItem("Pulse Width", outputBoxItemID++);

        }
    }
    
    else if (updateTo == "")
    {
        
        moduleComponents[index].reset();
        moduleButtons[index].reset();
        selectedModules[index] = "";
        
    }

    
    firstButton=true;
    buttonX = smallerBox.reduced(5).getX();
    
    for (int i = 0; i < 4; ++i)
    {
        if (moduleButtons[i]) // Check if the pointer is valid
        {
            if (firstButton){
                moduleButtons[i]->setBounds(buttonX, buttonY, buttonWidth, buttonHeight);
                firstButton=false;
            }else{
                moduleButtons[i]->setBounds(buttonX+=65, buttonY, buttonWidth, buttonHeight);
            }
        }
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
