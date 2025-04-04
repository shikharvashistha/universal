// complex.cpp: api to use fixpnt type in complex arithmetic operations
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include<universal/utility/directives.hpp>
#include <iostream>
#include <bitset>
#include <complex>
// Configure the fixpnt template environment
// first: enable general or specialized fixed-point configurations
#define FIXPNT_FAST_SPECIALIZATION
// second: enable fixpnt arithmetic exceptions
#define FIXPNT_THROW_ARITHMETIC_EXCEPTION 1

// minimum set of include files to reflect source code dependencies
#include <universal/number/fixpnt/fixpnt_impl.hpp>
#include <universal/number/fixpnt/manipulators.hpp>
#include <universal/number/fixpnt/math_functions.hpp>
#include <universal/verification/fixpnt_test_suite.hpp>

namespace sw::universal::complex_literals {

	std::complex<fixpnt<8, 4>> operator""_i(long double _Val)
	{	// return imaginary _Val
		return (std::complex<fixpnt<8, 4>>(0.0, static_cast<fixpnt<8, 4>>(_Val)));
	}

	std::complex<fixpnt<8, 4>> operator""_i(unsigned long long _Val)
	{	// return imaginary _Val
		return (std::complex<fixpnt<8, 4>>(0.0, static_cast<fixpnt<8, 4>>(_Val)));
	}

} // namespace sw::universal::complex_literals


namespace sw::universal {

	template<typename FixedPoint>
	bool isnan(std::complex<FixedPoint> x) {
		return (isnan(x.real()) || isnan(x.imag()));
	}
	template<typename FixedPoint>
	bool isinf(std::complex<FixedPoint> x) {
		return (isinf(x.real()) || isinf(x.imag()));
	}
	template<typename FixedPoint>
	std::complex<FixedPoint> copysign(std::complex<FixedPoint> x, std::complex<FixedPoint> y) {
		return std::complex<FixedPoint>(copysign(x.real(), y.real()), copysign(x.real(), y.real()));
	}

}

// conditional compile flags
#define MANUAL_TESTING 1
#define STRESS_TESTING 0

int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::universal;

	if (argc > 0) { cout << argv[0] << endl; }

	int nrOfFailedTestCases = 0;

#if MANUAL_TESTING

#if defined(__GNUG__)
	{
		// reference using native double precision floating point type
		using namespace std::complex_literals;
		std::cout << std::fixed << std::setprecision(1);

		std::complex<double> z1 = 1i * 1i;     // imaginary unit squared
		std::cout << "i * i = " << z1 << '\n';

//		std::complex<double> z2 = std::pow(1.0i, 2.0); // imaginary unit squared
//		std::cout << "pow(i, 2) = " << z2 << '\n';

//		double PI = std::acos(-1);
//		std::complex<double> z3 = std::exp(1i * PI); // Euler's formula
//		std::cout << "exp(i * pi) = " << z3 << '\n';

		std::complex<double> z4{1.0, +2.0}, z5{1.0, -2.0}; // conjugates
		std::cout << "(1+2i)*(1-2i) = " << z4 * z5 << '\n';
	}

#undef GPP_FIX
#ifdef GPP_FIX
	// for some reason the g++ doesn't compile this section as it is 
	// casting the constants differently than other compilers.
	// TODO: no idea how to fix the code below to make it compile with g++
	{
		using namespace sw::universal::complex_literals;
		using Real = sw::universal::fixpnt<8, 4>;
		std::cout << std::fixed << std::setprecision(1);

		std::complex<Real> z1 = 1i * 1i;     // imaginary unit squared
		std::cout << "i * i = " << z1 << '\n';

		std::complex<double> z2 = std::pow(1.0i, 2.0); // imaginary unit squared
		std::cout << "pow(i, 2) = " << z2 << '\n';

		double PI = std::acos(-1);
		std::complex<Real> z3 = std::exp(1.0i * PI); // Euler's formula
		std::cout << "exp(i * pi) = " << z3 << '\n';

		std::complex<Real> z4 = 1.0 + 2i, z5 = 1.0 - 2i; // conjugates
		std::cout << "(1+2i)*(1-2i) = " << z4 * z5 << '\n';
	}
#endif // !GPP_FIX
/*
	error: conversion from '__complex__ int' to non - scalar type 'std::complex<sw::universal::fixpnt<8, 4> >' requested
		std::complex<Real> z1 = 1i * 1i;     // imaginary unit squared
		                        ~~~^~~~
	error : conversion from '__complex__ double' to non - scalar type 'std::complex<sw::universal::fixpnt<8, 4> >' requested
        std::complex<Real> z4 = 1.0 + 2i, z5 = 1.0 - 2i; // conjugates
		                        ~~~~^~~~
	error : conversion from '__complex__ double' to non - scalar type 'std::complex<sw::universal::fixpnt<8, 4> >' requested
        std::complex<Real> z4 = 1.0 + 2i, z5 = 1.0 - 2i; // conjugates
		                                       ~~~~^~~~
*/
    // furthermore, the pow and exp functions don't match the correct complex<double> arguments in g++

#else  // GPP_FIX

	{
		// reference using native double precision floating point type
		using namespace std::complex_literals;
		std::cout << std::fixed << std::setprecision(1);

		std::complex<double> z1 = 1i * 1i;     // imaginary unit squared
		std::cout << "i * i = " << z1 << '\n';

		// the pow and exp functions don't match in g++
		// no idea how to fix the code below to make it compile with g++
		std::complex<double> z2 = std::pow(1.0i, 2.0); // imaginary unit squared
		std::cout << "pow(i, 2) = " << z2 << '\n';

		double PI = std::acos(-1);
		std::complex<double> z3 = std::exp(1i * PI); // Euler's formula
		std::cout << "exp(i * pi) = " << z3 << '\n';

		std::complex<double> z4 = 1. + 2i, z5 = 1. - 2i; // conjugates
		std::cout << "(1+2i)*(1-2i) = " << z4 * z5 << '\n';
	}

	{
		// all the literals are marshalled through the std library double native type for complex literals
		// defining them for fixpnt is syntactically unattractive as the "i" literal is reserved for native types,
		// so our literal type would need to be non-standard anyway as "_i"
		using namespace sw::universal::complex_literals;
		using Real = sw::universal::fixpnt<8, 4>;
		std::cout << std::fixed << std::setprecision(1);

		std::complex<Real> z1 = 1i * 1i;     // imaginary unit squared
		std::cout << "i * i = " << z1 << '\n';

		std::complex<double> z2 = std::pow(1.0i, 2.0); // imaginary unit squared
		std::cout << "pow(i, 2) = " << z2 << '\n';

		double PI = std::acos(-1);
		std::complex<Real> z3 = std::exp(1.0i * PI); // Euler's formula
		std::cout << "exp(i * pi) = " << z3 << '\n';

		std::complex<Real> z4 = 1.0 + 2i, z5 = 1.0 - 2i; // conjugates
		std::cout << "(1+2i)*(1-2i) = " << z4 * z5 << '\n';
	}
#endif

	{
		using FixedPoint = sw::universal::fixpnt<4, 3, sw::universal::Saturating, uint8_t>;
		FixedPoint one = 1;
		FixedPoint minus_one = -1;
		FixedPoint fp = 1.0f;
		std::complex<FixedPoint> z1{ 1.0f, 1.0f }, z2{ minus_one, minus_one }, z3;
		std::cout << "z1 : " << z1 << '\n';
		std::cout << "z2 : " << z2 << '\n';
		z3 = std::complex<FixedPoint>(0.0f, 0.0f);
		std::cout << "z3 : " << z3 << '\n';
		fp = copysign(one, minus_one);
		std::cout << "copysign(0.875, -1) : " << fp << '\n';
		z3 = copysign(z1, z2);
		std::cout << "z3 : " << z3 << '\n';
	}

#if STRESS_TESTING
	// manual exhaustive test
#endif

#else

	cout << "Complex Fixed-point use cases" << endl;

#if STRESS_TESTING
	// automated stress test

#endif  // STRESS_TESTING

#endif  // MANUAL_TESTING

	return (nrOfFailedTestCases > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}
catch (char const* msg) {
	std::cerr << msg << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::fixpnt_arithmetic_exception& err) {
	std::cerr << "Uncaught fixpnt arithmetic exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::fixpnt_internal_exception& err) {
	std::cerr << "Uncaught fixpnt internal exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::runtime_error& err) {
	std::cerr << "Uncaught runtime exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (...) {
	std::cerr << "Caught unknown exception" << std::endl;
	return EXIT_FAILURE;
}
