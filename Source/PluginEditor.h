/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MoonstoneAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    MoonstoneAudioProcessorEditor (MoonstoneAudioProcessor&);
    ~MoonstoneAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MoonstoneAudioProcessor& processor;

    Slider frequency;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoonstoneAudioProcessorEditor)
};
