/*
  ==============================================================================
    ADSR.cpp

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

#include "Adsr.h"

namespace adsp
{
    Adsr::Adsr() {}
    Adsr::~Adsr() {}

    void Adsr::reset(double _sampleRate)
    {
        sampleRate = _sampleRate;

        counter = 0.0;
        delta = 0.0;
        prevOutput = 0.0;
    }

    //==============================================================================

    AdsrParams Adsr::getParameters() { return parameters; }

    void Adsr::setParameters(const AdsrParams &_parameters)
    {
        parameters = _parameters;
    }

    //=============================================================================

    void Adsr::setGate(bool _gate)
    {
        gate = _gate;
    }

    //==============================================================================

    void Adsr::recalculateDelta()
    {
        double timeAdj = time;

        // Adjust time to compensate the curve "aiming higher" in ATTACK
        if (state == adsrStates::ATTACK)
        {
            timeAdj = 12.0 / 8.0 * time;
        }
        delta = 1 / (timeAdj * sampleRate);
    }

    void Adsr::transitionState()
    {
        switch (state)
        {
        case adsrStates::IDLE:
            // If gated in IDLE..
            if (gate)
            {
                // Change to ATTACK phase
                state = adsrStates::ATTACK;
                // Set new timing constant
                time = parameters.attack;
                // Update increment
                recalculateDelta();
                // Reset counter
                counter = 0.0;
                // Start ATTACK segment from zero
                segStart = 0.0;
            }
            break;

        case adsrStates::ATTACK:
            // If no longer gated..
            if (!gate)
            {
                // Change to RELEASE phase
                state = adsrStates::RELEASE;
                // Set new timing constant
                time = parameters.release;
                // Update increment
                recalculateDelta();
                // Reset counter
                counter = 0.0;
                // Start RELEASE segment from previous output value
                segStart = prevOutput;
            }
            // If gated and ATTACK phase is over..
            else if (prevOutput >= (PEAK_ENV_VOLTAGE - VOLTAGE_MARGIN))
            {
                // Change to DECAY phase
                state = adsrStates::DECAY;
                // Set new timing constant
                time = parameters.decay;
                // Update increment
                recalculateDelta();
                // Reset counter
                counter = 0.0;
            }
            break;

        case adsrStates::DECAY:
            // If no longer gated..
            if (!gate)
            {
                // Change to RELEASE phase
                state = adsrStates::RELEASE;
                // Set new timing constant
                time = parameters.release;
                // Update increment
                recalculateDelta();
                // Reset counter
                counter = 0.0;
                // Start RELEASE segment from previous output value
                segStart = prevOutput;
            }
            // If gated and DECAY phase is over..
            else if (prevOutput <= (parameters.sustain * PEAK_ENV_VOLTAGE + VOLTAGE_MARGIN))
            {
                // Change to SUSTAIN phase
                state = adsrStates::SUSTAIN;
            }
            break;

        case adsrStates::SUSTAIN:
            // If no longer gated..
            if (!gate)
            {
                // Change to RELEASE phase
                state = adsrStates::RELEASE;
                // Set new timing constant
                time = parameters.release;
                // Update increment
                recalculateDelta();
                // Reset counter
                counter = 0.0;
                // Start RELEASE segment from previous output value
                segStart = prevOutput;
            }
            break;

        case adsrStates::RELEASE:
            // If envelope is gated in RELEASE..
            if (gate)
            {
                // Change to ATTACK phase
                state = adsrStates::ATTACK;
                // Set new timing constant
                time = parameters.attack;
                // Update increment
                recalculateDelta();
                // Reset counter
                counter = 0.0;
                // Start ATTACK from current output value
                segStart = prevOutput;
            }
            // If RELEASE phase is over..
            else if (prevOutput <= (0.0 + VOLTAGE_MARGIN))
            {
                // Change back to IDLE phase
                state = adsrStates::IDLE;
            }
            break;
        }
    }

    double Adsr::render()
    {
        double output{0.0};

        // Find next output
        switch (state)
        {
        case adsrStates::IDLE:
            output = 0.0;
            break;

        case adsrStates::ATTACK:
            // Increment counter
            counter += delta;
            // Scale to desired linear segment
            // ATTACK "aims higher" to make the curve more linear, like in the analog circuit
            // 12 V is given by the Eurorack power specs (envelope output peaks at 8 V)
            output = adsp::skewNormalized(counter, 0.5);
            output = adsp::linMapNorm(output, segStart, 12.0);
            break;

        case adsrStates::DECAY:
            // Decrement counter
            counter += delta;
            // Scale to desired linear segment
            output = adsp::skewNormalized(counter, 2);
            output = adsp::linMapNorm(output, PEAK_ENV_VOLTAGE, parameters.sustain * PEAK_ENV_VOLTAGE);
            break;

        case adsrStates::SUSTAIN:
            output = parameters.sustain * PEAK_ENV_VOLTAGE;
            break;

        case adsrStates::RELEASE:
            // Decrement counter
            counter += delta;
            // Scale to desired linear segment
        output = adsp::skewNormalized(counter, 2);
            output = adsp::linMapNorm(output, segStart, 0.0);
            break;
        }

        // Check for state transitions
        transitionState();

        // Save output for future calculations
        prevOutput = output;

        return output;
    }
} // namespace adsp