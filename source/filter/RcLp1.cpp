/*
  ==============================================================================
    RcLp1.cpp

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

#include "RcLp1.h"

namespace adsp {
RcLp1::RcLp1() {}
RcLp1::~RcLp1() {}

void RcLp1::reset(double sampleRate) {
    sampleRate = sampleRate;

    // Setup biquad object

    // Default implementation
    BiquadParams biquadParams = biquad.getParameters();
    biquadParams.calculationType = biquadAlgorithm::transposedCanonical;
    biquad.setParameters(biquadParams);

    // Clear biquad state array
    biquad.reset();
}

double RcLp1::process(double x) { return biquad.process(x); }

//==============================================================================

void RcLp1::setSampleRate(double sampleRate) {
    sampleRate = sampleRate;

    // Calculate new coefficients
    calculateFilterCoefficients();
}

RcLp1Params RcLp1::getParameters() { return params; }

void RcLp1::setParameters(const RcLp1Params &parameters) {
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

void RcLp1::calculateFilterCoefficients() {
    // Clear coefficient array
    memset(&coefficientsArray[0], 0, sizeof(double) * numCoefficients);

    // Calculate coefficients
    const double w0 = 2.0 * PI * params.fc;
    const double gamma = 1.0 / sampleRate * w0;

    // Update coefficient array
    coefficientsArray[a0] = gamma / (gamma + 2.0);
    coefficientsArray[a1] = gamma / (gamma + 2.0);
    coefficientsArray[a2] = 0.0;
    coefficientsArray[b1] = (gamma - 2.0) / (gamma + 2.0);
    coefficientsArray[b2] = 0.0;

    biquad.setCoefficients(coefficientsArray);
}
}  // namespace adsp