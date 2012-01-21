/*
 * StoichiometryView-test.cpp
 *
 * Copyright (c) 2012 Marc Kirchner
 *
 */
#include <map>
#include <iostream>
#include "vigra/unittest.hxx"

#include <ipaca/StoichiometryView.hpp>

//
// preparations
//

// define a simple stoichiometry class
typedef std::pair<double, double> Isotope;
typedef std::pair<int, std::vector<Isotope> > Element;
typedef std::pair<Element, double> Entry;
typedef std::vector<Entry> MyStoichiometry;

// typedef the view
typedef ipaca::StoichiometryView<MyStoichiometry> MyView;

// forward decls of accessors
struct IsotopesAcc;
struct CountAcc;
struct MzAcc;
struct AbAcc;

// Specialize the implementations for the view on MyStoichiometry.
// This is an example of what needs to be done on the client side
// to make use of the library.
namespace ipaca {

/** Define some properties regarding the user-supplied stoichiometry
 * type.
 */
template<>
struct StoichiometryTraits<MyStoichiometry>
{
    typedef MyStoichiometry::iterator iterator_type;
    typedef MyStoichiometry::const_iterator const_iterator_type;
    typedef MyStoichiometry::value_type value_type;

    typedef MyStoichiometry StoichiometryContainerType;
    typedef  MyStoichiometry::value_type StoichiometryEntryType;
    typedef  std::vector<Isotope> IsotopeContainerType;
    typedef  IsotopeContainerType::value_type IsotopeType;
    typedef double CountType;

    typedef  IsotopesAcc IsotopesAccessorType;
    typedef  CountAcc CountAccessorType;
};

template<>
struct IsotopesTraits<MyStoichiometry>
{
    typedef  std::vector<Isotope> IsotopeContainerType;
    typedef  IsotopeContainerType::value_type IsotopeType;

    typedef  MzAcc MzAccessorType;
    typedef  AbAcc AbAccessorType;
};

/** Test the simple \c MyStoichiometry implementation for nonnegativity.
 * @return A boolean, indicating if the stoichiometry has only non-negative entries.
 */
template<>
bool ipaca::StoichiometryView<MyStoichiometry>::isNonNegative() const
{
    for (MyStoichiometry::const_iterator i = stoi_.begin(); i != stoi_.end(); ++i) {
        if (i->second < 0)
            return false;
    }
    return true;
}

} // namespace ipaca (for template specializations)

struct IsotopesAcc
{
    const ipaca::StoichiometryTraits<MyStoichiometry>::IsotopeContainerType& operator()(
        const ipaca::StoichiometryTraits<MyStoichiometry>::StoichiometryEntryType& x)
    {
        return x.first.second;
    }
};

struct CountAcc
{
    const ipaca::StoichiometryTraits<MyStoichiometry>::CountType& operator()(
        const ipaca::StoichiometryTraits<MyStoichiometry>::StoichiometryEntryType& x)
    {
        return x.second;
    }
};

struct MzAcc
{
    const double& operator()(
        const ipaca::StoichiometryTraits<MyStoichiometry>::IsotopeType& x)
    {
        return x.first;
    }
};

struct AbAcc
{
    const double& operator()(
        const ipaca::StoichiometryTraits<MyStoichiometry>::IsotopeType& x)
    {
        return x.second;
    }
};


//
// test suite
//

/** Test suite for the StoichiometryView
 */
struct StoichiometryViewTestSuite : vigra::test_suite
{
    /** Constructor.
     * The StoichiometryViewTestSuite constructor adds all StoichiometryView tests to
     * the test suite. If you write an additional test, add the test
     * case here.
     */
    StoichiometryViewTestSuite() :
        vigra::test_suite("StoichiometryView")
    {
        add(testCase(&StoichiometryViewTestSuite::testConstructionAndRef));
        add(testCase(&StoichiometryViewTestSuite::testIteration));
        add(testCase(&StoichiometryViewTestSuite::testIterationIsotopes));
    }

    /** Little helper function that add H2O (water) to a stoichiometry.
     * @param[inout] s The stoichiometry.
     */
    MyStoichiometry createH2O()
    {
        std::vector<Isotope> isoDistH;
        isoDistH.push_back(std::make_pair(1.0, 0.99));
        isoDistH.push_back(std::make_pair(2.0, 0.01));
        Entry h = std::make_pair(std::make_pair(1, isoDistH), 2.0);
        std::vector<Isotope> isoDistO;
        isoDistO.push_back(std::make_pair(16.0, 0.97));
        isoDistO.push_back(std::make_pair(17.0, 0.01));
        isoDistO.push_back(std::make_pair(18.0, 0.02));
        Entry o = std::make_pair(std::make_pair(16, isoDistO), 1.0);
        MyStoichiometry s;
        s.push_back(h);
        s.push_back(o);
        return s;
    }

    void testConstructionAndRef()
    {
        MyStoichiometry stoi = createH2O();
        // Construct the view
        MyView v(stoi);
        shouldEqual(stoi.size(), static_cast<size_t>(2));
        shouldEqual(v.isNonNegative(), true);
        // Construct a copy through the view.
        MyStoichiometry stoi2(v.ref());
        shouldEqual(stoi.size(), stoi2.size());
        stoi[1].second = -1.0;
        shouldEqual(v.isNonNegative(), false);
        // test addresses of ref()
        MyView v2(stoi);
        shouldEqual(&(v.ref()), &(v2.ref()));
        // copy construction
        MyView v3(v2);
        shouldEqual(&(v2.ref()), &(v3.ref()));

    }

    void testIteration()
    {
        MyStoichiometry stoi = createH2O();
        MyView v(stoi);
        typedef MyView::const_iterator CI;
        ipaca::StoichiometryTraits<MyStoichiometry>::CountAccessorType cAcc;
        double counts[] = { 2, 1 };
        size_t k = 0;
        for (CI i = v.begin(); i != v.end(); ++i, ++k) {
            shouldEqual(cAcc(*i), counts[k]);
        }
    }

    void testIterationIsotopes()
    {
        MyStoichiometry stoi = createH2O();
        MyView v(stoi);
        typedef MyView::const_iterator CI;
        ipaca::StoichiometryTraits<MyStoichiometry>::IsotopesAccessorType iAcc;
        ipaca::IsotopesTraits<MyStoichiometry>::MzAccessorType mzAcc;
        ipaca::IsotopesTraits<MyStoichiometry>::AbAccessorType abAcc;
        double masses[] = { 1.0, 2.0, 16.0, 17.0, 18.0 };
        double freqs[] = { 0.99, 0.01, 0.97, 0.01, 0.02};
        size_t k = 0;
        for (CI i = v.begin(); i != v.end(); ++i) {
            typedef ipaca::StoichiometryTraits<MyStoichiometry>::IsotopeContainerType IsotopeContainer;
            const IsotopeContainer& ic = iAcc(*i);
            typedef IsotopeContainer::const_iterator CI;
            for (CI j = ic.begin(); j != ic.end(); ++j, ++k) {
                //std::cerr << k << ": mz: " << mzAcc(*j) << ", ab:" << abAcc(*j) << std::endl;
                shouldEqual(mzAcc(*j), masses[k]);
                shouldEqual(abAcc(*j), freqs[k]);
            }
        }
    }

};

/** The main function that runs the tests for class StoichiometryView.
 * Under normal circumstances you need not edit this.
 */
int main()
{
    StoichiometryViewTestSuite test;
    int success = test.run();
    std::cout << test.report() << std::endl;
    return success;
}

