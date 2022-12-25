/*
  ==============================================================================
    LadderHp4Tpt.cpp

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

#include "LadderHp4Tpt.h"

namespace adsp
{
    LadderHp4Tpt::LadderHp4Tpt() {}
    LadderHp4Tpt::~LadderHp4Tpt() {}

    void LadderHp4Tpt::reset(double _sampleRate)
    {
        sampleRate = _sampleRate;
        for (int n = 0; n < 4; ++n)
        {
            hp[n].reset(_sampleRate);
        }
    }

    double LadderHp4Tpt::process(double x)
    {
        // Prewarp cutoff
        const double wd = adsp::TWO_PI * params.fc;
        const double wa = 2.0 * sampleRate * tan(wd / (2.0 * sampleRate));

        // Find signal going into the chain of low-pass filters

        const double g = wa / (2.0 * sampleRate);
        const double G = 1 / (1.0 + g);
        const double G4 = G * G * G * G;

        const double s1 = -1.0 * hp[0].getState();
        const double s2 = -1.0 * hp[1].getState();
        const double s3 = -1.0 * hp[2].getState();
        const double s4 = -1.0 * hp[3].getState();

        const double S = (G * G * G * s1 + G * G * s2 + G * s3 + s4) / (1.0 + g);

        double u = (x - k * S) / (1.0 + k * G4);

        // Process result through the filter chain to find the output
        for (int n = 0; n < 4; ++n)
        {
            u = hp[n].process(u);
        }

        return u;
    }

    //==============================================================================

    void LadderHp4Tpt::setSampleRate(double _sampleRate)
    {
        sampleRate = _sampleRate;

        // Update sample rate of the low-pass chain
        for (int n = 0; n < 4; ++n)
        {
            hp[n].setSampleRate(_sampleRate);
        }
    }

    LadderHp4TptParams LadderHp4Tpt::getParameters() { return params; }

    void LadderHp4Tpt::setParameters(const LadderHp4TptParams &parameters)
    {
        // Update parameters

        params = parameters;

        // Denormalize resonance parameter to find feedback gain coefficient
        k = 4.0 * params.res;

        // Update cutoff frequencies of the filter stages
        adsp::RcHp1TptParams lpParams = hp[0].getParameters();
        lpParams.fc = params.fc;
        for (int n = 0; n < 4; ++n)
        {
            hp[n].setParameters(lpParams);
        }
    }
} // namespace adsp