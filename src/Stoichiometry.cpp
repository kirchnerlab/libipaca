/*
 * Stoichiometry.cpp
 *
 *  Copyright (C) 2012 Marc Kirchner
 *
 */
#include <ipaca/Stoichiometry.hpp>
#include <cmath>
#include <iostream>

using namespace ipaca;

Bool detail::isPlausibleStoichiometry(const detail::Stoichiometry& s)
{
    typedef detail::Stoichiometry::const_iterator CI;
    bool allZero = true;
    for (CI i = s.begin(); i < s.end(); ++i) {
        Double c = i->count;
        if (c < 0) {
            return false;
        }
        if (allZero && c > 0) {
            allZero = false;
        }
    }
    if (!allZero) {
        return true;
    } else {
        return false;
    }
}

void detail::splitStoichiometry(const detail::Stoichiometry& s,
    detail::Stoichiometry& intStoi, detail::Stoichiometry& fracStoi)
{
    intStoi.clear();
    fracStoi.clear();
    typedef detail::Stoichiometry::const_iterator CI;
    for (CI i = s.begin(); i != s.end(); ++i) {
        Double integer = trunc(i->count);
        Double fractional = i->count - integer;
        if (integer > 0.0) {
            intStoi.push_back(*i);
            intStoi.back().count = integer;
        }
        if (fractional > 0.0) {
            fracStoi.push_back(*i);
            fracStoi.back().count = fractional;
        }
    }
}

std::ostream& detail::operator<<(std::ostream& os, const detail::Stoichiometry& s)
{
    typedef detail::Stoichiometry::const_iterator CI;
    os << "(";
    for (CI i = s.begin(); i != s.end(); ++i) {
        os << "(" << i->isotopes[0].mz << ", " << i->count << ")";
    }
    os << ")";
    return os;
}
