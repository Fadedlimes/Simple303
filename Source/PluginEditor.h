#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class Simple303LookAndFeel : public juce::LookAndFeel_V4
{
public:
    juce::Colour ledGlowColour      { 0xffff2828 };
    juce::Colour playheadColour     { 0xffffb703 };
    const juce::Colour darkCharcoalText { 0xff111215 };
    bool isTransPrideTheme = false;

    Simple303LookAndFeel()
    {
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        setColour(juce::Slider::textBoxTextColourId, darkCharcoalText);
        setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);

        setColour(juce::Label::textColourId, darkCharcoalText);
        setColour(juce::Label::outlineColourId, juce::Colours::transparentBlack);
        setColour(juce::Label::backgroundColourId, juce::Colours::transparentBlack);

        setColour(juce::TextEditor::textColourId, darkCharcoalText);
        setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
        setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);

        setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xffe8ebf0));
        setColour(juce::ComboBox::textColourId, darkCharcoalText);
        setColour(juce::ComboBox::outlineColourId, juce::Colour(0xff555963));
        setColour(juce::ComboBox::arrowColourId, darkCharcoalText);

        setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0xfff0f2f5));
        setColour(juce::PopupMenu::textColourId, darkCharcoalText);
        setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xffc5cad4));
        setColour(juce::PopupMenu::highlightedTextColourId, juce::Colour(0xff111215));
    }

    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        if (!label.isBeingEdited())
        {
            auto alpha = label.isEnabled() ? 1.0f : 0.5f;
            auto textCol = label.findColour(juce::Label::textColourId);
            g.setColour(textCol.withMultipliedAlpha(alpha));
            g.setFont(juce::FontOptions(10.5f).withStyle("bold"));

            auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());
            g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
                             juce::jmax(1, (int)((float)textArea.getHeight() / 10.5f)),
                             label.getMinimumHorizontalScale());
        }
    }

    void drawComboBox(juce::Graphics& g, int width, int height, bool,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox& box) override
                      {
                          auto bounds = juce::Rectangle<int>(0, 0, width, height).toFloat().reduced(0.5f);
                          auto bgCol = box.findColour(juce::ComboBox::backgroundColourId);
                          bool isDark = bgCol.getBrightness() < 0.5f;

                          if (isDark)
                          {
                              juce::ColourGradient boxGrad(juce::Colour(0xff2b2d35), bounds.getX(), bounds.getY(),
                                                           juce::Colour(0xff18191f), bounds.getX(), bounds.getBottom(), false);
                              g.setGradientFill(boxGrad);
                          }
                          else
                          {
                              juce::ColourGradient boxGrad(juce::Colour(0xfffafbfc), bounds.getX(), bounds.getY(),
                                                           juce::Colour(0xffd5d8de), bounds.getX(), bounds.getBottom(), false);
                              g.setGradientFill(boxGrad);
                          }

                          g.fillRoundedRectangle(bounds, 3.0f);
                          g.setColour(isDark ? juce::Colour(0xff06b6d4) : juce::Colour(0xff555963));
                          g.drawRoundedRectangle(bounds, 3.0f, 1.0f);

                          g.setColour(isDark ? juce::Colour(0xff3f434d) : juce::Colour(0xff8c9099));
                          g.drawLine((float)buttonX, bounds.getY() + 2.0f, (float)buttonX, bounds.getBottom() - 2.0f, 1.0f);

                          juce::Path p;
                          float arrowCenterX = buttonX + buttonW * 0.5f;
                          float arrowCenterY = buttonY + buttonH * 0.5f;
                          p.addTriangle(arrowCenterX - 3.5f, arrowCenterY - 2.0f,
                                        arrowCenterX + 3.5f, arrowCenterY - 2.0f,
                                        arrowCenterX, arrowCenterY + 3.0f);
                          g.setColour(box.findColour(juce::ComboBox::arrowColourId));
                          g.fillPath(p);
                      }

                      void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                            float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                            juce::Slider& slider) override
                                            {
                                                auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
                                                bool isPitchKnob = slider.getComponentID() == "pitchKnob";

                                                const float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.5f - (isPitchKnob ? 7.0f : 12.0f);
                                                const float centreX = bounds.getCentreX();
                                                const float centreY = bounds.getCentreY();

                                                const int numTicks = isPitchKnob ? 7 : 11;
                                                for (int i = 0; i < numTicks; ++i)
                                                {
                                                    const float angle = rotaryStartAngle + (rotaryEndAngle - rotaryStartAngle) * (static_cast<float>(i) / (numTicks - 1));
                                                    const float innerR = radius + 2.5f;
                                                    const float outerR = radius + (i == 0 || i == numTicks - 1 || i == numTicks / 2 ? (isPitchKnob ? 5.5f : 7.5f) : 4.0f);

                                                    const float x1 = centreX + innerR * std::sin(angle);
                                                    const float y1 = centreY - innerR * std::cos(angle);
                                                    const float x2 = centreX + outerR * std::sin(angle);
                                                    const float y2 = centreY - outerR * std::cos(angle);

                                                    g.setColour(juce::Colour(0xff52565e));
                                                    g.drawLine(x1, y1, x2, y2, 1.1f);
                                                }

                                                g.setColour(juce::Colour(0x35000000));
                                                g.fillEllipse(centreX - radius + 1.0f, centreY - radius + 2.5f, radius * 2.0f, radius * 2.0f);

                                                juce::ColourGradient bezelGrad(juce::Colour(0xfffafbfc), centreX - radius, centreY - radius,
                                                                               juce::Colour(0xff757982), centreX + radius, centreY + radius, false);
                                                g.setGradientFill(bezelGrad);
                                                g.fillEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);

                                                g.setColour(juce::Colour(0xff3a3d45));
                                                g.drawEllipse(centreX - radius + 0.5f, centreY - radius + 0.5f, (radius - 0.5f) * 2.0f, (radius - 0.5f) * 2.0f, 1.0f);

                                                const float innerR = radius - (isPitchKnob ? 1.8f : 2.5f);
                                                juce::ColourGradient dialGrad(juce::Colour(0xffeaecf0), centreX - innerR * 0.4f, centreY - innerR * 0.4f,
                                                                              juce::Colour(0xffa2a6af), centreX + innerR * 0.8f, centreY + innerR * 0.8f, true);
                                                g.setGradientFill(dialGrad);
                                                g.fillEllipse(centreX - innerR, centreY - innerR, innerR * 2.0f, innerR * 2.0f);

                                                const float capR = innerR * 0.60f;
                                                juce::ColourGradient capGrad(juce::Colour(0xff989ca5), centreX - capR * 0.2f, centreY - capR * 0.2f,
                                                                             juce::Colour(0xffd4d7dc), centreX + capR * 0.5f, centreY + capR * 0.5f, true);
                                                g.setGradientFill(capGrad);
                                                g.fillEllipse(centreX - capR, centreY - capR, capR * 2.0f, capR * 2.0f);

                                                const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
                                                juce::Path p;
                                                p.addRectangle(-1.1f, -innerR + 1.5f, 2.2f, innerR * 0.68f);
                                                g.setColour(juce::Colour(0xff1e2025));
                                                g.fillPath(p, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
                                            }

                                            void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                                                      const juce::Colour&, bool shouldDrawButtonAsHighlighted, bool) override
                                                                      {
                                                                          auto bounds = button.getLocalBounds().toFloat().reduced(0.5f);
                                                                          bool isTransport = button.getComponentID() == "transport";
                                                                          bool isSeqButton = button.getComponentID().startsWith("seq_");
                                                                          bool isPageBtn   = button.getComponentID().startsWith("page_");
                                                                          bool isBankBtn   = button.getComponentID().startsWith("bank_");
                                                                          bool isGeneralBtn = !isTransport && !isSeqButton && !isPageBtn && !isBankBtn;

                                                                          if (isTransport || isGeneralBtn || isPageBtn || isBankBtn)
                                                                          {
                                                                              auto baseCol = button.findColour(juce::TextButton::buttonColourId);
                                                                              juce::ColourGradient tGrad(shouldDrawButtonAsHighlighted ? baseCol.brighter(0.15f) : baseCol,
                                                                                                         bounds.getX(), bounds.getY(),
                                                                                                         baseCol.darker(0.3f), bounds.getX(), bounds.getBottom(), false);
                                                                              g.setGradientFill(tGrad);
                                                                              g.fillRoundedRectangle(bounds, 3.0f);
                                                                              g.setColour(juce::Colour(0xff181a1f));
                                                                              g.drawRoundedRectangle(bounds, 3.0f, 1.0f);
                                                                              return;
                                                                          }

                                                                          if (isSeqButton)
                                                                          {
                                                                              g.setColour(juce::Colour(0x28000000));
                                                                              g.fillRoundedRectangle(bounds.translated(0.0f, 1.0f), 2.5f);

                                                                              juce::ColourGradient btnGrad(shouldDrawButtonAsHighlighted ? juce::Colour(0xff4e515a) : juce::Colour(0xff3a3c44),
                                                                                                           bounds.getX(), bounds.getY(),
                                                                                                           juce::Colour(0xff23252a), bounds.getX(), bounds.getBottom(), false);
                                                                              g.setGradientFill(btnGrad);
                                                                              g.fillRoundedRectangle(bounds, 2.5f);

                                                                              g.setColour(juce::Colour(0xff181a1f));
                                                                              g.drawRoundedRectangle(bounds, 2.5f, 0.8f);

                                                                              auto innerLens = bounds.reduced(3.0f, 3.5f);
                                                                              bool isActive = button.getToggleState();

                                                                              if (isActive)
                                                                              {
                                                                                  if (isTransPrideTheme)
                                                                                  {
                                                                                      g.setColour(juce::Colour(0x66f472b6));
                                                                                      g.fillRoundedRectangle(innerLens.expanded(1.5f), 2.0f);

                                                                                      juce::ColourGradient prideGlow(juce::Colour(0xff5bcefa), innerLens.getX(), innerLens.getY(),
                                                                                                                     juce::Colour(0xfff472b6), innerLens.getX(), innerLens.getBottom(), false);
                                                                                      g.setGradientFill(prideGlow);
                                                                                      g.fillRoundedRectangle(innerLens, 2.0f);

                                                                                      g.setColour(juce::Colours::white);
                                                                                      g.fillEllipse(innerLens.getCentreX() - 1.2f, innerLens.getY() + 1.5f, 2.4f, 2.0f);
                                                                                  }
                                                                                  else
                                                                                  {
                                                                                      g.setColour(ledGlowColour.withAlpha(0.35f));
                                                                                      g.fillRoundedRectangle(innerLens.expanded(1.5f), 2.0f);

                                                                                      juce::ColourGradient lensGrad(juce::Colours::white, innerLens.getCentreX(), innerLens.getY(),
                                                                                                                    ledGlowColour, innerLens.getCentreX(), innerLens.getBottom(), false);
                                                                                      g.setGradientFill(lensGrad);
                                                                                      g.fillRoundedRectangle(innerLens, 2.0f);

                                                                                      g.setColour(juce::Colours::white);
                                                                                      g.fillEllipse(innerLens.getCentreX() - 1.2f, innerLens.getY() + 1.5f, 2.4f, 2.0f);
                                                                                  }
                                                                              }
                                                                              else
                                                                              {
                                                                                  juce::ColourGradient offLens(juce::Colour(0xff34363d), innerLens.getX(), innerLens.getY(),
                                                                                                               juce::Colour(0xff1a1b1f), innerLens.getX(), innerLens.getBottom(), false);
                                                                                  g.setGradientFill(offLens);
                                                                                  g.fillRoundedRectangle(innerLens, 2.0f);
                                                                              }

                                                                              g.setColour(juce::Colour(0xff111214));
                                                                              g.drawRoundedRectangle(innerLens, 2.0f, 0.6f);
                                                                          }
                                                                      }

                                                                      void drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                                                                          bool, bool) override
                                                                                          {
                                                                                              if (button.getComponentID() != "seq_gate" && button.getComponentID() != "seq_accent" && button.getComponentID() != "seq_slide")
                                                                                              {
                                                                                                  g.setFont(juce::FontOptions(11.0f).withStyle("bold"));
                                                                                                  g.setColour(juce::Colours::white);
                                                                                                  g.drawText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, false);
                                                                                              }
                                                                                          }
};

class Simple303AudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    Simple303AudioProcessorEditor(Simple303AudioProcessor&);
    ~Simple303AudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void updateThemeColors();
    void setViewPage(int newPage);
    void setEditPattern(int newPattern);
    void handleBankClick(int bankIndex);
    void refreshUiFromProcessor();

    Simple303AudioProcessor& audioProcessor;
    Simple303LookAndFeel customLookAndFeel;

    // View Navigation
    enum class TabView { Synth, FxRack, Settings };
    TabView currentTab = TabView::Synth;
    juce::TextButton synthTabBtn { "SYNTH" };
    juce::TextButton fxTabBtn { "FX RACK" };
    juce::TextButton settingsTabBtn { "SETTINGS" };

    // Standard 303 & Devil Fish Knobs
    juce::Slider tuningKnob, waveKnob, cutoffKnob, resonanceKnob, envModKnob, decayKnob, accentKnob, driveKnob, volumeKnob, tempoKnob;
    juce::Slider trackKnob, fmKnob, accDecayKnob, slideTimeKnob;

    // FX 1: Distortion 1 Controls
    juce::Slider ds1DistKnob, ds1ToneKnob, ds1LevelKnob, ds1MixKnob;

    // FX 2: Stereo Delay Controls
    juce::ComboBox dlyTimeBox;
    juce::Slider dlyFdbkKnob, dlyMixKnob;
    juce::ToggleButton dlyPingPongToggle { "PING PONG" };

    // FX 3: Plate Reverb Controls
    juce::Slider revSizeKnob, revDecayKnob, revPreDlyKnob, revHpfKnob, revLpfKnob, revMixKnob;

    // Attachments
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>> comboAttachments;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> buttonAttachments;

    // 16 Sequencer Step Knobs & Buttons
    std::array<juce::Slider, 16> pitchKnobs;
    std::array<juce::TextButton, 16> gateButtons;
    std::array<juce::TextButton, 16> accentButtons;
    std::array<juce::TextButton, 16> slideButtons;

    // Reorganized Bottom Bar Controls
    juce::TextButton playStopButton;
    juce::ComboBox arpModeBox;
    juce::TextButton arpHoldBtn { "HOLD" };
    std::array<juce::TextButton, 4> pageBtns;
    juce::ComboBox seqLengthBox;
    std::array<juce::TextButton, 4> bankBtns;
    juce::TextButton chainButton { "CHAIN" };
    juce::TextButton followBtn { "FOLLOW" };
    juce::TextButton randomButton { "RANDOMIZE" };
    juce::ComboBox randRootBox;
    juce::ComboBox randScaleBox;
    juce::TextButton copyBtn { "COPY" };
    juce::TextButton pasteBtn { "PASTE" };

    int currentEditPattern = 0;
    int currentViewPage = 0;
    bool isFollowMode = true;
    std::array<StepData, 16> pageClipboard;
    bool hasClipboardData = false;
    int timerBlinkCounter = 0;

    // Settings Components
    juce::ComboBox themeSelector;
    juce::TextButton savePresetBtn { "SAVE XML PRESET" };
    juce::TextButton loadPresetBtn { "LOAD XML PRESET" };
    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Simple303AudioProcessorEditor)
};
