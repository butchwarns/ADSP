/*
  ==============================================================================
    LadderLp4Tpt.cpp

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

#include "LadderLp4Tpt.h"

namespace adsp
{
    LadderLp4Tpt::LadderLp4Tpt() {}
    LadderLp4Tpt::~LadderLp4Tpt() {}

    void LadderLp4Tpt::reset(double _sampleRate)
    {
        sampleRate = _sampleRate;
        for (int n = 0; n < 4; ++n)
        {
            lp[n].reset(_sampleRate);
        }
    }

    double LadderLp4Tpt::process(double x)
    {
        // Prewarp cutoff
        const double wd = adsp::TWO_PI * params.fc;
        const double wa = 2.0 * sampleRate * tan(wd / (2.0 * sampleRate));

        // Find signal going into the chain of low-pass filters

        const double g = wa / (2.0 * sampleRate);
        const double G = g / (1.0 + g);
        const double G4 = G * G * G * G;

        const double s1 = lp[0].getState();
        const double s2 = lp[1].getState();
        const double s3 = lp[2].getState();
        const double s4 = lp[3].getState();

        const double S = (G * G * G * s1 + G * G * s2 + G * s3 + s4) / (1.0 + g);

        double u = (x - k * S) / (1.0 + k * G4);

        // Process result through the filter chain to find the output
        for (int n = 0; n < 4; ++n)
        {
            u = lp[n].process(u);
        }

        return u;
    }

    //==============================================================================

    void LadderLp4Tpt::setSampleRate(double _sampleRate)
    {
        sampleRate = _sampleRate;

        // Update sample rate of the low-pass chain
        for (int n = 0; n < 4; ++n)
        {
            lp[n].setSampleRate(_sampleRate);
        }
    }

    LadderLp4TptParams LadderLp4Tpt::getParameters() { return params; }

    void LadderLp4Tpt::setParameters(const LadderLp4TptParams &parameters)
    {
        // Update parameters

        params = parameters;

        // Denormalize resonance parameter to find feedback gain coefficient
        k = 4.0 * params.res;

        // Update cutoff frequencies of the filter stages
        adsp::RcLp1TptParams lpParams = lp[0].getParameters();
        lpParams.fc = params.fc;
        for (int n = 0; n < 4; ++n)
        {
            lp[n].setParameters(lpParams);
        }
    }
} // namespace adsp