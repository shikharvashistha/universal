// arbitrary fixed-point arithmetic type standard header
//
// Copyright (C) 2017-2020 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#ifndef _FIXPNT_STANDARD_HEADER_
#define _FIXPNT_STANDARD_HEADER_

////////////////////////////////////////////////////////////////////////////////////////
///  BEHAVIORAL COMPILATION SWITCHES

////////////////////////////////////////////////////////////////////////////////////////
// enable/disable the ability to use literals in binary logic and arithmetic operators
#if !defined(FIXPNT_ENABLE_LITERALS)
// default is to enable them
#define FIXPNT_ENABLE_LITERALS 1
#endif

////////////////////////////////////////////////////////////////////////////////////////
// enable throwing specific exceptions for integer arithmetic errors
// left to application to enable
#if !defined(FIXPNT_THROW_ARITHMETIC_EXCEPTION)
// default is to use std::cerr for signalling an error
#define FIXPNT_THROW_ARITHMETIC_EXCEPTION 0
#endif

////////////////////////////////////////////////////////////////////////////////////////
/// INCLUDE FILES that make up the library
#include <universal/number/fixpnt/fixpnt_impl.hpp>
#include <universal/number/fixpnt/numeric_limits.hpp>
#include <universal/number/fixpnt/exceptions.hpp>
#include <universal/traits/fixpnt_traits.hpp>

///////////////////////////////////////////////////////////////////////////////////////
/// math functions
#include <universal/number/fixpnt/math_functions.hpp>

#endif
