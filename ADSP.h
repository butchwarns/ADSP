/*
  ==============================================================================
    ADSP.h

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

/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.md file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 ADSP
  vendor:             Butch Warns
  version:            0.1.0
  name:               ADSP
  description:        Objects and functions for audio signal processing and synthesis.
  website:            http://butchwarns.de/
  license:            LICENSE
  minimumCppStandard: 17

  dependencies:       

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

#pragma once
#define ADSP_H_INCLUDED

#include "source/filter/Biquad.h"
#include "source/utility/utility.h"
#include "source/filter/RcLp1.h"
