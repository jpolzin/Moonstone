/*
  ==============================================================================

    SpectrumDisplay.h
    Created: 22 May 2019 4:15:53pm
    Author:  Joshua Polzin

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"


class SpectrumDisplay : public Component, public Timer
{
public:
    SpectrumDisplay();

    void addBuffer(AudioBuffer<float> &);

    void pushNextSampleIntoFifo (float sample);

    void drawNextFrameOfSpectrum();
    void drawFrame (Graphics &);

    void paint(Graphics &) override;
    void timerCallback() override;

    void resized() override;


private:
    static const int fftOrder  = 11;
    static const int fftSize   = 1 << fftOrder;
    static const int scopeSize = 2048;

    dsp::FFT forwardFFT;
    dsp::WindowingFunction<float> window;

    float fifo [fftSize];
    float fftData [2 * fftSize];
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;
    float scopeData [scopeSize];
};