/*
 * Mercury7.hpp
 *
 *  Copyright (C) 2012 Marc Kirchner
 *
 */

#ifndef __LIBIPACA_INCLUDE_IPACA_MERCURY7_HPP__
#define __LIBIPACA_INCLUDE_IPACA_MERCURY7_HPP__
#include <ipaca/config.hpp>
#include <ipaca/Mercury7Impl.hpp>
#include <ipaca/Spectrum.hpp>
#include <ipaca/Stoichiometry.hpp>
#include <ipaca/Types.hpp>
#include <ipaca/Traits.hpp>
#include <boost/shared_ptr.hpp>

namespace ipaca {

/** Calculates a theoretical isotope distribution from an
 *  elemental composition (stoichiometry).
 *
 *  The client is required to provide type information for the
 *  stoichiometry and the resulting spectra as well as two conversion
 *  function in \c ipaca::Traits<U,V> that specifiy how the user-
 *  defined stoichiometry and spectrum types are converted into
 *  the internale representation used by ipaca (i.e. \c ipaca::Spectrum
 *  and \c ipaca::Stoichiometry).
 *  See \c test/Mercury7-test.cpp for a simple example.
 */
template<typename StoichiometryType, typename SpectrumType>
class Mercury7
{
public:
    /** Constructor.
     */
    Mercury7();

    /** Functor method to calculate the theoretical isotope
     *         distribution of a compound.
     * @param stoichiometry The stoichiometry for which the isotope
     *                      distribution should be calculated.
     * @param limit The abundance limit below which peaks are pruned
     *              during the processing
     *
     * The procedure is based on Perttu Haimi's and Alan Rockwood's
     * sparse/binary convolution algorithm.
     */
    SpectrumType
    operator()(const StoichiometryType& stoichiometry,
        const Double limit = 1e-26) const;

    /** calculate the monoisotopic mass of a given stoichiometry
     *  @param stoichiometry The stoichiometry to calculate the mass for.
     *  @param charge The charge at which the monoisotopic mass is desired
     *                (zero for theoretical but unobservable mass).
     */
    Double
    getMonoisotopicMass(const StoichiometryType& stoichiometry) const;

    /** calculate the average mass of a given stoichiometry
     *  @param stoichiometry The stoichiometry to calculate the mass for.
     *  @param charge The charge at which the average mass is desired
     *                (zero for theoretical but unobservable mass).
     */
    Double getAverageMass(const StoichiometryType& stoichiometry) const;
private:
    boost::shared_ptr<detail::Mercury7Impl> pImpl_;
};

//
// template implementation
//

template<typename StoichiometryType, typename SpectrumType>
Mercury7<StoichiometryType, SpectrumType>::Mercury7() :
    pImpl_(new detail::Mercury7Impl)
{
}

template<typename StoichiometryType, typename SpectrumType>
SpectrumType Mercury7<StoichiometryType, SpectrumType>::operator()(
    const StoichiometryType& stoichiometry, const Double limit) const
{
    detail::Stoichiometry s;
    typename Traits<StoichiometryType, SpectrumType>::stoichiometry_converter stoi_conv;
    stoi_conv(stoichiometry, s);
    detail::Spectrum result = pImpl_->operator()(s, limit);
    SpectrumType spectrum;
    typename Traits<StoichiometryType, SpectrumType>::spectrum_converter spec_conv;
    spec_conv(result, spectrum);
    return spectrum;
}

template<typename StoichiometryType, typename SpectrumType>
Double Mercury7<StoichiometryType, SpectrumType>::getMonoisotopicMass(
    const StoichiometryType& stoichiometry) const
{
    detail::Stoichiometry s;
    typename Traits<StoichiometryType, SpectrumType>::stoichiometry_converter stoi_conv;
    stoi_conv(stoichiometry, s);
    return pImpl_->getMonoisotopicMass(s);
}

template<typename StoichiometryType, typename SpectrumType>
Double Mercury7<StoichiometryType, SpectrumType>::getAverageMass(
    const StoichiometryType& stoichiometry) const
{
    detail::Stoichiometry s;
    typename Traits<StoichiometryType, SpectrumType>::stoichiometry_converter stoi_conv;
    stoi_conv(stoichiometry, s);
    return pImpl_->getAverageMass(s);
}

} // namespace ipaca

#endif
