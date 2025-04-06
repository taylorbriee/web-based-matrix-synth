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


float convertVoltageToFrequency(float voltage)
{
    
    
//    DBG("Volt: "+ juce::String(voltage));

    voltage = juce::jlimit(-1.0f, 1.0f, voltage);
//    DBG("Volt: "+ juce::String(voltage));

    
    // Map -1.0 → 0.0 Hz, 0.0 → 2957.5 Hz, 1.0 → 5915.0 Hz
    return juce::jmap(voltage, -1.0f, 1.0f, 0.0f, 5915.0f);
}

void SynthVoice::calcOutputVoltages(){
    
    for(int y=0; y<4; y++){
        for(int x=0; x<4; x++){
            currentColumn += matrixValues[x][y];
        }
        
//        DBG("currentColumn: "+ juce::String(currentColumn));

        
        outputVoltages[y] = currentColumn;
        currentColumn=0;
        
        
//        DBG("Output "+juce::String(y)+" Volt: "+ juce::String(outputVoltages[y]));

        
//        DBG("Output Volt: "+ juce::String(outputVoltages[y]));


        
    }
}


void SynthVoice::OSC_Creation(int i, const juce::String &slot, juce::String Module) {
   
    bool isEnabled = apvts.getRawParameterValue(slot+"_"+Module+"_isEnabled")->load();

    juce::dsp::Oscillator<float>& selectedOscillator = InputOscillators.at(i);


    if (isEnabled){
        
        auto* waveformParam = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(slot+"_"+Module+"_WF"));
        juce::String currentWF = waveformParam->getCurrentChoiceName();
        
        //get that ossilators parameters
        float freq = apvts.getRawParameterValue(slot+"_"+Module+"_Freq")->load();
        
//        DBG("currentFreq: "+juce::String(freq));
//        DBG("Setting Frequency for osc " + juce::String(i) + ": " + juce::String(freq));
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
        
        //check is enabled
        juce::String slot = "Slot" + juce::String(i+1);
        
        
//        bool LFOisEnabled = apvts.getRawParameterValue(slot+"_LFO_isEnabled")->load();


        OSC_Creation(i, slot, "VCO");
        OSC_Creation(i, slot, "LFO");
        
        

        
    }
        
        //check which dropdown has Main Output selected
        //then depending on which do pass voltage to that osc and process it to th
        

    for (int i = 0; i < 4; i++)
    {
        auto* outputSelectParam = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter("Matrix_Output" + juce::String(i + 1)));
        juce::String outputDropdown = outputSelectParam->getCurrentChoiceName();
        isMainOutput[i] = outputDropdown;
        
        vcoModValues[i] = apvts.getRawParameterValue("Slot"+juce::String(i + 1)+"_VCO_Freq_Mod")->load();
        
//        DBG("Output "+ juce::String(i)+ juce::String(outputDropdown));
        
        //append to outputChoicesArray here, make sure you are converting the combo box text to parameter ID before you store it.
        
        
        //we need the box id to get the corripsonding parameter id from OutComboBoxIdToText.
        
//        outputSelections[i] = 
        
//        DBG("Output "+juce::String(i+1)+" choice: "+ juce::String(isMainOutput[i] ? "true" : "false"));

    }
    
    //Itterate over

    for (int sample = 0; sample < numSamples; ++sample)
    {
        float summedVoltage = 0.0f;

        populateMatrixValues();
        calcOutputVoltages();

        
        //looping over outputs
        for (int i = 0; i < 4; i++)
        {

            juce::String slot = "Slot"+juce::String(i+1);

//            DBG("Output "+juce::String(i)+" Volt: "+ juce::String(outputVoltages[i]));
//            DBG(slot+"_VCO_Freq");
//            DBG("Is Main Output:"+ isMainOutput[i]);
            if (isMainOutput[i] == "Main Output")
            {
                summedVoltage += outputVoltages[i];

            }
            
            else if (isMainOutput[i].contains("_VCO_Freq")){
                
                
//                DBG("Output Volt: "+ juce::String(outputVoltages[i]));

                
                juce::String text = isMainOutput[i]; // Example: "Slot1_VCO_Freq"

                if (text.matchesWildcard("Slot?_*", false))  // Ensures format before extracting
                {
                    juce::String numberPart = text.fromFirstOccurrenceOf("Slot", false, false)
                        .upToFirstOccurrenceOf("_", false, false);
                    
                    oscIndex = numberPart.getIntValue(); // Converts to int safely
                    
                }
                //ISSUE
//                DBG("Checking Frequency of osc " + juce::String(oscIndex - 1) + ": " +
//                    juce::String(InputOscillators[oscIndex - 1].getFrequency()));

                currentFreq = InputOscillators[oscIndex-1].getFrequency();
                currentDepthParam = vcoModValues[oscIndex-1];
                currentColumnSig = outputVoltages[i];
                
                
                
//                DBG("currentFreq: "+juce::String(currentFreq));
                
//                float convertedFreq = convertVoltageToFrequency(outputVoltages[i]);


                
                float modFreq = (currentDepthParam / 100.0f) * currentColumnSig * 1000.0;

                modFreq = currentFreq + (currentColumnSig * currentDepthParam);
                limitedFreq = juce::jlimit(0.0f, 5915.0f, modFreq);
                
//                DBG("convertedFreq: "+juce::String(convertedFreq));

                
//                DBG("currentFreq: "+juce::String(limitedFreq));

//                DBG("Limited freq: "+juce::String(limitedFreq));
                InputOscillators[oscIndex-1].setFrequency(limitedFreq);
                
            }
        }

        float audioSample = juce::jlimit(-1.0f, 1.0f, summedVoltage);
        outputBuffer.setSample(0, startSample + sample, audioSample);
    }

    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}


    
    

