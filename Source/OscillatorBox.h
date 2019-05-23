/*
  ==============================================================================

    OscillatorBox.h
    Created: 20 May 2019 10:55:57pm
    Author:  Joshua Polzin

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class OscillatorBox :
        public Component,
        public Slider::Listener
{
public:
    OscillatorBox(unsigned oscillatorID);

    // Component overrides
    void resized() override;

    // Slider::Listener overrides
    void sliderValueChanged(Slider *slider) override;


private:
    void refreshUI()
    {

    }

    Slider frequencyKnob;
    unsigned oscillatorID;
};