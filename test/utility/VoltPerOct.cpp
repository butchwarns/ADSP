#include "../../ADSP.h"
#include "../Catch2/src/catch2/catch_all.hpp"

using namespace Catch::literals;
using namespace Catch;

//==============================================================================
// Volt per octave / frequency conversions

TEST_CASE("Volt per octave / frequency conversions and detune", "[utility]") {
    // Set up conversion object (default tuned)
    adsp::VoltPerOct vpo;

    SECTION("Convert voltage to frequency") {
        // Default tuning

        double volt = 2.0;
        double freq = vpo.voltToFreq(volt);

        // Two octaves above zero volts
        REQUIRE(freq == Approx(vpo.getZeroVoltFreq() * 4.0));

        // Custom tuning

        // Change to arbitrary tuning
        vpo.setZeroVoltFreq(100.0);  // 100 Hz

        volt = 2.0;
        freq = vpo.voltToFreq(volt);

        // Two octaves above 100 Hz
        REQUIRE(freq == Approx(400.0));
    }

    SECTION("Convert frequency to voltage") {
        // Default tuning

        double freq = 8 * vpo.getZeroVoltFreq();
        double volt = vpo.freqToVolt(freq);

        // Three octaves above zero volt frequency
        REQUIRE(volt == Approx(3.0));

        // Custom tuning

        // Change to arbitrary tuning
        vpo.setZeroVoltFreq(100.0);  // 100 Hz

        freq = 200.0;
        volt = vpo.freqToVolt(freq);

        // Two octaves above 100 Hz
        REQUIRE(volt == Approx(1.0));
    }

    SECTION("Detune voltage by semitones") {
        constexpr double voltPerSemi = 1.0 / 12.0;

        constexpr double detuneAmt = 4.0;

        constexpr double volt = 1.0;
        const double voltDetuned = vpo.detuneSemitones(volt, detuneAmt);

        REQUIRE(voltDetuned == Approx(volt + detuneAmt * voltPerSemi));
    }

    SECTION("Detune voltage by cents") {
        constexpr double voltPerCent = 1.0 / (12.0 * 100.0);

        constexpr double detuneAmt = 15.0;

        constexpr double volt = 1.0;
        const double voltDetuned = vpo.detuneCents(volt, detuneAmt);

        REQUIRE(voltDetuned == Approx(volt + detuneAmt * voltPerCent));
    }
}