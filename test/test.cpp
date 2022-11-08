#include "Catch2/src/catch2/catch_all.hpp"
#include "../ADSP.h"

using namespace Catch::literals;
using namespace Catch;

TEST_CASE("clip", "[dsp_helpers]")
{
    double highValue{1005.0};
    double lowValue{-4.20};

    // Clip value to default interval bounds [-1.0, 1.0]
    SECTION("Clipping to default interval")
    {
        adsp::clip(highValue);
        REQUIRE(highValue == 1.0_a);

        adsp::clip(lowValue);
        REQUIRE(lowValue == -1.0_a);
    }

    // Clipping to custom interval [min, max]
    SECTION("Clipping to custom interval")
    {
        // Clip value to custom interval bounds [min, max]
        double min{-4.2};
        double max{2.49};

        adsp::clip(highValue, min, max);
        REQUIRE(highValue == Approx(max));

        adsp::clip(lowValue, min, max);
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
        adsp::clip(valA);
        REQUIRE(valA == valuePreClip);

        // Cuutom interval
        double valB{12.1};
        valuePreClip = valB;
        adsp::clip(valB, lowerLimit, upperLimit);
        REQUIRE(valB == valuePreClip);
    }
}

TEST_CASE("sgn", "[dsp_helpers]")
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

TEST_CASE("Bipolar unipolar conversion", "[dsp_helpers]")
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

TEST_CASE("dB raw gain conversion", "[dsp_helpers]")
{
    SECTION("Raw amplitude gain to dB converion")
    {
        // Unity
        REQUIRE(adsp::rawGainTodB(1.0) == Approx(0.0));

        // Double
        REQUIRE(adsp::rawGainTodB(2.0) == Approx(6.02).margin(0.1));

        // Half
        REQUIRE(adsp::rawGainTodB(0.5) == Approx(-6.02).margin(0.01));

        // Random amplification
        REQUIRE(adsp::rawGainTodB(123.57) == Approx(41.838));

        // Random attenuation
        REQUIRE(adsp::rawGainTodB(0.00420) == Approx(-47.535));
    }

    SECTION("dB to raw amplitude gain conversion")
    {
        // Test a few different conversions

        // Unity
        REQUIRE(adsp::dbToRawGain(0.0) == Approx(1.0));

        // Double
        REQUIRE(adsp::dbToRawGain(6.02) == Approx(1.999).margin(0.01));

        // Half
        REQUIRE(adsp::dbToRawGain(-6.02) == Approx(0.500).margin(0.01));

        // Random amplification
        REQUIRE(adsp::dbToRawGain(22.04) == Approx(12.6474));

        // Random attenuation
        REQUIRE(adsp::dbToRawGain(-68.0) == Approx(0.0003).margin(0.001));
    }
}
