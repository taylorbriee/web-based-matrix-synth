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

    

    
    
    
    myButton.setButtonText("VCO");  // Set text of the button
    addAndMakeVisible(myButton);  // Make the button visible in the UI
    
    
//    for (auto* dial : dials)  // Assuming `dials` is an array or vector of sliders
//    {
//        dial->setSliderStyle(juce::Slider::Rotary);
//        dial->setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
//        
//        dial->setRange(0.0, 10.0, 0.1);
//
//        addAndMakeVisible(*dial);
//    }
    


    for(int y=1; y<5; y++){
        for(int x=1; x<5; x++){
            
            int index = (y - 1) * 4 + (x - 1);
            dials[index]->setSliderStyle(juce::Slider::Rotary);
            dials[index]->setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);

            dials[index]->setRange(0.0, 10.0, 0.1);

            addAndMakeVisible(*dials[index]);
            
            juce::String paramID = juce::String(x)+"x"+juce::String(y) + "Dial";
            
            sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                apvts, paramID, *dials[index] ));

        }
    }
        

    //GENERATING ALL ITEMS FOR THE OUPUT BOXES
    
    

    
    for (auto* box : inputBoxes)  // Assuming `dials` is an array or vector of sliders
    {
        
        box->addItem(" ", 1);

        box->addSectionHeading("Generator Modules");

        box->addItem("VCO", 2);
        box->addItem("LFO", 3);

        box->setSelectedId(1, juce::dontSendNotification);
     
        
        addAndMakeVisible(box);
    }
    
    int id=1;
    
    OutComboBoxIdToText.insert(std::pair<juce::String, int>(" ", id));
    id++;
    
    OutComboBoxIdToText.insert(std::pair<juce::String, int>("Main Output", id));
    id++;

    //add all the ids up here
    for (int slots=1; slots<5; slots++){
        juce::String slot = "Slot "+juce::String(slots);

//        OutComboBoxIdToText[id] = slot+"_VCO_Freq";
//        id++;
//        OutComboBoxIdToText[id] = slot+"_VCO_PW";
//        id++;
//        OutComboBoxIdToText[id] = slot+"_LFO_Freq";
//        id++;
//        OutComboBoxIdToText[id] = slot+"_LFO_PW";
//        id++;
        
        OutComboBoxIdToText.insert(std::pair<juce::String, int>(slot+"_VCO_Freq", id));
        id++;
        
        OutComboBoxIdToText.insert(std::pair<juce::String, int>(slot+"_VCO_PW", id));
        id++;
        
        OutComboBoxIdToText.insert(std::pair<juce::String, int>(slot+"_LFO_Freq", id));
        id++;
        
        OutComboBoxIdToText.insert(std::pair<juce::String, int>(slot+"_LFO_PW", id));
        id++;
        
        

    }
    
    for (auto* box : outputBoxes)  // Assuming `dials` is an array or vector of sliders
    {
        id=1;
        
        box->addItem(" ", id);
        id++;
        
        box->addItem("Main Output", id);
        id++;
        
        for (int slots=1; slots<5; slots++){
            juce::String slot = "Slot "+juce::String(slots);

            box->addItem(slot+" VCO Freq", id);
            box->setItemEnabled(id, false);
            id++;
            box->addItem(slot+" VCO PW", id);
            box->setItemEnabled(id, false);

            id++;
            box->addItem(slot+" LFO Freq", id);
            box->setItemEnabled(id, false);

            id++;
            box->addItem(slot+" LFO PW", id);
            box->setItemEnabled(id, false);

            id++;
        }
        

        
        
        //Hide all options here.
        
        
        
        //add all items to each output box
        addAndMakeVisible(box);
    }
    
    Matrix_Output1 = std::make_unique<ComboBoxAttachment>(apvts, "Matrix_Output1", *outputBoxes[0]);
    Matrix_Output2 = std::make_unique<ComboBoxAttachment>(apvts, "Matrix_Output2", *outputBoxes[1]);
    Matrix_Output3 = std::make_unique<ComboBoxAttachment>(apvts, "Matrix_Output3", *outputBoxes[2]);
    Matrix_Output4 = std::make_unique<ComboBoxAttachment>(apvts, "Matrix_Output4", *outputBoxes[3]);
    
    
    
    inputBox1.onChange = [this]() {
        updateButtons(0, inputBox1.getText());
        updateOutputOptions("Slot 1", inputBox1.getText());
        
//        DBG("Box1 selected text: " + inputBox1.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }
    };
    
    inputBox2.onChange = [this]() {
        updateButtons(1, inputBox2.getText());
        updateOutputOptions("Slot 2", inputBox2.getText());

//        DBG("Box2 selected text: " + inputBox2.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }
    };
    
    inputBox3.onChange = [this]() {
        updateButtons(2, inputBox3.getText());
        updateOutputOptions("Slot 3", inputBox3.getText());

//        DBG("Box3 selected text: " + inputBox3.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }

    };
    
    inputBox4.onChange = [this]() {
        updateButtons(3, inputBox4.getText());
        updateOutputOptions("Slot 4", inputBox4.getText());

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

void PluginEditor::updateOutputOptions(juce::String Slot, juce::String Module)
{
    
    
    //hide then make all the buttons visible
    
    
    for (auto* box : outputBoxes)
    {
        //hide all elements of that slot if "" if selected
        
        if (Module == " "){
            
            boxID = OutComboBoxIdToText[Slot+"_VCO_Freq"];
            box->setItemEnabled(boxID, false);
            
            boxID =  OutComboBoxIdToText[Slot+"_VCO_PW"];
            box->setItemEnabled(boxID, false);

            boxID = OutComboBoxIdToText[Slot+"_LFO_Freq"];
            box->setItemEnabled(boxID, false);

            boxID =  OutComboBoxIdToText[Slot+"_LFO_PW"];
            box->setItemEnabled(boxID, false);
            
            box->repaint();
          
        }else{
            
            boxIDString = Slot+"_"+Module+"_Freq";
            boxID = OutComboBoxIdToText[boxIDString];
            box->setItemEnabled(boxID, true);
            
            boxIDString = Slot+"_"+Module+"_PW";
            boxID = OutComboBoxIdToText[boxIDString];
            box->setItemEnabled(boxID, true);
            
            box->repaint();
            
        }
    }
}


void PluginEditor::updateButtons(int index, juce::String updateTo)
{
    
    selectedModules[index]=updateTo;



    
    
    
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
//                Slot1_VCO_VM
            
//        audioProcessor.addModule("VCO", index);
        
        apvts.getParameter("Slot"+juce::String(index+1)+"_VCO_isEnabled")->setValueNotifyingHost(true);
        moduleComponents[index] = std::make_unique<OSCComponent>(apvts, juce::String("Slot"+juce::String(index+1)));
        

    
        //possibly create a new array storing strings of chosen modules

        
//            newButton = std::make_unique<juce::TextButton>("VCO " + juce::String(++vcoCounter));
        newButton = std::make_unique<juce::TextButton>("VCO Slot: " + juce::String(index+1));
        addAndMakeVisible(*newButton);
        
        newButton->onClick = [this, index]() {

            addAndMakeVisible(*moduleComponents[index]);
            moduleComponents[index]->setBounds(getLocalBounds());
        };
        
        moduleButtons[index]= std::move(newButton);
    }
    
    
    else if (updateTo == "LFO")
    {
        
        apvts.getParameter("Slot"+juce::String(index+1)+"_LFO_isEnabled")->setValueNotifyingHost(true);

        moduleComponents[index] = std::make_unique<LFOComponent>(apvts, juce::String("Slot"+juce::String(index+1)));
        
        audioProcessor.addModule("LFO", index);
        newButton = std::make_unique<juce::TextButton>("LFO Slot: " + juce::String(index+1));
        addAndMakeVisible(*newButton);

        
        newButton->onClick = [this, index]() {
            
            addAndMakeVisible(*moduleComponents[index]);
            moduleComponents[index]->setBounds(getLocalBounds());
        };
        
        moduleButtons[index]= std::move(newButton);
    }
    
    
    //check if previous was VCO
    else if (updateTo == " ")
    {
        apvts.getParameter("Slot"+juce::String(index+1)+"_VCO_isEnabled")->setValueNotifyingHost(false);
        moduleComponents[index].reset();
        moduleButtons[index].reset();
        selectedModules[index] = "";
        
    }else{
        moduleButtons[index].reset();
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
//    dial1.setBounds(getLocalBounds());
    

    
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
    
//    dial_1x1, dial_1x2, dial_1x3, dial_1x4, dial_2x1, dial_2x2, dial_2x3, dial_2x4, dial_3x1, dial_3x2, dial_3x3, dial_3x4, dial_4x1, dial_4x2, dial_4x3, dial_4x4,
    grid.items = {
        juce::GridItem(inputBox1), juce::GridItem(dial_1x1), juce::GridItem(dial_2x1), juce::GridItem(dial_3x1), juce::GridItem(dial_4x1),
        juce::GridItem(inputBox2), juce::GridItem(dial_1x2), juce::GridItem(dial_2x2), juce::GridItem(dial_3x2), juce::GridItem(dial_4x2),
        juce::GridItem(inputBox3), juce::GridItem(dial_1x3), juce::GridItem(dial_2x3), juce::GridItem(dial_3x3), juce::GridItem(dial_4x3),
        juce::GridItem(inputBox4), juce::GridItem(dial_1x4), juce::GridItem(dial_2x4), juce::GridItem(dial_3x4), juce::GridItem(dial_4x4),
        juce::GridItem(*myTextComponent), juce::GridItem(oDial1), juce::GridItem(oDial2), juce::GridItem(oDial3), juce::GridItem(oDial4),
        juce::GridItem(), juce::GridItem(outputBox1), juce::GridItem(outputBox2), juce::GridItem(outputBox3), juce::GridItem(outputBox4)
    };
    
 
    window = getLocalBounds();
    
    topArea1 = window.removeFromTop(window.getHeight() * 0.70);
    
    auto gridArea = topArea1.removeFromBottom(topArea1.getHeight() * 0.95);
    
    grid.performLayout(gridArea);
    

    repaint();

}
