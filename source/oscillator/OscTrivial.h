/*
  ==============================================================================
    OscTrivial.h

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
 * @file OscTrivial.h
 *
 * @brief Oscillator without any form of aliasing prevention
 */

#pragma once

#include <cmath>

#include "../utility/utility.h"

namespace adsp
{

    /**
     * @brief Simultaneous output of all waveforms (single sample)
     */
    struct OscTrivialOut
    {
        double saw;
        double tri;
        double pulse;
    };

    /**
     * @brief OscTrivial parameter structure
     */
    struct OscTrivialParams
    {
        OscTrivialParams(){};

        OscTrivialParams &operator=(const OscTrivialParams &params)
        {
            if (this == &params)
            {
                return *this;
            }
            else
            {
                // Update parameters
                frequency = params.frequency;
                phase = params.phase;

                return *this;
            }
        }

        // Defaults

        /**
         * @brief Frequency [Hz]
         */
        double frequency{440.0};

        /**
         * @brief Initial phase angle in range [0.0 to 1.0] (relative to one period length)
         *
         */
        double phase{0.0};
    };

    //==============================================================================

    /**
     * @brief Oscillator with trivially generated waveforms
     *
     * Does not feature any form of aliasing prevention.
     * Outputs different waveforms simultaneously.
     * Can be retriggered and started at a given initial phase angle.
     * Works as an LFO.
     */
    class OscTrivial
    {
    public:
        OscTrivial();
        ~OscTrivial();

        //==============================================================================

        /**
         * @brief Reinitialize oscillator with new sample rate
         *
         * @param sampleRate New sample rate
         */
        void reset(double sampleRate);

        /**
         * @brief Get oscillator parameters
         *
         * @return Parameters
         */
        OscTrivialParams getParams();

        /**
         * @brief Set parameters
         *
         * @param _params New parameters
         */
        void setParams(const OscTrivialParams &_params);

        //==============================================================================

        /**
         * @brief Render output (single sample per waveform)
         *
         * @return Next oscillator output
         */
        virtual OscTrivialOut renderOutputs();

        /**
         * @brief Reset the internal phase accumulator (start waves from beginning)
         */
        void retrigger();

    protected:
        /**
         * @brief Oscillator parameters
         */
        OscTrivialParams params;

        /**
         * @brief Wrap counter if its value is larger than 1.0 (modulo)
         *
         * @param c Counter to wrap
         */
        void wrap(double &c);

        /**
         * @brief Internal sample rate
         */
        double sampleRate{48000.0};

        /**
         * @brief Internal sampling interval
         */
        double T = {1.0 / 48000.0};

        /**
         * @brief Modulo counter for the oscillator algorithms
         */
        double counter{0.0};

        /**
         * @brief Phase-aligned modulo counter for triangle wave
         */
        double counterTri{0.5};

        /**
         * @brief Counter step size (changes with oscillator freqeuncy)
         */
        double delta{0.0};
    };
} // namespace adsp