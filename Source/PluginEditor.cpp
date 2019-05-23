/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MoonstoneAudioProcessorEditor::MoonstoneAudioProcessorEditor (MoonstoneAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), testBox(0)
{
    addAndMakeVisible(testBox);
    addAndMakeVisible(spectrum);
    setSize (900, 900);
}

MoonstoneAudioProcessorEditor::~MoonstoneAudioProcessorEditor()
{
}

//==============================================================================
void MoonstoneAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void MoonstoneAudioProcessorEditor::resized()
{
    testBox.setBounds(100, 0, getWidth(), getHeight());
    spectrum.setBounds(0, getHeight() - 300, getWidth(), 300);
}
