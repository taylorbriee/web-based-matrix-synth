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
        updateButtons();
        
        DBG("Box1 selected text: " + box1.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }
        
        
    };
    
    box2.onChange = [this]() {
        selectedModules[1] = box2.getText();
        updateButtons();
        
        DBG("Box2 selected text: " + box2.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }
    };
    
    box3.onChange = [this]() {
        selectedModules[2] = box3.getText();
        updateButtons();
        
        DBG("Box3 selected text: " + box3.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }

    };
    
    box4.onChange = [this]() {
        selectedModules[3] = box4.getText();
        updateButtons();
        
        DBG("Box4 selected text: " + box4.getText());
        for (int i = 0; i < 4; i++) {
            std::cout << selectedModules[i] << std::endl;
        }
    };
    
    
    moduleButtons.clear();
    moduleButtons.reserve(4);
    
    int oscCounter = 1, lfoCounter = 1, vcoCounter = 1;
    for (int i = 0; i < 4; ++i)
    {
        juce::String module = selectedModules[i];
        std::unique_ptr<juce::TextButton> newButton = std::make_unique<juce::TextButton>();

        
        if (module == "None"){
            
            moduleButtons.push_back(nullptr);

        }
        else if (module == "OSC")
        {
            newButton->setButtonText("OSC " + juce::String(oscCounter));

            oscCounter++;
            
            moduleButtons.push_back(std::move(newButton));


            
                        
            addAndMakeVisible(*newButton);

        }
        else if (module == "LFO")
        {
            newButton->setButtonText("LFO " + juce::String(lfoCounter));

            lfoCounter++;
            
            moduleButtons.push_back(std::move(newButton));

            
            addAndMakeVisible(*newButton);

        }
        else if (module == "VCO")
        {
            newButton->setButtonText("VCO " + juce::String(vcoCounter));
            
            vcoCounter++;
            
            moduleButtons.push_back(std::move(newButton));

            
            
            addAndMakeVisible(*newButton);

        }

        DBG("Added button: " + newButton->getButtonText());

    }
    
    addAndMakeVisible (oscComponent);

    
    
    
//    int padding = 5;
//
    smallerBox = window.reduced(10); // Smaller margins
//    
//    int buttonWidth = smallerBox.getWidth()/10;
//    int buttonHeight = smallerBox.reduced(padding).getHeight();
//    int buttonX  = smallerBox.reduced(padding).getX();
//    int buttonY = smallerBox.reduced(padding).getY();
//    
//    
//    
//    for (auto* button : moduleButtons)  // Assuming `dials` is an array or vector of sliders
//    {
//        
//        //this line causing issues.
//        
//        if (button != nullptr){
//
//            button->setBounds(buttonX, buttonY, buttonWidth, buttonHeight);
//            
//            buttonX+=65;
//            
//        }
//
//    }
    
    repaint();

}


void WebMatrixSynthAudioProcessorEditor::updateButtons()
{

    moduleButtons.clear();

    // Counters for numbering each module
    int oscCounter = 1, lfoCounter = 1, vcoCounter = 1;
    
    int padding = 5;

    
    int buttonWidth = smallerBox.getWidth()/10;
    int buttonHeight = smallerBox.reduced(padding).getHeight();
    int buttonX  = smallerBox.reduced(padding).getX();
    int buttonY = smallerBox.reduced(padding).getY();

    // Create new buttons based on selectedModules
    for (int i = 0; i < 4; ++i)
    {
    

        
        juce::String module = selectedModules[i];

        if (module == "OSC")
        {
            newButton = std::make_unique<juce::TextButton>("OSC " + juce::String(oscCounter++));
        }
        else if (module == "LFO")
        {
            newButton = std::make_unique<juce::TextButton>("LFO " + juce::String(lfoCounter++));
        }
        else if (module == "VCO")
        {
            newButton = std::make_unique<juce::TextButton>("VCO " + juce::String(vcoCounter++));
        }
        if (newButton != nullptr)
        {
            
            addAndMakeVisible(*newButton);
            
            newButton->setBounds(buttonX, buttonY, buttonWidth, buttonHeight);
            
            buttonX+=65;
            
            moduleButtons.push_back(std::move(newButton));

        }
    }

    // Update layout
    resized();
    repaint();
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
    
    myTextComponent = std::make_unique<TextComponent>("Output: ");
    addAndMakeVisible(*myTextComponent);

    
    // Add components to the grid
    grid.items = {
        juce::GridItem(box1), juce::GridItem(dial1), juce::GridItem(dial2), juce::GridItem(dial3), juce::GridItem(dial4),
        juce::GridItem(box2), juce::GridItem(dial5), juce::GridItem(dial6), juce::GridItem(dial7), juce::GridItem(dial8),
        juce::GridItem(box3), juce::GridItem(dial9), juce::GridItem(dial10), juce::GridItem(dial11), juce::GridItem(dial12),
        juce::GridItem(box4), juce::GridItem(dial13), juce::GridItem(dial14), juce::GridItem(dial15), juce::GridItem(dial16),
        juce::GridItem(*myTextComponent), juce::GridItem(oDial1), juce::GridItem(oDial2), juce::GridItem(oDial3), juce::GridItem(oDial4),
    };
    
 
    window = getLocalBounds();
    
    
    topArea1 = window.removeFromTop(window.getHeight() * 0.70);
    
    auto gridArea = topArea1.removeFromBottom(topArea1.getHeight() * 0.80);
    
    grid.performLayout(gridArea);
    

    

    
//    myButton.setBounds(buttonX, buttonY, buttonWidth, buttonHeight);


    oscComponent.setBounds (getLocalBounds());
    
    repaint();

}
