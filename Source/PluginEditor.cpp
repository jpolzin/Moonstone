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
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    frequency.setSliderStyle (Slider::LinearBarVertical);
    frequency.setRange(100.0, 8000.0, 1.0);
    frequency.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    frequency.setPopupDisplayEnabled (true, false, this);
    frequency.setTextValueSuffix (" Hz");
    frequency.setValue(1.0);

//    addAndMakeVisible (&frequency);
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
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void MoonstoneAudioProcessorEditor::resized()
{
    frequency.setBounds (40, 30, 20, getHeight() - 60);
}
