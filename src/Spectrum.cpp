/*
 * Spectrum.cpp
 *
 *  Copyright (C) 2012 Marc Kirchner
 *
 */
#include <ipaca/Spectrum.hpp>
#include <iostream>

using namespace ipaca;

std::ostream& detail::operator<<(std::ostream& os, const detail::Spectrum& s)
{
    typedef detail::Spectrum::const_iterator CI;
    for (CI i = s.begin(); i != s.end(); ++i) {
        os << i->mz << ", " << i->ab << "\n";
    }
    return os;
}
