/*
 * Traits.hpp
 *
 *  Copyright (C) 2012 Marc Kirchner
 *
 */

#ifndef __LIBIPACA_INCLUDE_IPACA_TRAITS_HPP__
#define __LIBIPACA_INCLUDE_IPACA_TRAITS_HPP__

#include <ipaca/config.hpp>
#include <ipaca/Stoichiometry.hpp>
#include <ipaca/Types.hpp>
#include <ipaca/Error.hpp>

namespace ipaca {

namespace detail {

// default convenience implementations
detail::Element getHydrogens(const Size n);
Bool isHydrogen(const detail::Element&);
Double getElectronMass();

}

template<typename StoichiometryType, typename SpectrumType>
struct Traits
{
    typedef typename StoichiometryType::converter stoichiometry_converter;
    typedef typename SpectrumType::converter spectrum_converter;
    static detail::Element getHydrogens(const Size n);
    static Bool isHydrogen(const detail::Element&);
    static Double getElectronMass();
};

/** Adjust the stoichiometry according to charge and charge type.
 * This adjusts the stoichiometry to reflect the changes induced by adding
 * charges and allows \c libipaca to keep all charge calculations out of the
 * distribution calculation logic. The function is a no-op for electron-based
 * charges, but adjusts the number of hydrogens in protonation-based charge
 * situations. Hence the isotopic distribution of the charge protons
 * contributes to the isotope pattern (as it should be).
 * @param s The uncharged stoichiometry.
 * @param charge The charge, signed.
 * @param p The type of charge mechanics used: either PROTON or ELECTRON.
 */
template<typename StoichiometryType, typename SpectrumType>
void adjustStoichiometryForProtonation(detail::Stoichiometry& s, const Int charge);

}

//
// template implementations
//
namespace ipaca {

namespace detail {

template<typename StoichiometryType, typename SpectrumType>
void adjustStoichiometryForProtonation(detail::Stoichiometry& s,
    const Int charge)
{
    // find the hydrogen entry
    typedef Stoichiometry::iterator IT;
    IT h = std::find_if(s.begin(), s.end(),
        Traits<StoichiometryType, SpectrumType>::isHydrogen);
    Double c = static_cast<Double>(charge);
    if (h != s.end()) {
        h->count = charge > 0 ? h->count + c : h->count - c;
        if (h->count < 0) {
            throw ParameterError("Requested deprotonation but number of "
                "hydrogens is insufficient.");
        }
    } else {
        if (charge > 0) {
        s.push_back(Traits<StoichiometryType, SpectrumType>::getHydrogens(charge));
        } else {
            throw ParameterError("Requested deprotonation but no hydrogens present.");
        }
    }
}

} // namespace detail

} // namespace ipaca

#endif /* __LIBIPACA_INCLUDE_IPACA_TRAITS_HPP__ */
