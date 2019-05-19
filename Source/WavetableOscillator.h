/*
  ==============================================================================

    WavetableOscillator.h
    Created: 13 May 2019 5:04:34pm
    Author:  Joshua Polzin

  ==============================================================================
*/

#pragma once
#include "FMVoice.h"

#include <cassert>
#include <cmath>
#include <vector>
#include <map>

enum WaveType {
    SINE,
    SAW,
    SQUARE,
    TRIANGLE
};

struct WavetableSynthesiserSound : public SynthesiserSound
{
public:
    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
};

class WavetableOscillator
{
    struct VoicePosition {
        double currentSample = 0;
        double cyclesPerSample = 0;
        unsigned currentSampleID = 0;
    };

    static const unsigned DEFAULT_WAVETABLE_LENGTH = 128;

    static unsigned sample_rate;

    std::vector<float> wavetable;
    std::vector<WavetableOscillator*> modulators;
    
    std::map<SynthesiserVoice*, VoicePosition> voices;

public:
    WavetableOscillator(WaveType waveType) : voices() {
        switch (waveType)
        {
            case WaveType::SINE:
                generateSineWave(DEFAULT_WAVETABLE_LENGTH);
        }
    }
    
    float getCurrentSample(SynthesiserVoice* voiceID)
    {
        return getSample(voices[voiceID].currentSample);
    }
    
    void advanceToNextSample(SynthesiserVoice* voiceID, unsigned sampleID)
    {
        // don't advance if the id has already been used for this update
        if (voices[voiceID].currentSampleID == sampleID)
        {
            return;
        }
        voices[voiceID].currentSampleID = sampleID;
        
        // advance any dependencies
        for (auto modulator : modulators)
        {
            modulator->advanceToNextSample(voiceID, sampleID);
        }
        
        // calculate frequency modulations
        float modulation = 0;
        for (auto modulator : modulators)
        {
            modulation += modulator->getCurrentSample(voiceID);
        }
        
        // advance sample based on calculated modulation
        voices[voiceID].currentSample += (modulation + 1) * voices[voiceID].cyclesPerSample;
        if (voices[voiceID].currentSample >= 1.0)
        {
            voices[voiceID].currentSample -= 1.0;
        }
    }

    void startNote(SynthesiserVoice* voiceID, double newCyclesPerSample)
    {
        // set default cycle delta for this and dependency oscillators
        voices[voiceID].cyclesPerSample = newCyclesPerSample;
        for (auto modulator : modulators)
        {
            modulator->startNote(voiceID, voices[voiceID].cyclesPerSample);
        }
    }

    void stopNote(SynthesiserVoice* voiceID)
    {
        voices[voiceID].cyclesPerSample = 0.0;
        voices[voiceID].currentSample = 0.0;
        voices[voiceID].currentSampleID = 0;

        for (auto modulator : modulators)
        {
            modulator->stopNote(voiceID);
        }
    }

    void registerVoice(SynthesiserVoice *voice)
    {

    }


private:
    // RQ: 0 <= location < wavetable.size()
    // RT: interpolated sample at fractional location in the table
    float getSample(float location)
    {
        assert(location >= 0 && location < 1);

        // setup interpolation values
        float sampleFloat = location * wavetable.size();
        unsigned samplePrev = unsigned(location);
        unsigned sampleNext = samplePrev + 1;
        
        // reset on overflow
        if (sampleNext == wavetable.size())
        {
            sampleNext = 0;
        }
        
        return wavetable[samplePrev] + (wavetable[sampleNext] - wavetable[samplePrev]) * (sampleFloat - float(samplePrev));
    }
    
    void generateSineWave(unsigned length)
    {
        double angle = 0;
        double angleDelta = MathConstants<double>::twoPi / length;
        wavetable = std::vector<float>(length);
        
        for (unsigned i = 0; i < length; ++i)
        {
            wavetable[i] = sin(angle);
            angle += angleDelta;
        }
    }
};



















