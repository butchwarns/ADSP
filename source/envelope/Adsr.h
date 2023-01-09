/*
  ==============================================================================
    ADSR.h

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
 * @file Adsr.h
 *
 * @brief ADSR envelope
 */

#pragma once

#include "../utility/utility.h"

namespace adsp
{
    /**
     * @brief ADSR parameter structure
     *
     */
    struct AdsrParams
    {
        AdsrParams(){};

        AdsrParams &operator=(const AdsrParams &parameters)
        {
            if (this == &parameters)
            {
                return *this;
            }
            else
            {
                attack = parameters.attack;
                decay = parameters.decay;
                sustain = parameters.sustain;
                release = parameters.release;
                return *this;
            }
        }

        // Default Parameters

        double attack = 0.25; // [s]
        double decay = 0.5;   // [s]
        double sustain = 0.4; // [1]
        double release = 1.5; // [s]
    };

    /**
     * @brief Possible envelope states
     */
    enum class adsrStates
    {
        IDLE,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE,
    };

    /**
     * @brief ADSR evelope
     *
     * Analog modeled
     */
    class Adsr
    {
    public:
        Adsr();
        ~Adsr();

        //==============================================================================

        /**
         * @brief Set new sample rate and reset internal state
         *
         * @param _sampleRate New sample rate
         */
        void reset(double _sampleRate);

        //==============================================================================

        /**
         * @brief Get parameters
         *
         * @return ADSR parameters
         */
        AdsrParams getParameters();

        /**
         * @brief Set parameters
         *
         * @param New ADSR parameters
         */
        void setParameters(const AdsrParams &parameters);

        //=============================================================================

        void setGate(bool _gate);

        //==============================================================================

        /**
         * @brief Generate next output sample
         *
         * @return Output sample
         */
        double render();

    protected:
        /**
         * @brief Sample rate
         */
        double sampleRate{48000.0};

        /**
         * @brief ADSR parameters
         */
        AdsrParams parameters;

        //==============================================================================

        /**
         * @brief Peak voltage of envelope signal
         */
        const double PEAK_ENV_VOLTAGE{8.0};

        /**
         * @brief Maximum output voltage of Eurorack system
         */
        const double MAX_VOLTAGE{12.0};

        /**
         * @brief Truncation margin for state transitions
         */
        const double VOLTAGE_MARGIN{0.00001};

        //==============================================================================

        /**
         * @brief Current state of the envelope
         */
        adsrStates state{adsrStates::IDLE};

        /**
         * @brief Envelope gate signal
         */
        bool gate{false};

        /**
         * @brief Previous output sample
         */
        double prevOutput{0.0};

        /**
         * @brief Time constant of current segment
         */
        double time{0.0};

        /**
         * @brief Start value for linear segments
         */
        double segStart{0.0};

        /**
         * @brief Counter for the linear segments
         */
        double counter{0.0};

        /**
         * @brief Increment per sample for the linear segments
         */
        double delta{0.0};

        /**
         * @brief Update delta to match new timing parameters
         *
         * @param t Target rise time of normalized envelope segment
         */
        void recalculateDelta();

        /**
         * @brief Transition state conditionally
         */
        void transitionState();
    };
} // namespace adsp