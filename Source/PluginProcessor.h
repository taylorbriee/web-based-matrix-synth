/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SynthSound.h"
#include "SynthVoice.h"

#include <vector>
#include <memory>


//==============================================================================
/**
*/
class WebMatrixSynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    WebMatrixSynthAudioProcessor();
    ~WebMatrixSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void createGridParam(int x, int y, std::vector<std::unique_ptr<juce::RangedAudioParameter>>& params);

    void addVoice(juce::String);
    
    void addModule(const juce::String moduleType, int slot)
    {
        selectedModules[slot]=moduleType;
    }
    
    juce::String getModule(int slot) const
    {
        return selectedModules[slot];
    }
    
    juce::AudioProcessorValueTreeState apvts;

    juce::String selectedModules[4] = {"", "", "", ""};
    
    


private:
    //==============================================================================
    
    juce::String slot;
    juce::Synthesiser synth;
    std::vector<std::unique_ptr<SynthVoice>> currentVoices;
    juce::String OSCSlots[4] = {"", "", "", ""};


    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    float VCO_Freq;
    

    



    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WebMatrixSynthAudioProcessor)
};
