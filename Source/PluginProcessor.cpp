/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WebMatrixSynthAudioProcessor::WebMatrixSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
                     #if !JucePlugin_IsMidiEffect
                      #if !JucePlugin_IsSynth
                       .withInput  ("Input", juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
    , apvts(*this, nullptr, "Parameters", createParams())
{
    synth.addSound (new SynthSound());
//    synth.addVoice (new SynthVoice());
    
    for (int i = 0; i < 8; ++i) // 8 voices
        synth.addVoice(new SynthVoice());
}

WebMatrixSynthAudioProcessor::~WebMatrixSynthAudioProcessor()
{
}

//==============================================================================
const juce::String WebMatrixSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WebMatrixSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WebMatrixSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WebMatrixSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WebMatrixSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WebMatrixSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WebMatrixSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WebMatrixSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WebMatrixSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void WebMatrixSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WebMatrixSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{


    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void WebMatrixSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WebMatrixSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WebMatrixSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    //original issue here with increment post fix
    for (int i=0; i<synth.getNumSounds(); ++i){
        if (auto voice = dynamic_cast<SynthVoice* >(synth.getVoice(i))){
            // Osc controls
            // ADSR
            // LFO
            

            auto Slot1_VCO_Freq = apvts.getRawParameterValue("Slot1_VCO_Freq");
            DBG("Slot1_VCO_Freq: " + juce::String(Slot1_VCO_Freq->load()));

//            voice->paramUpdateVoice(Slot1_VCO_Freq);
            
    
        }
    }
    
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool WebMatrixSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WebMatrixSynthAudioProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
void WebMatrixSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WebMatrixSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WebMatrixSynthAudioProcessor();
}


juce::AudioProcessorValueTreeState::ParameterLayout WebMatrixSynthAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    

    juce::String slot;
    
    juce::String paramID;
    
    for(int i=1; i<5; i++){
        
        
        slot = "Slot" + juce::String(i);

        
        //VCO:
        //Frequency
        //Pulse Width
        //Sine, Saw, Square, Noise
        //inc MIDI, Drone
        //monophonic, polyphonic
        

        paramID = slot+"_VCO_Freq";
        
        params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(paramID, 1), slot+" VCO Frequency", juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.01f }, 0.0f));

        
        DBG("VCO ID: "+ paramID);
        
        
        paramID = slot+"_VCO_PW";
        
        params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(paramID, 1), slot+" VCO Pulse Width", juce::NormalisableRange<float> { 0.0f, 100.0f, 0.01f }, 0.0f));
        
        
        paramID = slot+"_VCO_WF";
        
        params.push_back(std::make_unique<juce::AudioParameterChoice> (juce::ParameterID(paramID, 1), slot+" VCO Waveform", juce::StringArray {"Sine","Saw","Square","Noise"}, 0));
        
        paramID = slot+"_VCO_Inputs";
        
        params.push_back(std::make_unique<juce::AudioParameterChoice> (juce::ParameterID(paramID, 1), slot+" VCO Inputs", juce::StringArray {"inc MIDI", "Drone"}, 0));
        
        
        paramID = slot+"_VCO_VM";
        
        params.push_back(std::make_unique<juce::AudioParameterChoice> (juce::ParameterID(paramID, 1), slot+" VCO Voice Mode", juce::StringArray {"monophonic", "polyphonic"}, 0));
        
        
        
        
        //LFO:
        
        //Frequency
        //Pulse Width
        //Sine, Saw, Square, Noise
        
        paramID = slot+"_LFO_Freq";
        
        params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(paramID, 1), slot+" LFO Frequency", juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.01f }, 0.0f, "Hz"));
        
        
        paramID = slot+"_LFO_PW";
        
        params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(paramID, 1), slot+" LFO Pulse Width", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.0f, "%"));
        
        
        paramID = slot+"_LFO_WF";
        
        params.push_back(std::make_unique<juce::AudioParameterChoice> (juce::ParameterID(paramID, 1), slot+" LFO Waveform", juce::StringArray {"Sine","Saw","Square","Noise"}, 0));

        
        //Slot1_VCO_Freq
    
        
    }
    
    
    
    return { params.begin(), params.end() };
}
