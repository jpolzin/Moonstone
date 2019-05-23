/*
  ==============================================================================

    OscillatorBox.cpp
    Created: 20 May 2019 10:55:57pm
    Author:  Joshua Polzin

  ==============================================================================
*/


#include "OscillatorBox.h"
#include "WavetableOscillator.h"

OscillatorBox::OscillatorBox(unsigned oscillatorID) :
    frequencyKnob(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::TextBoxBelow)
{
    frequencyKnob.addListener(this);

    frequencyKnob.setRange(.5, 16, .5);
    addAndMakeVisible(frequencyKnob);
}

void OscillatorBox::resized()
{
    frequencyKnob.setBounds(0, 0, 80, 100);
}

void OscillatorBox::sliderValueChanged(Slider *slider)
{
    if (slider == &frequencyKnob)
    {
        WavetableOscillator::getOsc(oscillatorID)->setSyncFrequencyMultiplier(slider->getValue());
    }


}