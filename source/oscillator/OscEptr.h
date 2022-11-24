/*
  ==============================================================================
    OscEptr.h

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
* @file OscEptr.h
*
* @brief Oscillator based on the "Efficient Polynomial Transition Region Algorithm"
*/

#pragma once

#include <math.h>
#include <stdint.h>

namespace adsp {
/**
* @brief Simultaneous output of all waveforms (single sample)
*/
struct OscEptrOut {
    /**
     * @brief Sawtooth waveform
     */
    double saw;

    /**
     * @brief Triangle wave
     * 
     * Can be shaped continuously into a sawtooth or reverse sawtooth depending on pulse width parameter.  
     * 
     */
    double tri;
    double pulse;
};

/**
* @brief OscEptr parameter structure
*/
struct OscEptrParams {
    OscEptrParams(){};

    OscEptrParams &operator=(const OscEptrParams &params) {
        if (this == &params) {
            return *this;
        } else {
            // Update parameters
            frequency = params.frequency;
            pulseWidth = params.pulseWidth;

            return *this;
        }
    }

    /**
    * @brief Frequency [Hz]
    */
    double frequency = 440.0;

    /**
     * @brief Pulse width in range [0.01, 0.99]
     */
    double pulseWidth = 0.5;
};

/**
 * @brief Current direction of the output gradient, used in waveform calculations
 */
enum class oscDirection {
    UP,
    DOWN,
};

/*
 * @brief Enum to index correction coefficients array of EPTR oscillator
 */
enum eptrCorrCoeffs { v0, v1, v2, w0, w1, w2, numCoeffs };

/**
 * @brief Oscillator based on the "Efficient Polynomial Transition Region Algorithm" (EPTR) [Amrits and Bank 2013]
 * 
 * Basically, a more efficient way to implement first-order "Differentialted Polynomial Wave" [Valimaki et al. 2010].
 */
class OscEptr {
   public:
    OscEptr();
    ~OscEptr();

    //==============================================================================

    /**
     * @brief Reinitialize oscillator with new sample rate
     * 
     * @param sampleRate New sample rate
     */
    void reset(double sampleRate);

    //==============================================================================

    /**
    * @brief Get oscillator parameters
    * 
    * @return Parameters 
    */
    OscEptrParams getParams();

    /**
    * @brief Set parameters
    * 
    * @param _params New parameters
    */
    void setParams(const OscEptrParams &_params);

    //==============================================================================

    /**
    * @brief Render output (single sample per waveform)
    * 
    * @return Next oscillator output
    */
    OscEptrOut renderOutputs();

   protected:
    /**
    * @brief Oscillator parameters
    */
    OscEptrParams params;

    /**
     * @brief Single cycle period length 
     */
    double period{0.0};

    /**
     * @brief Internal sample rate
     */
    double sampleRate{48000.0};

    /**
     * @brief Internal sampling interval
     */
    double T = {1.0 / 48000.0};

    /**
     * @brief Modulo counter for sawtooth generation
     */
    double counterSaw{0.0};

    /**
     * @brief Modulo counter for triangle generation
     */
    double counterTri{0.0};

    /**
     * @brief Counter step size for set oscillator frequency
     */
    double delta{0.0};

    /**
     * @brief Current direction for asymmetric triangle wave generation
     */
    oscDirection dir{oscDirection::UP};

    /**
     * @brief Gradient of the rising part of the generated asymmetric triangle wave
     */
    double gradientUp{1.0};

    /**
     * @brief Gradient of the falling part of the generated asymmetric triangle wave
     */
    double gradientDown{1.0};

    // Correction coefficients for asymmetric triangle
    double corrCoeffs[eptrCorrCoeffs::numCoeffs];

    // State variable for first order difference to generate pulse wave
    double differenceState{0.0};

    //==============================================================================

    // Function used to correct SAW output at the discontinuities
    double correctCounterSaw();

    // Functions to correct asymmetric TRI output
    double correctCounterTriMin();
    double correctCounterTriMax();
};
}  // namespace adsp