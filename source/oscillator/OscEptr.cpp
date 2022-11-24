/*
  ==============================================================================
    OscEptr.cpp

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

#include "OscEptr.h"

namespace adsp {
OscEptr::OscEptr() {}
OscEptr::~OscEptr() {}

void OscEptr::reset(double _sampleRate) {
    sampleRate = _sampleRate;
    T = 1.0 / _sampleRate;

    counterSaw = 0.0;
    counterTri = 0.0;
    delta = 0.0;

    dir = oscDirection::UP;

    differenceState = 0.0;
}

//==============================================================================

OscEptrParams OscEptr::getParams() { return params; }

void OscEptr::setParams(const OscEptrParams &_params) {
    // Only if new params differ..
    if (params.frequency != _params.frequency ||
        params.pulseWidth != _params.pulseWidth) {
        // Update params
        params = _params;

        // Calculate delta according to new frequency
        delta = _params.frequency * T;

        // Calculate period length
        period = 1.0 / params.frequency;

        // Find gradients for asymmetric triangle
        // gradients of 2.0 give regular triangle wave
        gradientUp = 1.0 / params.pulseWidth;
        gradientDown = -gradientUp / (gradientUp - 1.0);

        // Correction coefficients for asymmetric TRI
        corrCoeffs[eptrCorrCoeffs::a0] =
            -1.0 * ((gradientUp * delta - 1.0) * (gradientUp * delta - 1.0)) /
            (4.0 * delta * (gradientUp - 1.0));
        corrCoeffs[eptrCorrCoeffs::a1] =
            (2.0 * gradientUp * delta - 4.0 * delta + 2.0) /
            (4.0 * delta * (gradientUp - 1.0));
        corrCoeffs[eptrCorrCoeffs::a2] =
            -1.0 / (4.0 * delta * (gradientUp - 1.0));

        corrCoeffs[eptrCorrCoeffs::b0] = -1.0 * (gradientDown * delta + 1.0) *
                                         (gradientDown * delta + 1.0) /
                                         (4.0 * delta * (gradientDown + 1.0));
        corrCoeffs[eptrCorrCoeffs::b1] =
            (2.0 * gradientDown * delta + 4.0 * delta - 2.0) /
            (4.0 * delta * (gradientDown + 1.0));
        corrCoeffs[eptrCorrCoeffs::b2] =
            -1.0 / (4.0 * delta * (gradientDown + 1.0));
    }
}

//==============================================================================

double OscEptr::correctCounterSaw() {
    return counterSaw - (counterSaw / delta) + (1.0 / delta) - 1.0;
}

//==============================================================================

double OscEptr::correctCounterTriMin() {
    return corrCoeffs[eptrCorrCoeffs::b2] * counterTri * counterTri +
           corrCoeffs[eptrCorrCoeffs::b1] * counterTri +
           corrCoeffs[eptrCorrCoeffs::b0];
}

double OscEptr::correctCounterTriMax() {
    return corrCoeffs[eptrCorrCoeffs::a2] * counterTri * counterTri +
           corrCoeffs[eptrCorrCoeffs::a1] * counterTri +
           corrCoeffs[eptrCorrCoeffs::a0];
}

OscEptrOut OscEptr::renderOutputs() {
    //==============================================================================
    // SAW

    double saw{0.0};

    counterSaw += 2.0 * delta;
    if (counterSaw > (1.0 - delta)) {
        saw = correctCounterSaw();
        counterSaw -= 2.0;
    } else {
        saw = counterSaw;
    }

    //==============================================================================
    // TRIANGLE

    double tri{0.0};

    // Upwards
    if (dir == oscDirection::UP) {
        counterTri += 2.0 * gradientUp * delta;
        // Transition
        if (counterTri > 1.0 - gradientUp * delta) {
            tri = correctCounterTriMax();
            counterTri = 1.0 + (counterTri - 1.0) * gradientDown / gradientUp;
            // Reverse direction
            dir = oscDirection::DOWN;
        }
        // Linear section
        else {
            tri = counterTri;
        }
    }
    // Downwards
    else {
        counterTri += 2.0 * gradientDown * delta;
        // Transition
        if (counterTri < -1.0 - gradientDown * delta) {
            tri = correctCounterTriMin();
            counterTri = -1.0 + (counterTri + 1.0) * gradientUp / gradientDown;
            // Reverse direction
            dir = oscDirection::UP;
        }
        // Linear section
        else {
            tri = counterTri;
        }
    }

    //==============================================================================
    // PULSE

    double pulse = 0.0;

    // Upper half wave (triangle going upwards)
    if (dir == oscDirection::UP) {
        // Transition region
        if (tri > 1.0 - 2.0 * gradientUp * T) {
            // Correct discontinuity like in the saw case
            pulse = correctCounterSaw();
        } else {
            pulse = 1.0;
        }
    }
    // Lower half wave (triangle goind downwards)
    else {
        // Transition region
        if (tri < -1.0 + 2.0 * gradientDown * T) {
            // Correct discontinuity like in the saw case (use symmetry)
            pulse = -1.0 * correctCounterSaw();
        } else {
            pulse = -1.0;
        }
    }

    //==============================================================================
    // Compensate for dc offset resulting from changing pulse width

    // Pulse wave offset compensation
    const double pulseOffset =
        (params.pulseWidth - (1.0 - params.pulseWidth)) * period;
    pulse -= pulseOffset;

    // Triangle wave offset comepnsation
    const double triOffset = pulseOffset * 0.5;
    tri -= triOffset;

    //==============================================================================
    // Output

    OscEptrOut output;
    output.saw = saw;
    output.tri = tri;
    output.pulse = pulse;

    return output;
}
}  // namespace adsp
