/*
  ==============================================================================

    WavetableOscillator.cpp
    Created: 20 May 2019 10:02:34pm
    Author:  Joshua Polzin

  ==============================================================================
*/

#include "WavetableOscillator.h"

unsigned WavetableOscillator::sample_rate = 0;
std::vector<WavetableOscillator> WavetableOscillator::bank = std::vector<WavetableOscillator>();

WavetableOscillator::WavetableOscillator() : voices(Constants::NUM_VOICES), syncFrequencyMultiplier(1) {
    generateSineWave(1024);
}

void WavetableOscillator::createOscillators()
{
    if (bank.size() != 0)
    {
        return;
    }

    bank = std::vector<WavetableOscillator>(Constants::NUM_OSCILLATORS);
}

WavetableOscillator* WavetableOscillator::getOsc(unsigned oscillatorID)
{
    return &bank[oscillatorID];
}

float WavetableOscillator::getCurrentSample(unsigned voiceID)
{
    return interpolateToLocation(voices[voiceID].currentSample) * outputLevel;
}

void WavetableOscillator::advanceToNextSample(unsigned voiceID, unsigned sampleID)
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
        getOsc(modulator)->advanceToNextSample(voiceID, sampleID);
    }

    // calculate frequency modulations
    float modulation = 0;
    for (auto modulator : modulators)
    {
        modulation += getOsc(modulator)->getCurrentSample(voiceID);
    }

    // advance sample based on calculated modulation
    voices[voiceID].currentSample += (modulation + 1) * voices[voiceID].defaultCyclesPerSample * syncFrequencyMultiplier;
    while (int(voices[voiceID].currentSample) >= 1)
    {
        voices[voiceID].currentSample -= int(voices[voiceID].currentSample);
    }
}

void WavetableOscillator::startNote(unsigned voiceID, double newCyclesPerSample)
{
    // set default cycle delta for this and dependency oscillators
    voices[voiceID].defaultCyclesPerSample = newCyclesPerSample;
    for (auto modulator : modulators)
    {
        getOsc(modulator)->startNote(voiceID, voices[voiceID].defaultCyclesPerSample);
    }
}

void WavetableOscillator::stopNote(unsigned voiceID)
{
    voices[voiceID].defaultCyclesPerSample = 0.0;
    voices[voiceID].currentSample = 0.0;
    voices[voiceID].currentSampleID = 0;

    for (auto modulator : modulators)
    {
        getOsc(modulator)->stopNote(voiceID);
    }
}

// adds modulator
void WavetableOscillator::addModulator(unsigned oscillatorID)
{
    modulators.insert(oscillatorID);
}

void WavetableOscillator::removeModulator(unsigned oscillatorID)
{
    modulators.erase(oscillatorID);
}

void WavetableOscillator::removeAllModulators()
{
    modulators.clear();
}


void WavetableOscillator::setSyncFrequencyMultiplier(double multiplier)
{
    syncFrequencyMultiplier = multiplier;
}

void WavetableOscillator::setOutputLevel(double level)
{
    outputLevel = level;
}

void WavetableOscillator::setWaveType(WavetableOscillator::WaveType waveType)
{
    generateSineWave(Constants::DEFAULT_WAVETABLE_SIZE);
}


float WavetableOscillator::interpolateToLocation(double location)
{
    if(location < 0 || location >= 1)
    {
        std::cerr << location << std::endl;
        assert(false);
    }

    // setup interpolation values
    double sampleFloat = location * wavetable.size();
    unsigned samplePrev = unsigned(sampleFloat);
    unsigned sampleNext = samplePrev + 1;

    // reset on overflow
    if (sampleNext == wavetable.size())
    {
        sampleNext = 0;
    }

    return wavetable[samplePrev] + (wavetable[sampleNext] - wavetable[samplePrev]) * (sampleFloat - double(samplePrev));
}

void WavetableOscillator::generateSineWave(unsigned length)
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