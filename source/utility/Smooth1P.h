/*
  ==============================================================================
    Smooth1P.h

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
 * @file Smooth1P.h
 *
 * @brief One-pole filter for smoothing parameter value changes
 */

#pragma once
#include "utility.h"

namespace adsp
{
    /**
     * @brief One-pole filter for smoothing parameter value changes
     */
    class Smooth1P
    {
    public:
        Smooth1P() {}
        ~Smooth1P() {}

        //==============================================================================

        /**
         * @brief Reset, clear internal state and set new sample rate and smoothing time
         *
         * @param smoothingTime Timing constant [s]
         * @param sampleRate Sample rate
         */
        void reset(float smoothingTime, float sampleRate)
        {
            b1 = exp(-adsp::TWO_PI / (smoothingTime * sampleRate));
            a0 = 1.0f - b1;
            z1 = 0.0f;
        }

        //==============================================================================

        /**
         * @brief Process a single input sample
         *
         * @param x Input sample
         * @return Ouptut sample
         */
        inline double process(double x)
        {
            z1 = (x * a0) + (z1 * b1);
            return z1;
        }

    private:
        /**
         * @brief Numerator coefficient a0
         */
        double a0;

        /**
         * @brief Denominator coefficient b1
         */
        double b1;

        /**
         * @brief State register
         */
        double z1;
    };
} // namespace adsp