/*
  ==============================================================================
    ApLp1.cpp

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

#include "ApLp1.h"

namespace adsp {
ApLp1::ApLp1() {}
ApLp1::~ApLp1() {}

void ApLp1::reset(double sampleRate) {
    sampleRate = sampleRate;

    // Setup biquad object

    // Default implementation
    BiquadParams biquadParams = biquad.getParameters();
    biquadParams.calculationType = biquadAlgorithm::transposedCanonical;
    biquad.setParameters(biquadParams);

    // Clear biquad state array
    biquad.reset();
}

double ApLp1::process(double x) { return biquad.process(x); }

//==============================================================================

void ApLp1::setSampleRate(double sampleRate) {
    sampleRate = sampleRate;

    // Calculate new coefficients
    calculateFilterCoefficients();
}

ApLp1Params ApLp1::getParameters() { return params; }

void ApLp1::setParameters(const ApLp1Params &parameters) {
    // If new parameters differ..
    if (params.fc != parameters.fc) {
        // Update the parameters
        params = parameters;

        // Calculate coefficients with new parameters
        calculateFilterCoefficients();
    } else {
        // Otherwise do nothing
        return;
    }
}

void ApLp1::calculateFilterCoefficients() {
    // Clear coefficient array
    memset(&coefficientsArray[0], 0, sizeof(double) * numCoefficients);

    // Calculate coefficients
    const double w0 = 2.0 * PI * params.fc;

    const double a = 1.0 / sampleRate * w0;
    const double a0b1 = (a - 2.0) / (a + 2.0);

    // Update coefficient array
    coefficientsArray[a0] = a0b1;
    coefficientsArray[a1] = 1;
    coefficientsArray[a2] = 0.0;
    coefficientsArray[b1] = a0b1;
    coefficientsArray[b2] = 0.0;

    biquad.setCoefficients(coefficientsArray);
}
}  // namespace adsp