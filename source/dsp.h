/*
  ==============================================================================
    LICENSE
  ==============================================================================
*/

#pragma once

#include <cmath>

namespace adsp
{
    //==============================================================================

    const double PI = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899;
    const double TWO_PI = 2.0 * 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899;
    const double FRAC_ONE_PI = 1 / PI; // 1 / PI

    const double SQRT_TWO = pow(2.0, 0.5);
    const double FRAC_ONE_SQRT_TWO = pow(2.0, -0.5); // 1 / sqrt(2)

    const double SMALLEST_POSITIVE_FLOAT_VALUE = 1.175494351e-38;
    const double SMALLEST_NEGATIVE_FLOAT_VALUE = -1.175494351e-38;

    const double MIN_FILTER_FREQUENCY = 20.0;
    const double MAX_FILTER_FREQUENCY = 20480.0; // 10 octaves above 20 Hz

    //==============================================================================

    // sgn(x)
    template <typename T>
    inline int sgn(T val)
    {
        return (static_cast<T>(0) < val) - (val < static_cast<T>(0));
    }

    // Fix possible float underflows when converting back from double
    inline bool fixFloatUnderflow(double &value)
    {
        // Positive underflow
        if (value > 0.0 && value < SMALLEST_POSITIVE_FLOAT_VALUE)
        {
            value = 0; // Fix value
            return true;
            // negative underflow
        }
        else if (value < 0.0 && value > SMALLEST_NEGATIVE_FLOAT_VALUE)
        {
            value = 0; // Fix value
            return true;
        }
        return false;
    }

    // Clip value to upper and lower bounds
    inline void clip(double &x, double min = -1.0, double max = 1.0)
    {
        x = fmin(max, x); // Upper bound
        x = fmax(min, x); // Lower bound
    }

    // Convert value from bipolar to unipolar
    inline double bipolarToUnipolar(double x)
    {
        return 0.5 * x + 0.5;
    }

    // Convert value from unipolar to bipolar
    inline double unipolarToBipolar(double x)
    {
        return (x - 0.5) * 2.0;
    }

    // Convert raw amplitude gain to dB
    inline double rawGainTodB(double gain)
    {
        return 20.0 * log10(gain);
    }

    // Convert from dB to raw amplitude gain
    inline double dbToRawGain(double dB)
    {
        return pow(10.0, dB / 20.0);
    }

    //==============================================================================

} // namespace adsp
