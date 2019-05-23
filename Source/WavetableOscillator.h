/*
  ==============================================================================

    WavetableOscillator.h
    Created: 13 May 2019 5:04:34pm
    Author:  Joshua Polzin

  ==============================================================================
*/

#pragma once

#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>
#include <set>

#include "../JuceLibraryCode/JuceHeader.h"
#include "Constants.h"


class WavetableOscillator
{
public:
    struct VoicePosition {
        double currentSample = 0;
        double defaultCyclesPerSample = 0;
        unsigned currentSampleID = 0;
    };

    enum WaveType {
        SINE,
        SAW,
        SQUARE,
        TRIANGLE
    };

    WavetableOscillator();

    static void createOscillators();
    static WavetableOscillator* getOsc(unsigned oscillatorID);
    
    float getCurrentSample(unsigned voiceID);
    void advanceToNextSample(unsigned voiceID, unsigned sampleID);

    void startNote(unsigned voiceID, double newCyclesPerSample);
    void stopNote(unsigned voiceID);

    void addModulator(unsigned oscillatorID);
    void removeModulator(unsigned oscillatorID);
    void removeAllModulators();

    void setSyncFrequencyMultiplier(double multiplier);
    void setOutputLevel(double level);
    void setWaveType(WaveType waveType);

private:
    // RQ: 0 <= location < 1
    // RT: interpolated sample at fractional location in the table
    float interpolateToLocation(double location);
    
    void generateSineWave(unsigned length);

    static unsigned sample_rate;
    static std::vector<WavetableOscillator> bank;

    std::vector<float> wavetable;
    std::set<unsigned> modulators;

    std::vector<VoicePosition> voices;

    // oscillator-wide setttings
    double syncFrequencyMultiplier = 1;
    double outputLevel = 1;
};

