/*
 * Traits.cpp
 *
 *  Copyright (C) 2012 Marc Kirchner
 *
 */
#include <ipaca/Traits.hpp>
#include <ipaca/Stoichiometry.hpp>

namespace ipaca {

namespace detail {

Element getHydrogens(const Size n)
{
    // function static object is initialized on first pass
    Element e;
    if (e.isotopes.empty()) {
        detail::Isotope i1, i2;
        i1.mz = 1.007825;
        i1.ab = 0.99985;
        i2.mz = 2.01410178;
        i2.ab = 0.00015;
        detail::Isotopes i;
        i.push_back(i1);
        i.push_back(i2);
        e.isotopes = i;
    }
    e.count = static_cast<Double> (n);
    return e;
}

Double getElectronMass()
{
    return 0.00054857990946;
}

Bool isHydrogen(const detail::Element& e)
{
    return e.isotopes[0].mz == 1.007825 && e.isotopes[1].mz == 2.01410178;
}

}

}
