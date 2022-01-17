#include "PluginProcessor.h"
#include "PluginEditor.h"


ResonateAudioProcessorEditor::ResonateAudioProcessorEditor (ResonateAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
    // ATTACH SLIDERS AS PARAMETERS
    dampingSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.pluginParameters, DAMPING_ID, dampingKnob);
    sizeSliderAttachment    = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.pluginParameters, SIZE_ID,    sizeKnob);
    widthSliderAttachment   = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.pluginParameters, WIDTH_ID,   widthKnob);
    wetSliderAttachment     = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.pluginParameters, WET_ID,     wetKnob);
    drySliderAttachment     = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.pluginParameters, DRY_ID,     dryKnob);

    // SET TYPE OF SLIDER OBJECT FOR EACH KNOB (ROTARY)
    dampingKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sizeKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    widthKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    wetKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    dryKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);

    // SET RANGE OF EACH KNOB (MIN/MAX VALUES)
    dampingKnob.setRange(0.0f, 2.0f, 0.01f);
    sizeKnob.setRange(0.0f, 1.0f, 0.01f);
    widthKnob.setRange(0.0f, 5.0f, 0.01f);
    wetKnob.setRange(0.0f, 1.0f, 0.01f);
    dryKnob.setRange(0.0f, 1.0f, 0.01f);

    // INITIALIZE EACH ELEMENT IN VIEW FOR THE USER
    addAndMakeVisible(dampingKnob);
    addAndMakeVisible(sizeKnob);
    addAndMakeVisible(widthKnob);
    addAndMakeVisible(wetKnob);
    addAndMakeVisible(dryKnob);

    // ADD LISTENERS FOR EACH KNOB
    dampingKnob.addListener(this);
    sizeKnob.addListener(this);
    widthKnob.addListener(this);
    wetKnob.addListener(this);
    dryKnob.addListener(this);

    // SET EACH KNOB'S TEXTBOX BELOW
    dampingKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    sizeKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    widthKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    wetKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    dryKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

    // SET SIZE OF THE VST PLUGIN WINDOW
    setSize(620, 376);
}

ResonateAudioProcessorEditor::~ResonateAudioProcessorEditor() {}

void ResonateAudioProcessorEditor::paint (juce::Graphics& g) {
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::plugin_background_png, BinaryData::plugin_background_pngSize);
    g.drawImageAt(background, 0, 0);
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colour::fromRGBA(56, 4, 174, 240));
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colour::fromRGBA(56, 4, 174, 100));
}

void ResonateAudioProcessorEditor::resized() {
    // LAYOUT KNOBS IN GUI
    dampingKnob.setBounds(getWidth()/4 - 130, (getHeight()/3)*2 - 130, 220, 220);
    sizeKnob.setBounds(getWidth()/2 - 30, getHeight()/3 - 90, 140, 140);
    widthKnob.setBounds((getWidth()/3)*2 + 40, getHeight()/3 - 90, 140, 140);
    wetKnob.setBounds(getWidth()/2 - 30, (getHeight()/3)*2 - 50, 140, 140);
    dryKnob.setBounds((getWidth()/3)*2 + 40, (getHeight()/3)*2 - 50, 140, 140);
}

void ResonateAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    // ATTACH AUDIO VARIABLES IN <PluginProcessor.h> TO KNOBS IN <PluginEditor.h>
    if (slider == &dampingKnob) audioProcessor.dampingVal = dampingKnob.getValue();
    if (slider == &sizeKnob)    audioProcessor.sizeVal    = sizeKnob.getValue();
    if (slider == &widthKnob)   audioProcessor.widthVal   = widthKnob.getValue();
    if (slider == &wetKnob)     audioProcessor.wetVal     = wetKnob.getValue();
    if (slider == &dryKnob)     audioProcessor.dryVal     = dryKnob.getValue();
}
