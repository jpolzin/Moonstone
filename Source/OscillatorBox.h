/*
  ==============================================================================

    OscillatorBox.h
    Created: 20 May 2019 10:55:57pm
    Author:  Joshua Polzin

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WavetableOscillator.h"

class OscillatorBox :
        public Component,
        public Slider::Listener,
        public ComboBox::Listener
{
public:
    OscillatorBox(unsigned oscillatorID);

    // Component overrides
    void resized() override;
    void paint(Graphics &g) override;

    void mouseDown(const MouseEvent &e) override;
    void mouseDrag(const MouseEvent &e) override;

    // Slider::Listener overrides
    void sliderValueChanged(Slider *slider) override;
    void comboBoxChanged(ComboBox *comboBox) override;


private:
    void updateUIFromOscillator();
    inline WavetableOscillator * getOsc() { return WavetableOscillator::getOsc(oscillatorID); }

    ComboBox waveTypeSelector;
    Slider frequencyKnob;
    Slider outputKnob;

    ComponentDragger dragger;
    unsigned oscillatorID;
};