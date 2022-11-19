/*
  ==============================================================================
    Biquad.h

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

/**
* @file Biquad.h
* 
* @brief Biquadratic filter stage
*/

#pragma once

#include <cmath>
#include <cstring>

#include "../utility/utility.h"

namespace adsp {
/**
* @brief Second-order filter coefficients
* 
* For use with an array of filter coefficients.  
*
* @f$ ax @f$ are the numerator coefficients (feed-forward)  
* @f$ bx @f$ are the denominator coefficients (feed-back)  
*
* @f$ H(z) = (a_0 + a_1 z^{-1} + a2 z^{-2}) / (1 + b_1 z^{-1} + b2 z^{-2}) @f$  
*
*/
enum filterCoefficients { a0, a1, a2, b1, b2, numCoefficients };

/**
* @brief State registers for a second-order filter (only two needed for canonical forms)
*
* For use with an array of state registers.
*/
enum stateRegisters { x_z1, x_z2, y_z1, y_z2, numRegisters };

/**
* @brief Different structures (algorithms) implementing the second-order difference equation
*/
enum class biquadAlgorithm {
    direct,
    canonical,
    transposedDirect,
    transposedCanonical
};

/**
* @brief Biquad parameter structure
* 
*/
struct BiquadParams {
    BiquadParams() {}

    BiquadParams &operator=(const BiquadParams &parameters) {
        if (this == &parameters) {
            return *this;
        } else {
            calculationType = parameters.calculationType;
            return *this;
        }
    }

    // Currently selected algorithm for the second-order difference equation
    biquadAlgorithm calculationType = biquadAlgorithm::direct;
};

//==============================================================================

/**
* @brief Biquadratic filter
* 
* Second-order structure to filter input signals given a set of filter coefficients.  
* Different algorithms implementing the difference equation can be chosen.  
* Higher order filters are usually built up from multiple biquad stages.  
*/
class Biquad {
   public:
    Biquad();
    ~Biquad();

    //==============================================================================

    /**
    * @brief Sets all state registers to zero  
    * 
    */
    void reset();

    /**
    * @brief Process a single sample   
    * 
    * Uses difference equation implemented by the selected algorithm  
    * 
    * @param x Input sample
    * @return Output sample 
    */
    double process(double x);

    //==============================================================================

    /**
    * @brief Get the biquad parameters
    * 
    * @return Parameters 
    */
    BiquadParams getParameters();

    /**
    * @brief Set new Parameters
    * 
    * @param _parameters New biquad parameters
    */
    void setParameters(BiquadParams &_parameters);

    /**
    * @brief Set new coefficients
    * 
    * @param coefficients Array of filter coefficients
    */
    void setCoefficients(double *coefficients);

    /**
    * @brief Get current coefficients
    * 
    * @return Array of coefficients
    */
    double *getCoefficients();

    /**
    * @brief Get current state array
    * 
    * @return State array
    */
    double *getStateArray();

    //==============================================================================

   protected:
    /**
     * @brief Array of filter coefficients
     */
    double coefficientsArray[numCoefficients] = {0.0, 0.0, 0.0, 0.0, 0.0};

    /**
     * @brief State array
     */
    double stateArray[numRegisters] = {0.0, 0.0, 0.0, 0.0};

    /**
     * @brief Biquad parameters
     */
    BiquadParams parameters;
};
}  // namespace adsp