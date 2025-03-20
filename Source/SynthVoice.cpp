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
//    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
    return true;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
//    osc.setFrequency (juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber));
//    adsr.noteOn();
    
}


void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
//    adsr.noteOff();
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
      
    gain.prepare(spec);
    osc.prepare(spec);
    
    osc.setFrequency (220.0f);
    gain.setGainLinear (0.5f);
    
    isPrepared = true;
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    
    jassert(isPrepared);
    
    juce::dsp::AudioBlock<float> audioBlock { outputBuffer };
    
    //line below is causing apple audio thread issues
    osc.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
//    adsr.applyEnvelopeToBuffer(outputBuffer, startSample,  numSamples);
}
