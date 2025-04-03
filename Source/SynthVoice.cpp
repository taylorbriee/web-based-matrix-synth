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



float voltageToFrequency(float voltage, float referenceFrequency = 440.0f)
{
    return referenceFrequency * std::pow(2.0f, voltage);
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
        InputOscillators[i].prepare(spec);
        InputOscillators[i].initialise([](float x) { return 0.0f; });
    }
    
    
    
    
    isPrepared = true;
}

void SynthVoice::paramUpdateVoice(const float freq){
    osc.setFrequency(freq);
}

void SynthVoice::populateMatrixValues(){
    
    //add dials to apvts parameters
    
    for(int y=0; y<4; y++){
        
        //check if osc is enabled, then do this
        //debug the line below
        
        float oscValue = InputOscillators[y].processSample(0.0f);
//        float scaledOSCValue = juce::jmap(oscValue, -1.0f, 1.0f, -5.0f, 5.0f);
        
        
        
        for(int x=0; x<4; x++){
            //acess all the dial values one by one
            float dialValue = apvts.getRawParameterValue(juce::String(x+1)+"x"+juce::String(y+1)+"Dial")->load();
            //scaling the rows value but them and adding the values to their space in the matrix
            
            //fix this matrix.
            matrixValues[y][x] = oscValue * (dialValue/10);
            
//            DBG(juce::String(x+1)+"x"+juce::String(y+1)+"Dial: "+ juce::String(matrixValues[y][x]));

        }
    }
}


void SynthVoice::calcOutputVoltages(){
    
    for(int y=0; y<4; y++){
        for(int x=0; x<4; x++){
            currentColumn += matrixValues[x][y];
        }
        
        outputVoltages[y] = currentColumn;
        currentColumn=0;
        
        
//        DBG("Output "+juce::String(y)+" Volt: "+ juce::String(outputVoltages[y]));

        
//        DBG("Output Volt: "+ juce::String(outputVoltages[y]));


        
    }
}


void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);
    
    juce::dsp::AudioBlock<float> audioBlock { outputBuffer };


    
    //for the columns that have main output selected, sum up the values in that column
    //create an ossilator with the initilize function just being the varaible of the summed voltage values.
    //call prepare on those ossilators.
    

        
    for (int i=0; i<4; i++){
        
        juce::dsp::Oscillator<float>& selectedOscillator = InputOscillators.at(i);
        //check is enabled
        juce::String slot = "Slot" + juce::String(i+1);
        bool isEnabled = apvts.getRawParameterValue(slot+"_VCO_isEnabled")->load();
        
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
            
            //            selectedOscillator.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
            
        }
    }
        
        //check which dropdown has Main Output selected
        //then depending on which do pass voltage to that osc and process it to th
        

    std::array<bool, 4> isMainOutput;
    for (int i = 0; i < 4; i++)
    {
        auto* outputSelectParam = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter("Matrix_Output" + juce::String(i + 1)));
        juce::String outputDropdown = outputSelectParam->getCurrentChoiceName();
        isMainOutput[i] = (outputDropdown == "Main Output");
        
//        DBG("Output "+juce::String(i+1)+" choice: "+ juce::String(isMainOutput[i] ? "true" : "false"));

    }

    for (int sample = 0; sample < numSamples; ++sample)
    {
        float summedVoltage = 0.0f;

        populateMatrixValues();
        calcOutputVoltages();
        
        for (int i = 0; i < 4; i++)
        {
//            DBG("Output "+juce::String(i)+" Volt: "+ juce::String(outputVoltages[i]));
            
            if (isMainOutput[i])
            {
                summedVoltage += outputVoltages[i];
//                DBG("Output Volt: "+ juce::String(outputVoltages[i]));
//                DBG("Output "+juce::String(i)+" Volt: "+ juce::String(outputVoltages[i]));

            }
        }

        float audioSample = juce::jlimit(-1.0f, 1.0f, summedVoltage);
        outputBuffer.setSample(0, startSample + sample, audioSample);
    }

    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}


    
    

