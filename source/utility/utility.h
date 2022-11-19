/*
  ==============================================================================
    utility.h

    Copyright (C) 2022 Butch Warns
    All rights reserved.

    contact@butchwarns.de

    BSD 2-Clause License

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
        list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the documentation
        and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  ==============================================================================
*/

/**
 * @file utility.h
 *
 * @brief Different dsp utility functions and useful constants
 */

#pragma once

#include <cmath>

namespace adsp {
//==============================================================================

/**
* @brief @f$ \pi @f$
*/
constexpr double PI =
    3.14159265358979323846264338327950288419716939937510582097494459230781640628620899;

/**
* @brief @f$ 2\pi @f$
*/
constexpr double TWO_PI =
    2.0 *
    3.14159265358979323846264338327950288419716939937510582097494459230781640628620899;

/**
* @brief @f$ 1/\pi @f$
*/
constexpr double FRAC_ONE_PI = 1.0 / PI;  // 1 / PI

/**
* @brief @f$ \sqrt{2} @f$
*/
const double SQRT_TWO = pow(2.0, 0.5);

/**
* @brief @f$ 1/\sqrt{2} @f$
*/
const double FRAC_ONE_SQRT_TWO = pow(2.0, -0.5);  // 1 / sqrt(2)

/**
* @brief Smallest positive float value
*/
constexpr double MIN_FLOAT_VAL_POS = 1.175494351e-38;

/**
* @brief Smallest negative float value
*/
constexpr double MIN_FLOAT_VAL_NEG = -1.175494351e-38;

/**
* @brief Minimum filter cutoff frequency: 20 Hz (lower end of human hearing)
*/
constexpr double MIN_FILTER_FREQ = 20.0;

/**
* @brief Maximum filter cutoff frequency: 20480 Hz (10 octaves above 20 Hz)
*
*/
constexpr double MAX_FILTER_FREQ = 20480.0;

//==============================================================================

/**
* @brief sgn(x), the standard signum function.
*
* @tparam T Integral or floating point type
* @param val Value to take the sign of
* @return 1 if val is positive,
* 0 if val is zero,
* -1 if val is negative
*/
template <typename T = double>
inline int sgn(T val) {
    return (static_cast<T>(0) < val) - (val < static_cast<T>(0));
}

//==============================================================================

/**
* @brief Fix possible float underflows before casting from double to avoid denormals
*
* @param d double value to be checked for possible underflow
* @return true if underflow was fixed
* @return false if no underflow occured
*/
inline bool fixUnderflow(double &d) {
    // Positive underflow
    if (d > 0.0 && d < MIN_FLOAT_VAL_POS) {
        // Fix to zero
        d = 0.0;
        return true;
    }
    // Negative underflow
    else if (d < 0.0 && d > MIN_FLOAT_VAL_NEG) {
        // Fix to zero
        d = 0.0;
        return true;
    }

    return false;
}

//==============================================================================
// Clipping

/**
* @brief Clip value to given bounds
*
* @param x Value to be bounded
* @param min Lower bound, defaults to -1.0
* @param max Upper bound, defaults to 1.0
* @return Clipped value
*/
inline double clip(double x, const double min = -1.0f,
                   const double max = 1.0f) {
    // Clip upper bound
    x = fmin(max, x);

    // Clip lower bound
    x = fmax(min, x);

    return x;
}

/**
* @brief Clip value to given bounds
*
* @param x Value to be bounded
* @param min Lower bound, defaults to -1.0
* @param max Upper bound, defaults to 1.0
* @return Clipped value
*/
inline float clip(float x, const float min = -1.0f, const float max = 1.0f) {
    // Clip upper bound
    x = fminf(max, x);

    // Clip lower bound
    x = fmaxf(min, x);

    return x;
}

/**
* @brief Clip value in place to given bounds
*
* @param x Value to be bounded
* @param min Lower bound, defaults to -1.0
* @param max Upper bound, defaults to 1.0
*/
inline void clipInPlace(double &x, const double min = -1.0,
                        const double max = 1.0) {
    // Clip upper bound
    x = fmin(max, x);

    // Clip lower bound
    x = fmax(min, x);
}

/**
* @brief Clip value in place to given bounds
*
* @param x Value to be bounded
* @param min Lower bound, defaults to -1.0
* @param max Upper bound, defaults to 1.0
*/
inline void clipInPlace(float &x, const float min = -1.0f,
                        const float max = 1.0f) {
    // Clip upper bound
    x = fminf(max, x);

    // Clip lower bound
    x = fmaxf(min, x);
}

//==============================================================================
// MIDI pitch <-> frequency conversions

/**
* @brief Convert MIDI pitch (note number) to frequency
*
* @param pitch MIDI note number to convert (works for non-integer numbers!)
* @return Corresponding frequency
*/
inline double pitchToFreq(const double pitch) {
    // Pitch 69 is A4:440.0 Hz
    return 440.0 * pow(2.0, (pitch - 69.0) / 12.0);
}

/**
* @brief Convert MIDI pitch (note number) to frequency
*
* @param pitch MIDI note number to convert (works for non-integer numbers!)
* @return Corresponding frequency
*/
inline float pitchToFreq(const float pitch) {
    // Pitch 69 is A4:440.0 Hz
    return 440.0f * powf(2.0f, (pitch - 69.0f) / 12.0f);
}

/**
* @brief Convert frequency to MIDI pitch (note number)
*
* @param freq Frequency to convert
* @return Corresponding MIDI pitch
*/
inline double freqToPitch(const double freq) {
    return 69.0 + 12.0 * log2(freq / 440.0);
}

/**
* @brief Convert frequency to MIDI pitch (note number)
*
* @param freq Frequency to convert
* @return Corresponding MIDI pitch
*/
inline float freqToPitch(float freq) {
    return 69.0f + 12.0f * log2f(freq / 440.0f);
}

//==============================================================================
// Unipolar <-> bipolar conversions

/**
* @brief Convert value from bipolar to unipolar range
*
* @tparam T float or double
* @param x Bipolar value in range [-1.0, 1.0]
* @return Corresponding unipolar value in range [0.0, 1.0]
*/
template <typename T = double>
inline T bipolarToUnipolar(T x) {
    return static_cast<T>(0.5) * x + static_cast<T>(0.5);
}

/**
* @brief Convert value from unipolar to bipolar range
*
* @tparam T float or double
* @param x Unipolar value in range [0.0, 1.0]
* @return Corresponding bipolar value in range [-1.0, 1.0]
*/
template <typename T = double>
inline T unipolarToBipolar(T x) {
    return (x - static_cast<T>(0.5)) * static_cast<T>(2.0);
}

//==============================================================================
// dB <-> gain conversions

/**
* @brief Convert raw amplitude gain to decibels
*
* @param gain Raw amplitude gain factor [1]
* @return Corresponding decibel value [dB]
*/
inline double rawGainTodB(double gain) { return 20.0 * log10(gain); }

/**
* @brief Convert raw amplitude gain to decibels
*
* @param gain Raw amplitude gain factor [1]
* @return Corresponding decibel value [dB]
*/
inline float rawGainTodB(float gain) { return 20.0f * log10f(gain); }

/**
* @brief Convert from decibels to raw amplitude gain
*
* @param dB Value to convert [dB]
* @return Corresponding amplitude gain factor [1]
*/
inline double dbToRawGain(double dB) { return pow(10.0, dB / 20.0); }

/**
* @brief Convert from decibels to raw amplitude gain
*
* @param dB Value to convert [dB]
* @return Corresponding amplitude gain factor [1]
*/
inline float dbToRawGain(float dB) { return powf(10.0f, dB / 20.0f); }

/**
* @brief Convert from decibels to raw amplitude gain, clipping the signal when below a threshold
*
* @param dB Value to convert [dB]
* @param offdB Off-threshold [dB]
* @return Corresponding amplitude gain factor [1]
*/
inline double dbToRawGainOff(const double dB, const double offdB) {
    return (dB >= offdB) * pow(10.0, dB / 20.0);
}

/**
* @brief Convert from decibels to raw amplitude gain, clipping the signal when below a threshold
*
* @param dB Value to convert [dB]
* @param offdB Off-threshold [dB]
* @return Corresponding amplitude gain factor [1]
*/
inline float dbToRawGainOff(const float dB, const float offdB) {
    return (dB >= offdB) * powf(10.0f, dB / 20.0f);
}

//==============================================================================
// Mappings and skews

/**
* @brief Linear mapping of one continuous range to another
*
*  (Linear interpolation)
*
* @tparam T float or double
* @param val Value to map
* @param inMin Input lower bound
* @param inMax Input upper bound
* @param outMin Output lower bound
* @param outMax Output range upper bound
* @return Mapped value
*/
template <typename T = double>
inline T linMap(const T val, const T inMin, const T inMax, const T outMin,
                const T outMax) {
    return (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

/**
* @brief Linear mapping of value in [-1.0, 1.0] to another continuous range
*
*  (Linear interpolation)
*
* @tparam T float or double
* @param val Normalised value to map
* @param inMin Input lower bound
* @param inMax Input upper bound
* @param outMin Output lower bound
* @param outMax Output upper bound
* @return Mapped value
*/
template <typename T = double>
inline T linMapNorm(const T val, const T outMin, const T outMax) {
    return val * (outMax - outMin) + outMin;
}

/**
* @brief Map a value in [0.0, 1.0] to the same interval with a skew factor
*
* skew = 1 gives a linear map  
* skew in [0.0, 1.0[ gives more resolution in the lower range  
* skew in [1.0, INF[ gives more resolution in the upper range  
*
* @param NormVal Normalised value to map
* @param skew Skew factor
* @return Mapped value
*/
inline double skewNormalized(const double NormVal, const double skew) {
    return pow(NormVal, 1.0 / skew);
}

/**
* @brief Map a value in [0.0, 1.0] to the same interval with a skew factor
*
* skew = 1 gives a linear map  
* skew in [0.0, 1.0[ gives more resolution in the lower range  
* skew in [1.0, INF[ gives more resolution in the upper range  
*
* @param NormVal Normalised value to map
* @param skew Skew factor
* @return Mapped value
*/
inline float skewNormalized(const float NormVal, const float skew) {
    return powf(NormVal, 1.0f / skew);
}

//==============================================================================
// Fast function approximations

/**
* @brief Faster (and less precise) log2 function
*
* Based on ideas on stack overflow by users Louis Geoffroy, CuriousGeorge and netvope  
* https://stackoverflow.com/questions/9411823/fast-log2float-x-implementation-c
*
* @param val Input value
* @return log2(val)
*/
inline float fastLog2(float val) {
    union {
        float val;
        int32_t x;
    } u = {val};
    float log_2 = (float)(((u.x >> 23) & 255) - 128);
    u.x &= ~(255 << 23);
    u.x += 127 << 23;
    log_2 += ((-0.34484843f) * u.val + 2.02466578f) * u.val - 0.67487759f;
    return (log_2);
}
}  // namespace adsp
