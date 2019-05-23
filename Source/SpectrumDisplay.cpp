/*
  ==============================================================================

    SpectrumDisplay.cpp
    Created: 22 May 2019 4:15:53pm
    Author:  Joshua Polzin

  ==============================================================================
*/

#include "SpectrumDisplay.h"

SpectrumDisplay::SpectrumDisplay() : forwardFFT(fftOrder),
    window(fftSize, dsp::WindowingFunction<float>::blackman)
{
    setOpaque (true);
    startTimerHz (20);
    setSize (700, 500);
}

void SpectrumDisplay::addBuffer(AudioBuffer<float> &buffer)
{
    if (buffer.getNumChannels() > 0)
    {
        const float* readPtr = buffer.getReadPointer (0);

        for (auto i = 0; i < buffer.getNumSamples(); ++i)
            pushNextSampleIntoFifo (readPtr[i]);
    }
}

//==============================================================================
void SpectrumDisplay::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    g.setOpacity (1.0f);
    g.setColour (Colours::white);
    drawFrame (g);
}

void SpectrumDisplay::timerCallback()
{
    if (nextFFTBlockReady)
    {
        drawNextFrameOfSpectrum();
        nextFFTBlockReady = false;
        repaint();
    }
}

void SpectrumDisplay::pushNextSampleIntoFifo (float sample)
{
    // if the fifo contains enough data, set a flag to say
    // that the next frame should now be rendered..
    if (fifoIndex == fftSize)
    {
        if (! nextFFTBlockReady)
        {
            zeromem (fftData, sizeof (fftData));
            memcpy (fftData, fifo, sizeof (fifo));
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[fifoIndex++] = sample;
}

void SpectrumDisplay::drawNextFrameOfSpectrum()
{
    // first apply a windowing function to our data
    window.multiplyWithWindowingTable (fftData, fftSize);

    // then render our FFT data..
    forwardFFT.performFrequencyOnlyForwardTransform (fftData);

    auto mindB = -100.0f;
    auto maxdB =    0.0f;

    for (int i = 0; i < scopeSize; ++i)
    {
        auto skewedProportionX = 1.0f - std::exp (std::log (1.0f - i / (float) scopeSize) * 0.2f);
        auto fftDataIndex = jlimit (0, fftSize / 2, (int) (skewedProportionX * fftSize / 2));
        auto level = jmap (jlimit (mindB, maxdB, Decibels::gainToDecibels (fftData[fftDataIndex])
                                                 - Decibels::gainToDecibels ((float) fftSize)),
                           mindB, maxdB, 0.0f, 1.0f);

        scopeData[i] = level;
    }
}

void SpectrumDisplay::drawFrame(Graphics& g)
{
    auto width  = getLocalBounds().getWidth();
    auto height = getLocalBounds().getHeight();

    for (int i = 1; i < scopeSize; ++i)
    {
        g.drawLine ({ (float) jmap (i - 1, 0, scopeSize - 1, 0, width),
                      jmap (scopeData[i - 1], 0.0f, 1.0f, (float) height, 0.0f),
                      (float) jmap (i,     0, scopeSize - 1, 0, width),
                      jmap (scopeData[i],     0.0f, 1.0f, (float) height, 0.0f) });
    }

    float delta = 100 * scopeSize * 4 / 44100;
    float freq = delta + 3;
    int count = 1;

    while (freq < 44100 / 2)
    {
        if (count % 10 == 0)
        {
            g.setOpacity(0.8f);
        }
        else if (count % 5 == 0)
        {
            g.setOpacity(0.5f);
        }
        else
        {
            g.setOpacity(0.25f);
        }

        g.drawLine(freq, 0, freq, float(height));
        freq += delta;
        ++count;
    }
}

void SpectrumDisplay::resized()
{
}