/*
 * Types.hpp
 *
 * Copyright (c) 2012 Marc Kirchner
 *
 */

#ifndef __LIBIPACA_INCLUDE_IPACA_TYPES_HPP__
#define __LIBIPACA_INCLUDE_IPACA_TYPES_HPP__

#include <string>

namespace ipaca {

typedef int Int;
typedef double Double;
typedef size_t Size;
typedef bool Bool;
typedef std::string String;

enum ChargedParticle {
    ELECTRON, PROTON
};

} // namespace ipaca

#endif
