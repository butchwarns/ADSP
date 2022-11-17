/*
  ==============================================================================
    utility.h

    Copyright (C) 2022 Butch Warns
    contact@butchwarns.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
  ==============================================================================
*/

#pragma once

#include <cmath>

namespace adsp
{
    //==============================================================================

    const double PI = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899;
    const double TWO_PI = 2.0 * 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899;
    const double FRAC_ONE_PI = 1 / PI; // 1 / PI

    const double SQRT_TWO = pow(2.0, 0.5);
    const double FRAC_ONE_SQRT_TWO = pow(2.0, -0.5); // 1 / sqrt(2)

    const double SMALLEST_POSITIVE_FLOAT_VALUE = 1.175494351e-38;
    const double SMALLEST_NEGATIVE_FLOAT_VALUE = -1.175494351e-38;

    const double MIN_FILTER_FREQUENCY = 20.0;
    const double MAX_FILTER_FREQUENCY = 20480.0; // 10 octaves above 20 Hz

    //==============================================================================

    // sgn(x)
    template <typename T = double>
    inline int sgn(T val)
    {
        return (static_cast<T>(0) < val) - (val < static_cast<T>(0));
    }

    //==============================================================================
    // Fix flaoting point underflows

    // Fix possible float underflows
    template <typename T = float>
    inline bool fixFloatUnderflow(T &value)
    {
        // Positive underflow
        if (value > static_cast<T>(0) && value < static_cast<T>(SMALLEST_POSITIVE_FLOAT_VALUE))
        {
            value = static_cast<T>(0);
            return true;
        }
        // Negative underflow
        else if (value < static_cast<T>(0) && value > static_cast<T>(SMALLEST_NEGATIVE_FLOAT_VALUE))
        {
            // Truncate to zero
            value = static_cast<T>(0);
            return true;
        }
        return false;
    }

    //==============================================================================
    // Clipping

    // Clip value to given bounds
    inline double clip(double x, const double min = -1.0f, const double max = 1.0f)
    {
        x = fmin(max, x); // Upper bound
        x = fmax(min, x); // Lower bound
        return x;
    }

    // Clip value to given bounds
    inline float clip(float x, const float min = -1.0f, const float max = 1.0f)
    {
        x = fminf(max, x); // Upper bound
        x = fmaxf(min, x); // Lower bound
        return x;
    }

    // Clip value in place
    inline void clipInPlace(double &x, const double min = -1.0, const double max = 1.0)
    {
        x = fmin(max, x); // Upper bound
        x = fmax(min, x); // Lower bound
    }

    // Clip value in place
    inline void clipInPlace(float &x, const float min = -1.0f, const float max = 1.0f)
    {
        x = fminf(max, x); // Upper bound
        x = fmaxf(min, x); // Lower bound
    }

    //==============================================================================
    // MIDI pitch <-> frequency conversions

    // Convert MIDI pitch to frequency (note number)
    // Pitch 69 is A4 (440.0 Hz)
    // (Calculation based on ideas in Miller Puckette's book:
    // The Theory and Technique of Electronic Music)
    inline double pitchToFreq(const double pitch)
    {
        return 440.0 * pow(2.0, (pitch - 69.0) / 12.0);
    }

    // Convert MIDI pitch to frequency (note number)
    // Pitch 69 is A4 (440.0 Hz)
    // (Calculation based on ideas in Miller Puckette's book:
    // The Theory and Technique of Electronic Music)
    inline float pitchToFreq(const float pitch)
    {
        return 440.0f * powf(2.0f, (pitch - 69.0f) / 12.0f);
    }

    // Convert frequency to MIDI pitch (note number)
    // (Calculation based on ideas in Miller Puckette's book:
    // The Theory and Technique of Electronic Music)
    inline double freqToPitch(const double freq)
    {
        return 69.0 + 12.0 * log2(freq / 440.0);
    }

    // Convert frequency to MIDI pitch (note number)
    // (Calculation based on ideas in Miller Puckette's book:
    // The Theory and Technique of Electronic Music)
    inline float freqToPitch(float freq)
    {
        return 69.0f + 12.0f * log2f(freq / 440.0f);
    }

    //==============================================================================
    // Unipolar <-> bipolar conversions

    // Convert value from bipolar to unipolar
    template <typename T = double>
    inline T bipolarToUnipolar(T x)
    {
        return static_cast<T>(0.5) * x + static_cast<T>(0.5);
    }

    // Convert value from unipolar to bipolar
    template <typename T = double>
    inline T unipolarToBipolar(T x)
    {
        return (x - static_cast<T>(0.5)) * static_cast<T>(2.0);
    }

    //==============================================================================
    // dB <-> gain conversions

    // Convert raw amplitude gain to dB
    inline double rawGainTodB(double gain)
    {
        return 20.0 * log10(gain);
    }

    // Convert raw amplitude gain to dB
    inline float rawGainTodB(float gain)
    {
        return 20.0f * log10f(gain);
    }

    // Convert from dB to raw amplitude gain
    inline double dbToRawGain(double dB)
    {
        return pow(10.0, dB / 20.0);
    }

    // Convert from dB to raw amplitude gain
    inline float dbToRawGain(float dB)
    {
        return powf(10.0f, dB / 20.0f);
    }

    // Convert from dB to raw amplitude gain, clipping the signal when below a threshold
    inline double dbToRawGainOff(const double dB, const double offdB)
    {
        return (dB >= offdB) * pow(10.0, dB / 20.0);
    }

    // Convert from dB to raw amplitude gain, clipping the signal when below a threshold
    inline float dbToRawGainOff(const float dB, const float offdB)
    {
        return (dB >= offdB) * powf(10.0f, dB / 20.0f);
    }

    //==============================================================================
    // Mappings and skews

    // Linear mapping of one range to another
    template <typename T = double>
    inline T linMap(const T val, const T inMin, const T inMax, const T outMin, const T outMax)
    {
        return (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }

    // Linear mapping of normalised value ([-1.0, 1.0]) to a given range
    template <typename T = double>
    inline T linMapNorm(const T val, const T outMin, const T outMax)
    {
        return val * (outMax - outMin) + outMin;
    }

    // Map a value in 0..1 to 0..1 with a skew factor
    // skew = 1 gives a linear map
    // skew = 0..1 gives more resolution in te lower range
    // skew = 1..INF gives more resolution in the upper range
    inline double skewNormalized(const double NormVal, const double skew)
    {
        return pow(NormVal, 1.0 / skew);
    }

    // Map a value in 0..1 to 0..1 with a skew factor
    // skew = 1 gives a linear map
    // skew = 0..1 gives more resolution in te lower range
    // skew = 1..INF gives more resolution in the upper range
    inline float skewNormalized(const float NormVal, const float skew)
    {
        return powf(NormVal, 1.0f / skew);
    }

    //==============================================================================

    // Linear interpolation on interval [a, b] with parameter t in range [0.0, 1.0]
    template <typename T>
    inline T interpolateLinear(const T a, const T b, const T t)
    {
        return a * (static_cast<T>(1) - t) + b * t;
    }

    //==============================================================================
    // Fast function approximations

    // Faster log2 function
    // Based on ideas on stack overflow by users Louis Geoffroy, CuriousGeorge and netvope
    // https://stackoverflow.com/questions/9411823/fast-log2float-x-implementation-c
    inline float fastLog2(float val)
    {
        union
        {
            float val;
            int32_t x;
        } u = {val};
        float log_2 = (float)(((u.x >> 23) & 255) - 128);
        u.x &= ~(255 << 23);
        u.x += 127 << 23;
        log_2 += ((-0.34484843f) * u.val + 2.02466578f) * u.val - 0.67487759f;
        return (log_2);
    }

} // namespace adsp
