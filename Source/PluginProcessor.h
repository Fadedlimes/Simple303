#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "rosic_Open303.h"
#include "SequencerTypes.h"
#include <array>
#include <atomic>
#include <vector>

class Simple303AudioProcessor : public juce::AudioProcessor
{
public:
    Simple303AudioProcessor();
    ~Simple303AudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Simple303"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // XML Preset & Randomizer
    juce::String exportPresetToXmlString();
    bool importPresetFromXmlString(const juce::String& xmlString);
    void randomizeCurrentPattern(int targetPattern, int targetPage);
    int getSequenceLength() const;

    // 4 Pattern Banks (A, B, C, D) x 64 Steps = 256 Steps Total
    std::array<std::array<StepData, 64>, 4> patterns;

    std::atomic<bool> isPlaying { false };
    std::atomic<bool> isChainingActive { false };
    std::atomic<int> currentPlayPatternIndex { 0 };
    std::atomic<int> currentEditPatternIndex { 0 };
    std::atomic<int> queuedPatternIndex { -1 };
    std::atomic<int> startStepOffset { 0 };
    std::atomic<int> currentStepIndex { 0 };
    std::atomic<bool> patternWasUpdated { false };

    juce::AudioProcessorValueTreeState apvts;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    float applyDrive(float input, float driveAmount);
    void processDs1Distortion(float& left, float& right, float dist, float tone, float level, float mix);
    void processStereoDelay(float& left, float& right, int timeChoice, float fdbk, bool pingpong, float mix, float tempo);
    void processPlateReverb(float& left, float& right, float size, float decay, float predly, float hpf, float lpf, float mix);
    void handleMidiInput(const juce::MidiBuffer& midiMessages, float tempo, int numSamples);

    rosic::Open303 synth;

    double currentSampleRate = 44100.0;
    double samplesPerStep = 0.0;
    double sampleCounter = 0.0;
    int currentStep = 0;
    int stepCounter = 0;
    int playingPattern = 0;
    bool stepGateActive = false;
    bool currentStepIsSliding = false;
    int currentlyPlayingNote = -1;
    bool wasPlaying = false;
    bool wasHostPlaying = false;

    // Arpeggiator & Direct MIDI State
    std::vector<int> activeMidiNotes;
    std::vector<int> heldChordNotes;
    int arpIndex = 0;
    bool arpDirectionUp = true;
    double arpSampleCounter = 0.0;
    int arpLastNotePlayed = -1;
    bool anyKeyPhysicallyDown = false;

    // DC Blocker
    float dcPrevInL = 0.0f, dcPrevOutL = 0.0f;
    float dcPrevInR = 0.0f, dcPrevOutR = 0.0f;

    // DS-1 Tone Filters
    float ds1LpfL = 0.0f, ds1LpfR = 0.0f;
    float ds1HpfL = 0.0f, ds1HpfR = 0.0f;
    float ds1PrevInL = 0.0f, ds1PrevInR = 0.0f;

    // Delay Buffers
    std::vector<float> delayBufferL;
    std::vector<float> delayBufferR;
    int delayWritePos = 0;
    float delayDampL = 0.0f, delayDampR = 0.0f;

    // Plate Reverb Buffers & Filters
    std::vector<float> preDelayBuffer;
    int preDelayWritePos = 0;
    std::array<std::vector<float>, 4> combBuffersL;
    std::array<std::vector<float>, 4> combBuffersR;
    std::array<int, 4> combWritePosL { 0, 0, 0, 0 };
    std::array<int, 4> combWritePosR { 0, 0, 0, 0 };
    std::array<float, 4> combFilterL { 0.0f, 0.0f, 0.0f, 0.0f };
    std::array<float, 4> combFilterR { 0.0f, 0.0f, 0.0f, 0.0f };
    std::array<std::vector<float>, 2> allPassBuffersL;
    std::array<std::vector<float>, 2> allPassBuffersR;
    std::array<int, 2> allPassWritePosL { 0, 0 };
    std::array<int, 2> allPassWritePosR { 0, 0 };
    float revHpfL = 0.0f, revHpfR = 0.0f;
    float revHpfPrevL = 0.0f, revHpfPrevR = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Simple303AudioProcessor)
};
