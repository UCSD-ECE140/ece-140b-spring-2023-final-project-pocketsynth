/*
  ==============================================================================

    Wavetable.cpp
    Created: 14 Jul 2022 8:51:19pm
    Author:  Speechrezz

  ==============================================================================
*/

#include "Wavetable.h"

namespace xynth
{

Wavetable::Wavetable() : fft(FFT_ORDER)
{
    setSound(0);
}

void Wavetable::prepare(const juce::dsp::ProcessSpec& spec)
{
    nyquistLimit = (float)spec.sampleRate / 2.f;
}

void Wavetable::setFrequency(const float frequency)
{
    float numHarmonics = std::floor(nyquistLimit / frequency);

    // Forward FFT
    std::copy(std::begin(fftFreq), std::end(fftFreq), std::begin(fftFreqCopy));

    // Remove harmonics above Nyquist
    for (int i = FFT_SIZE - 1; i >= numHarmonics; --i)
        fftFreqCopy[i] = std::complex<float>(0.f, 0.f);

    // Inverse FFT
    fft.perform(fftFreqCopy, fftTime, true);

    // Convert back to time domain
    for (int i = 0; i < FFT_SIZE; ++i)
        wavetable[i + 1] = fftTime[i].real();

    // Adjust wavetable
    wavetable[0] = wavetable[FFT_SIZE];
    wavetable[FFT_SIZE + 1] = wavetable[1];
    wavetable[FFT_SIZE + 2] = wavetable[2];
}

float Wavetable::getSample(const float currentAngle)
{
    const int i0 = int(currentAngle);
    const float offset = currentAngle - std::floor(currentAngle);
    return hermite(wavetable[i0], wavetable[i0 + 1], wavetable[i0 + 2], wavetable[i0 + 3], offset);
}

void Wavetable::setSound(const int soundIndex)
{
    // Initialize wavetable
    const auto inc = 2.f / float(FFT_SIZE - 1);
    const auto inc2 = 2.f * inc; // For triangle wave
    const float pd = juce::MathConstants<float>::twoPi / float(FFT_SIZE);

    for (int i = 0; i < FFT_SIZE; ++i)
    {
        switch (soundIndex)
        {
        case 0:
            // Triangle:
            if (i < FFT_SIZE / 4)
                fftTime[i] = float(i) * inc2; // Linear ramp up for first quarter
            else if (i < FFT_SIZE * 3 / 4)
                fftTime[i] = 1.f - (float(i - FFT_SIZE / 4) * inc2); // Linear ramp down for second and third quarters
            else
                fftTime[i] = float(i - FFT_SIZE * 3 / 4) * inc2 - 1.f; // Linear ramp up for fourth quarter
            break;

        case 1:
            fftTime[i] = float(i) * inc - 1.f; // Saw
            break;

        case 2:
            fftTime[i] = i < FFT_SIZE / 2 ? 1.f : -1.f; //Square
            break;

        case 3:
            fftTime[i] = std::sin(float(i) * pd); // sine
            break;

        default:
            jassertfalse; // Out of bounds
        }
    }

    // Perform forward fft
    fft.perform(fftTime, fftFreq, false);
}

float Wavetable::hermite(float v0, float v1, float v2, float v3, float offset)
{
    float slope0 = (v2 - v0) * 0.5f;
    float slope1 = (v3 - v1) * 0.5f;
    float v = v1 - v2;
    float w = slope0 + v;
    float a = w + v + slope1;
    float b_neg = w + a;
    float stage1 = a * offset - b_neg;
    float stage2 = stage1 * offset + slope0;
    return stage2 * offset + v1;
}

}