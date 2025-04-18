// jacobi.cpp: Jacobi iterative method
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
// Authors: Theodore Omtzigt, Allan Leal
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#ifdef _MSC_VER
#pragma warning(disable : 4514)   // unreferenced inline function has been removed
#pragma warning(disable : 4710)   // 'int sprintf_s(char *const ,const size_t,const char *const ,...)': function not inlined
#pragma warning(disable : 4820)   // 'sw::universal::value<23>': '3' bytes padding added after data member 'sw::universal::value<23>::_sign'
#pragma warning(disable : 5045)   // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#endif

// standard library
#include <limits>
// Configure the posit library with arithmetic exceptions
// enable posit arithmetic exceptions
#define POSIT_THROW_ARITHMETIC_EXCEPTION 1
#include <universal/number/posit/posit.hpp>
#include <universal/blas/blas.hpp>
#include <universal/blas/generators.hpp>
#include <universal/blas/solvers/jacobi.hpp>


// specialized for native floating-point
template<typename Scalar,
	typename = typename std::enable_if<std::is_floating_point<Scalar>::value>::type>
Scalar normL1(const sw::universal::blas::vector<float>& v) {
	float L1Norm{ 0 };
	for (auto e : v) {
		L1Norm += std::abs(e);
	}
	return L1Norm;
}

int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::universal;
	using namespace sw::universal::blas;

//	constexpr size_t nbits = 16;
//	constexpr size_t es = 1;
//	using Scalar = posit<nbits, es>;
	using Scalar = float;
	using Matrix = sw::universal::blas::matrix<Scalar>;
	using Vector = sw::universal::blas::vector<Scalar>;

	if (argc == 1) cout << argv[0] << '\n';
	int nrOfFailedTestCases = 0;

	// Initialize 'A' 'b' & intial guess 'x' * _
	Matrix A = { 
		{ 5, -2,  3,  0},
		{-3,  9,  1, -2},
		{ 2, -1, -7,  1},
		{ 4,  3, -5,  7} };
	Vector b = { -1, 2, 3, 0.5 };
	Vector x = {  0, 0, 0, 0 };

	cout << A << endl;
	cout << b << endl;
	size_t iterations = Jacobi(A, b, x);
	cout << "solution in " << iterations << " iterations" << endl;
	cout << "solution is " << x << '\n';
	cout << A * x << " = " << b << endl;

	return (nrOfFailedTestCases > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}
catch (char const* msg) {
	std::cerr << "Caught exception: " << msg << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::posit_arithmetic_exception& err) {
	std::cerr << "Uncaught posit arithmetic exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::quire_exception& err) {
	std::cerr << "Uncaught quire exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::posit_internal_exception& err) {
	std::cerr << "Uncaught posit internal exception: " << err.what() << std::endl;
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
