//
//  FMVoice.h
//  SynthUsingMidiInputTutorial - App
//
//  Created by Joshua Polzin on 5/13/19.
//  Copyright © 2019 JUCE. All rights reserved.
//

#ifndef FMVoice_h
#define FMVoice_h

#include "WavetableOscillator.h"
#include "FMSynthParams.h"


//==============================================================================
struct FMVoice   : public SynthesiserVoice
{
    FMVoice(WaveType waveType) : outputOscillator(new WavetableOscillator(waveType))
    {

    }
    
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<WavetableSynthesiserSound*> (sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        level = velocity * 0.15;
        
        auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        cyclesPerSample = cyclesPerSecond / getSampleRate();
        outputOscillator->startNote(0, cyclesPerSample);
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        clearCurrentNote();
        cyclesPerSample = 0.0;
        outputOscillator->stopNote(0);
    }
    
    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}
    
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override {
        if(cyclesPerSample != 0.0) {
            while(--numSamples >= 0) {
                auto currentSample = (float) (outputOscillator->getCurrentSample(0) * level);
                outputOscillator->advanceToNextSample(0, numSamples);

                for(auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample(i, startSample, currentSample);

                ++startSample;
            }
        }
    }
    
private:
    double cyclesPerSample;
    double level = 0.0;
    WavetableOscillator* outputOscillator;
};

#endif /* FMVoice_h */
