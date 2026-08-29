#include "PluginProcessor.h"
#include "PluginEditor.h"

static juce::String midiNoteToName(int note)
{
    static const char* const noteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    int oct = (note / 12) - 1;
    return juce::String(noteNames[note % 12]) + " " + juce::String(oct);
}

Simple303AudioProcessorEditor::Simple303AudioProcessorEditor(Simple303AudioProcessor& p)
: AudioProcessorEditor(&p), audioProcessor(p)
{
    setLookAndFeel(&customLookAndFeel);
    setSize(1060, 475);

    auto setupMainKnob = [this](juce::Slider& slider)
    {
        slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 14);
        slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        slider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xff111215));
        slider.setColour(juce::Label::textColourId, juce::Colour(0xff111215));
        addAndMakeVisible(slider);
    };

    auto setupFxKnob = [this](juce::Slider& slider)
    {
        slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 14);
        slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        slider.setColour(juce::Label::textColourId, juce::Colours::white);
        addChildComponent(slider);
    };

    // Main 303 Controls
    setupMainKnob(tuningKnob);
    setupMainKnob(waveKnob);
    setupMainKnob(cutoffKnob);
    setupMainKnob(resonanceKnob);
    setupMainKnob(envModKnob);
    setupMainKnob(decayKnob);
    setupMainKnob(accentKnob);
    setupMainKnob(driveKnob);
    setupMainKnob(volumeKnob);
    setupMainKnob(tempoKnob);

    waveKnob.textFromValueFunction = [](double val) {
        if (val < 10.0) return juce::String("SAW");
        if (val > 90.0) return juce::String("SQR");
        return juce::String((int)val) + "%";
    };

    setupMainKnob(trackKnob);
    setupMainKnob(fmKnob);
    setupMainKnob(accDecayKnob);
    setupMainKnob(slideTimeKnob);

    // FX 1: Distortion 1 Knobs
    setupFxKnob(ds1DistKnob);
    setupFxKnob(ds1ToneKnob);
    setupFxKnob(ds1LevelKnob);
    setupFxKnob(ds1MixKnob);

    // FX 2: Stereo Delay Controls
    dlyTimeBox.addItemList(juce::StringArray { "1/16", "1/16D", "1/16T", "1/8", "1/8D", "1/8T", "1/4", "1/4D", "1/4T", "1/2" }, 1);
    dlyTimeBox.setJustificationType(juce::Justification::centred);
    dlyTimeBox.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff181a1f));
    dlyTimeBox.setColour(juce::ComboBox::textColourId, juce::Colours::white);
    dlyTimeBox.setColour(juce::ComboBox::arrowColourId, juce::Colours::white);
    dlyTimeBox.setColour(juce::Label::textColourId, juce::Colours::white);
    addChildComponent(dlyTimeBox);

    setupFxKnob(dlyFdbkKnob);
    setupFxKnob(dlyMixKnob);
    dlyPingPongToggle.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    addChildComponent(dlyPingPongToggle);

    // FX 3: Plate Reverb Knobs
    setupFxKnob(revSizeKnob);
    setupFxKnob(revDecayKnob);
    setupFxKnob(revPreDlyKnob);
    setupFxKnob(revHpfKnob);
    setupFxKnob(revLpfKnob);
    setupFxKnob(revMixKnob);

    // APVTS Attachments
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TUNING", tuningKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "WAVEFORM", waveKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CUTOFF", cutoffKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RESONANCE", resonanceKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ENVMOD", envModKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DECAY", decayKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ACCENT", accentKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DRIVE", driveKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VOLUME", volumeKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TEMPO", tempoKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TRACKING", trackKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FM", fmKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ACCDECAY", accDecayKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SLIDETIME", slideTimeKnob));

    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DS1_DIST", ds1DistKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DS1_TONE", ds1ToneKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DS1_LEVEL", ds1LevelKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DS1_MIX", ds1MixKnob));

    comboAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "DLY_TIME", dlyTimeBox));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DLY_FDBK", dlyFdbkKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DLY_MIX", dlyMixKnob));
    buttonAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "DLY_PINGPONG", dlyPingPongToggle));

    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REV_SIZE", revSizeKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REV_DECAY", revDecayKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REV_PREDLY", revPreDlyKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REV_HPF", revHpfKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REV_LPF", revLpfKnob));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REV_MIX", revMixKnob));

    // Header Tab Navigation
    synthTabBtn.onClick    = [this]() { currentTab = TabView::Synth; resized(); repaint(); };
    fxTabBtn.onClick       = [this]() { currentTab = TabView::FxRack; resized(); repaint(); };
    settingsTabBtn.onClick = [this]() { currentTab = TabView::Settings; resized(); repaint(); };

    addAndMakeVisible(synthTabBtn);
    addAndMakeVisible(fxTabBtn);
    addAndMakeVisible(settingsTabBtn);

    // 16 Sequencer Step Knobs & Buttons
    for (int i = 0; i < 16; ++i)
    {
        auto& pKnob = pitchKnobs[i];
        pKnob.setComponentID("pitchKnob");
        pKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        pKnob.setRange(24, 72, 1);
        pKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 48, 14);
        pKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        pKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xff111215));
        pKnob.setColour(juce::Label::textColourId, juce::Colour(0xff111215));
        pKnob.textFromValueFunction = [](double val) { return midiNoteToName(static_cast<int>(val)); };
        pKnob.onValueChange = [this, i]() {
            int stepIdx = currentViewPage * 16 + i;
            audioProcessor.patterns[currentEditPattern][stepIdx].note = static_cast<int>(pitchKnobs[i].getValue());
        };
        addAndMakeVisible(pKnob);

        auto& gBtn = gateButtons[i];
        gBtn.setComponentID("seq_gate");
        gBtn.setButtonText("");
        gBtn.setClickingTogglesState(true);
        gBtn.onClick = [this, i]() {
            int stepIdx = currentViewPage * 16 + i;
            audioProcessor.patterns[currentEditPattern][stepIdx].gate = gateButtons[i].getToggleState();
        };
        addAndMakeVisible(gBtn);

        auto& aBtn = accentButtons[i];
        aBtn.setComponentID("seq_accent");
        aBtn.setButtonText("");
        aBtn.setClickingTogglesState(true);
        aBtn.onClick = [this, i]() {
            int stepIdx = currentViewPage * 16 + i;
            audioProcessor.patterns[currentEditPattern][stepIdx].accent = accentButtons[i].getToggleState();
        };
        addAndMakeVisible(aBtn);

        auto& sBtn = slideButtons[i];
        sBtn.setComponentID("seq_slide");
        sBtn.setButtonText("");
        sBtn.setClickingTogglesState(true);
        sBtn.onClick = [this, i]() {
            int stepIdx = currentViewPage * 16 + i;
            audioProcessor.patterns[currentEditPattern][stepIdx].slide = slideButtons[i].getToggleState();
        };
        addAndMakeVisible(sBtn);
    }

    // 1. RUN / STOP Button
    playStopButton.setComponentID("transport");
    playStopButton.setClickingTogglesState(true);
    playStopButton.setButtonText("RUN");
    playStopButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2d3139));
    playStopButton.onClick = [this]() {
        audioProcessor.startStepOffset.store(currentViewPage * 16);
        audioProcessor.isPlaying.store(playStopButton.getToggleState());
    };
    addAndMakeVisible(playStopButton);

    // 2. ARP SECTION
    arpModeBox.addItemList(juce::StringArray { "ARP OFF", "ARP UP", "ARP DOWN", "UP-DOWN", "RANDOM" }, 1);
    arpModeBox.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(arpModeBox);
    comboAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "ARPMODE", arpModeBox));

    arpHoldBtn.setClickingTogglesState(true);
    arpHoldBtn.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2d3139));
    addAndMakeVisible(arpHoldBtn);
    buttonAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "ARPHOLD", arpHoldBtn));

    // 3. PAGE BUTTONS (1, 2, 3, 4)
    for (int p = 0; p < 4; ++p)
    {
        pageBtns[p].setComponentID("page_" + juce::String(p + 1));
        pageBtns[p].setButtonText(juce::String(p + 1));
        pageBtns[p].onClick = [this, p]() { setViewPage(p); };
        addAndMakeVisible(pageBtns[p]);
    }

    // 4. STEP LENGTH DROPDOWN
    seqLengthBox.addItem("16 Steps", 1);
    seqLengthBox.addItem("32 Steps", 2);
    seqLengthBox.addItem("48 Steps", 3);
    seqLengthBox.addItem("64 Steps", 4);
    seqLengthBox.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(seqLengthBox);
    comboAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "SEQLEN", seqLengthBox));

    // 5. PATTERN BANKS (A, B, C, D)
    static const char* const bankLabels[] = { "A", "B", "C", "D" };
    for (int b = 0; b < 4; ++b)
    {
        bankBtns[b].setComponentID("bank_" + juce::String(bankLabels[b]));
        bankBtns[b].setButtonText(bankLabels[b]);
        bankBtns[b].onClick = [this, b]() { handleBankClick(b); };
        addAndMakeVisible(bankBtns[b]);
    }

    // 6. CHAIN BUTTON
    chainButton.setClickingTogglesState(true);
    chainButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2d3139));
    chainButton.onClick = [this]() {
        bool chainOn = chainButton.getToggleState();
        audioProcessor.isChainingActive.store(chainOn);
        if (!chainOn)
            audioProcessor.queuedPatternIndex.store(-1);
    };
    addAndMakeVisible(chainButton);

    // 7. FOLLOW BUTTON
    followBtn.setClickingTogglesState(true);
    followBtn.setToggleState(isFollowMode, juce::dontSendNotification);
    followBtn.onClick = [this]() { isFollowMode = followBtn.getToggleState(); };
    addAndMakeVisible(followBtn);

    // 8. RANDOMIZE BUTTON (Page-Aware Randomizer)
    randomButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff3f424b));
    randomButton.onClick = [this]() {
        audioProcessor.randomizeCurrentPattern(currentEditPattern, currentViewPage);
        refreshUiFromProcessor();
    };
    addAndMakeVisible(randomButton);

    // 9. ROOT NOTE DROPDOWN (Expanded for "C#", "F#", etc.)
    static const char* const rootNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    for (int r = 0; r < 12; ++r)
        randRootBox.addItem(rootNames[r], r + 1);
    randRootBox.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(randRootBox);
    comboAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "RANDROOT", randRootBox));

    // 10. SCALE DROPDOWN
    randScaleBox.addItem("Min Pentatonic", 1);
    randScaleBox.addItem("Natural Minor", 2);
    randScaleBox.addItem("Dorian", 3);
    randScaleBox.addItem("Phrygian", 4);
    randScaleBox.addItem("Acid Blues", 5);
    randScaleBox.addItem("Maj Pentatonic", 6);
    randScaleBox.addItem("Major", 7);
    randScaleBox.addItem("Chromatic", 8);
    randScaleBox.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(randScaleBox);
    comboAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "RANDSCALE", randScaleBox));

    // 11. COPY & PASTE
    copyBtn.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff3f424b));
    copyBtn.onClick = [this]() {
        for (int i = 0; i < 16; ++i)
            pageClipboard[i] = audioProcessor.patterns[currentEditPattern][currentViewPage * 16 + i];
        hasClipboardData = true;
    };
    addAndMakeVisible(copyBtn);

    pasteBtn.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff3f424b));
    pasteBtn.onClick = [this]() {
        if (hasClipboardData)
        {
            for (int i = 0; i < 16; ++i)
                audioProcessor.patterns[currentEditPattern][currentViewPage * 16 + i] = pageClipboard[i];
            refreshUiFromProcessor();
        }
    };
    addAndMakeVisible(pasteBtn);

    // Settings Page
    themeSelector.addItem("Classic Red (TB-303)", 1);
    themeSelector.addItem("Acid Green", 2);
    themeSelector.addItem("Cyber Cyan", 3);
    themeSelector.addItem("Electric Violet", 4);
    themeSelector.addItem("Solar Amber", 5);
    themeSelector.addItem("Trans Pride", 6);
    themeSelector.onChange = [this]() { updateThemeColors(); };
    addChildComponent(themeSelector);
    comboAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "THEME", themeSelector));

    savePresetBtn.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2d3139));
    savePresetBtn.onClick = [this]()
    {
        fileChooser = std::make_unique<juce::FileChooser>("Save Simple303 Preset", juce::File::getSpecialLocation(juce::File::userHomeDirectory), "*.s303;*.xml");
        auto flags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::warnAboutOverwriting;
        fileChooser->launchAsync(flags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != juce::File())
                file.replaceWithText(audioProcessor.exportPresetToXmlString());
        });
    };
    addChildComponent(savePresetBtn);

    loadPresetBtn.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2d3139));
    loadPresetBtn.onClick = [this]()
    {
        fileChooser = std::make_unique<juce::FileChooser>("Load Simple303 Preset", juce::File::getSpecialLocation(juce::File::userHomeDirectory), "*.s303;*.xml");
        auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        fileChooser->launchAsync(flags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file.existsAsFile())
            {
                audioProcessor.importPresetFromXmlString(file.loadFileAsString());
                refreshUiFromProcessor();
            }
        });
    };
    addChildComponent(loadPresetBtn);

    setEditPattern(0);
    setViewPage(0);
    updateThemeColors();
    startTimerHz(30);
}

Simple303AudioProcessorEditor::~Simple303AudioProcessorEditor()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

void Simple303AudioProcessorEditor::handleBankClick(int bankIndex)
{
    bool isPlay = audioProcessor.isPlaying.load();
    bool isChain = audioProcessor.isChainingActive.load();

    if (isPlay && isChain)
        audioProcessor.queuedPatternIndex.store(bankIndex);
    else if (!isPlay)
        audioProcessor.currentPlayPatternIndex.store(bankIndex);

    setEditPattern(bankIndex);
}

void Simple303AudioProcessorEditor::setEditPattern(int newPattern)
{
    currentEditPattern = juce::jlimit(0, 3, newPattern);
    audioProcessor.currentEditPatternIndex.store(currentEditPattern);
    refreshUiFromProcessor();
}

void Simple303AudioProcessorEditor::setViewPage(int newPage)
{
    currentViewPage = juce::jlimit(0, 3, newPage);
    audioProcessor.startStepOffset.store(currentViewPage * 16);
    refreshUiFromProcessor();
}

void Simple303AudioProcessorEditor::refreshUiFromProcessor()
{
    for (int i = 0; i < 16; ++i)
    {
        int stepIdx = currentViewPage * 16 + i;
        const auto& step = audioProcessor.patterns[currentEditPattern][stepIdx];
        pitchKnobs[i].setValue(step.note, juce::dontSendNotification);
        pitchKnobs[i].updateText();
        gateButtons[i].setToggleState(step.gate, juce::dontSendNotification);
        accentButtons[i].setToggleState(step.accent, juce::dontSendNotification);
        slideButtons[i].setToggleState(step.slide, juce::dontSendNotification);
    }
}

void Simple303AudioProcessorEditor::updateThemeColors()
{
    int themeId = themeSelector.getSelectedId();
    customLookAndFeel.isTransPrideTheme = (themeId == 6);

    switch (themeId)
    {
        case 1: customLookAndFeel.ledGlowColour = juce::Colour(0xffff2828); customLookAndFeel.playheadColour = juce::Colour(0xffffb703); break;
        case 2: customLookAndFeel.ledGlowColour = juce::Colour(0xff39ff14); customLookAndFeel.playheadColour = juce::Colour(0xffccff00); break;
        case 3: customLookAndFeel.ledGlowColour = juce::Colour(0xff00f5d4); customLookAndFeel.playheadColour = juce::Colour(0xff00bbf9); break;
        case 4: customLookAndFeel.ledGlowColour = juce::Colour(0xffc084fc); customLookAndFeel.playheadColour = juce::Colour(0xfff72585); break;
        case 5: customLookAndFeel.ledGlowColour = juce::Colour(0xffff9e00); customLookAndFeel.playheadColour = juce::Colour(0xffffea00); break;
        case 6: customLookAndFeel.ledGlowColour = juce::Colour(0xfff472b6); customLookAndFeel.playheadColour = juce::Colour(0xff5bcefa); break;
        default: break;
    }
    repaint();
}

void Simple303AudioProcessorEditor::timerCallback()
{
    timerBlinkCounter++;

    // 1. Navigation Tabs Active Highlight
    synthTabBtn.setColour(juce::TextButton::buttonColourId, (currentTab == TabView::Synth) ? customLookAndFeel.ledGlowColour : juce::Colour(0xff2d3139));
    fxTabBtn.setColour(juce::TextButton::buttonColourId, (currentTab == TabView::FxRack) ? customLookAndFeel.ledGlowColour : juce::Colour(0xff2d3139));
    settingsTabBtn.setColour(juce::TextButton::buttonColourId, (currentTab == TabView::Settings) ? customLookAndFeel.ledGlowColour : juce::Colour(0xff2d3139));

    // 2. Play / Stop Button State
    bool isPlay = audioProcessor.isPlaying.load();
    if (playStopButton.getToggleState() != isPlay)
        playStopButton.setToggleState(isPlay, juce::dontSendNotification);

    playStopButton.setButtonText(isPlay ? "STOP" : "RUN");
    playStopButton.setColour(juce::TextButton::buttonColourId, isPlay ? juce::Colour(0xffc52222) : juce::Colour(0xff2d3139));

    // 3. Arp Hold, Chain, Follow Button Colors
    bool arpHold = audioProcessor.apvts.getRawParameterValue("ARPHOLD")->load() > 0.5f;
    arpHoldBtn.setColour(juce::TextButton::buttonColourId, arpHold ? customLookAndFeel.ledGlowColour : juce::Colour(0xff2d3139));

    bool isChain = audioProcessor.isChainingActive.load();
    chainButton.setColour(juce::TextButton::buttonColourId, isChain ? customLookAndFeel.ledGlowColour : juce::Colour(0xff2d3139));

    followBtn.setColour(juce::TextButton::buttonColourId, isFollowMode ? customLookAndFeel.ledGlowColour : juce::Colour(0xff2d3139));

    int curStep = audioProcessor.currentStepIndex.load();
    int playBank = audioProcessor.currentPlayPatternIndex.load();
    int playPage = curStep / 16;
    int queuedBank  = audioProcessor.queuedPatternIndex.load();

    // Auto Follow Playhead across Pages 1..4 in real-time
    if (isPlay && isFollowMode && playBank == currentEditPattern && playPage != currentViewPage)
    {
        currentViewPage = playPage;
        refreshUiFromProcessor();
    }

    // Bank Buttons Performance Highlighting
    for (int b = 0; b < 4; ++b)
    {
        if (b == queuedBank && isPlay)
        {
            bool blinkOn = (timerBlinkCounter % 6 < 3);
            bankBtns[b].setColour(juce::TextButton::buttonColourId, blinkOn ? customLookAndFeel.playheadColour : juce::Colour(0xff2d3139));
        }
        else if (isPlay && b == playBank && b != currentEditPattern)
        {
            bankBtns[b].setColour(juce::TextButton::buttonColourId, customLookAndFeel.playheadColour);
        }
        else if (b == currentEditPattern)
        {
            bankBtns[b].setColour(juce::TextButton::buttonColourId, customLookAndFeel.ledGlowColour);
        }
        else
        {
            bankBtns[b].setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2d3139));
        }
    }

    // Page Buttons Highlighting
    for (int p = 0; p < 4; ++p)
    {
        if (p == currentViewPage)
            pageBtns[p].setColour(juce::TextButton::buttonColourId, customLookAndFeel.ledGlowColour);
        else if (isPlay && p == playPage && playBank == currentEditPattern)
            pageBtns[p].setColour(juce::TextButton::buttonColourId, customLookAndFeel.playheadColour);
        else
            pageBtns[p].setColour(juce::TextButton::buttonColourId, juce::Colour(0xff2d3139));
    }

    if (audioProcessor.patternWasUpdated.exchange(false))
        refreshUiFromProcessor();

    if (currentTab == TabView::Synth)
        repaint(60, 150, 980, 320);
}

void Simple303AudioProcessorEditor::paint(juce::Graphics& g)
{
    juce::ColourGradient chassisGrad(juce::Colour(0xffe2e5e9), 0, 0,
                                     juce::Colour(0xffcacdd2), 0, (float)getHeight(), false);
    g.setGradientFill(chassisGrad);
    g.fillRect(getLocalBounds());

    // Top Header Banner
    g.setColour(juce::Colour(0xff181a1f));
    g.fillRect(0, 0, getWidth(), 42);

    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(20.0f).withStyle("bold"));
    g.drawText("Simple303", 20, 0, 140, 42, juce::Justification::centredLeft);

    g.setFont(juce::FontOptions(11.0f).withStyle("bold"));
    g.setColour(juce::Colour(0xff989ca6));
    g.drawText("BASS LINE SYNTHESIZER // ANALOGUE MIDI SEQUENCER", 155, 1, 550, 42, juce::Justification::centredLeft);

    // --- TAB 1: SYNTH VIEW ---
    if (currentTab == TabView::Synth)
    {
        g.setColour(juce::Colour(0xff8e939d));
        g.drawLine(20, 154, getWidth() - 20, 154, 1.2f);
        g.drawLine(20, 157, getWidth() - 20, 157, 0.8f);

        g.setColour(juce::Colour(0xff111215));
        g.setFont(juce::FontOptions(10.5f).withStyle("bold"));

        g.drawText("TUNING", 20, 46, 56, 14, juce::Justification::centred);
        g.drawText("WAVE", 78, 46, 56, 14, juce::Justification::centred);
        g.drawText("CUTOFF", 136, 46, 56, 14, juce::Justification::centred);
        g.drawText("RESO", 194, 46, 56, 14, juce::Justification::centred);
        g.drawText("ENV MOD", 252, 46, 56, 14, juce::Justification::centred);
        g.drawText("DECAY", 310, 46, 56, 14, juce::Justification::centred);
        g.drawText("ACCENT", 368, 46, 56, 14, juce::Justification::centred);
        g.drawText("DRIVE", 426, 46, 56, 14, juce::Justification::centred);
        g.drawText("VOLUME", 484, 46, 56, 14, juce::Justification::centred);
        g.drawText("TEMPO", 542, 46, 56, 14, juce::Justification::centred);

        // Devil Fish Labels
        g.setColour(juce::Colour(0xff992222));
        g.drawText("TRACK", 640, 46, 56, 14, juce::Justification::centred);
        g.drawText("FM", 698, 46, 56, 14, juce::Justification::centred);
        g.drawText("ACC DEC", 756, 46, 56, 14, juce::Justification::centred);
        g.drawText("SLIDE T", 814, 46, 56, 14, juce::Justification::centred);

        // Acid Smiley Face
        {
            float faceX = 948.0f;
            float faceY = 52.0f;
            float faceSize = 68.0f;
            float cx = faceX + faceSize * 0.5f;
            float cy = faceY + faceSize * 0.5f;
            float r  = faceSize * 0.5f;

            g.setColour(juce::Colour(0x35000000));
            g.fillEllipse(faceX + 1.5f, faceY + 2.5f, faceSize, faceSize);

            juce::ColourGradient faceGrad(juce::Colour(0xfffff733), cx - r * 0.35f, cy - r * 0.35f,
                                          juce::Colour(0xffffea00), cx + r * 0.8f, cy + r * 0.8f, true);
            g.setGradientFill(faceGrad);
            g.fillEllipse(faceX, faceY, faceSize, faceSize);

            g.setColour(juce::Colour(0xff111215));
            g.drawEllipse(faceX, faceY, faceSize, faceSize, faceSize * 0.045f);

            float eyeW = faceSize * 0.132f;
            float eyeH = faceSize * 0.218f;
            float eyeY = cy - faceSize * 0.250f;
            float leftEyeX  = cx - faceSize * 0.140f - eyeW * 0.5f;
            float rightEyeX = cx + faceSize * 0.140f - eyeW * 0.5f;

            g.fillEllipse(leftEyeX, eyeY, eyeW, eyeH);
            g.fillEllipse(rightEyeX, eyeY, eyeW, eyeH);

            float startX = cx - faceSize * 0.315f;
            float startY = cy + faceSize * 0.075f;
            float endX   = cx + faceSize * 0.315f;
            float endY   = startY;

            juce::Path smile;
            smile.startNewSubPath(startX, startY);
            smile.cubicTo(cx - faceSize * 0.260f, cy + faceSize * 0.345f,
                          cx + faceSize * 0.260f, cy + faceSize * 0.345f,
                          endX, endY);
            g.strokePath(smile, juce::PathStrokeType(faceSize * 0.044f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

            juce::Path leftDimple, rightDimple;
            leftDimple.startNewSubPath(startX - faceSize * 0.040f, startY - faceSize * 0.020f);
            leftDimple.quadraticTo(startX, startY - faceSize * 0.042f,
                                   startX + faceSize * 0.038f, startY + faceSize * 0.005f);

            rightDimple.startNewSubPath(endX - faceSize * 0.038f, endY + faceSize * 0.005f);
            rightDimple.quadraticTo(endX, endY - faceSize * 0.042f,
                                    endX + faceSize * 0.040f, endY - faceSize * 0.020f);

            g.strokePath(leftDimple,  juce::PathStrokeType(faceSize * 0.042f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            g.strokePath(rightDimple, juce::PathStrokeType(faceSize * 0.042f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        // Sequencer Side Labels
        g.setFont(juce::FontOptions(10.5f).withStyle("bold"));
        g.setColour(juce::Colour(0xff22242a));
        g.drawText("PITCH", 10, 180, 52, 20, juce::Justification::centredRight);
        g.drawText("GATE", 10, 267, 52, 20, juce::Justification::centredRight);
        g.drawText("ACC", 10, 315, 52, 20, juce::Justification::centredRight);
        g.drawText("SLIDE", 10, 363, 52, 20, juce::Justification::centredRight);

        // Playhead Track
        int curStep = audioProcessor.currentStepIndex.load();
        bool playing = audioProcessor.isPlaying.load();
        int playBank = audioProcessor.currentPlayPatternIndex.load();
        int playPage = curStep / 16;
        int playStepInPage = curStep % 16;

        int startX = 72;
        int colWidth = 54;
        int gap = 5;

        for (int i = 0; i < 16; ++i)
        {
            int colX = startX + i * (colWidth + gap);
            bool isPlayStep = (playing && (playBank == currentEditPattern) && (playPage == currentViewPage) && (playStepInPage == i));

            float dotX = colX + colWidth * 0.5f;
            float dotY = 166.0f;

            if (isPlayStep)
            {
                g.setColour(customLookAndFeel.playheadColour.withAlpha(0.6f));
                g.fillEllipse(dotX - 4.5f, dotY - 4.5f, 9.0f, 9.0f);

                g.setColour(juce::Colours::white);
                g.fillEllipse(dotX - 2.0f, dotY - 2.0f, 4.0f, 4.0f);

                g.setColour(customLookAndFeel.playheadColour.withAlpha(0.10f));
                g.fillRoundedRectangle((float)colX, 172.0f, (float)colWidth, 230.0f, 4.0f);

                g.setColour(customLookAndFeel.playheadColour.withAlpha(0.45f));
                g.drawRoundedRectangle((float)colX, 172.0f, (float)colWidth, 230.0f, 4.0f, 1.0f);
            }
            else
            {
                g.setColour(juce::Colour(0xff40444d));
                g.fillEllipse(dotX - 2.0f, dotY - 2.0f, 4.0f, 4.0f);
            }
        }
    }
    // --- TAB 2: FX RACK VIEW ---
    else if (currentTab == TabView::FxRack)
    {
        // 1. Distortion 1 Module Box
        auto ds1Box = juce::Rectangle<int>(30, 70, 310, 320).toFloat();
        g.setColour(juce::Colour(0xff24262d));
        g.fillRoundedRectangle(ds1Box, 6.0f);
        g.setColour(juce::Colour(0xfff97316));
        g.drawRoundedRectangle(ds1Box, 6.0f, 1.5f);
        g.setFont(juce::FontOptions(14.0f).withStyle("bold"));
        g.drawText("DISTORTION 1", ds1Box.toNearestInt().removeFromTop(35), juce::Justification::centred);

        // 2. Stereo Delay Module Box
        auto dlyBox = juce::Rectangle<int>(365, 70, 310, 320).toFloat();
        g.setColour(juce::Colour(0xff24262d));
        g.fillRoundedRectangle(dlyBox, 6.0f);
        g.setColour(juce::Colour(0xff06b6d4));
        g.drawRoundedRectangle(dlyBox, 6.0f, 1.5f);
        g.setFont(juce::FontOptions(14.0f).withStyle("bold"));
        g.drawText("STEREO SYNCED DELAY", dlyBox.toNearestInt().removeFromTop(35), juce::Justification::centred);

        // 3. Plate Reverb Module Box
        auto revBox = juce::Rectangle<int>(700, 70, 330, 320).toFloat();
        g.setColour(juce::Colour(0xff24262d));
        g.fillRoundedRectangle(revBox, 6.0f);
        g.setColour(juce::Colour(0xffa855f7));
        g.drawRoundedRectangle(revBox, 6.0f, 1.5f);
        g.setFont(juce::FontOptions(14.0f).withStyle("bold"));
        g.drawText("PLATE REVERB", revBox.toNearestInt().removeFromTop(35), juce::Justification::centred);

        // High-Contrast White Labels
        g.setColour(juce::Colours::white);
        g.setFont(juce::FontOptions(10.5f).withStyle("bold"));

        // Distortion 1 Labels
        g.drawText("DIST", 50, 115, 60, 14, juce::Justification::centred);
        g.drawText("TONE", 125, 115, 60, 14, juce::Justification::centred);
        g.drawText("LEVEL", 200, 115, 60, 14, juce::Justification::centred);
        g.drawText("MIX", 270, 115, 60, 14, juce::Justification::centred);

        // Delay Labels
        g.drawText("TIME", 390, 115, 80, 14, juce::Justification::centred);
        g.drawText("FEEDBACK", 495, 115, 75, 14, juce::Justification::centred);
        g.drawText("MIX", 590, 115, 60, 14, juce::Justification::centred);

        // Reverb Labels
        g.drawText("SIZE", 720, 115, 60, 14, juce::Justification::centred);
        g.drawText("DECAY", 790, 115, 60, 14, juce::Justification::centred);
        g.drawText("PRE-DLY", 860, 115, 60, 14, juce::Justification::centred);
        g.drawText("MIX", 940, 115, 60, 14, juce::Justification::centred);

        g.drawText("HPF", 760, 235, 60, 14, juce::Justification::centred);
        g.drawText("LPF", 890, 235, 60, 14, juce::Justification::centred);
    }
    // --- TAB 3: SETTINGS VIEW ---
    else if (currentTab == TabView::Settings)
    {
        g.setColour(juce::Colour(0xff111215));
        g.setFont(juce::FontOptions(15.0f).withStyle("bold"));
        g.drawText("COLOR THEME", 60, 80, 200, 25, juce::Justification::centredLeft);
        g.drawText("PRESET MANAGEMENT", 60, 180, 200, 25, juce::Justification::centredLeft);

        g.setFont(juce::FontOptions(12.0f));
        g.setColour(juce::Colour(0xff4a4d56));
        g.drawText("Select custom LED and sequencer playhead glow scheme:", 60, 105, 400, 20, juce::Justification::centredLeft);
        g.drawText("Save and load human-readable XML presets (Banks A-D, 256 Steps):", 60, 205, 450, 20, juce::Justification::centredLeft);
    }
}

void Simple303AudioProcessorEditor::resized()
{
    // Header Tab Buttons
    synthTabBtn.setBounds(getWidth() - 245, 7, 75, 28);
    fxTabBtn.setBounds(getWidth() - 165, 7, 75, 28);
    settingsTabBtn.setBounds(getWidth() - 85, 7, 75, 28);

    bool isSynth = (currentTab == TabView::Synth);
    bool isFx    = (currentTab == TabView::FxRack);
    bool isSet   = (currentTab == TabView::Settings);

    // Synth Controls Visibility
    tuningKnob.setVisible(isSynth);
    waveKnob.setVisible(isSynth);
    cutoffKnob.setVisible(isSynth);
    resonanceKnob.setVisible(isSynth);
    envModKnob.setVisible(isSynth);
    decayKnob.setVisible(isSynth);
    accentKnob.setVisible(isSynth);
    driveKnob.setVisible(isSynth);
    volumeKnob.setVisible(isSynth);
    tempoKnob.setVisible(isSynth);
    trackKnob.setVisible(isSynth);
    fmKnob.setVisible(isSynth);
    accDecayKnob.setVisible(isSynth);
    slideTimeKnob.setVisible(isSynth);

    for (int i = 0; i < 16; ++i)
    {
        pitchKnobs[i].setVisible(isSynth);
        gateButtons[i].setVisible(isSynth);
        accentButtons[i].setVisible(isSynth);
        slideButtons[i].setVisible(isSynth);
    }

    // FX Controls Visibility
    ds1DistKnob.setVisible(isFx);
    ds1ToneKnob.setVisible(isFx);
    ds1LevelKnob.setVisible(isFx);
    ds1MixKnob.setVisible(isFx);

    dlyTimeBox.setVisible(isFx);
    dlyFdbkKnob.setVisible(isFx);
    dlyMixKnob.setVisible(isFx);
    dlyPingPongToggle.setVisible(isFx);

    revSizeKnob.setVisible(isFx);
    revDecayKnob.setVisible(isFx);
    revPreDlyKnob.setVisible(isFx);
    revHpfKnob.setVisible(isFx);
    revLpfKnob.setVisible(isFx);
    revMixKnob.setVisible(isFx);

    // Settings Controls Visibility
    themeSelector.setVisible(isSet);
    savePresetBtn.setVisible(isSet);
    loadPresetBtn.setVisible(isSet);

    if (isSet)
    {
        themeSelector.setBounds(60, 130, 240, 28);
        savePresetBtn.setBounds(60, 235, 160, 32);
        loadPresetBtn.setBounds(230, 235, 160, 32);
    }
    else if (isFx)
    {
        ds1DistKnob.setBounds(50, 140, 60, 80);
        ds1ToneKnob.setBounds(125, 140, 60, 80);
        ds1LevelKnob.setBounds(200, 140, 60, 80);
        ds1MixKnob.setBounds(270, 140, 60, 80);

        dlyTimeBox.setBounds(390, 155, 80, 28);
        dlyFdbkKnob.setBounds(500, 140, 65, 80);
        dlyMixKnob.setBounds(590, 140, 65, 80);
        dlyPingPongToggle.setBounds(465, 260, 110, 30);

        revSizeKnob.setBounds(720, 140, 60, 80);
        revDecayKnob.setBounds(790, 140, 60, 80);
        revPreDlyKnob.setBounds(860, 140, 60, 80);
        revMixKnob.setBounds(940, 140, 60, 80);

        revHpfKnob.setBounds(760, 260, 60, 80);
        revLpfKnob.setBounds(890, 260, 60, 80);
    }
    else if (isSynth)
    {
        int knobY = 60;
        int knobSize = 54;
        int spacing = 58;

        tuningKnob.setBounds(20, knobY, knobSize, knobSize + 20);
        waveKnob.setBounds(20 + spacing, knobY, knobSize, knobSize + 20);
        cutoffKnob.setBounds(20 + spacing * 2, knobY, knobSize, knobSize + 20);
        resonanceKnob.setBounds(20 + spacing * 3, knobY, knobSize, knobSize + 20);
        envModKnob.setBounds(20 + spacing * 4, knobY, knobSize, knobSize + 20);
        decayKnob.setBounds(20 + spacing * 5, knobY, knobSize, knobSize + 20);
        accentKnob.setBounds(20 + spacing * 6, knobY, knobSize, knobSize + 20);
        driveKnob.setBounds(20 + spacing * 7, knobY, knobSize, knobSize + 20);
        volumeKnob.setBounds(20 + spacing * 8, knobY, knobSize, knobSize + 20);
        tempoKnob.setBounds(20 + spacing * 9, knobY, knobSize, knobSize + 20);

        trackKnob.setBounds(640, knobY, knobSize, knobSize + 20);
        fmKnob.setBounds(640 + spacing, knobY, knobSize, knobSize + 20);
        accDecayKnob.setBounds(640 + spacing * 2, knobY, knobSize, knobSize + 20);
        slideTimeKnob.setBounds(640 + spacing * 3, knobY, knobSize, knobSize + 20);

        int startX = 72;
        int colWidth = 54;
        int gap = 5;
        int btnW = 24;
        int btnH = 34;
        int btnOffsetX = (colWidth - btnW) / 2;

        for (int i = 0; i < 16; ++i)
        {
            int x = startX + i * (colWidth + gap);
            pitchKnobs[i].setBounds(x, 175, colWidth, 70);
            gateButtons[i].setBounds(x + btnOffsetX, 260, btnW, btnH);
            accentButtons[i].setBounds(x + btnOffsetX, 308, btnW, btnH);
            slideButtons[i].setBounds(x + btnOffsetX, 356, btnW, btnH);
        }
    }

    // --- REORGANIZED BOTTOM BAR ---
    int by = 422;
    int bh = 28;

    // 1. RUN / STOP Button
    playStopButton.setBounds(15, by, 66, bh);

    // 2. ARP SECTION (Mode Box + Expanded Tactile Hold Button)
    arpModeBox.setBounds(86, by, 84, bh);
    arpHoldBtn.setBounds(175, by, 58, bh);

    // 3. SEQUENCER PAGE BUTTONS (1, 2, 3, 4)
    for (int p = 0; p < 4; ++p)
        pageBtns[p].setBounds(238 + p * 25, by, 23, bh);

    // 4. STEP LENGTH DROPDOWN
    seqLengthBox.setBounds(340, by, 76, bh);

    // 5. PATTERN BANKS (A, B, C, D)
    for (int b = 0; b < 4; ++b)
        bankBtns[b].setBounds(421 + b * 25, by, 23, bh);

    // 6. CHAIN BUTTON
    chainButton.setBounds(524, by, 48, bh);

    // 7. FOLLOW BUTTON
    followBtn.setBounds(576, by, 54, bh);

    // 8. RANDOMIZE BUTTON
    randomButton.setBounds(634, by, 84, bh);

    // 9. ROOT NOTE DROPDOWN (Expanded to 58px for "C#", "F#", etc.)
    randRootBox.setBounds(722, by, 58, bh);

    // 10. SCALE DROPDOWN
    randScaleBox.setBounds(784, by, 118, bh);

    // 11. COPY & PASTE
    copyBtn.setBounds(908, by, 44, bh);
    pasteBtn.setBounds(956, by, 44, bh);
}
