/*
  ==============================================================================
    RcLp1.h

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

#pragma once

#include "Biquad.h"

namespace adsp {
/**
* @brief RC low-pass parameter structure
* 
*/
struct RcLp1Params {
    RcLp1Params(){};

    RcLp1Params &operator=(const RcLp1Params &parameters) {
        if (this == &parameters) {
            return *this;
        } else {
            fc = parameters.fc;
            return *this;
        }
    }

    // Cutoff frequency
    double fc = 100.0;  // Hz
};

/**
* @brief RC low-pass filter
* 
* First order RC low-pass filter.  
* Analog modelled by means of a prewarped bilinear transformation.  
* Prewarping was chosen to match cutoff frequencies.  
* This filter has not been decramped.  
*/
class RcLp1 {
   public:
    RcLp1();
    ~RcLp1();

    //==============================================================================

    /**
    * @brief Reset to default parameters, clear internal state and set sample rate
    * 
    * @param sampleRate New sample rate
    */
    void reset(double sampleRate);

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
    * @param sampleRate 
    */
    void setSampleRate(double sampleRate);

    /**
    * @brief Get parameters
    * 
    * @return Filter parameters 
    */
    RcLp1Params getParameters();

    /**
     * @brief Set parameters
     * 
     * @param New filter parameters
     */
    void setParameters(const RcLp1Params &parameters);

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
    RcLp1Params params;

    /**
    * @brief Recalculate coefficients, is called when filter parameters change
    */
    void calculateFilterCoefficients();
};
}  // namespace adsp