// arithmetic_add.cpp: test suite runner for posit addition
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.

// Configure the posit template environment
// first: enable general or specialized posit configurations
//#define POSIT_FAST_SPECIALIZATION
// second: enable/disable posit arithmetic exceptions
#define POSIT_THROW_ARITHMETIC_EXCEPTION 0
// third: enable tracing 
// when you define POSIT_VERBOSE_OUTPUT executing an ADD the code will print intermediate results
//#define POSIT_VERBOSE_OUTPUT
#define POSIT_TRACE_ADD

// minimum set of include files to reflect source code dependencies
#include <universal/number/posit/posit_impl.hpp>
#include <universal/number/posit/numeric_limits.hpp>
#include <universal/number/posit/specializations.hpp>
#include <universal/number/posit/math_functions.hpp>
#include <universal/number/posit/manipulators.hpp>
#include <universal/verification/posit_test_suite.hpp>
#include <universal/verification/posit_test_randoms.hpp>

// generate specific test case that you can trace with the trace conditions in posit.h
// for most bugs they are traceable with _trace_conversion and _trace_add
template<size_t nbits, size_t es, typename Ty>
void GenerateTestCase(Ty a, Ty b) {
	Ty ref;
	sw::universal::posit<nbits, es> pa, pb, pref, psum;
	pa = a;
	pb = b;
	ref = a + b;
	pref = ref;
	psum = pa + pb;
	std::streamsize oldPrecision = std::cout.precision();
	std::cout << std::setprecision(nbits - 2);
	std::cout << std::setw(nbits) << a << " + " << std::setw(nbits) << b << " = " << std::setw(nbits) << ref << std::endl;
	std::cout << pa.get() << " + " << pb.get() << " = " << psum.get() << " (reference: " << pref.get() << ")   " ;
	std::cout << (pref == psum ? "PASS" : "FAIL") << std::endl << std::endl;
	std::cout << std::dec << std::setprecision(oldPrecision);
}

#define MANUAL_TESTING 0
#define STRESS_TESTING 0

int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::universal;

	bool bReportIndividualTestCases = false;
	int nrOfFailedTestCases = 0;

	std::string tag = "Addition failed: ";

#if MANUAL_TESTING
	// generate individual testcases to hand trace/debug
	GenerateTestCase<6, 3, double>(INFINITY, INFINITY);
	GenerateTestCase<8, 4, float>(0.5f, -0.5f);
	GenerateTestCase<3, 0>(0.5f, 1.0f);

	// manual exhaustive test
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<3, 0>("Manual Testing", true), "posit<3,0>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<3, 1>("Manual Testing", true), "posit<3,1>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<3, 2>("Manual Testing", true), "posit<3,2>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<3, 3>("Manual Testing", true), "posit<3,3>", "addition");


//	nrOfFailedTestCases += ReportTestResult(VerifyThroughRandoms<16, 1>(tag, true, OPCODE_ADD, 1000), "posit<16,1>", "addition");
//	nrOfFailedTestCases += ReportTestResult(VerifyThroughRandoms<64, 2>(tag, true, OPCODE_ADD, 1000), "posit<64,2>", "addition");

#else

	cout << "Posit addition validation" << endl;

	nrOfFailedTestCases += ReportTestResult(VerifyAddition<2, 0>(bReportIndividualTestCases), "posit<2,0>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyAddition<3, 0>(bReportIndividualTestCases), "posit<3,0>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<3, 1>(bReportIndividualTestCases), "posit<3,1>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyAddition<4, 0>(bReportIndividualTestCases), "posit<4,0>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<4, 1>(bReportIndividualTestCases), "posit<4,1>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<4, 2>(bReportIndividualTestCases), "posit<4,2>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyAddition<5, 0>(bReportIndividualTestCases), "posit<5,0>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<5, 1>(bReportIndividualTestCases), "posit<5,1>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<5, 2>(bReportIndividualTestCases), "posit<5,2>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<5, 3>(bReportIndividualTestCases), "posit<5,3>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyAddition<6, 0>(bReportIndividualTestCases), "posit<6,0>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<6, 1>(bReportIndividualTestCases), "posit<6,1>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<6, 2>(bReportIndividualTestCases), "posit<6,2>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<6, 3>(bReportIndividualTestCases), "posit<6,3>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<6, 4>(bReportIndividualTestCases), "posit<6,4>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyAddition<7, 0>(bReportIndividualTestCases), "posit<7,0>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<7, 1>(bReportIndividualTestCases), "posit<7,1>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<7, 2>(bReportIndividualTestCases), "posit<7,2>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<7, 3>(bReportIndividualTestCases), "posit<7,3>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<7, 4>(bReportIndividualTestCases), "posit<7,4>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<7, 5>(bReportIndividualTestCases), "posit<7,5>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyAddition<8, 0>(bReportIndividualTestCases), "posit<8,0>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<8, 1>(bReportIndividualTestCases), "posit<8,1>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<8, 2>(bReportIndividualTestCases), "posit<8,2>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<8, 3>(bReportIndividualTestCases), "posit<8,3>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<8, 4>(bReportIndividualTestCases), "posit<8,4>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<8, 5>(bReportIndividualTestCases), "posit<8,5>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<8, 6>(bReportIndividualTestCases), "posit<8,6>", "addition");

	nrOfFailedTestCases += ReportTestResult(VerifyBinaryOperatorThroughRandoms<16, 1>(bReportIndividualTestCases, OPCODE_ADD, 1000), "posit<16,1>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyBinaryOperatorThroughRandoms<24, 1>(bReportIndividualTestCases, OPCODE_ADD, 1000), "posit<24,1>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyBinaryOperatorThroughRandoms<32, 1>(bReportIndividualTestCases, OPCODE_ADD, 1000), "posit<32,1>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyBinaryOperatorThroughRandoms<32, 2>(bReportIndividualTestCases, OPCODE_ADD, 1000), "posit<32,2>", "addition");

#if STRESS_TESTING
	// nbits=48 also shows failures
	nrOfFailedTestCases += ReportTestResult(VerifyThroughRandoms<48, 2>(bReportIndividualTestCases, OPCODE_ADD, 1000), "posit<48,2>", "addition");

	// nbits=64 requires long double compiler support
	nrOfFailedTestCases += ReportTestResult(VerifyThroughRandoms<64, 2>(bReportIndividualTestCases, OPCODE_ADD, 1000), "posit<64,2>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyThroughRandoms<64, 3>(bReportIndividualTestCases, OPCODE_ADD, 1000), "posit<64,3>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyThroughRandoms<64, 4>(bReportIndividualTestCases, OPCODE_ADD, 1000), "posit<64,4>", "addition");


	nrOfFailedTestCases += ReportTestResult(VerifyAddition<10, 1>(bReportIndividualTestCases), "posit<10,1>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<12, 1>(bReportIndividualTestCases), "posit<12,1>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<14, 1>(bReportIndividualTestCases), "posit<14,1>", "addition");
	nrOfFailedTestCases += ReportTestResult(VerifyAddition<16, 1>(bReportIndividualTestCases), "posit<16,1>", "addition");
#endif  // STRESS_TESTING

#endif  // MANUAL_TESTING

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
