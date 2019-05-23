/*
  ==============================================================================

    guiUtilClasses.h
    Created: 21 May 2019 4:00:29pm
    Author:  Joshua Polzin

  ==============================================================================
*/

#pragma once

#include <string>

#include "../JuceLibraryCode/JuceHeader.h"

class RotationalSlider : public Component, public Slider::Listener
{
public:
    RotationalSlider(double min, double max, std::string suffix);

    void resized() override;
    void paint(Graphics &g) override;

    void sliderValueChanged(Slider *slider) override;


    Slider slider;
//    Label label;
};