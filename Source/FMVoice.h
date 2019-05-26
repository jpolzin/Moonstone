//
//  FMVoice.h
//  SynthUsingMidiInputTutorial - App
//
//  Created by Joshua Polzin on 5/13/19.
//  Copyright © 2019 JUCE. All rights reserved.
//

#pragma once

#include "WavetableOscillator.h"

struct WavetableSynthesiserSound : public SynthesiserSound
{
public:
    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
};

//==============================================================================
struct FMVoice : public SynthesiserVoice
{
    FMVoice(unsigned voiceID, unsigned oscID) : voiceID(voiceID), outputOscillatorID(oscID) {}
    
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<WavetableSynthesiserSound*> (sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        level = velocity * 0.2;
        
        auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        cyclesPerSample = cyclesPerSecond / getSampleRate();
        getOsc()->startNote(voiceID, cyclesPerSample);
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        clearCurrentNote();
        cyclesPerSample = 0.0;
        getOsc()->stopNote(voiceID);
    }
    
    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}
    
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if(cyclesPerSample != 0.0) {
            while(--numSamples >= 0) {
                auto currentSample = (float) (getOsc()->getCurrentSample(voiceID) * level);
                getOsc()->advanceToNextSample(voiceID, numSamples);

                for(auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample(i, startSample, currentSample);

                ++startSample;
            }
        }
    }
    
private:
    WavetableOscillator *getOsc()
    {
        return WavetableOscillator::getOsc(outputOscillatorID);
    }

    double cyclesPerSample;
    double level = 0.0;
    unsigned voiceID;
    unsigned outputOscillatorID;
};

