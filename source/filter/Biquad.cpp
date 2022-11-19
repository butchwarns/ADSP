/*
  ==============================================================================
    Biquad.cpp

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

#include "Biquad.h"

namespace adsp {
Biquad::Biquad() {}
Biquad::~Biquad() {}

//==============================================================================

void Biquad::reset() {
    memset(&stateArray[0], 0, sizeof(double) * numRegisters);
}

double Biquad::process(double x) {
    switch (parameters.calculationType) {
        // Direct form
        case biquadAlgorithm::direct: {
            // y[n] = a0*x[n] + a1*x[n-1] + a2*x[n-2] - b1*y[n-1] - b2*y[n-2]
            double y = coefficientsArray[a0] * x +
                       coefficientsArray[a1] * stateArray[x_z1] +
                       coefficientsArray[a2] * stateArray[x_z2] -
                       coefficientsArray[b1] * stateArray[y_z1] -
                       coefficientsArray[b2] * stateArray[y_z2];

            fixUnderflow(y);

            // Update state registers
            stateArray[x_z2] = stateArray[x_z1];
            stateArray[x_z1] = x;

            stateArray[y_z2] = stateArray[y_z1];
            stateArray[y_z1] = y;

            // Output
            return y;
        }

        // Canonical form, uses only two state registers
        case biquadAlgorithm::canonical: {
            // w[n] = x[n] - b1*w[n-1] - b2*w[n-2]
            double w = x - coefficientsArray[b1] * stateArray[x_z1] -
                       coefficientsArray[b2] * stateArray[x_z2];

            // y[n] = a0*w[n] + a1*w[n-1] + a2*w[n-2]
            double y = coefficientsArray[a0] * w +
                       coefficientsArray[a1] * stateArray[x_z1] +
                       coefficientsArray[a2] * stateArray[x_z2];

            fixUnderflow(y);

            // Update state registers
            stateArray[x_z2] = stateArray[x_z1];
            stateArray[x_z1] = w;

            // Output
            return y;
        }

        // Transposed direct form
        case biquadAlgorithm::transposedDirect: {
            // w[n] =  x[n] + stateArray[y_z1]
            double w = x + stateArray[y_z1];
            // y[n] = a0*w[n] + stateArray[x_z1]
            double y = coefficientsArray[a0] * w + stateArray[x_z1];

            fixUnderflow(y);

            // Update state registers
            stateArray[y_z1] = stateArray[y_z2] - coefficientsArray[b1] * w;
            stateArray[y_z2] = -coefficientsArray[b2] * w;

            stateArray[x_z1] = stateArray[x_z2] + coefficientsArray[a1] * w;
            stateArray[x_z2] = coefficientsArray[a2] * w;

            // Output
            return y;
        }

        // Transposed canonical form
        case biquadAlgorithm::transposedCanonical: {
            // y[n] = a0*x[n] + stateArray[x_z1]
            double y = coefficientsArray[a0] * x + stateArray[x_z1];

            fixUnderflow(y);

            // Update state registers
            stateArray[x_z1] = coefficientsArray[a1] * x -
                               coefficientsArray[b1] * y + stateArray[x_z2];

            stateArray[x_z2] =
                coefficientsArray[a2] * x - coefficientsArray[b2] * y;

            // Output
            return y;
        }

        default: {
            return x;  // Did not process sample
        }
    }
}

//==============================================================================

BiquadParams Biquad::getParameters() { return parameters; }

void Biquad::setParameters(BiquadParams &_parameters) {
    parameters = _parameters;
}

void Biquad::setCoefficients(double *coefficients) {
    memcpy(&coefficientsArray[0], &coefficients[0],
           sizeof(double) * numCoefficients);
}

double *Biquad::getCoefficients() { return &coefficientsArray[0]; }

double *Biquad::getStateArray() { return &stateArray[0]; }
}  // namespace adsp