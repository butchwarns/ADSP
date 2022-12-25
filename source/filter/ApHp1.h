/*
  ==============================================================================
    ApHp1.h

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
* @file ApHp1.h
*
* @brief All-pass filter based on the RC high-pass, first-order
*/

#pragma once

#include "Biquad.h"

namespace adsp {
/**
* @brief ApHp1 parameter structure
* 
*/
struct ApHp1Params {
    ApHp1Params(){};

    ApHp1Params &operator=(const ApHp1Params &parameters) {
        if (this == &parameters) {
            return *this;
        } else {
            fc = parameters.fc;
            return *this;
        }
    }

    // Cutoff frequency
    double fc = 4000.0;  // Hz
};

/**
* @brief All-pass filter based on the RC high-pass, first-order 
* 
* Analog modeled by means of a prewarped bilinear transformation.  
* Prewarping was chosen to match center frequencies.  
* This filter has not been decramped.  
*/
class ApHp1 {
   public:
    ApHp1();
    ~ApHp1();

    //==============================================================================

    /**
    * @brief Reset to default parameters, clear internal state and set sample rate
    * 
    * @param _sampleRate New sample rate
    */
    void reset(double _sampleRate);

    /**
    * @brief Proess a single sample
    * 
    * @param x Input sample
    * @return Output sample 
    */
    double process(double x);

    //==============================================================================

    /**
    * @brief Set sample rate, recalculate coefficients
    * 
    * @param _sampleRate 
    */
    void setSampleRate(double _sampleRate);

    /**
    * @brief Get parameters
    * 
    * @return Filter parameters 
    */
    ApHp1Params getParameters();

    /**
    * @brief Set parameters
    * 
    * @param New filter parameters
    */
    void setParameters(const ApHp1Params &parameters);

   protected:
    /**
    * @brief Sample rate
    */
    double sampleRate{48000.0};

    /**
    * @brief  Object implementing the difference equation
    */
    Biquad biquad;

    /**
    * @brief Filter coefficients
    */
    double coefficientsArray[numCoefficients] = {0.0, 0.0, 0.0, 0.0, 0.0};

    /**
    * @brief Filter parameters
    */
    ApHp1Params params;

    /**
    * @brief Recalculate coefficients, is called when filter parameters change
    */
    void calculateFilterCoefficients();
};
}  // namespace adsp