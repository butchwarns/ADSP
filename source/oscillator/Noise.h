/*
  ==============================================================================
    Noise.h

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
 * @file Noise.h
 *
 * @brief Pseudorandom experimental noise generator
 */

#pragma once

#include <cmath>

#include "../utility/utility.h"

namespace adsp
{
    /**
     * @brief Pseudorandom experimental noise generator
     * 
     * Noise generator presented on musicdsp.com
     * https://www.musicdsp.org/en/latest/Synthesis/217-easy-noise-generation.html
     */
    class Noise
    {
    public:
        Noise();
        ~Noise();

        //==============================================================================

        /**
         * @brief Render output sample
         *
         * @return Output sample
         */
        double render();

    private:
        double b_noise = 19.1919191919191919191919191919191919191919;
    };
} // namespace adsp