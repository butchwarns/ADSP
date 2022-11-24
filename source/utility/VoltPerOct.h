/*
  ==============================================================================
    VoltPerOct.h

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
* @file VoltPerOct.h
*
* @brief Volt per octave <-> frequency conversion class
*/

#pragma once

#include <cmath>

namespace adsp {
/**
* @brief Volt per octave / frequency conversion class
* 
* Conversion is tuned to a given zero-volt-frequency.
*/
class VoltPerOct {
   public:
    // C0 = 16.35Hz
    VoltPerOct(double _zeroVoltFreq = 16.35) : zeroVoltFreq(_zeroVoltFreq) {}
    ~VoltPerOct() {}

    //==============================================================================

    /**
    * @brief Tune the conversion to a given frequency at zero voltage
    * 
    * @param _zeroVoltFreq Frequency at zero volts
    */
    void setZeroVoltFreq(const double _zeroVoltFreq = 16.35) {
        zeroVoltFreq = _zeroVoltFreq;
    }

    /**
    * @brief Get current tuning
    * 
    * @return Frequency corresponding to zero voltage
    */
    double getZeroVoltFreq() { return zeroVoltFreq; }

    //==============================================================================

    /**
    * @brief Convert voltage to frequency
    * 
    * Uses currently set tuning.
    * 
    * @param volt Voltage to convert
    * @return Frequency 
    */
    inline double voltToFreq(const double &volt) {
        return zeroVoltFreq * pow(2.0, volt);
    }

    /**
    * @brief Convert frequency to voltage
    * 
    * Uses currently set tuning.
    * 
    * @param freq Frequency to convert
    * @return Voltage 
    */
    inline double freqToVolt(const double &freq) {
        return log2(freq / zeroVoltFreq);
    }

    //==============================================================================

    /**
    * @brief Detune voltage by semitones
    * 
    * @param volt Voltage to detune
    * @param semitones Amount to detune by in semitones
    * @return Detuned voltage
    */
    static inline double detuneSemitones(const double &volt,
                                         const double &semitones) {
        constexpr double voltPerSemitone = 1.0 / 12.0;
        return volt + semitones * voltPerSemitone;
    }

    /**
    * @brief Detune voltage by cents
    * 
    * @param volt Voltage to detune
    * @param cents Amount to detune by in cents
    * @return Detuned voltage
    */
    static inline double detuneCents(const float &volt, const float &cents) {
        constexpr double voltPerCent = 1.0 / (12.0 * 100.0);
        return volt + cents * voltPerCent;
    }

   protected:
    /**
    * @brief Tuning (frequency at zero volts)
    */
    double zeroVoltFreq{16.35};
};
}  // namespace adsp