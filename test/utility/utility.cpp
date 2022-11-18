#include "../Catch2/src/catch2/catch_all.hpp"
#include "../../ADSP.h"

using namespace Catch::literals;
using namespace Catch;

//==============================================================================
// sgn(x)

TEST_CASE("sgn(x)", "[utility]")
{
    SECTION("Sign of double values")
    {
        REQUIRE(adsp::sgn(24.59) == 1);
        REQUIRE(adsp::sgn(0.0) == 0);
        REQUIRE(adsp::sgn(-0.0) == 0);
        REQUIRE(adsp::sgn(-4.20) == -1);
    }

    SECTION("Sign of float values")
    {
        REQUIRE(adsp::sgn(24.59f) == 1);
        REQUIRE(adsp::sgn(0.0f) == 0);
        REQUIRE(adsp::sgn(-0.0f) == 0);
        REQUIRE(adsp::sgn(-4.20f) == -1);
    }

    SECTION("Sign of int values")
    {
        REQUIRE(adsp::sgn(24) == 1);
        REQUIRE(adsp::sgn(0) == 0);
        REQUIRE(adsp::sgn(-0) == 0);
        REQUIRE(adsp::sgn(-4) == -1);
    }
}

//==============================================================================
// Fix flaoting point underflows

TEST_CASE("Fix floating point underflow", "[utility]")
{
    SECTION("Positive float underflow")
    {
        double tooSmall = adsp::MIN_FLOAT_VAL_POS / 2.0;

        adsp::fixUnderflow(tooSmallF);

        REQUIRE(tooSmall == 0.0);
    }

    SECTION("Negative float underflow")
    {
        double tooSmall = adsp::MIN_FLAOT_VAL_NEG / 2.0;

        adsp::fixUnderflow(tooSmallD);

        REQUIRE(tooSmall == 0.0);
    }
}

//==============================================================================
// Clipping

// // Clip value to given bounds
// inline double clip(double x, const double min = -1.0f, const double max = 1.0f);

// // Clip value to given bounds
// inline float clip(float x, const float min = -1.0f, const float max = 1.0f);

// // Clip value in place
// inline void clipInPlace(double &x, const double min = -1.0, const double max = 1.0);

// // Clip value in place
// inline void clipInPlace(float &x, const float min = -1.0f, const float max = 1.0f);

TEST_CASE("Clip in place", "[utility]")
{
    double highValue{1005.0};
    double lowValue{-4.20};

    // Clip value to default interval bounds [-1.0, 1.0]
    SECTION("Clipping to default interval")
    {
        adsp::clipInPlace(highValue);
        REQUIRE(highValue == 1.0_a);

        adsp::clipInPlace(lowValue);
        REQUIRE(lowValue == -1.0_a);
    }

    // Clipping to custom interval [min, max]
    SECTION("Clipping to custom interval")
    {
        // Clip value to custom interval bounds [min, max]
        double min{-4.2};
        double max{2.49};

        adsp::clipInPlace(highValue, min, max);
        REQUIRE(highValue == Approx(max));

        adsp::clipInPlace(lowValue, min, max);
        REQUIRE(lowValue == Approx(min));
    }

    SECTION("Values within bounds should not be changed")
    {
        double valuePreClip = 0.0;
        double upperLimit = 15.0f;
        double lowerLimit = -15.0f;

        // Default inteval
        double valA{0.132};
        valuePreClip = valA;
        adsp::clipInPlace(valA);
        REQUIRE(valA == valuePreClip);

        // Custom interval
        double valB{12.1};
        valuePreClip = valB;
        adsp::clipInPlace(valB, lowerLimit, upperLimit);
        REQUIRE(valB == valuePreClip);
    }
}

//==============================================================================
// MIDI pitch <-> frequency conversions

// inline double pitchToFreq(const double pitch);

// inline float pitchToFreq(const float pitch);

// inline double freqToPitch(const double freq);

// inline float freqToPitch(float freq);

//==============================================================================
// Unipolar <-> bipolar conversions

TEST_CASE("Unipolar <-> bipolar signal conversions", "[utility]")
{
    SECTION("Bipolar to unipolar conversion")
    {
        // Check bounds
        REQUIRE(adsp::bipolarToUnipolar(-1.0) == 0.0_a);
        REQUIRE(adsp::bipolarToUnipolar(1.0) == 1.0_a);

        // Check one value within ]-1.0, 1.0[
        REQUIRE(adsp::bipolarToUnipolar(0.0) == 0.5_a); // Half of the way up
    }

    SECTION("Unipolar to bipolar conversion")
    {
        // Check bounds
        REQUIRE(adsp::unipolarToBipolar(0.0) == -1.0_a);
        REQUIRE(adsp::unipolarToBipolar(1.0) == 1.0_a);

        // Check one value within ]0.0, 1.0[
        REQUIRE(adsp::unipolarToBipolar(0.25) == -0.5_a); // A quarter of the way up
    }
}

//==============================================================================
// dB <-> gain conversions

TEST_CASE("dB <-> raw gain conversions", "[utility]")
{
    SECTION("Raw amplitude gain to dB converion")
    {
        // Test with floats and doubles

        // Unity
        REQUIRE(adsp::rawGainTodB(1.0f) == Approx(0.0f));
        REQUIRE(adsp::rawGainTodB(1.0) == Approx(0.0));

        // Double
        REQUIRE(adsp::rawGainTodB(2.0f) == Approx(6.02f).margin(0.1f));
        REQUIRE(adsp::rawGainTodB(2.0) == Approx(6.02).margin(0.1));

        // Half
        REQUIRE(adsp::rawGainTodB(0.5f) == Approx(-6.02f).margin(0.01f));
        REQUIRE(adsp::rawGainTodB(0.5) == Approx(-6.02).margin(0.01));

        // Random amplification
        REQUIRE(adsp::rawGainTodB(123.57f) == Approx(41.838f));
        REQUIRE(adsp::rawGainTodB(123.57) == Approx(41.838));

        // Random attenuation
        REQUIRE(adsp::rawGainTodB(0.00420f) == Approx(-47.535f));
        REQUIRE(adsp::rawGainTodB(0.00420) == Approx(-47.535));
    }

    SECTION("dB to raw amplitude gain conversion")
    {
        // Test with floats and doubles

        // Unity
        REQUIRE(adsp::dbToRawGain(0.0f) == Approx(1.0f));
        REQUIRE(adsp::dbToRawGain(0.0) == Approx(1.0));

        // Double
        REQUIRE(adsp::dbToRawGain(6.02f) == Approx(1.999f).margin(0.01f));
        REQUIRE(adsp::dbToRawGain(6.02) == Approx(1.999).margin(0.01));

        // Half
        REQUIRE(adsp::dbToRawGain(-6.02f) == Approx(0.500f).margin(0.01f));
        REQUIRE(adsp::dbToRawGain(-6.02) == Approx(0.500).margin(0.01));

        // Random amplification
        REQUIRE(adsp::dbToRawGain(22.04f) == Approx(12.6474f));
        REQUIRE(adsp::dbToRawGain(22.04) == Approx(12.6474));

        // Random attenuation
        REQUIRE(adsp::dbToRawGain(-68.0f) == Approx(0.0003f).margin(0.001f));
        REQUIRE(adsp::dbToRawGain(-68.0) == Approx(0.0003).margin(0.001));
    }

    SECTION("dB to raw amplitude gain conversion with off threshold")
    {
        // Test with floats and doubles

        // -24.0 dB should mute the signal
        const double offThreshD = -60.0;
        const float offThreshF = -60.0;

        // Unity
        REQUIRE(adsp::dbToRawGainOff(0.0f, offThreshF) == Approx(1.0f));
        REQUIRE(adsp::dbToRawGainOff(0.0, offThreshD) == Approx(1.0));

        // Double
        REQUIRE(adsp::dbToRawGainOff(6.02f, offThreshF) == Approx(1.999f).margin(0.001f));
        REQUIRE(adsp::dbToRawGainOff(6.02, offThreshD) == Approx(1.999).margin(0.001));

        // Half
        REQUIRE(adsp::dbToRawGainOff(-6.02f, offThreshF) == Approx(0.500f).margin(0.001f));
        REQUIRE(adsp::dbToRawGainOff(-6.02, offThreshD) == Approx(0.500).margin(0.001));

        // Random amplification
        REQUIRE(adsp::dbToRawGainOff(22.04f, offThreshF) == Approx(12.6474f));
        REQUIRE(adsp::dbToRawGainOff(22.04, offThreshD) == Approx(12.6474));

        // Random attenuation (should not mute)
        REQUIRE(adsp::dbToRawGainOff(-12.0f, offThreshF) == Approx(0.2512f).margin(0.001f));
        REQUIRE(adsp::dbToRawGainOff(-12.0, offThreshD) == Approx(0.2512).margin(0.001f));

        // Random attenuation (should mute)
        REQUIRE(adsp::dbToRawGainOff(-68.0f, offThreshF) == 0.0f);
        REQUIRE(adsp::dbToRawGainOff(-68.0, offThreshD) == 0.0);
    }
}

//==============================================================================
// Mappings and skews

// template <typename T = double>
// inline T linMap(const T val, const T inMin, const T inMax, const T outMin, const T outMax);

// // Linear mapping of normalised value ([-1.0, 1.0]) to a given range
// template <typename T = double>
// inline T linMapNorm(const T val, const T outMin, const T outMax);

// inline double skewNormalized(const double NormVal, const double skew);

// inline float skewNormalized(const float NormVal, const float skew);

//==============================================================================
// Interpolations

TEST_CASE("Interpolations", "[utility]")
{
    SECTION("Linear interpolation")
    {
        REQUIRE(adsp::interpolateLinear(0.0, 1.0, 0.5) == Approx(0.5));
        REQUIRE(adsp::interpolateLinear(0.0, 10.0, 0.5) == Approx(5.0));
    }
}

//==============================================================================
// Fast function approximations

TEST_CASE("Fast function approximations", "[utility]")
{
    SECTION("Fast log2")
    {
        REQUIRE(adsp::fastLog2(16.0f) == Approx(4.0f).margin(0.005));
        REQUIRE(adsp::fastLog2(5.0f) == Approx(2.322f).margin(0.005));
    }
}