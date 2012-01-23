/*
 * Spectrum.hpp
 *
 *  Copyright (C) 2012 Marc Kirchner
 *
 */

#ifndef __LIBIPACAP_INCLUDE_IPACA_SPECTRUM_HPP__
#define __LIBIPACAP_INCLUDE_IPACA_SPECTRUM_HPP__

#include <ipaca/config.hpp>
#include <vector>
#include <ipaca/Stoichiometry.hpp>

namespace ipaca {

namespace detail {

/** A mass spectrum is the same as an isotope distribution.
 */
typedef Isotope SpectrumElement;
typedef std::vector<SpectrumElement> Spectrum;

/** A stream operator for the Spectrum class.
 *
 * @param os The stream.
 * @param s The spectrum.
 * @return The stream after adding the spectrum info.
 */
std::ostream& operator<<(std::ostream& os, const Spectrum& s);

} // namespace detail

} // namespace ipaca

#endif /* __LIBIPACAP_INCLUDE_IPACA_SPECTRUM_HPP__ */
