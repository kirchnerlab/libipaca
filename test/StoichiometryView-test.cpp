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
typedef std::map<int, double> MyStoichiometry;

// typedef the view
typedef ipaca::StoichiometryView<MyStoichiometry> MyView;

// accessors
struct ElementAcc
{
    const MyStoichiometry::key_type& operator()(const MyStoichiometry::value_type& x)
    {
        return x.first;
    }
};

struct CountAcc
{
    double& operator()(MyStoichiometry::value_type& x)
    {
        return x.second;
    }
    const double& operator()(const MyStoichiometry::value_type& x)
    {
        return x.second;
    }
};

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
};

/** Define how the framework can access the contents of a stoichiometry.
 */
template<>
struct StoichiometryValueTraits<MyStoichiometry>
{
    typedef ElementAcc element_accessor;
    typedef CountAcc count_accessor;
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

}

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
    }

    /** Little helper function that add H2O (water) to a stoichiometry.
     * @param[inout] s The stoichiometry.
     */
    void addH2O(MyStoichiometry& s)
    {
        s[1] += 2.0;
        s[16] += 1.0;
    }

    void testConstructionAndRef()
    {
        MyStoichiometry stoi;
        // Construct the view
        MyView v(stoi);
        // Edit the underlying object and see if we pick it up.
        addH2O(stoi);
        shouldEqual(stoi.size(), static_cast<size_t>(2));
        shouldEqual(v.isNonNegative(), true);
        // Construct a copy through the view.
        MyStoichiometry stoi2(v.ref());
        shouldEqual(stoi.size(), stoi2.size());
        stoi[1] = -1.0;
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
        MyStoichiometry stoi;
        addH2O(stoi);
        MyView v(stoi);
        typedef MyView::const_iterator CI;
        ipaca::StoichiometryValueTraits<MyStoichiometry>::element_accessor
                eAcc;
        ipaca::StoichiometryValueTraits<MyStoichiometry>::count_accessor cAcc;
        int elems[] = {1, 16};
        double counts[] = {2, 1};
        size_t k = 0;
        for (CI i = v.begin(); i != v.end(); ++i, ++k) {
            shouldEqual(eAcc(*i), elems[k]);
            shouldEqual(cAcc(*i), counts[k]);
        }
        typedef MyView::iterator IT;
        for (IT i = v.begin(); i != v.end(); ++i) {
            cAcc(*i) = 27.3;
        }
        for (CI i = v.begin(); i != v.end(); ++i) {
            shouldEqual(cAcc(*i), 27.3);
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

