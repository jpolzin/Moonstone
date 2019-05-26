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
#include <map>

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

    enum {
        SINE = 1,
        SAW = 2,
        SQUARE = 3,
        TRIANGLE = 4
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
    void setWaveType(int waveType);

    const std::map<std::string, int>& getWaveTypes();

    inline double getFrequencyMulitplier() { return syncFrequencyMultiplier; }
    inline double getOutputLevel() { return outputLevel; }

private:
    // RQ: 0 <= location < 1
    // RT: interpolated sample at fractional location in the table
    float interpolateToLocation(double location);
    
    void generateSineWave(unsigned length = Constants::DEFAULT_WAVETABLE_SIZE);
    void generateSawWave(unsigned length = Constants::DEFAULT_WAVETABLE_SIZE);
    void generateSquareWave(unsigned length = Constants::DEFAULT_WAVETABLE_SIZE);
    void generateTriangleWave(unsigned length = Constants::DEFAULT_WAVETABLE_SIZE);

    static unsigned sample_rate;
    static std::vector<WavetableOscillator> bank;
    static std::map<std::string, int> waveTypesMap;

    std::vector<float> wavetable;
    std::set<unsigned> modulators;

    std::vector<VoicePosition> voices;

    // oscillator-wide settings
    double syncFrequencyMultiplier = 1;
    double outputLevel = .2;
};

