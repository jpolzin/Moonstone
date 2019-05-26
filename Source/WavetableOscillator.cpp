/*
  ==============================================================================

    WavetableOscillator.cpp
    Created: 20 May 2019 10:02:34pm
    Author:  Joshua Polzin

  ==============================================================================
*/

#include "WavetableOscillator.h"

unsigned WavetableOscillator::sample_rate = 44100;
std::vector<WavetableOscillator> WavetableOscillator::bank = std::vector<WavetableOscillator>();
std::map<std::string, int> WavetableOscillator::waveTypesMap = std::map<std::string, int>();

WavetableOscillator::WavetableOscillator() : voices(Constants::NUM_VOICES), syncFrequencyMultiplier(1) {
    generateSineWave(1024);
}

void WavetableOscillator::createOscillators()
{
    if (bank.empty())
    {
        bank = std::vector<WavetableOscillator>(Constants::NUM_OSCILLATORS);
    }
    if (waveTypesMap.empty())
    {
        waveTypesMap["Sine"] = SINE;
        waveTypesMap["Saw"] = SAW;
        waveTypesMap["Square"] = SQUARE;
        waveTypesMap["Triangle"] = TRIANGLE;
    }

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

void WavetableOscillator::setWaveType(int waveType)
{
    switch (waveType)
    {
    case SINE:
        generateSineWave();
        break;
    case SAW:
        generateSawWave();
        break;
    case SQUARE:
        generateSquareWave();
        break;
    case TRIANGLE:
        generateTriangleWave();
        break;
    default:
        generateSineWave(Constants::DEFAULT_WAVETABLE_SIZE);
    }
}

const std::map<std::string, int>&  WavetableOscillator::getWaveTypes()
{
    return waveTypesMap;
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
    std::vector<float> tempTable = std::vector<float>(length);
    double angle = 0;
    double angleDelta = MathConstants<double>::twoPi / length;

    for (unsigned i = 0; i < length; ++i)
    {
        tempTable[i] = sin(angle);
        angle += angleDelta;
    }

    wavetable = tempTable;
}

void WavetableOscillator::generateSawWave(unsigned length)
{
    std::vector<float> tempTable = std::vector<float>(length);

    const float delta = 2.0 / length;
    float accumulator = -1.0;
    unsigned sample = 0;

    for (; sample < length; ++sample)
    {
        tempTable[sample] = accumulator;
        accumulator += delta;
    }

    wavetable = tempTable;
}
void WavetableOscillator::generateSquareWave(unsigned length)
{
    std::vector<float> tempTable = std::vector<float>(length);

    unsigned sample = 0;
    for (; sample < length / 2; ++sample)
    {
        tempTable[sample] = 1.0;
    }
    for (; sample < length; ++sample)
    {
        tempTable[sample] = -1.0;
    }

    wavetable = tempTable;
}
void WavetableOscillator::generateTriangleWave(unsigned length)
{
    std::vector<float> tempTable = std::vector<float>(length);

    float delta = 4.0 / length;
    float accumulator = 0.0;
    unsigned sample = 0;

    for (; sample < length / 4; ++sample)
    {
        tempTable[sample] = accumulator;
        accumulator += delta;
    }
    for (; sample < 3 * length /4; ++sample)
    {
        tempTable[sample] = accumulator;
        accumulator -= delta;
    }
    for (; sample < length; ++sample)
    {
        tempTable[sample] = accumulator;
        accumulator += delta;
    }

    wavetable = tempTable;
}
