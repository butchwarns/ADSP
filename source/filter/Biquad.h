/*
  ==============================================================================
    Biquad.h

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