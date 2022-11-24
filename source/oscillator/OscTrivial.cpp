/*
  ==============================================================================
    OscTrivial.cpp

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

#include "OscTrivial.h"

namespace adsp {
OscTrivial::OscTrivial() {}
OscTrivial::~OscTrivial() {}

void OscTrivial::reset(double _sampleRate) {
    sampleRate = _sampleRate;
    T = 1.0 / _sampleRate;

    counter = 0.0;
    delta = 0.0;
    counterTri = 0.5;
}

//==============================================================================

OscTrivialParams OscTrivial::getParams() { return params; }

void OscTrivial::setParams(const OscTrivialParams &_params) {
    // Update params
    params = _params;

    // Calculate delta according to new frequency
    delta = _params.frequency * T;
}

//==============================================================================

OscTrivialOut OscTrivial::renderOutputs() {
    //==============================================================================
    // SAW

    // Bipolar modulo counter
    counter += delta;
    wrap(counter);
    const double saw = unipolarToBipolar(counter);

    //==============================================================================
    // TRIANGLE

    // Rectify, scale and offset saw to create triangle
    // Offset counter to align phases between waveforms
    counterTri += delta;
    wrap(counterTri);
    const double sawTri = unipolarToBipolar(counterTri);
    const double tri = unipolarToBipolar(fabs(sawTri));

    //==============================================================================
    // PULSE

    const double pulse = (saw < 0.0) ? 1.0 : -1.0;

    //==============================================================================
    // Output

    OscTrivialOut output;
    output.saw = saw;
    output.tri = tri;
    output.pulse = pulse;

    return output;
}

void OscTrivial::retrigger() {
    // Reset counter to initial phase angle
    counter = params.phase;

    // Set triangle counter to correct initial phase (with alignment to other waveforms)
    double phaseTri = params.phase + 0.5;
    wrap(phaseTri);
    counterTri = phaseTri;
}

void OscTrivial::wrap(double &counter) {
    if (counter >= 1.0) {
        counter -= 1.0;
    }
}
}  // namespace adsp
