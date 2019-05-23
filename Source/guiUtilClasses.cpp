/*
  ==============================================================================

    guiUtilClasses.cpp
    Created: 21 May 2019 4:00:29pm
    Author:  Joshua Polzin

  ==============================================================================
*/

#include "guiUtilClasses.h"

RotationalSlider::RotationalSlider(double min, double max, std::string suffix)
{
    slider.addListener(this);
    slider.setRange(min, max);
    slider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);

    addAndMakeVisible(slider);
//    addAndMakeVisible(label);
}

void RotationalSlider::resized()
{
    std::cerr << getWidth();
    slider.setBounds(0, 0, getWidth(), getWidth());
//    label.setBounds(0, getWidth(), getWidth(), getHeight() - getWidth());
}

void RotationalSlider::paint(Graphics &g)
{
    g.setColour(Colours::azure);
}

void RotationalSlider::sliderValueChanged(Slider *slider)
{

}