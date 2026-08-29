#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout Simple303AudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WAVEFORM", 1), "Waveform", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("TUNING", 1), "Tuning", juce::NormalisableRange<float>(400.0f, 480.0f, 0.1f), 440.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("CUTOFF", 1), "Cutoff", juce::NormalisableRange<float>(300.0f, 2500.0f, 0.1f, 0.35f), 750.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("RESONANCE", 1), "Resonance", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 70.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("ENVMOD", 1), "Env Mod", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 65.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("DECAY", 1), "Decay", juce::NormalisableRange<float>(200.0f, 2000.0f, 1.0f, 0.5f), 400.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("ACCENT", 1), "Accent", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 80.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("DRIVE", 1), "Drive", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 15.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("VOLUME", 1), "Volume", juce::NormalisableRange<float>(-36.0f, 6.0f, 0.1f), -4.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("TEMPO", 1), "Tempo", juce::NormalisableRange<float>(40.0f, 240.0f, 0.1f), 125.0f));

    // Devil Fish Mods
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("TRACKING", 1), "Key Tracking", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("FM", 1), "Filter FM", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("ACCDECAY", 1), "Accent Decay", juce::NormalisableRange<float>(20.0f, 500.0f, 1.0f, 0.5f), 150.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("SLIDETIME", 1), "Slide Time", juce::NormalisableRange<float>(10.0f, 400.0f, 1.0f, 0.5f), 60.0f));

    // Sequencer Settings
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("SEQLEN", 1), "Pattern Length", juce::StringArray { "16 Steps", "32 Steps", "48 Steps", "64 Steps" }, 3));
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("THEME", 1), "Theme", juce::StringArray { "Classic Red", "Acid Green", "Cyber Cyan", "Electric Violet", "Solar Amber", "Trans Pride" }, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("RANDROOT", 1), "Random Root", juce::StringArray { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" }, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("RANDSCALE", 1), "Random Scale", juce::StringArray {
        "Minor Pentatonic", "Natural Minor", "Dorian", "Phrygian", "Acid Blues", "Major Pentatonic", "Major", "Chromatic"
    }, 0));

    // Arpeggiator Parameters
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("ARPMODE", 1), "Arp Mode", juce::StringArray { "ARP OFF", "ARP UP", "ARP DOWN", "UP-DOWN", "RANDOM" }, 0));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("ARPHOLD", 1), "Arp Hold", false));

    // FX 1: Distortion 1
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("DS1_DIST", 1), "Distortion", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("DS1_TONE", 1), "Tone", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("DS1_LEVEL", 1), "Level", juce::NormalisableRange<float>(-24.0f, 12.0f, 0.1f), 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("DS1_MIX", 1), "Distortion Mix", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f));

    // FX 2: Musical Synced Stereo Delay
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("DLY_TIME", 1), "Delay Time", juce::StringArray {
        "1/16", "1/16D", "1/16T", "1/8", "1/8D", "1/8T", "1/4", "1/4D", "1/4T", "1/2"
    }, 3));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("DLY_FDBK", 1), "Delay Feedback", juce::NormalisableRange<float>(0.0f, 95.0f, 0.1f), 40.0f));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("DLY_PINGPONG", 1), "Ping Pong", true));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("DLY_MIX", 1), "Delay Mix", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f));

    // FX 3: Plate Reverb
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("REV_SIZE", 1), "Reverb Size", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 65.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("REV_DECAY", 1), "Reverb Decay", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 55.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("REV_PREDLY", 1), "Reverb Pre-Delay", juce::NormalisableRange<float>(0.0f, 150.0f, 1.0f), 20.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("REV_HPF", 1), "Reverb HPF", juce::NormalisableRange<float>(20.0f, 1000.0f, 1.0f, 0.4f), 120.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("REV_LPF", 1), "Reverb LPF", juce::NormalisableRange<float>(1000.0f, 16000.0f, 1.0f, 0.4f), 4500.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("REV_MIX", 1), "Reverb Mix", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f));

    return layout;
}

Simple303AudioProcessor::Simple303AudioProcessor()
: AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    const int defaultNotes[16] = { 36, 36, 48, 36, 39, 36, 46, 36, 36, 48, 39, 36, 51, 50, 48, 36 };

    for (int b = 0; b < 4; ++b)
    {
        for (int p = 0; p < 4; ++p)
        {
            for (int i = 0; i < 16; ++i)
            {
                int idx = p * 16 + i;
                patterns[b][idx].gate = (i % 2 == 0 || i == 7 || i == 11);
                patterns[b][idx].accent = (i == 0 || i == 6 || i == 12);
                patterns[b][idx].slide = (i == 2 || i == 8 || i == 14);
                patterns[b][idx].note = defaultNotes[i] + (b * 2) + (p % 2 == 1 ? 12 : 0);
            }
        }
    }
}

Simple303AudioProcessor::~Simple303AudioProcessor() {}

int Simple303AudioProcessor::getSequenceLength() const
{
    if (auto* p = apvts.getRawParameterValue("SEQLEN"))
    {
        int choice = static_cast<int>(p->load());
        return (choice + 1) * 16;
    }
    return 64;
}

void Simple303AudioProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    currentSampleRate = sampleRate;
    synth.setSampleRate(sampleRate);

    sampleCounter = 0.0;
    currentStep = 0;
    stepCounter = 0;
    playingPattern = currentEditPatternIndex.load();
    stepGateActive = false;
    currentStepIsSliding = false;
    currentlyPlayingNote = -1;
    wasPlaying = false;

    arpSampleCounter = 0.0;
    arpIndex = 0;
    arpDirectionUp = true;
    arpLastNotePlayed = -1;
    activeMidiNotes.clear();
    heldChordNotes.clear();

    dcPrevInL = 0.0f; dcPrevOutL = 0.0f;
    dcPrevInR = 0.0f; dcPrevOutR = 0.0f;

    ds1LpfL = 0.0f; ds1LpfR = 0.0f;
    ds1HpfL = 0.0f; ds1HpfR = 0.0f;
    ds1PrevInL = 0.0f; ds1PrevInR = 0.0f;

    int maxDelaySamples = static_cast<int>(sampleRate * 2.5);
    delayBufferL.assign(maxDelaySamples, 0.0f);
    delayBufferR.assign(maxDelaySamples, 0.0f);
    delayWritePos = 0;
    delayDampL = 0.0f; delayDampR = 0.0f;

    int maxPreDelay = static_cast<int>(sampleRate * 0.2);
    preDelayBuffer.assign(maxPreDelay, 0.0f);
    preDelayWritePos = 0;

    const int combLengths[4] = { 1116, 1188, 1277, 1356 };
    const int allPassLengths[2] = { 225, 341 };
    for (int i = 0; i < 4; ++i)
    {
        int scaledComb = static_cast<int>(combLengths[i] * (sampleRate / 44100.0));
        combBuffersL[i].assign(scaledComb, 0.0f);
        combBuffersR[i].assign(scaledComb + 23, 0.0f);
    }
    for (int i = 0; i < 2; ++i)
    {
        int scaledAp = static_cast<int>(allPassLengths[i] * (sampleRate / 44100.0));
        allPassBuffersL[i].assign(scaledAp, 0.0f);
        allPassBuffersR[i].assign(scaledAp + 17, 0.0f);
    }

    synth.allNotesOff();
}

void Simple303AudioProcessor::releaseResources()
{
    synth.allNotesOff();
}

float Simple303AudioProcessor::applyDrive(float input, float driveAmount)
{
    if (driveAmount <= 0.001f)
        return input;

    float gain = 1.0f + (driveAmount * 0.08f);
    float x = input * gain;
    float driven = (x >= 0.0f) ? std::tanh(x) : (std::tanh(x * 1.25f) / 1.25f);
    float makeup = 1.0f / std::sqrt(1.0f + driveAmount * 0.05f);
    return driven * makeup;
}

void Simple303AudioProcessor::processDs1Distortion(float& left, float& right, float dist, float tone, float level, float mix)
{
    if (mix <= 0.001f)
        return;

    float dryL = left;
    float dryR = right;

    float driveGain = 1.0f + (dist * 0.35f);
    float inL = left * driveGain;
    float inR = right * driveGain;

    auto clipDs1 = [](float x) -> float {
        float x2 = (x >= 0.0f) ? std::tanh(x * 1.6f) : std::tanh(x * 1.2f) * 1.15f;
        return std::clamp(x2, -1.0f, 1.0f);
    };

    float clippedL = clipDs1(inL);
    float clippedR = clipDs1(inR);

    float t = tone * 0.01f;
    float lpfAlpha = 0.08f;
    float hpfAlpha = 0.75f;

    ds1LpfL += lpfAlpha * (clippedL - ds1LpfL);
    ds1LpfR += lpfAlpha * (clippedR - ds1LpfR);

    ds1HpfL = hpfAlpha * (ds1HpfL + clippedL - ds1PrevInL);
    ds1HpfR = hpfAlpha * (ds1HpfR + clippedR - ds1PrevInR);
    ds1PrevInL = clippedL;
    ds1PrevInR = clippedR;

    float toneOutL = ((1.0f - t) * ds1LpfL + t * ds1HpfL) * 1.45f;
    float toneOutR = ((1.0f - t) * ds1LpfR + t * ds1HpfR) * 1.45f;

    float levelGain = juce::Decibels::decibelsToGain(level);
    float wetL = toneOutL * levelGain;
    float wetR = toneOutR * levelGain;

    float m = mix * 0.01f;
    left = dryL * (1.0f - m) + wetL * m;
    right = dryR * (1.0f - m) + wetR * m;
}

void Simple303AudioProcessor::processStereoDelay(float& left, float& right, int timeChoice, float fdbk, bool pingpong, float mix, float tempo)
{
    if (mix <= 0.001f || delayBufferL.empty())
        return;

    float beatSamples = static_cast<float>((60.0 / tempo) * currentSampleRate);
    static const float timeMultipliers[10] = {
        0.25f, 0.375f, 0.1667f, 0.50f, 0.75f, 0.3333f, 1.0f, 1.5f, 0.6667f, 2.0f
    };

    float delayTimeSamples = beatSamples * timeMultipliers[juce::jlimit(0, 9, timeChoice)];
    int dSamplesL = juce::jlimit(1, static_cast<int>(delayBufferL.size()) - 1, static_cast<int>(delayTimeSamples));
    int dSamplesR = pingpong ? juce::jlimit(1, static_cast<int>(delayBufferR.size()) - 1, static_cast<int>(delayTimeSamples * 1.5f)) : dSamplesL;

    int bufSize = static_cast<int>(delayBufferL.size());
    int readPosL = (delayWritePos - dSamplesL + bufSize) % bufSize;
    int readPosR = (delayWritePos - dSamplesR + bufSize) % bufSize;

    float delayedL = delayBufferL[static_cast<size_t>(readPosL)];
    float delayedR = delayBufferR[static_cast<size_t>(readPosR)];

    delayDampL += 0.35f * (delayedL - delayDampL);
    delayDampR += 0.35f * (delayedR - delayDampR);

    float fb = fdbk * 0.01f;
    float dryL = left;
    float dryR = right;

    if (pingpong)
    {
        delayBufferL[static_cast<size_t>(delayWritePos)] = left + delayDampR * fb;
        delayBufferR[static_cast<size_t>(delayWritePos)] = right + delayDampL * fb;
    }
    else
    {
        delayBufferL[static_cast<size_t>(delayWritePos)] = left + delayDampL * fb;
        delayBufferR[static_cast<size_t>(delayWritePos)] = right + delayDampR * fb;
    }

    delayWritePos = (delayWritePos + 1) % bufSize;

    float m = mix * 0.01f;
    left = dryL * (1.0f - m) + delayedL * m;
    right = dryR * (1.0f - m) + delayedR * m;
}

void Simple303AudioProcessor::processPlateReverb(float& left, float& right, float /*size*/, float decay, float predly, float hpf, float lpf, float mix)
{
    if (mix <= 0.001f || preDelayBuffer.empty())
        return;

    float dryL = left;
    float dryR = right;
    float monoIn = (left + right) * 0.5f;

    float hpfAlpha = juce::jlimit(0.01f, 0.99f, 1.0f - std::exp(-juce::MathConstants<float>::twoPi * hpf / static_cast<float>(currentSampleRate)));
    revHpfL = (1.0f - hpfAlpha) * (revHpfL + monoIn - revHpfPrevL);
    revHpfPrevL = monoIn;
    float filteredIn = revHpfL;

    int preDelaySamples = juce::jlimit(1, static_cast<int>(preDelayBuffer.size()) - 1, static_cast<int>(predly * 0.001f * currentSampleRate));
    int preReadPos = (preDelayWritePos - preDelaySamples + static_cast<int>(preDelayBuffer.size())) % static_cast<int>(preDelayBuffer.size());
    float preDelayed = preDelayBuffer[static_cast<size_t>(preReadPos)];
    preDelayBuffer[static_cast<size_t>(preDelayWritePos)] = filteredIn;
    preDelayWritePos = (preDelayWritePos + 1) % static_cast<int>(preDelayBuffer.size());

    float feedback = juce::jlimit(0.2f, 0.92f, 0.4f + (decay * 0.005f));
    float lpfDamp = juce::jlimit(0.05f, 0.85f, lpf / 18000.0f);
    float combSumL = 0.0f;
    float combSumR = 0.0f;

    for (int i = 0; i < 4; ++i)
    {
        int bufLenL = static_cast<int>(combBuffersL[i].size());
        int bufLenR = static_cast<int>(combBuffersR[i].size());

        float outL = combBuffersL[i][static_cast<size_t>(combWritePosL[i])];
        float outR = combBuffersR[i][static_cast<size_t>(combWritePosR[i])];

        combFilterL[i] = outL * lpfDamp + combFilterL[i] * (1.0f - lpfDamp);
        combFilterR[i] = outR * lpfDamp + combFilterR[i] * (1.0f - lpfDamp);

        combBuffersL[i][static_cast<size_t>(combWritePosL[i])] = preDelayed + combFilterL[i] * feedback;
        combBuffersR[i][static_cast<size_t>(combWritePosR[i])] = preDelayed + combFilterR[i] * feedback;

        combWritePosL[i] = (combWritePosL[i] + 1) % bufLenL;
        combWritePosR[i] = (combWritePosR[i] + 1) % bufLenR;

        combSumL += outL;
        combSumR += outR;
    }

    float apL = combSumL * 0.25f;
    float apR = combSumR * 0.25f;

    for (int i = 0; i < 2; ++i)
    {
        int bufLenL = static_cast<int>(allPassBuffersL[i].size());
        int bufLenR = static_cast<int>(allPassBuffersR[i].size());

        float bufOutL = allPassBuffersL[i][static_cast<size_t>(allPassWritePosL[i])];
        float bufOutR = allPassBuffersR[i][static_cast<size_t>(allPassWritePosR[i])];

        allPassBuffersL[i][static_cast<size_t>(allPassWritePosL[i])] = apL + bufOutL * 0.5f;
        allPassBuffersR[i][static_cast<size_t>(allPassWritePosR[i])] = apR + bufOutR * 0.5f;

        allPassWritePosL[i] = (allPassWritePosL[i] + 1) % bufLenL;
        allPassWritePosR[i] = (allPassWritePosR[i] + 1) % bufLenR;

        apL = -apL + bufOutL;
        apR = -apR + bufOutR;
    }

    float m = mix * 0.01f;
    left = dryL * (1.0f - m) + apL * m;
    right = dryR * (1.0f - m) + apR * m;
}

void Simple303AudioProcessor::handleArpeggiator(juce::MidiBuffer& midiMessages, float tempo, int numSamples)
{
    int arpMode = static_cast<int>(apvts.getRawParameterValue("ARPMODE")->load());
    bool arpHold = apvts.getRawParameterValue("ARPHOLD")->load() > 0.5f;

    for (const auto metadata : midiMessages)
    {
        auto msg = metadata.getMessage();
        if (msg.isNoteOn())
        {
            if (arpHold && !anyKeyPhysicallyDown)
                heldChordNotes.clear();

            activeMidiNotes.push_back(msg.getNoteNumber());
            heldChordNotes.push_back(msg.getNoteNumber());
            std::sort(activeMidiNotes.begin(), activeMidiNotes.end());
            std::sort(heldChordNotes.begin(), heldChordNotes.end());
            anyKeyPhysicallyDown = true;
        }
        else if (msg.isNoteOff())
        {
            auto it = std::find(activeMidiNotes.begin(), activeMidiNotes.end(), msg.getNoteNumber());
            if (it != activeMidiNotes.end())
                activeMidiNotes.erase(it);

            if (activeMidiNotes.empty())
                anyKeyPhysicallyDown = false;
        }
        else if (msg.isAllNotesOff())
        {
            activeMidiNotes.clear();
            if (!arpHold)
                heldChordNotes.clear();
            anyKeyPhysicallyDown = false;
        }
    }

    if (arpMode == 0)
        return;

    const auto& currentNotes = arpHold ? heldChordNotes : activeMidiNotes;
    if (currentNotes.empty())
    {
        if (arpLastNotePlayed != -1)
        {
            synth.noteOn(arpLastNotePlayed, 0, 0.0);
            arpLastNotePlayed = -1;
        }
        return;
    }

    double arpStepSamples = (15.0 / tempo) * currentSampleRate;

    arpSampleCounter += numSamples;
    if (arpSampleCounter >= arpStepSamples)
    {
        arpSampleCounter = 0.0;
        int numNotes = static_cast<int>(currentNotes.size());

        if (arpMode == 1) // UP
        {
            arpIndex = (arpIndex + 1) % numNotes;
        }
        else if (arpMode == 2) // DOWN
        {
            arpIndex = (arpIndex - 1 + numNotes) % numNotes;
        }
        else if (arpMode == 3) // UP-DOWN
        {
            if (arpDirectionUp)
            {
                arpIndex++;
                if (arpIndex >= numNotes - 1)
                {
                    arpIndex = numNotes - 1;
                    arpDirectionUp = false;
                }
            }
            else
            {
                arpIndex--;
                if (arpIndex <= 0)
                {
                    arpIndex = 0;
                    arpDirectionUp = true;
                }
            }
            arpIndex = juce::jlimit(0, numNotes - 1, arpIndex);
        }
        else if (arpMode == 4) // RANDOM
        {
            arpIndex = juce::Random::getSystemRandom().nextInt(numNotes);
        }

        int noteToPlay = currentNotes[static_cast<size_t>(arpIndex)];
        if (arpLastNotePlayed != -1)
            synth.noteOn(arpLastNotePlayed, 0, 0.0);

        synth.noteOn(noteToPlay, 100, 0.0);
        arpLastNotePlayed = noteToPlay;
    }
}

// Smart Page-Aware Musically-Biased Acid Pattern Generator
void Simple303AudioProcessor::randomizeCurrentPattern(int targetPattern, int targetPage)
{
    int rootKey = static_cast<int>(apvts.getRawParameterValue("RANDROOT")->load());
    int scaleChoice = static_cast<int>(apvts.getRawParameterValue("RANDSCALE")->load());

    static const std::vector<std::vector<int>> scaleIntervals = {
        { 0, 3, 5, 7, 10 },              // Minor Pentatonic
        { 0, 2, 3, 5, 7, 8, 10 },        // Natural Minor
        { 0, 2, 3, 5, 7, 9, 10 },        // Dorian
        { 0, 1, 3, 5, 7, 8, 10 },        // Phrygian
        { 0, 3, 5, 6, 7, 10 },           // Acid Blues
        { 0, 2, 4, 7, 9 },               // Major Pentatonic
        { 0, 2, 4, 5, 7, 9, 11 },        // Major
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 } // Chromatic
    };

    const auto& intervals = scaleIntervals[juce::jlimit(0, 7, scaleChoice)];
    auto& r = juce::Random::getSystemRandom();
    int b = juce::jlimit(0, 3, targetPattern);

    // Tonal Anchors: Roots, Fifths, Octaves
    int rootC1 = 24 + rootKey;
    int rootC2 = 36 + rootKey;
    int rootC3 = 48 + rootKey;
    int fifthC1 = rootC1 + 7;
    int fifthC2 = rootC2 + 7;

    std::vector<int> flavorNotes;
    for (int oct = 2; oct <= 4; ++oct)
    {
        for (int interval : intervals)
        {
            int note = (oct * 12) + rootKey + interval;
            if (note >= 24 && note <= 60)
                flavorNotes.push_back(note);
        }
    }
    if (flavorNotes.empty())
        flavorNotes = { rootC2, rootC2 + 3, rootC2 + 5, rootC2 + 7, rootC2 + 10 };

    int startIdx = 0;
    int endIdx = getSequenceLength();

    // Page-specific targeting: If on Page 2, 3, or 4, randomize ONLY that page!
    if (targetPage > 0 && targetPage < 4)
    {
        startIdx = targetPage * 16;
        endIdx = startIdx + 16;
    }

    int motif = r.nextInt(3);

    // 1. Pass 1: Rhythms & Tonal Note Assignment
    for (int i = startIdx; i < endIdx; ++i)
    {
        int stepInBar = i % 16;
        bool isDownbeat = (stepInBar % 4 == 0);

        if (motif == 0)
            patterns[b][i].gate = (r.nextFloat() > 0.22f);
        else if (motif == 1)
            patterns[b][i].gate = (stepInBar == 0 || stepInBar == 3 || stepInBar == 6 || stepInBar == 8 || stepInBar == 11 || stepInBar == 14 || r.nextFloat() > 0.45f);
        else
            patterns[b][i].gate = (isDownbeat || stepInBar == 2 || stepInBar == 7 || stepInBar == 10 || stepInBar == 15 || r.nextFloat() > 0.50f);

        if (patterns[b][i].gate)
        {
            float noteRoll = r.nextFloat();
            if (noteRoll < 0.45f)
                patterns[b][i].note = (r.nextFloat() > 0.3f) ? rootC2 : rootC1;
            else if (noteRoll < 0.70f)
                patterns[b][i].note = (r.nextFloat() > 0.5f) ? fifthC2 : fifthC1;
            else if (noteRoll < 0.85f)
                patterns[b][i].note = rootC3;
            else
                patterns[b][i].note = flavorNotes[static_cast<size_t>(r.nextInt(static_cast<int>(flavorNotes.size())))];

            patterns[b][i].accent = (isDownbeat && r.nextFloat() > 0.4f) || (patterns[b][i].note >= rootC3) || (r.nextFloat() > 0.75f);
        }
        else
        {
            patterns[b][i].accent = false;
            patterns[b][i].slide = false;
        }
    }

    // 2. Pass 2: Strict Legato Slide Validation
    for (int i = startIdx; i < endIdx; ++i)
    {
        int nextStep = (i + 1) % 64;

        if (patterns[b][i].gate && patterns[b][nextStep].gate)
        {
            bool pitchChanges = (patterns[b][i].note != patterns[b][nextStep].note);
            patterns[b][i].slide = pitchChanges ? (r.nextFloat() > 0.55f) : (r.nextFloat() > 0.80f);
        }
        else
        {
            patterns[b][i].slide = false;
        }
    }

    patternWasUpdated.store(true);
}

void Simple303AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    float waveVal = apvts.getRawParameterValue("WAVEFORM")->load() * 0.01f;
    synth.setWaveform(static_cast<double>(waveVal));

    synth.setTuning(apvts.getRawParameterValue("TUNING")->load());

    float baseCutoff = apvts.getRawParameterValue("CUTOFF")->load();
    float tracking = apvts.getRawParameterValue("TRACKING")->load();

    float trackMult = 1.0f;
    if (currentlyPlayingNote != -1)
    {
        float semitones = static_cast<float>(currentlyPlayingNote - 36);
        trackMult = std::pow(2.0f, (semitones / 12.0f) * (tracking / 100.0f - 0.5f));
    }
    synth.setCutoff(juce::jlimit(100.0f, 16000.0f, baseCutoff * trackMult));

    synth.setResonance(apvts.getRawParameterValue("RESONANCE")->load());
    synth.setEnvMod(apvts.getRawParameterValue("ENVMOD")->load());
    synth.setDecay(apvts.getRawParameterValue("DECAY")->load());
    synth.setAccent(apvts.getRawParameterValue("ACCENT")->load());
    synth.setVolume(apvts.getRawParameterValue("VOLUME")->load());
    synth.setSlideTime(apvts.getRawParameterValue("SLIDETIME")->load());

    float driveAmount = apvts.getRawParameterValue("DRIVE")->load();
    float fmAmount = apvts.getRawParameterValue("FM")->load() * 0.01f;
    float tempo = apvts.getRawParameterValue("TEMPO")->load();

    handleArpeggiator(midiMessages, tempo, buffer.getNumSamples());

    juce::MidiBuffer outMidi;
    samplesPerStep = (15.0 / tempo) * currentSampleRate;
    double gateLengthSamples = samplesPerStep * 0.75;
    int seqLen = getSequenceLength();
    int pageOffset = startStepOffset.load();

    bool playing = isPlaying.load();

    if (playing && !wasPlaying)
    {
        outMidi.addEvent(juce::MidiMessage::midiStart(), 0);

        int queued = queuedPatternIndex.exchange(-1);
        if (queued >= 0 && queued < 4)
            playingPattern = queued;
        else
            playingPattern = currentEditPatternIndex.load();

        currentStep = pageOffset;
        stepCounter = 0;
        sampleCounter = samplesPerStep;
        currentlyPlayingNote = -1;
        stepGateActive = false;
        currentStepIsSliding = false;
    }
    else if (!playing && wasPlaying)
    {
        synth.allNotesOff();
        outMidi.addEvent(juce::MidiMessage::allNotesOff(1), 0);
        outMidi.addEvent(juce::MidiMessage::midiStop(), 0);
        currentlyPlayingNote = -1;
        stepGateActive = false;
        currentStepIsSliding = false;
        currentStep = pageOffset;
        sampleCounter = 0.0;
    }
    wasPlaying = playing;

    auto* leftChannel = buffer.getWritePointer(0);
    auto* rightChannel = totalNumOutputChannels > 1 ? buffer.getWritePointer(1) : nullptr;

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        if (playing)
        {
            if (sampleCounter >= samplesPerStep)
            {
                sampleCounter = 0.0;

                if (stepCounter > 0)
                {
                    currentStep++;

                    if (seqLen == 64)
                    {
                        if (currentStep >= 64)
                        {
                            currentStep = 0;
                            int queued = queuedPatternIndex.exchange(-1);
                            if (queued >= 0 && queued < 4)
                                playingPattern = queued;
                        }
                    }
                    else
                    {
                        if (currentStep >= pageOffset + seqLen || currentStep >= 64)
                        {
                            currentStep = pageOffset;
                            int queued = queuedPatternIndex.exchange(-1);
                            if (queued >= 0 && queued < 4)
                                playingPattern = queued;
                        }
                    }
                }

                stepCounter++;
                currentStepIndex.store(currentStep);
                currentPlayPatternIndex.store(playingPattern);

                const auto& step = patterns[playingPattern][currentStep];

                if (step.gate)
                {
                    int vel = step.accent ? 127 : 64;

                    if (currentlyPlayingNote != -1 && currentStepIsSliding)
                    {
                        int oldNote = currentlyPlayingNote;
                        synth.noteOn(step.note, vel, 0.0);
                        synth.noteOn(oldNote, 0, 0.0);

                        outMidi.addEvent(juce::MidiMessage::noteOn(1, step.note, (juce::uint8)vel), sample);
                        outMidi.addEvent(juce::MidiMessage::noteOff(1, oldNote), sample);
                    }
                    else
                    {
                        if (currentlyPlayingNote != -1)
                        {
                            synth.noteOn(currentlyPlayingNote, 0, 0.0);
                            outMidi.addEvent(juce::MidiMessage::noteOff(1, currentlyPlayingNote), sample);
                        }

                        synth.noteOn(step.note, vel, 0.0);
                        outMidi.addEvent(juce::MidiMessage::noteOn(1, step.note, (juce::uint8)vel), sample);
                    }

                    currentlyPlayingNote = step.note;
                    currentStepIsSliding = step.slide;
                    stepGateActive = true;
                }
                else
                {
                    if (currentlyPlayingNote != -1)
                    {
                        synth.noteOn(currentlyPlayingNote, 0, 0.0);
                        outMidi.addEvent(juce::MidiMessage::noteOff(1, currentlyPlayingNote), sample);
                        currentlyPlayingNote = -1;
                    }
                    currentStepIsSliding = false;
                    stepGateActive = false;
                }
            }

            if (stepGateActive && !currentStepIsSliding && sampleCounter >= gateLengthSamples)
            {
                if (currentlyPlayingNote != -1)
                {
                    synth.noteOn(currentlyPlayingNote, 0, 0.0);
                    outMidi.addEvent(juce::MidiMessage::noteOff(1, currentlyPlayingNote), sample);
                    currentlyPlayingNote = -1;
                }
                stepGateActive = false;
            }

            sampleCounter += 1.0;
        }

        float rawSample = static_cast<float>(synth.getSample());

        if (fmAmount > 0.001f)
            rawSample += std::sin(rawSample * juce::MathConstants<float>::twoPi) * (fmAmount * 0.4f);

        float drivenSample = applyDrive(rawSample, driveAmount);

        float outL = drivenSample - dcPrevInL + 0.995f * dcPrevOutL;
        dcPrevInL = drivenSample;
        dcPrevOutL = outL;

        leftChannel[sample] = outL;
        if (rightChannel != nullptr)
            rightChannel[sample] = outL;
    }

    float ds1Dist = apvts.getRawParameterValue("DS1_DIST")->load();
    float ds1Tone = apvts.getRawParameterValue("DS1_TONE")->load();
    float ds1Level = apvts.getRawParameterValue("DS1_LEVEL")->load();
    float ds1Mix = apvts.getRawParameterValue("DS1_MIX")->load();

    int dlyTime = static_cast<int>(apvts.getRawParameterValue("DLY_TIME")->load());
    float dlyFdbk = apvts.getRawParameterValue("DLY_FDBK")->load();
    bool dlyPingPong = apvts.getRawParameterValue("DLY_PINGPONG")->load() > 0.5f;
    float dlyMix = apvts.getRawParameterValue("DLY_MIX")->load();

    float revSize = apvts.getRawParameterValue("REV_SIZE")->load();
    float revDecay = apvts.getRawParameterValue("REV_DECAY")->load();
    float revPreDly = apvts.getRawParameterValue("REV_PREDLY")->load();
    float revHpf = apvts.getRawParameterValue("REV_HPF")->load();
    float revLpf = apvts.getRawParameterValue("REV_LPF")->load();
    float revMix = apvts.getRawParameterValue("REV_MIX")->load();

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float l = leftChannel[sample];
        float r = (rightChannel != nullptr) ? rightChannel[sample] : l;

        processDs1Distortion(l, r, ds1Dist, ds1Tone, ds1Level, ds1Mix);
        processStereoDelay(l, r, dlyTime, dlyFdbk, dlyPingPong, dlyMix, tempo);
        processPlateReverb(l, r, revSize, revDecay, revPreDly, revHpf, revLpf, revMix);

        leftChannel[sample] = l;
        if (rightChannel != nullptr)
            rightChannel[sample] = r;
    }

    midiMessages.swapWith(outMidi);
}

juce::String Simple303AudioProcessor::exportPresetToXmlString()
{
    juce::XmlElement root("Simple303Preset");
    root.setAttribute("version", "1.2.0");

    auto* paramsXml = root.createNewChildElement("Parameters");
    for (auto* param : getParameters())
    {
        if (auto* p = dynamic_cast<juce::AudioProcessorParameterWithID*>(param))
        {
            auto* pElem = paramsXml->createNewChildElement("Param");
            pElem->setAttribute("id", p->paramID);
            pElem->setAttribute("value", p->getValue());
        }
    }

    auto* banksXml = root.createNewChildElement("Patterns");
    static const char* const bankNames[] = { "A", "B", "C", "D" };

    for (int b = 0; b < 4; ++b)
    {
        auto* bankXml = banksXml->createNewChildElement("Pattern");
        bankXml->setAttribute("name", bankNames[b]);

        for (int i = 0; i < 64; ++i)
        {
            auto* sElem = bankXml->createNewChildElement("Step");
            sElem->setAttribute("index", i);
            sElem->setAttribute("gate", patterns[b][i].gate ? 1 : 0);
            sElem->setAttribute("accent", patterns[b][i].accent ? 1 : 0);
            sElem->setAttribute("slide", patterns[b][i].slide ? 1 : 0);
            sElem->setAttribute("note", patterns[b][i].note);
        }
    }

    return root.toString();
}

bool Simple303AudioProcessor::importPresetFromXmlString(const juce::String& xmlString)
{
    auto xml = juce::XmlDocument::parse(xmlString);
    if (xml == nullptr || !xml->hasTagName("Simple303Preset"))
        return false;

    if (auto* paramsXml = xml->getChildByName("Parameters"))
    {
        for (auto* pElem : paramsXml->getChildIterator())
        {
            auto pid = pElem->getStringAttribute("id");
            float val = pElem->getDoubleAttribute("value", 0.0);
            if (auto* p = apvts.getParameter(pid))
                p->setValueNotifyingHost(val);
        }
    }

    if (auto* banksXml = xml->getChildByName("Patterns"))
    {
        int bIdx = 0;
        for (auto* bankXml : banksXml->getChildIterator())
        {
            if (bIdx >= 4) break;
            for (auto* sElem : bankXml->getChildIterator())
            {
                int idx = sElem->getIntAttribute("index", -1);
                if (idx >= 0 && idx < 64)
                {
                    patterns[bIdx][idx].gate = sElem->getIntAttribute("gate", 0) != 0;
                    patterns[bIdx][idx].accent = sElem->getIntAttribute("accent", 0) != 0;
                    patterns[bIdx][idx].slide = sElem->getIntAttribute("slide", 0) != 0;
                    patterns[bIdx][idx].note = sElem->getIntAttribute("note", 36);
                }
            }
            bIdx++;
        }
        patternWasUpdated.store(true);
    }

    return true;
}

void Simple303AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto xmlString = exportPresetToXmlString();
    destData.append(xmlString.toRawUTF8(), xmlString.getNumBytesAsUTF8());
}

void Simple303AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    juce::String xmlString = juce::String::createStringFromData(data, sizeInBytes);
    importPresetFromXmlString(xmlString);
}

juce::AudioProcessorEditor* Simple303AudioProcessor::createEditor()
{
    return new Simple303AudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Simple303AudioProcessor();
}
