#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"


class ResonateAudioProcessorEditor : public juce::AudioProcessorEditor,
                                     public juce::Slider::Listener {
    public:
        ResonateAudioProcessorEditor (ResonateAudioProcessor&);
        ~ResonateAudioProcessorEditor() override;

        void paint(juce::Graphics&) override;
        void resized() override;
        void sliderValueChanged(juce::Slider* slider) override;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dampingSliderAttachment,
                                                                              sizeSliderAttachment,
                                                                              widthSliderAttachment,
                                                                              wetSliderAttachment,
                                                                              drySliderAttachment;

    private:
        juce::Slider dampingKnob;
        juce::Slider sizeKnob;
        juce::Slider widthKnob;
        juce::Slider wetKnob;
        juce::Slider dryKnob;

        ResonateAudioProcessor& audioProcessor;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResonateAudioProcessorEditor)
};
