/*
 * Traits.hpp
 *
 *  Copyright (C) 2012 Marc Kirchner
 *
 */

#ifndef __LIBIPACA_INCLUDE_IPACA_TRAITS_HPP__
#define __LIBIPACA_INCLUDE_IPACA_TRAITS_HPP__

namespace ipaca {

template<typename StoichiometryType, typename SpectrumType>
struct Traits
{
    typedef typename StoichiometryType::converter stoichiometry_converter;
    typedef typename SpectrumType::converter spectrum_converter;
};

}

#endif /* __LIBIPACA_INCLUDE_IPACA_TRAITS_HPP__ */
