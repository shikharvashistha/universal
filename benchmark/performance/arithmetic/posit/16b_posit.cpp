// 16b_posit.cpp: performance characterization of standard posit<16,1> configuration
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.

// Configure the posit template environment
// first: enable fast specialized posit<16,1>
#define POSIT_FAST_POSIT_16_1 1
// second: disable posit arithmetic exceptions
#define POSIT_THROW_ARITHMETIC_EXCEPTION 0
#include <universal/number/posit/posit.hpp>
#include <universal/performance/number_system.hpp>

int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::universal;

	constexpr size_t nbits = 16;
	constexpr size_t es = 1;
	posit<nbits, es> number;
	OperatorPerformance perfReport;
	GeneratePerformanceReport(number, perfReport);
	cout << ReportPerformance(number, perfReport);
	cout << endl;
	return EXIT_SUCCESS;
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
