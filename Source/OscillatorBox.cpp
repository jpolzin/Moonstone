/*
  ==============================================================================

    OscillatorBox.cpp
    Created: 20 May 2019 10:55:57pm
    Author:  Joshua Polzin

  ==============================================================================
*/

#include <map>
#include <string>

#include "OscillatorBox.h"
#include "WavetableOscillator.h"

OscillatorBox::OscillatorBox(unsigned oscillatorID) :
    frequencyKnob(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::TextBoxBelow),
    outputKnob(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::TextBoxBelow)
{
    // frequencyKnob
    frequencyKnob.addListener(this);
    frequencyKnob.setRange(.5, 16, .5);
    addAndMakeVisible(frequencyKnob);

    // outputKnob
    outputKnob.addListener(this);
    outputKnob.setRange(0, 1, 0.01);
    addAndMakeVisible(outputKnob);

    // waveTypeSelector
    waveTypeSelector.addListener(this);
    auto waveTypesMap = getOsc()->getWaveTypes();
    for (const auto & wavePair : waveTypesMap)
    {
        waveTypeSelector.addItem(wavePair.first, wavePair.second);
    }
    waveTypeSelector.setSelectedId(WavetableOscillator::SINE);
    addAndMakeVisible(waveTypeSelector);

    updateUIFromOscillator();
}

void OscillatorBox::resized()
{
    frequencyKnob.setBounds(10, 40, 80, 80);
    outputKnob.setBounds(100, 40, 80, 80);
    waveTypeSelector.setBounds(10, 10, 150, 20);
}

void OscillatorBox::paint(Graphics &g)
{
    g.fillAll(Colours::dimgrey);
}

void OscillatorBox::mouseDown(const MouseEvent &e)
{
    dragger.startDraggingComponent (this, e);
}
void OscillatorBox::mouseDrag(const MouseEvent &e)
{
    dragger.dragComponent(this, e, nullptr);
}


void OscillatorBox::sliderValueChanged(Slider *slider)
{
    if (slider == &frequencyKnob)
    {
        getOsc()->setSyncFrequencyMultiplier(slider->getValue());
    }
    else if (slider == &outputKnob)
    {
        getOsc()->setOutputLevel(slider->getValue());
    }

}

void OscillatorBox::comboBoxChanged(juce::ComboBox *comboBox)
{
    if (comboBox == &waveTypeSelector)
    {
        getOsc()->setWaveType(comboBox->getSelectedId());
    }
}


void OscillatorBox::updateUIFromOscillator()
{
    frequencyKnob.setValue(getOsc()->getFrequencyMulitplier());
    outputKnob.setValue(getOsc()->getOutputLevel());
}