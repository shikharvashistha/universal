// linspace.cpp: test suite for linspace/logspace/geomspace sequence generators
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

//constexpr double pi = 3.14159265358979323846;  // best practice for C++

template<typename Scalar>
void TestRangeGeneration() {
	using namespace std;
	using namespace sw::universal::blas;
	using std::pow;
	using Vector = sw::universal::blas::vector<Scalar>;
	Vector v = linspace<Scalar>(0, 10, 5);
	cout << "linspace = " << v << endl;
	v = linspace<Scalar>(0, 10, 5, false);
	cout << "linspace = " << v << endl;

	v = logspace<Scalar>(0, 10, 5);
	cout << "logspace = " << v << endl;
	v = logspace<Scalar>(0, 10, 5, false);
	cout << "logspace = " << v << endl;

	Scalar x{ 10 }, y{ 1.5 };
	cout << "x^y = " << pow(x, y) << endl;

	v = geomspace<Scalar>(0, 10, 5);
	cout << "geomspace = " << v << endl;
	v = geomspace<Scalar>(0, 10, 5, false);
	cout << "geomspace = " << v << endl;
}

int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::universal::blas;

	int nrOfFailedTestCases = 0;

    TestRangeGeneration<sw::universal::posit<32,2>>();
	TestRangeGeneration<float>();

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
