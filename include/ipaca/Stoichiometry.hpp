/*
 * Stoichiometry.hpp
 *
 *  Copyright (C) 2012 Marc Kirchner
 *
 */

#ifndef __LIBIPACAP_INCLUDE_IPACA_STOICHIOMETRY_HPP__
#define __LIBIPACAP_INCLUDE_IPACA_STOICHIOMETRY_HPP__

#include <ipaca/config.hpp>
#include <vector>
#include <iosfwd>
#include <ipaca/Types.hpp>

namespace ipaca {

namespace detail {

/*
 * Keep things as simple as possible.
 */

/** An isotope.
 */
struct Isotope
{
    Double mz, ab;
};

/** An isotope distribution.
 */
typedef std::vector<Isotope> Isotopes;

/** A mass spectrum is the same as an isotope distribution.
 */
typedef Isotope SpectrumElement;
typedef std::vector<SpectrumElement> Spectrum;

/** The relevant element information in a stoichiometry. We only need
 * the isotopic distribution and the number of occurences.
 */
struct Element
{
    Isotopes isotopes;
    Double count;
};

/** A stoichiometry is simply a list of elements with their isotopic
 * distributions and number of occurences.
 */
typedef std::vector<Element> Stoichiometry;

//
// a few free functions
//

/** Check if all entries in a stoichiometry are non-negative and if there is
 * at least one entry that is positive. Here, 'plausible' means that
 * a meaningful calculation is possible, not necessarily that the stoichiometry
 * is chemically feasible (e.g O_32 would be considered plausible, but O_{-3}
 * and H_{0}C_{0} would not).
 * @param s The \c detail::Stoichiometry object to test.
 * @return A boolean, true if the stoichiometry is plausible.
 */
Bool isPlausibleStoichiometry(const Stoichiometry& s);

/** Split a stoichiometry into integer and fractional contributions.
 *
 * @param s The \c Stoichiometry to split.
 * @param intStoi All integer contributions to the stoichiometry.
 * @param fracStoi All fractional contributions.
 */
void splitStoichiometry(const Stoichiometry& s, Stoichiometry& intStoi,
    Stoichiometry& fracStoi);

/** Stream operator.
 *
 * @param os A reference to the outstream.
 * @param s The Stoichiometry to stream.
 * @return A reference to the same outstream.
 */
std::ostream& operator<<(std::ostream& os, const Stoichiometry& s);

} // namespace detail

} // namespace ipaca

#endif /* __LIBIPACAP_INCLUDE_IPACA_STOICHIOMETRY_HPP__ */
