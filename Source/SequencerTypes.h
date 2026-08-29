#pragma once
#include <juce_core/juce_core.h>

struct StepData
{
    bool gate = false;      // Note on/off
    bool accent = false;    // Accent flag (velocity >= 100)
    bool slide = false;     // Legato / slide into next step
    int note = 36;          // MIDI Note number (36 = C2)
};
