/*
  ==============================================================================

    SynthVoice.h
    Created: 2 Feb 2025 5:22:49pm
    Author:  Taylot Brierley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
    public:
    SynthVoice(juce::AudioProcessorValueTreeState& state)
        : apvts(state), matrixValues(4, std::vector<float>(4, 0.0f)) // Incorrect semicolon
    {
        // Initialization code
    }
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    void paramUpdateVoice(const float freq);
    float voltageToFrequency(float voltage, float referenceFrequency = 440.0f);
    void populateMatrixValues();
    void calcOutputVoltages();
    
    
    private:
    

    
    std::vector<float> values;
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    
    juce::dsp::ProcessSpec spec;
    juce::dsp::Oscillator<float> osc { [](float x) { return std::sin (x); }};
    juce::dsp::Gain<float> gain;
    bool isPrepared{false};
    std::vector<juce::dsp::Oscillator<float>> InputOscillators { 4 };
    float summedVoltage;
    
    std::vector<float> outputVoltages {0.f, 0.f, 0.f, 0.f};
    float currentColumn = 0.f;

    std::vector<juce::String> previousWaveforms {"","","",""};
    juce::AudioProcessorValueTreeState& apvts;
    
    std::vector<std::vector<float>> matrixValues;
    
    int columnSum;
    
    // [](float x) { return std::sin (x);} //Sine Wave
    // [](float x) {return x / MathConstants<float>::pi;} // Saw Wave
    // [](float x) {return x < 0.0f ? -1.0f : 1.0f;}  // Square Wave
    
};
