#include "PluginProcessor.h"
#include "PluginEditor.h"


ResonateAudioProcessor::ResonateAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor(BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        pluginParameters(*this, nullptr)
#endif
{
    juce::NormalisableRange<float> dampingRange(0.0f, 2.0f, 0.01f),
                                   sizeRange(0.0f, 1.0f, 0.01f),
                                   widthRange(0.0f, 5.0f, 0.01f),
                                   wetRange(0.0f, 1.0f, 0.01f),
                                   dryRange(0.0f, 1.0f, 0.01f);
    pluginParameters.createAndAddParameter(DAMPING_ID, DAMPING_NAME, DAMPING_NAME, dampingRange, 1.25f, nullptr, nullptr);
    pluginParameters.createAndAddParameter(SIZE_ID, SIZE_NAME, SIZE_NAME, sizeRange, 0.5f, nullptr, nullptr);
    pluginParameters.createAndAddParameter(WIDTH_ID, WIDTH_NAME, WIDTH_NAME, widthRange, 3.0f, nullptr, nullptr);
    pluginParameters.createAndAddParameter(WET_ID, WET_NAME, WET_NAME, wetRange, 0.5f, nullptr, nullptr);
    pluginParameters.createAndAddParameter(DRY_ID, DRY_NAME, DRY_NAME, dryRange, 0.75f, nullptr, nullptr);
    pluginParameters.state = juce::ValueTree("savedPluginParameters");
}

ResonateAudioProcessor::~ResonateAudioProcessor() {}
const juce::String ResonateAudioProcessor::getName() const { return JucePlugin_Name; }
double ResonateAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int ResonateAudioProcessor::getNumPrograms() { return 1; }
int ResonateAudioProcessor::getCurrentProgram() { return 0; }
void ResonateAudioProcessor::setCurrentProgram(int index) {}
const juce::String ResonateAudioProcessor::getProgramName(int index) { return {}; }
void ResonateAudioProcessor::changeProgramName(int index, const juce::String& newName) {}
void ResonateAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {}
void ResonateAudioProcessor::releaseResources() {}
bool ResonateAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* ResonateAudioProcessor::createEditor() { return new ResonateAudioProcessorEditor(*this); }
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new ResonateAudioProcessor(); }

bool ResonateAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ResonateAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ResonateAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ResonateAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ResonateAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    reverbParameters.damping  = dampingVal;
    reverbParameters.roomSize = sizeVal;
    reverbParameters.width    = widthVal;
    reverbParameters.wetLevel = wetVal;
    reverbParameters.dryLevel = dryVal;

    leftStereoReverb.setParameters(reverbParameters);
    rightStereoReverb.setParameters(reverbParameters);

    juce::dsp::AudioBlock<float> block(buffer);
    auto leftStereoReverbBlock{block.getSingleChannelBlock(0)},
         rightStereoReverbBlock{block.getSingleChannelBlock(1)};
    juce::dsp::ProcessContextReplacing<float> leftStereoReverbContext(leftStereoReverbBlock),
                                              rightStereoReverbContext(rightStereoReverbBlock);

    leftStereoReverb.process(leftStereoReverbContext);
    rightStereoReverb.process(rightStereoReverbContext);
}

void ResonateAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    std::unique_ptr<juce::XmlElement> getStateXML(pluginParameters.state.createXml());
    copyXmlToBinary(*getStateXML, destData);
}

void ResonateAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> setStateXML(getXmlFromBinary(data, sizeInBytes));
    if (setStateXML != nullptr) {
        if (setStateXML->hasTagName(pluginParameters.state.getType())) {
            pluginParameters.state = juce::ValueTree::fromXml(*setStateXML);
        }
    }
}
