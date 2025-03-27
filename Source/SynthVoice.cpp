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

    //initialise the OSCVoice list.

    adsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
      
    gain.prepare(spec);
    
    gain.setGainLinear (0.3f);
        
    for (int i=0; i<4; i++){
        oscillators[i].prepare(spec);
        oscillators[i].initialise([](float x) { return 0.0f; });
    }
    
    isPrepared = true;
}

void SynthVoice::paramUpdateVoice(const float freq){
    osc.setFrequency(freq);
}


void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);
    
    juce::dsp::AudioBlock<float> audioBlock { outputBuffer };

        
    for (int i=0; i<4; i++){
        juce::dsp::Oscillator<float>& selectedOscillator = oscillators.at(i);
                
        
        //check is enabled
        juce::String slot = "Slot" + juce::String(i+1);
        
        
        bool isEnabled = apvts.getRawParameterValue(slot+"_VCO_isEnabled")->load();
        
        DBG(slot+juce::String(isEnabled ? "true" : "false"));


        if (isEnabled){
            
            
            auto* waveformParam = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(slot+"_VCO_WF"));
            juce::String currentWF = waveformParam->getCurrentChoiceName();
            
            //get that ossilators parameters
            float freq = apvts.getRawParameterValue(slot+"_VCO_Freq")->load();
            selectedOscillator.setFrequency(freq);
            
            
            if (currentWF != previousWaveforms[i]){
                previousWaveforms[i] = currentWF;
                
                
            
                if (currentWF == "Sine"){
                    
                    selectedOscillator.initialise([](float x) { return std::sin (x); });
                    
                }else if(currentWF == "Saw"){
                    
                    selectedOscillator.initialise([](float x) {return x / juce::MathConstants<float>::pi;});
                    
                }else if(currentWF == "Square"){
                    
                    selectedOscillator.initialise([](float x) {return x < 0.0f ? -1.0f : 1.0f;} );
                    
                }else if(currentWF == "Noise"){
                    
                    selectedOscillator.initialise([](float) {return juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;});
                
                }
            }
            selectedOscillator.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
            gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));

        }
        
        
    }
    
}
