/*
 * StoichiometryView.hpp
 *
 *  Copyright (C) 2012 Marc Kirchner
 *
 */
#ifndef __LIBIPACA_INCLUDE_IPACA_STOICHIOMETRYVIEW_HPP__
#define __LIBIPACA_INCLUDE_IPACA_STOICHIOMETRYVIEW_HPP__

namespace ipaca {

template<typename T>
struct StoichiometryTraits
{
    typedef typename T::iterator iterator_type;
    typedef typename T::const_iterator const_iterator_type;
    typedef typename T::value_type value_type;
};

template<typename T>
struct StoichiometryValueTraits
{
    // must be specified by the user
};

/** A generic view on stoichiometries.
 * This class declares the interface that \c libipaca expects from a
 * stoichiometry. The client is required to specify stoichiometry
 * propertie (iterator types, value types and accessor functors in the
 * respective trait classes \c StoichiometryTraits and
 * \c StoichiometryValueTraits.
 */
template<typename T>
class StoichiometryView
{
public:
    typedef typename StoichiometryTraits<T>::iterator_type iterator;
    typedef typename StoichiometryTraits<T>::const_iterator_type
            const_iterator;

    StoichiometryView(T&);
    StoichiometryView(const StoichiometryView<T>& rhs);

    const_iterator begin() const;
    const_iterator end() const;

    // TODO: figure out if we REALLY need the non-const iterator. Eliminate if possible.
    iterator begin();
    iterator end();

    const T& ref() const;
    bool isNonNegative() const;

private:
    T& stoi_;
};

}

//
// template implementation
//

template<typename T>
ipaca::StoichiometryView<T>::StoichiometryView(T& stoi) :
    stoi_(stoi)
{
}

template<typename T>
ipaca::StoichiometryView<T>::StoichiometryView(const StoichiometryView<T>& rhs) :
    stoi_(rhs.stoi_)
{
}

template<typename T>
typename ipaca::StoichiometryView<T>::const_iterator ipaca::StoichiometryView<
        T>::begin() const
{
    return stoi_.begin();
}

template<typename T>
typename ipaca::StoichiometryView<T>::const_iterator ipaca::StoichiometryView<
        T>::end() const
{
    return stoi_.end();
}

template<typename T>
typename ipaca::StoichiometryView<T>::iterator ipaca::StoichiometryView<T>::begin()
{
    return stoi_.begin();
}

template<typename T>
typename ipaca::StoichiometryView<T>::iterator ipaca::StoichiometryView<T>::end()
{
    return stoi_.end();
}

template<typename T>
const T& ipaca::StoichiometryView<T>::ref() const
{
    return stoi_;
}

#endif /* __LIBIPACA_INCLUDE_IPACA_STOICHIOMETRYVIEW_HPP__ */
