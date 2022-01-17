#pragma once
#include <JuceHeader.h>
#define DAMPING_ID "damping"
#define DAMPING_NAME "Damping"
#define SIZE_ID "size"
#define SIZE_NAME "Size"
#define WIDTH_ID "width"
#define WIDTH_NAME "Width"
#define WET_ID "wet"
#define WET_NAME "Wet"
#define DRY_ID "dry"
#define DRY_NAME "Dry"


class ResonateAudioProcessor  : public juce::AudioProcessor {
    public:
        ResonateAudioProcessor();
        ~ResonateAudioProcessor() override;

        void prepareToPlay(double sampleRate, int samplesPerBlock) override;
        void releaseResources() override;

       #ifndef JucePlugin_PreferredChannelConfigurations
        bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
       #endif

        void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

        juce::AudioProcessorEditor* createEditor() override;
        bool hasEditor() const override;

        const juce::String getName() const override;

        bool acceptsMidi() const override;
        bool producesMidi() const override;
        bool isMidiEffect() const override;
        double getTailLengthSeconds() const override;
        float dampingVal{1.25f}, sizeVal{0.5f}, widthVal{3.0f}, wetVal{0.5f}, dryVal{0.75f};
        juce::AudioProcessorValueTreeState pluginParameters; // Used to save/load plugin parameters through XML

        int getNumPrograms() override;
        int getCurrentProgram() override;
        void setCurrentProgram(int index) override;
        const juce::String getProgramName(int index) override;
        void changeProgramName(int index, const juce::String& newName) override;

        void getStateInformation(juce::MemoryBlock& destData) override;
        void setStateInformation(const void* data, int sizeInBytes) override;

    private:
        juce::dsp::Reverb leftStereoReverb, rightStereoReverb;
        juce::dsp::Reverb::Parameters reverbParameters;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ResonateAudioProcessor)
};
