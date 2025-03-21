// rand_spectral.cpp: random matrix with a given spectrum
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#ifdef _MSC_VER
#pragma warning(disable : 4100) // argc/argv unreferenced formal parameter
#pragma warning(disable : 4514 4571)
#pragma warning(disable : 4625 4626) // 4625: copy constructor was implicitly defined as deleted, 4626: assignment operator was implicitely defined as deleted
#pragma warning(disable : 5025 5026 5027 5045)
#pragma warning(disable : 4710 4774)
#pragma warning(disable : 4820)
#endif
// enable the following define to show the intermediate steps in the fused-dot product
// #define POSIT_VERBOSE_OUTPUT
#define POSIT_TRACE_MUL
#define QUIRE_TRACE_ADD
// configure posit environment
#define POSIT_FAST_POSIT_8_0 1
#define POSIT_FAST_POSIT_16_1 1
#define POSIT_FAST_POSIT_32_2 1
// enable posit arithmetic exceptions
#define POSIT_THROW_ARITHMETIC_EXCEPTION 1
#include <universal/number/posit/posit.hpp>
#define BLAS_TRACE_ROUNDING_EVENTS 1
#include <universal/blas/blas.hpp>
#include <universal/blas/generators.hpp>


int main(int argc, char* argv[])
try {
	using namespace std;
	using namespace sw::universal::blas;


	if (argc == 1) cout << argv[0] << endl;

	// Av = lambda * v
	// AQ = Q * Sigma
	// A  = Q * Sigma * Q^-1
	
	// generate a random base matrix for Q
	using Scalar = float;
	using Matrix = matrix<Scalar>;
	constexpr size_t M = 5;
	constexpr size_t N = 2;
	Matrix Qbase = uniform_random<Matrix>(M, N);
	cout << Qbase << endl;

	// normalize the column vectors
	auto total = sum(Qbase);
	cout << "Total    : " << total << endl;
	auto rowSums = sum(Qbase, 1);
	cout << "Row sums : " << rowSums << endl;
	auto colSums = sum(Qbase, 2);
	cout << "Col sums : " << colSums << endl;

	normalize(Qbase, 2);  // normalize columns so they are unit length
	cout << Qbase << endl;
	auto colNorms = matrixNorm(Qbase, 2);
	cout << "Col norms: " << colNorms << endl;

	return EXIT_SUCCESS;
}
catch (char const* msg) {
	std::cerr << msg << std::endl;
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
