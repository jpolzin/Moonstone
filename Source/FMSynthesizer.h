/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2017 - ROLI Ltd.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

#pragma once

#include <iostream>

#include "FMVoice.h"
#include "WavetableOscillator.h"


//==============================================================================
class FMSynthesiser   : public Synthesiser
{
public:
    FMSynthesiser ()
    {
        WavetableOscillator::createOscillators();

        for (unsigned voiceID = 0; voiceID < Constants::NUM_VOICES; ++voiceID)
        {
            addVoice (new FMVoice(voiceID, 0 /* outputOsc */));
        }

        addSound (new WavetableSynthesiserSound());
    }
};

