// numeric_limits.cpp: test suite runner of the numeric_limits specialization for posits
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <universal/utility/number_system_properties.hpp> //minmax_range etc. for native types
#include <universal/number/posit/posit.hpp>
#include <universal/verification/posit_math_test_suite.hpp>

int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::universal;

	//bool bReportIndividualTestCases = false;
	int nrOfFailedTestCases = 0;

	streamsize precision = cout.precision();

	numberTraits<int16_t>(cout);
	numberTraits<float>(cout);
	numberTraits<posit<32, 2>>(cout);

	cout << minmax_range<float>() << endl;
	cout << minmax_range<posit<32, 2>>() << endl;

	cout << dynamic_range<float>() << endl;
	cout << dynamic_range<posit<32, 2>>() << endl;

	cout << symmetry<float>() << endl;
	cout << symmetry<posit<32, 2>>() << endl;

	using Float = float;
	using Posit = sw::universal::posit<32, 2>;
	compareNumberTraits<Float, Posit>(cout);

	cout << endl;
	cout << "std::numeric_limits<T>::min():\n"
		<< "\tfloat: " << std::numeric_limits<float>::min()
		<< " or " << std::hexfloat << std::numeric_limits<float>::min() << '\n'
		<< "\tdouble: " << std::defaultfloat << std::numeric_limits<double>::min()
		<< " or " << std::hexfloat << std::numeric_limits<double>::min() << '\n';
	cout << "std::numeric_limits<T>::lowest():\n"
		<< "\tfloat: " << std::defaultfloat << std::numeric_limits<float>::lowest()
		<< " or " << std::hexfloat << std::numeric_limits<float>::lowest() << '\n'
		<< "\tdouble: " << std::defaultfloat << std::numeric_limits<double>::lowest()
		<< " or " << std::hexfloat << std::numeric_limits<double>::lowest() << '\n';
	cout << "std::numeric_limits<T>::max():\n"
		<< "\tfloat: " << std::defaultfloat << std::numeric_limits<float>::max()
		<< " or " << std::hexfloat << std::numeric_limits<float>::max() << '\n'
		<< "\tdouble: " << std::defaultfloat << std::numeric_limits<double>::max()
		<< " or " << std::hexfloat << std::numeric_limits<double>::max() << '\n';

	cout << setprecision(precision);

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
