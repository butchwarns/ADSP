/*
  ==============================================================================
    Biquad.cpp

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