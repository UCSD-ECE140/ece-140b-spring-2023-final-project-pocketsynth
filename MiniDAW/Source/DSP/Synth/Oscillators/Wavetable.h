/*
  ==============================================================================

    Wavetable.h
    Created: 14 Jul 2022 8:51:19pm
    Author:  Speechrezz

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum
{
    FFT_ORDER = 11, // 11
    FFT_SIZE = 1 << FFT_ORDER // 2048
};

namespace xynth
{

inline const juce::StringArray wavetableTypes{ "Triangle", "Saw", "Square", "Sine" };

class Wavetable
{
public:
    Wavetable();
    ~Wavetable() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);

    // Sets the frequency of the oscillator, which recalculate the inverse FFT.
    void setFrequency(const float frequency);

    // Returns sample at position currentAngle. Uses Hermite interpolation.
    // Note: currentAngle must be between 0 and FFT_SIZE - 1.
    float getSample(const float currentAngle);

    float operator[](const float currentAngle) 
    {
        return getSample(currentAngle);
    }

    void setSound(const int soundIndex);

private:
    float nyquistLimit = -1.f;

    // Hermite interpolation. 0 <= offset < 1.
    float hermite(float v0, float v1, float v2, float v3, float offset);

    // In order to avoid wrapping when doing hermite interpolation, extend the wavetable a bit.
    // 1 extra sample at the start, 2 at the end.
    float wavetable[FFT_SIZE + 3];

    juce::dsp::FFT fft;

    std::complex<float> fftTime[FFT_SIZE];     // Time domain FFT data
    std::complex<float> fftFreq[FFT_SIZE];     // Frequency domain FFT data
    std::complex<float> fftFreqCopy[FFT_SIZE]; // Frequency domain FFT data (copy)
};

}