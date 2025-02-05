/*
  ==============================================================================

    SynthVoice.cpp
    Created: 2 Feb 2025 5:22:49pm
    Author:  Taylot Brierley

  ==============================================================================
*/

#include "SynthVoice.h"


bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{

    
}


void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    
}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate); 
    
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    osc.prepare (spec);
    gain.prepare (spec);
    
    osc.setFrequency (220.0f);
    gain.setGainLinear (0.01f);
    
    isPrepared = true;
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    
    jassert(isPrepared);
    
//    juce::dsp::AudioBlock<float> audioBlock { outputBuffer };
//    osc.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
//    gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
}
