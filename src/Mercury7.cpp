/*
 * Mercury.cpp
 *
 * Copyright (c) 2007-2012 Marc Kirchner
 * Copyright (c) 2007 Xinghua Lou
 * Copyright (c) 2007 Bjoern Voss
 * Copyright (c) 2008 Thorben Kroeger
 *
 * Based on the emass implementation of Perttu Haimi.
 * 
 */
#include <ipaca/Mercury7.hpp>
#include <cassert>
#include <cmath>
// switch off the assert() calls in release code
#ifndef IPACA_DEBUG
#define NDEBUG
#endif

using namespace ipaca;

// TODO: remove the following if we do not need it.
//
///** The mass of an electron, in atomic mass units (u).
// */
//const double Mercury7::ELECTRON_MASS = 0.00054857990946;
//
///** The mass of a proton, in atomic mass units (u).
// */
//const double Mercury7::PROTON_MASS = 1.007276466812;
//
//double Mercury7::getElectronMass() const
//{
//    return ELECTRON_MASS;
//}
//
//double Mercury7::getProtonMass() const
//{
//    return PROTON_MASS;
//}

void Mercury7::convolve(detail::Spectrum& result, const detail::Spectrum& s1,
    const detail::Spectrum& s2) const
{
    // Check if the input is non-empty. We use size() instead of
    // empty() because we need the values later.
    Size n1 = s1.size();
    Size n2 = s2.size();
    if ((n1 + n2) == 0) {
        // nothing to do
        return;
    }
    // No need to clear out the return values, we will overwrite them
    // anyways. Hence, simply make sure the elements exist and that we
    // will not need to reallocate inside the loop.
    result.resize(n1 + n2);
    for (size_t k = 0; k < n1 + n2 - 1; k++) {
        double totalAbundance = 0.0;
        double massExpectation = 0.0;
        size_t start = k < (n2 - 1) ? 0 : k - n2 + 1; // max(0, k-n2+1)
        size_t end = k < (n1 - 1) ? k : n1 - 1; // min(n1-1, k)
        // calculate the convolution of the k-th peak with everything else
        for (size_t i = start; i <= end; i++) {
            double ithAbundance = s1[i].ab * s2[k - i].ab;
            if (ithAbundance > 0.0) {
                // calculate the expected mass position
                totalAbundance += ithAbundance;
                double ithMass = s1[i].mz + s2[k - i].mz;
                massExpectation += ithAbundance * ithMass;
            }
        }
        // We cannot simply throw away isotopes with zero probability, as
        // this would mess up the isotope count k.
        result[k].mz = totalAbundance > 0 ? (massExpectation / totalAbundance)
                : 0;
        result[k].ab = totalAbundance;
    }
}

void Mercury7::prune(detail::Spectrum& s, const double limit) const
{
    size_t i = 0;
    // prune from the front; if the monoisotopic mass is pruned, it will be
    // re-added at the end of the Mercury algorithm 
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i].ab > limit) {
            break;
        }
    }
    s.erase(s.begin(), s.begin() + i);
    // prune the end
    for (i = s.size() - 1; i >= 0; --i) {
        if (s[i].ab > limit) {
            break;
        }
    }
    s.resize(i + 1);
}

void Mercury7::integerMercury(const detail::Stoichiometry& stoichiometry,
    const double limit, detail::Spectrum& msa) const
{
    msa.clear();
    detail::Spectrum tmp, esa;
    Bool msa_initialized = false;

    // walk through the elements
    typedef detail::Stoichiometry::const_iterator SCI;
    for (SCI iter = stoichiometry.begin(); iter != stoichiometry.end(); ++iter) {
        // number of atoms at iterator position
        Size n = iter->count;
        // if the element is present in the composition,
        // then calculate ESA and update MSA
        if (n) {
            // initialize ESA
            esa.assign(iter->isotopes.begin(), iter->isotopes.end());
            assert(!esa.empty());
            while (1) {
                // check if we need to do the MSA update
                if (n & 1) {
                    // MSA update
                    if (msa_initialized) {
                        // normal update
                        convolve(tmp, msa, esa);
                        msa = tmp;
                    } else {
                        // initialize MSA=ESA
                        msa = esa;
                        msa_initialized = true;
                    }
                    prune(msa, limit);
                }
                // the ESA update is always carried out (with the exception of
                // the last time, i.e. when n==1)
                if (n == 1) {
                    break;
                }
                convolve(tmp, esa, esa);
                esa = tmp;
                prune(esa, limit);
                n = n >> 1;
            }
        }
    }
}

void Mercury7::fractionalMercury(const detail::Stoichiometry& s, double limit,
    detail::Spectrum& frac) const
{
    frac.clear();
    typedef detail::Stoichiometry::const_iterator SCI;
    for (SCI i = s.begin(); i != s.end(); ++i) {
        // the last spectrum
        detail::Spectrum temp(frac);
        // initialize ESA
        detail::Spectrum esa;
        for (Size u = 0; u < i->isotopes.size(); ++u) {
            if (i->isotopes[u].ab <= 0.0) {
                continue;
            }
            detail::SpectrumElement se;
            if (u > 0) {
                se.mz = i->isotopes[u].mz - i->isotopes[0].mz + esa.front().mz;
                se.ab = i->isotopes[u].ab * i->count;
                esa.push_back(se);
            } else {
                se.mz = i->isotopes[0].mz * i->count;
                se.ab = (1 - i->count) + i->isotopes[0].ab * i->count;
            }
            esa.push_back(se);
        }
        if (i == s.begin()) {
            frac = esa;
        } else {
            convolve(frac, esa, temp);
        }
    }
}
detail::Spectrum Mercury7::operator()(
    const detail::Stoichiometry& stoichiometry, const double limit) const
{
    // split the stoichiometry into integer and fractional parts
    detail::Stoichiometry intStoi;
    detail::Stoichiometry fracStoi;
    detail::splitStoichiometry(stoichiometry, intStoi, fracStoi);
    // check if there is any integer contribution, and calculate the mz and
    // abundance vectors if yes
    detail::Spectrum intSpec;
    bool hasValidIntegerStoichiometry = detail::isPlausibleStoichiometry(
        intStoi);
    if (hasValidIntegerStoichiometry) {
        integerMercury(stoichiometry, limit, intSpec);
    }
    // check if there is any fractional contribution and calculate the mz and
    // abundance vectors if yes
    detail::Spectrum fracSpec;
    bool hasValidFractionalStoichiometry = detail::isPlausibleStoichiometry(
        fracStoi);
    if (hasValidFractionalStoichiometry) {
        fractionalMercury(fracStoi, limit, fracSpec);
    }
    // if we have integer and fractional contributions, we need to convolve the
    // two; otherwise assign the resepctive non-zero contribution.
    detail::Spectrum result;
    if (hasValidIntegerStoichiometry && hasValidFractionalStoichiometry) {
        Mercury7::convolve(result, intSpec, fracSpec);
        Mercury7::prune(result, limit);
    } else {
        if (hasValidIntegerStoichiometry) {
            result = intSpec;
        } else {
            result = fracSpec;
        }
    }
    /* TODO: decide if we want this functionality in Mercury (or pull it out).
     if (includeMonoisotopicMass) {
     // calculate the monoisotopic mass of the compound and check if we
     // lost it on the way (e.g. due to heavy label incorporation)
     double monoMass = getMonoisotopicMass(stoi, charge);
     if ((std::abs)(result[0].mz - monoMass) > 1e-6) {
     //MSTK_LOG(logDEBUG3) << "[f] " << result_mz[0] << " vs. "
     //  << monoMass << "; diff=" << (std::abs)(result_mz[0] - monoMass);
     // with a vector, this requires a copy *duh*
     detail::SpectrumElement se;
     se.mz = monoMass;
     se.ab = 0.0;
     result.insert(result.begin(), se);
     }
     }
     */
    return result;
}

double Mercury7::getMonoisotopicMass(
    const detail::Stoichiometry& stoichiometry) const
{
    double mass = 0.0;
    // multiply the number of atoms for each element with the element's
    // monoisotopic mass
    typedef detail::Stoichiometry::const_iterator CI;
    for (CI i = stoichiometry.begin(); i != stoichiometry.end(); ++i) {
        mass += i->count * i->isotopes[0].mz;
    }
    return mass;
}

double Mercury7::getAverageMass(const detail::Stoichiometry& stoichiometry) const
{
    double avg = 0.0;
    typedef detail::Stoichiometry::const_iterator CI;

    for (CI i = stoichiometry.begin(); i != stoichiometry.end(); ++i) {
        // get the average mass of the stoichiometry entry
        double entryAvg = 0.0;
        typedef detail::Isotopes::const_iterator ICI;
        for (ICI j = i->isotopes.begin(); j != i->isotopes.end(); ++j) {
            entryAvg += j->mz * j->ab;
        }
        avg += entryAvg;
    }
    return avg;
}

