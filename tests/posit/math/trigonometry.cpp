﻿// function_trigonometry.cpp: test suite runner for trigonometric functions (sin/cos/tan/atan/acos/asin)
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <universal/utility/directives.hpp>

// when you define POSIT_VERBOSE_OUTPUT the code will print intermediate results for selected arithmetic operations
//#define POSIT_VERBOSE_OUTPUT
#define POSIT_TRACE_SQRT

// use default number system library configuration
#include <universal/number/posit/posit.hpp>
#include <universal/verification/posit_math_test_suite.hpp>

/* 
Writes result sine result sin(πa) to the location pointed to by sp
Writes result cosine result cos(πa) to the location pointed to by cp

In extensive testing, no errors > 0.97 ulp were found in either the sine
or cosine results, suggesting the results returned are faithfully rounded.

Reference:
https://stackoverflow.com/questions/42792939/implementation-of-sinpi-and-cospi-using-standard-c-math-library
*/
void my_sincospi(double a, double *sp, double *cp)
{
	double c, r, s, t, az;
	int64_t i;

	az = a * 0.0; // must be evaluated with IEEE-754 semantics
				  /* for |a| >= 2**53, cospi(a) = 1.0, but cospi(Inf) = NaN */
	a = (fabs(a) < 9.0071992547409920e+15) ? a : az;  // 0x1.0p53
													  /* reduce argument to primary approximation interval (-0.25, 0.25) */
	r = nearbyint(a + a); // must use IEEE-754 "to nearest" rounding
	i = (int64_t)r;
	t = fma(-0.5, r, a);
	/* compute core approximations */
	s = t * t;
	/* Approximate cos(pi*x) for x in [-0.25,0.25] */
	r = -1.0369917389758117e-4;
	r = fma(r, s, 1.9294935641298806e-3);
	r = fma(r, s, -2.5806887942825395e-2);
	r = fma(r, s, 2.3533063028328211e-1);
	r = fma(r, s, -1.3352627688538006e+0);
	r = fma(r, s, 4.0587121264167623e+0);
	r = fma(r, s, -4.9348022005446790e+0);
	c = fma(r, s, 1.0000000000000000e+0);
	/* Approximate sin(pi*x) for x in [-0.25,0.25] */
	r = 4.6151442520157035e-4;
	r = fma(r, s, -7.3700183130883555e-3);
	r = fma(r, s, 8.2145868949323936e-2);
	r = fma(r, s, -5.9926452893214921e-1);
	r = fma(r, s, 2.5501640398732688e+0);
	r = fma(r, s, -5.1677127800499516e+0);
	s = s * t;
	r = r * s;
	s = fma(t, 3.1415926535897931e+0, r);
	/* map results according to quadrant */
	if (i & 2) {
		s = 0.0 - s; // must be evaluated with IEEE-754 semantics
		c = 0.0 - c; // must be evaluated with IEEE-754 semantics
	}
	if (i & 1) {
		t = 0.0 - s; // must be evaluated with IEEE-754 semantics
		s = c;
		c = t;
	}
	/* IEEE-754: sinPi(+n) is +0 and sinPi(-n) is -0 for positive integers n */
	if (a == floor(a)) s = az;
	*sp = s;
	*cp = c;
}

double sinpi(double arg) {
	double s, c;
	my_sincospi(arg, &s, &c);
	return s;
}

double cospi(double arg) {
	double s, c;
	my_sincospi(arg, &s, &c);
	return c;
}

#ifdef CPP17_HEXFLOAT_LITERALS
/* 
Writes result sine result sin(πa) to the location pointed to by sp
Writes result cosine result cos(πa) to the location pointed to by cp

In exhaustive testing, the maximum error in sine results was 0.96677 ulp,
the maximum error in cosine results was 0.96563 ulp, meaning results are
faithfully rounded.
*/
void my_sincospif(float a, float *sp, float *cp)
{
	using namespace std;
	float az, t, c, r, s;
	int32_t i;

	az = a * 0.0f; // must be evaluated with IEEE-754 semantics
				   /* for |a| > 2**24, cospi(a) = 1.0f, but cospi(Inf) = NaN */
	a = (fabsf(a) < 0x1.0p24f) ? a : az;
	r = nearbyintf(a + a); // must use IEEE-754 "to nearest" rounding
	i = (int32_t)r;
	t = fmaf(-0.5f, r, a);
	/* compute core approximations */
	s = t * t;
	/* Approximate cos(pi*x) for x in [-0.25,0.25] */
	r = 0x1.d9e000p-3f;
	r = fmaf(r, s, -0x1.55c400p+0f);
	r = fmaf(r, s, 0x1.03c1cep+2f);
	r = fmaf(r, s, -0x1.3bd3ccp+2f);
	c = fmaf(r, s, 0x1.000000p+0f);
	/* Approximate sin(pi*x) for x in [-0.25,0.25] */
	r = -0x1.310000p-1f;
	r = fmaf(r, s, 0x1.46737ep+1f);
	r = fmaf(r, s, -0x1.4abbfep+2f);
	r = (t * s) * r;
	s = fmaf(t, 0x1.921fb6p+1f, r);
	if (i & 2) {
		s = 0.0f - s; // must be evaluated with IEEE-754 semantics
		c = 0.0f - c; // must be evaluated with IEEE-754 semantics
	}
	if (i & 1) {
		t = 0.0f - s; // must be evaluated with IEEE-754 semantics
		s = c;
		c = t;
	}
	/* IEEE-754: sinPi(+n) is +0 and sinPi(-n) is -0 for positive integers n */
	if (a == floorf(a)) s = az;
	*sp = s;
	*cp = c;
}
#endif

/* 
This function computes the great-circle distance of two points on earth
using the Haversine formula, assuming spherical shape of the planet. A
well-known numerical issue with the formula is reduced accuracy in the
case of near antipodal points.

lat1, lon1  latitude and longitude of first point, in degrees [-90,+90]
lat2, lon2  latitude and longitude of second point, in degrees [-180,+180]
radius      radius of the earth in user-defined units, e.g. 6378.2 km or
3963.2 miles

returns:    distance of the two points, in the same units as radius

Reference: http://en.wikipedia.org/wiki/Great-circle_distance
*/
double haversine(double lat1, double lon1, double lat2, double lon2, double radius) {
	double dlat, dlon, c1, c2, d1, d2, a, c, t;

	c1 = cospi(lat1 / 180.0);
	c2 = cospi(lat2 / 180.0);
	dlat = lat2 - lat1;
	dlon = lon2 - lon1;
	d1 = sinpi(dlat / 360.0);
	d2 = sinpi(dlon / 360.0);
	t = d2 * d2 * c1 * c2;
	a = d1 * d1 + t;
	c = 2.0 * asin(fmin(1.0, sqrt(a)));
	return radius * c;
}

// generate specific test case that you can trace with the trace conditions in posit.h
// for most bugs they are traceable with _trace_conversion and _trace_add
template<size_t nbits, size_t es, typename Ty>
void GenerateTestCase(Ty a) {
	Ty ref;
	sw::universal::posit<nbits, es> pa, pref, psin;
	pa = a;
	ref = std::sin(a);
	pref = ref;
	psin = sw::universal::sin(pa);
	std::cout << std::setprecision(nbits - 2);
	std::cout << std::setw(nbits) << a << " -> sin(" << a << ") = " << std::setw(nbits) << ref << std::endl;
	std::cout << pa.get() << " -> sin( " << pa << ") = " << psin.get() << " (reference: " << pref.get() << ")   " ;
	std::cout << (pref == psin ? "PASS" : "FAIL") << std::endl << std::endl;
	std::cout << std::setprecision(5);
}

#define MANUAL_TESTING 1
#define STRESS_TESTING 0


int main()
try {
	using namespace std;
	using namespace sw::universal;

	//bool bReportIndividualTestCases = true;
	int nrOfFailedTestCases = 0;

	std::string tag = "Addition failed: ";

#if MANUAL_TESTING
	// generate individual testcases to hand trace/debug
//	GenerateTestCase<8, 0, double>(m_pi);
//	GenerateTestCase<16, 1, double>(m_pi);
//	GenerateTestCase<32, 2, double>(m_pi);
//	GenerateTestCase<64, 3, float>(m_pi);
//	GenerateTestCase<128, 4, float>(m_pi);
//	GenerateTestCase<256, 5, float>(m_pi);

	cout << "Standard sin(pi/2) : " << std::sin(m_pi*0.5) << " vs sinpi(0.5): " << sinpi(0.5) << endl;
	cout << "Standard sin(pi)   : " << std::sin(m_pi)     << " vs sinpi(1.0): " << sinpi(1.0) << endl;
	cout << "Standard sin(3pi/2): " << std::sin(m_pi*1.5) << " vs sinpi(1.5): " << sinpi(1.5) << endl;
	cout << "Standard sin(2pi)  : " << std::sin(m_pi*2)   << " vs sinpi(2.0): " << sinpi(2.0) << endl;

	cout << "haversine(0.0, 0.0, 90.0, 0.0, 1.0)  = " << haversine(0.0, 0.0, 90.0, 0.0, 1.0) << endl;
	cout << "haversine(0.0, 0.0, 180.0, 0.0, 1.0)  = " << haversine(0.0, 0.0, 180, 0.0, 1.0) << endl;

	GenerateTestCase<16, 1, double>(m_pi_2);

	cout << endl;

	// manual exhaustive test
	nrOfFailedTestCases += ReportTestResult(VerifySine<2, 0>(true), "posit<2,0>", "sin");

	nrOfFailedTestCases += ReportTestResult(VerifySine<3, 0>(true), "posit<3,0>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<3, 1>(true), "posit<3,1>", "sin");

	nrOfFailedTestCases += ReportTestResult(VerifySine<4, 0>(true), "posit<4,0>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<4, 1>(true), "posit<4,1>", "sin");

	nrOfFailedTestCases += ReportTestResult(VerifySine<5, 0>(true), "posit<5,0>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<5, 1>(true), "posit<5,1>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<5, 2>(true), "posit<5,2>", "sin");

	nrOfFailedTestCases += ReportTestResult(VerifySine<8, 0>(true), "posit<8,0>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifyCosine<8, 0>(true), "posit<8,0>", "cos");
	nrOfFailedTestCases += ReportTestResult(VerifyTangent<8, 0>(true), "posit<8,0>", "tan");
	nrOfFailedTestCases += ReportTestResult(VerifyAtan<8, 0>(true), "posit<8,0>", "atan");
	nrOfFailedTestCases += ReportTestResult(VerifyAsin<8, 0>(true), "posit<8,0>", "asin");
	nrOfFailedTestCases += ReportTestResult(VerifyAcos<8, 0>(true), "posit<8,0>", "acos");
#else

	cout << "Posit sine function validation" << endl;

	nrOfFailedTestCases += ReportTestResult(VerifySine<2, 0>(bReportIndividualTestCases), "posit<2,0>", "sin");

	nrOfFailedTestCases += ReportTestResult(VerifySine<3, 0>(bReportIndividualTestCases), "posit<3,0>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<3, 1>(bReportIndividualTestCases), "posit<3,1>", "sin");

	nrOfFailedTestCases += ReportTestResult(VerifySine<4, 0>(bReportIndividualTestCases), "posit<4,0>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<4, 1>(bReportIndividualTestCases), "posit<4,1>", "sin");

	nrOfFailedTestCases += ReportTestResult(VerifySine<5, 0>(bReportIndividualTestCases), "posit<5,0>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<5, 1>(bReportIndividualTestCases), "posit<5,1>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<5, 2>(bReportIndividualTestCases), "posit<5,2>", "sin");

	nrOfFailedTestCases += ReportTestResult(VerifySine<6, 0>(bReportIndividualTestCases), "posit<6,0>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<6, 1>(bReportIndividualTestCases), "posit<6,1>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<6, 2>(bReportIndividualTestCases), "posit<6,2>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<6, 3>(bReportIndividualTestCases), "posit<6,3>", "sin");

	nrOfFailedTestCases += ReportTestResult(VerifySine<7, 0>(bReportIndividualTestCases), "posit<7,0>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<7, 1>(bReportIndividualTestCases), "posit<7,1>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<7, 2>(bReportIndividualTestCases), "posit<7,2>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<7, 3>(bReportIndividualTestCases), "posit<7,3>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<7, 4>(bReportIndividualTestCases), "posit<7,4>", "sin");

	nrOfFailedTestCases += ReportTestResult(VerifySine<8, 0>(bReportIndividualTestCases), "posit<8,0>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<8, 1>(bReportIndividualTestCases), "posit<8,1>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<8, 2>(bReportIndividualTestCases), "posit<8,2>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<8, 3>(bReportIndividualTestCases), "posit<8,3>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<8, 4>(bReportIndividualTestCases), "posit<8,4>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<8, 5>(bReportIndividualTestCases), "posit<8,5>", "sin");

	nrOfFailedTestCases += ReportTestResult(VerifySine<9, 0>(bReportIndividualTestCases), "posit<9,0>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<9, 1>(bReportIndividualTestCases), "posit<9,1>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<9, 2>(bReportIndividualTestCases), "posit<9,2>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<9, 3>(bReportIndividualTestCases), "posit<9,3>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<9, 4>(bReportIndividualTestCases), "posit<9,4>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<9, 5>(bReportIndividualTestCases), "posit<9,5>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<9, 6>(bReportIndividualTestCases), "posit<9,6>", "sin");
	
	nrOfFailedTestCases += ReportTestResult(VerifySine<10, 0>(bReportIndividualTestCases), "posit<10,0>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<10, 1>(bReportIndividualTestCases), "posit<10,1>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<10, 2>(bReportIndividualTestCases), "posit<10,2>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<10, 7>(bReportIndividualTestCases), "posit<10,7>", "sin");

	nrOfFailedTestCases += ReportTestResult(VerifySine<12, 0>(bReportIndividualTestCases), "posit<12,0>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<12, 1>(bReportIndividualTestCases), "posit<12,1>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<12, 2>(bReportIndividualTestCases), "posit<12,2>", "sin");

	nrOfFailedTestCases += ReportTestResult(VerifySine<16, 0>(bReportIndividualTestCases), "posit<16,0>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<16, 1>(bReportIndividualTestCases), "posit<16,1>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<16, 2>(bReportIndividualTestCases), "posit<16,2>", "sin");


#if STRESS_TESTING
	// nbits=64 requires long double compiler support
	// nrOfFailedTestCases += ReportTestResult(VerifyThroughRandoms<64, 2>(bReportIndividualTestCases, OPCODE_SQRT, 1000), "posit<64,2>", "sin");


	nrOfFailedTestCases += ReportTestResult(VerifySine<10, 1>(bReportIndividualTestCases), "posit<10,1>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<12, 1>(bReportIndividualTestCases), "posit<12,1>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<14, 1>(bReportIndividualTestCases), "posit<14,1>", "sin");
	nrOfFailedTestCases += ReportTestResult(VerifySine<16, 1>(bReportIndividualTestCases), "posit<16,1>", "sin");
	
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
