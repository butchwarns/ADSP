/*
  ==============================================================================
    RcHp1Tpt.cpp

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

#include "RcHp1Tpt.h"

namespace adsp
{
    RcHp1Tpt::RcHp1Tpt() {}
    RcHp1Tpt::~RcHp1Tpt() {}

    void RcHp1Tpt::reset(double _sampleRate)
    {
        sampleRate = _sampleRate;
        state = 0.0;
    }

    double RcHp1Tpt::process(double x)
    {
        const double y = G * (x - state);
        state = state + g2 * y;

        return y;
    }

    //==============================================================================

    void RcHp1Tpt::setSampleRate(double _sampleRate)
    {
        sampleRate = _sampleRate;
    }

    RcHp1TptParams RcHp1Tpt::getParameters() { return params; }

    void RcHp1Tpt::setParameters(const RcHp1TptParams &parameters)
    {
        // Update the parameters
        params = parameters;

        // Prewarp cutoff
        const double wd = adsp::TWO_PI * params.fc;
        const double wa = 2.0 * sampleRate * tan(wd * 1.0 / (2.0 * sampleRate));
        const double g = wa / (2.0 * sampleRate);
        G = 1.0 / (1.0 + g);
        g2 = 2.0 * g;
    }

    //==============================================================================

    double RcHp1Tpt::getState()
    {
        return state;
    }

} // namespace adsp