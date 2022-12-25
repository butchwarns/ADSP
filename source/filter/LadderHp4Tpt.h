/*
  ==============================================================================
    LadderHp4Tpt.h

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
 * @file LadderHp4Tpt.h
 *
 * @brief Fourth-order ladder high-pass filter
 */

#pragma once

#include "../utility/utility.h"
#include "../filter/RcHp1Tpt.h"

namespace adsp
{
    /**
     * @brief LadderHp4Tpt parameter structure
     *
     */
    struct LadderHp4TptParams
    {
        LadderHp4TptParams(){};

        LadderHp4TptParams &operator=(const LadderHp4TptParams &parameters)
        {
            if (this == &parameters)
            {
                return *this;
            }
            else
            {
                fc = parameters.fc;
                res = parameters.res;
                return *this;
            }
        }

        // Cutoff frequency
        double fc{100.0}; // Hz

        // Resonance
        double res{0.0};
    };

    /**
     * @brief Fourth-order ladder high-pass filter
     *
     * Analog modeled by means of the topology-preserving transform,
     * as per Zavalishsin's "The Art of VA Filter Design".
     * Prewarping was chosen to match cutoff frequencies.
     * This filter has not been decramped.
     */
    class LadderHp4Tpt
    {
    public:
        LadderHp4Tpt();
        ~LadderHp4Tpt();

        //==============================================================================

        /**
         * @brief Reset to default parameters, clear internal state and set sample rate
         *
         * @param sampleRate New sample rate
         */
        void reset(double sampleRate);

        /**
         * @brief Proess a single sample
         *
         * @param x Input sample
         * @return Output sample
         */
        double process(double x);

        //==============================================================================

        /**
         * @brief Set sample rate, recalculate coefficients
         *
         * @param _sampleRate
         */
        void setSampleRate(double _sampleRate);

        /**
         * @brief Get parameters
         *
         * @return Filter parameters
         */
        LadderHp4TptParams getParameters();

        /**
         * @brief Set parameters
         *
         * @param New filter parameters
         */
        void setParameters(const LadderHp4TptParams &parameters);

        //==============================================================================

        double getState();

    protected:
        /**
         * @brief Sample rate
         */
        double sampleRate{48000.0};

        /**
         * @brief Individual high-pass stages
         */
        adsp::RcHp1Tpt hp[4];

        /**
         * @brief Ladder structure feedback gain coefficient
         * @f$ k \in [0.0, 4.0] @f$
         * Filter will self oscillate for k = 4.
         */
        double k{0.0};

        /**
         * @brief Filter parameters
         */
        LadderHp4TptParams params;
    };
} // namespace adsp