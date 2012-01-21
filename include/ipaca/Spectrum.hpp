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

} // namespace detail

} // namespace ipaca

#endif /* __LIBIPACAP_INCLUDE_IPACA_SPECTRUM_HPP__ */
