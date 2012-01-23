/*
 * Mercury-test.cpp
 *
 * Copyright (c) 2012 Marc Kirchner
 *
 */
#include <ipaca/Mercury7.hpp>
#include <ipaca/Mercury7Impl.hpp>
#include <ipaca/Spectrum.hpp>
#include <ipaca/Stoichiometry.hpp>
#include <ipaca/Traits.hpp>
#include <ipaca/Types.hpp>
#include <iostream>
#include "vigra/unittest.hxx"

/** Test suite for the Mercury7 interface.
 */

typedef ipaca::detail::Spectrum MySpectrum;
typedef ipaca::detail::Stoichiometry MyStoichiometry;

struct SpectrumConverter
{
    void operator()(const ipaca::detail::Spectrum& lhs, MySpectrum& rhs)
    {
        rhs = lhs;
    }
};

struct StoichiometryConverter
{
    void operator()(const MyStoichiometry& lhs,
        ipaca::detail::Stoichiometry& rhs)
    {
        rhs = lhs;
    }
};

namespace ipaca {

template<>
struct Traits<MyStoichiometry, MySpectrum>
{
    typedef SpectrumConverter spectrum_converter;
    typedef StoichiometryConverter stoichiometry_converter;
};

}

using namespace ipaca;

struct MercuryTestSuite : vigra::test_suite
{
    /** Constructor.
     * The MercuryTestSuite constructor adds all Mercury tests to
     * the test suite. If you write an additional test, add the test
     * case here.
     */
    MercuryTestSuite() :
        vigra::test_suite("Mercury")
    {
        add(testCase(&MercuryTestSuite::test));
    }

    MyStoichiometry createIntegerH2O()
    {
        detail::Stoichiometry h2o;
        detail::Isotope i;
        detail::Element h;
        double massesH[] = { 1.0, 2.0 };
        double freqsH[] = { 0.99, 0.01 };
        for (size_t k = 0; k < 2; ++k) {
            i.mz = massesH[k];
            i.ab = freqsH[k];
            h.isotopes.push_back(i);
        }
        h.count = 2.0;
        detail::Element o;
        double massesO[] = { 16.0, 17.0, 18.0 };
        double freqsO[] = { 0.97, 0.01, 0.02 };
        for (size_t k = 0; k < 3; ++k) {
            i.mz = massesO[k];
            i.ab = freqsO[k];
            o.isotopes.push_back(i);
        }
        o.count = 1.0;
        h2o.push_back(h);
        h2o.push_back(o);
        return h2o;
    }

    void test()
    {
        Mercury7<MyStoichiometry, MySpectrum> m;
        MyStoichiometry s = createIntegerH2O();
        MySpectrum spectrum = m(s);
        //std::cerr << spectrum << std::endl;

        // test against Impl
        detail::Mercury7Impl mimpl;
        MySpectrum spectrum2 = mimpl(s);

        shouldEqual(spectrum.size(), spectrum2.size());
        for (Size k = 0; k < spectrum.size(); ++k) {
            shouldEqualTolerance(spectrum[k].mz, spectrum2[k].mz, 1e-12);
            shouldEqualTolerance(spectrum[k].ab, spectrum2[k].ab, 1e-12);
        }
    }
};

/** The main function that runs the tests for class Mercury.
 * Under normal circumstances you need not edit this.
 */
int main()
{
    MercuryTestSuite test;
    int success = test.run();
    std::cout << test.report() << std::endl;
    return success;
}

