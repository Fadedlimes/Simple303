# Simple303

<div align="center">

![Simple303 Banner](https://raw.githubusercontent.com/Fadedlimes/Simple303/main/Source/preview.png)

**A simple, hands-on, and accessible virtual TB-303 bassline synthesizer and analogue-style 16-step sequencer.**

[![Build & Release](https://github.com/Fadedlimes/Simple303/actions/workflows/build.yml/badge.svg)](https://github.com/Fadedlimes/Simple303/actions/workflows/build.yml)
[![Release](https://img.shields.io/github/v/release/Fadedlimes/Simple303?include_prereleases&color=orange)](https://github.com/Fadedlimes/Simple303/releases)
[![License](https://img.shields.io/badge/license-GPLv3-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey.svg)](https://github.com/Fadedlimes/Simple303/releases)

<img width="1060" height="477" alt="image" src="https://github.com/user-attachments/assets/37bc927b-bcbd-40e3-b6b3-1a7153a4090c" />
<img width="1061" height="475" alt="image" src="https://github.com/user-attachments/assets/9c6c60de-a154-41fc-a1ed-38309277d78f" />

</div>

---

## 📖 About the Project

**Simple303** was created as part of the "Simple" series of virtual instruments (alongside [Simple606](https://github.com/Fadedlimes/Simple606)). 

Modern Digital Audio Workstations (DAWs) and software synthesizers are often overloaded with complex routing matrices, deep menus, and steep learning curves. This project was born out of a desire to make electronic music-making immediately accessible, tactile, and fun for anyone—especially for people like my partner, for whom a traditional DAW is simply too overwhelming.

Simple303 focuses on immediate creative flow: turn knobs, punch in steps, hit random, chain patterns together, and make acid basslines without getting bogged down by software complexity.

---

## ✨ Features

### 🎛️ Synthesizer Engine & Devil Fish Mods
* **Dynamic Waveform Morphing**: Smooth rotary morph between pure **Sawtooth (0%)** and **Square (100%)** waveforms.
* **Classic TB-303 Filter Section**: Dedicated controls for *Tuning*, *Cutoff*, *Resonance*, *Envelope Modulation*, *Decay*, and *Accent*.
* **Analogue Drive**: Built-in diode-clipping saturation for subtle warmth to gritty acid crunch.
* **Devil Fish Modifications**:
  * **`TRACKING`**: Variable keyboard/sequence filter key tracking (0% to 100%).
  * **`FILTER FM`**: Audio-rate filter cutoff modulation driven directly by the oscillator.
  * **`ACC DECAY`**: Independent accent envelope decay length control (20 ms to 500 ms).
  * **`SLIDE TIME`**: Variable portamento glide rate (10 ms to 400 ms).

### 🕹️ 256-Step Multi-Bank Performance Sequencer
* **4 Independent Pattern Banks (`A`, `B`, `C`, `D`)**: Each bank holds a full 64-step sequence (4 pages of 16 steps) for 256 total programmable steps.
* **Performance Pattern Chaining (`CHAIN`)**: Quantized pattern queueing that loops the current bank until you queue the next bank to drop seamlessly on the downbeat.
* **Live Background Editing**: Switch between banks and tweak notes silently while the audio engine keeps playing the active bank in the background.
* **Auto-Follow Playhead (`FOLLOW`)**: Automatically flips pages (1 → 2 → 3 → 4) in real-time as the sequence progresses.
* **Instant Page Auditioning**: Pressing `RUN` immediately starts playback on whichever page you are currently viewing (e.g., Page 3 starts on step 33 instantly).
* **Cross-Bank Copy & Paste**: Copy any 16-step page from any bank and paste it anywhere else.

### 🎲 Musical Scale-Aware Randomizer
* **Root Key Selector**: Quantize randomized basslines to any key (`C` through `B`).
* **8 Musical Scales & Modes**:
  * *Minor Pentatonic* (Classic Acid Techno & House)
  * *Natural Minor (Aeolian)* (Deep melodic techno)
  * *Dorian* (Chicago & Detroit acid grooves)
  * *Phrygian* (Dark Goa & Psytrance acid)
  * *Acid Blues* (Tritone $\flat5$ blue note grit)
  * *Major Pentatonic* & *Major (Ionian)*
  * *Chromatic* (Free unquantized generation)
* **Musical Weighting**: Biased towards root notes (45%), fifths (25%), and octave jumps with syncopated acid rhythm templates.
* **Smart Legato Slide Validation**: Slides are strictly placed only when transitioning into another active note—never leaving an orphaned slide hanging into a rest.
* **Page-Aware**: Randomizing while on Page 1 randomizes the full sequence; randomizing while on Page 2, 3, or 4 randomizes *only* that specific 16-step page.

### 🎹 Arpeggiator & Chord Hold
* **5 Playback Modes**: `ARP OFF`, `UP`, `DOWN`, `UP-DOWN`, `RANDOM`.
* **`HOLD` Mode**: Play a chord on your MIDI controller and lock it in. The arpeggiator will continuously cycle the chord in tempo, keeping your hands free to tweak filter and resonance knobs live.

### 🎚️ Integrated FX Rack
* **Distortion 1**: Hard-clipping distortion with input gain, level makeup, dry/wet mix, and a classic passive tilt tone filter.
* **Stereo Synced Delay**: Tempo-synchronized subdivisions (1/16, 1/16D, 1/16T, 1/8, 1/8D, 1/8T, 1/4, 1/4D, 1/4T, 1/2) with damping and stereo **Ping-Pong** mode.
* **Plate Reverb**: Early 90's warehouse dark plate algorithm with pre-delay, low-end cleanup (`HPF`), high-frequency damping (`LPF`), and size/decay parameters.

### 🎨 Customization & Preset Management
* **6 Visual LED Themes**:
  * *Classic Red (TB-303)*
  * *Acid Green*
  * *Cyber Cyan*
  * *Electric Violet*
  * *Solar Amber*
  * *Trans Pride* (dual-tone pastel blue/white/pink gradient glow across all illuminated buttons)
* **Human-Readable XML Presets (`.s303` / `.xml`)**: Clean XML preset format saving all 256 sequence steps, synth parameters, Devil Fish mods, FX rack settings, and active theme for easy sharing.

---

## 📦 Downloads & Installation

Pre-built binaries for **Linux**, **macOS** (Universal: Apple Silicon & Intel), and **Windows** are automatically generated on every release.

👉 **[Download the Latest Release](https://github.com/Fadedlimes/Simple303/releases)**

### Formats Available:
* **VST3** (64-bit plugin for your DAW of choice)
* **AU (Audio Unit)** (macOS)
* **Standalone Application** (No DAW required—run directly on your desktop with ALSA/JACK/DirectSound/CoreAudio)

---

## 🛠️ Building from Source

### Prerequisites
* **CMake** 3.22 or higher
* **C++17** compatible compiler (`gcc`, `clang`, or `MSVC`)

#### Linux (Debian / Ubuntu / Fedora):
```bash
# Debian / Ubuntu
sudo apt-get install build-essential cmake libasound2-dev libjack-jackd2-dev \
    libgl1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev \
    libxcursor-dev libfreetype6-dev libxcomposite-dev

# Fedora
sudo dnf install gcc-c++ cmake alsa-lib-devel pipewire-jack-audio-connection-kit-devel \
    mesa-libGL-devel libX11-devel libXrandr-devel libXinerama-devel \
    libXcursor-devel freetype-devel libXcomposite-devel
