/*
 * Error.hpp
 *
 * Copyright (c) 2009-2012 Marc Kirchner
 * Copyright (c) 2009 Bernhard Kausler
 */

#ifndef __LIBIPACA_INCLUDE_IPACA_ERROR_HPP__
#define __LIBIPACA_INCLUDE_IPACA_ERROR_HPP__

#include <exception>
#include <string>
#include <cassert>

namespace ipaca {

/** Base class for all \c libipaca exceptions.
 */
class Exception : public std::exception
{
public:
    /** Constructor to pass C-style strings.
     *  @param message C-style string describing the error condition.
     *                 The char will be copied into an internal 
     *                 std::string upon construction, hence the caller
     *                 is responsible for freeing the memory.
     */
    explicit Exception(const char* message)
    {
        what_ = message;
    }

    /** Constructor taking C++-style strings.
     *  @param message String describing the error condition.
     */
    explicit Exception(const std::string& message)
    {
        what_ = message;
    }

    /** Virtual destructor.
     */
    virtual ~Exception() throw ()
    {
    }

    /** C-string-style access to the description of the error condition.
     *  @return A pointer to a C-string. The memory is owned by the 
     *          Exception instance and clients should NEVER try to
     *          destroy/free the returned pointer.
     */
    virtual const char * what() const throw ()
    {
        return what_.c_str();
    }

protected:
    /** Stores the string that describes the error condition.
     * The implementation leaves the variable as 'protected' to
     * simplify reimplentations of 'what()' in derived classes.
     */
    std::string what_;
}; /* class Exception */

/**
 *   Base class for all logic errors.
 *
 *   Use LogicError for defects, which in principle could be detected by codeflow
 *   analysis.  In general logic errors are originated from invalid external
 *   input or code bugs, which are causing the ms++ to run outside expected
 *   parameter ranges.
 */
class LogicError : public ipaca::Exception
{
public:
    explicit LogicError(const char * message) :
        Exception(message)
    {
    }
    explicit LogicError(const std::string& message) :
        Exception(message)
    {
    }
    virtual ~LogicError() throw ()
    {
    }
}; /* class LogicError */

/**
 *   Base class for all runtime errors.
 *
 *   Use RuntimeError for defects, which could only happen or be detected during
 *   runtime.  Runtime errors are caused by not acquirable system resources (like
 *   memory, file handles, network etc.), race conditions of different threads or
 *   processes and other unforseeable failures.
 */
class RuntimeError : public ipaca::Exception
{
public:
    explicit RuntimeError(const char* message) :
        Exception(message)
    {
    }
    explicit RuntimeError(const std::string& message) :
        Exception(message)
    {
    }
    virtual ~RuntimeError() throw ()
    {
    }
}; /* class RuntimeError */

//////////////////////////////////
// now, some general subclasses //
//////////////////////////////////

class PreconditionViolation : public LogicError
{
public:
    explicit PreconditionViolation(const char* message) :
        LogicError(message)
    {
    }
    explicit PreconditionViolation(const std::string& message) :
        LogicError(message)
    {
    }
    virtual ~PreconditionViolation() throw ()
    {
    }
}; /* class PreconditionViolation */

class PostconditionViolation : public LogicError
{
public:
    explicit PostconditionViolation(const char* message) :
        LogicError(message)
    {
    }
    explicit PostconditionViolation(const std::string& message) :
        LogicError(message)
    {
    }
    virtual ~PostconditionViolation() throw ()
    {
    }
}; /* class PostconditionViolation */

class InvariantViolation : public LogicError
{
public:
    explicit InvariantViolation(const char* message) :
        LogicError(message)
    {
    }
    explicit InvariantViolation(const std::string& message) :
        LogicError(message)
    {
    }
    virtual ~InvariantViolation() throw ()
    {
    }
}; /* class InvariantViolation */

class ParameterError : public LogicError
{
public:
    explicit ParameterError(const char* message) :
        LogicError(message)
    {
    }
    explicit ParameterError(const std::string& message) :
        LogicError(message)
    {
    }
    virtual ~ParameterError() throw ()
    {
    }
}; /* class ParameterError */

//////////////////////////////////////////////////////
// some helper functions to throw exceptions easily //
// dont't use them directly, use the macros below   //
//////////////////////////////////////////////////////

inline
void throw_invariant_error(bool predicate, const char* message)
{
    if (!predicate)
        throw ipaca::InvariantViolation(message);
}

inline
void throw_precondition_error(bool predicate, const char* message)
{
    if (!predicate)
        throw ipaca::PreconditionViolation(message);
}

inline
void throw_postcondition_error(bool predicate, const char* message)
{
    if (!predicate)
        throw ipaca::PostconditionViolation(message);
}

inline
void throw_invariant_error(bool predicate, const std::string& message)
{
    if (!predicate)
        throw ipaca::InvariantViolation(message);
}

inline
void throw_precondition_error(bool predicate, const std::string& message)
{
    if (!predicate)
        throw ipaca::PreconditionViolation(message);
}

inline
void throw_postcondition_error(bool predicate, const std::string& message)
{
    if (!predicate)
        throw ipaca::PostconditionViolation(message);
}

///////////////////////////////////////////////
// Macros to write quick throwing statements //
///////////////////////////////////////////////

/**
 * Throws a ipaca::PreconditionViolation, if the PREDICATE is false.
 */
#define ipaca_precondition(PREDICATE, MESSAGE) ipaca::throw_precondition_error((PREDICATE), MESSAGE)
/**
 * Throws a ipaca::PostconditionViolation, if the PREDICATE is false.
 */
#define ipaca_postcondition(PREDICATE, MESSAGE) ipaca::throw_postcondition_error((PREDICATE), MESSAGE)
/**
 * Throws a ipaca::InvariantViolation, if the PREDICATE is false.
 */
#define ipaca_invariant(PREDICATE, MESSAGE) ipaca::throw_invariant_error((PREDICATE), MESSAGE)
/**
 * Throws a RuntimeError.
 */
#define ipaca_fail(MESSAGE) throw ipaca::RuntimeError(MESSAGE)
} /* namespace ipaca */
#endif /* __IPACA_INCLUDE_IPACA_ERROR_H__ */
