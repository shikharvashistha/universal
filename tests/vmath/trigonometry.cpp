// trigonometry.cpp: test suite for vectorized trigonometry math functions
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <cmath>
// Configure the posit library with arithmetic exceptions
// enable posit arithmetic exceptions
#define POSIT_THROW_ARITHMETIC_EXCEPTION 1
#include <universal/number/posit/posit.hpp>
#include <universal/blas/blas.hpp>

constexpr double PI = 3.14159265358979323846;  // best practice for C++

template<typename Scalar>
void TestTriangleVmath(size_t N = 12) {
	using namespace std;
	using namespace sw::universal::blas;
	using std::pow;
	using Vector = sw::universal::blas::vector<Scalar>;
	Vector v = linspace<Scalar>(0, 2*PI, N);
	cout << "radians  = " << v << endl;
	auto cosines = sw::universal::blas::cos(v);
	cout << "cosines  = " << cosines << endl;
	auto sines = sin(v);
	cout << "sines    = " << sines << endl;
	auto tangents = tan(v);
	cout << "tangents = " << tangents << endl;
}

int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::universal::blas;

	int nrOfFailedTestCases = 0;

	TestTriangleVmath<sw::universal::posit<32,2>>();
	TestTriangleVmath<float>();

	return (nrOfFailedTestCases > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
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
