//  quire_accumulations.cpp : computational path experiments with quires
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.

// set to 1 if you want to generate hw test vectors
#define HARDWARE_QA_OUTPUT 0

// type definitions for the important types, posit<> and quire<>
#include <universal/number/posit/posit_impl.hpp>
#include <universal/traits/posit_traits.hpp>
#include <universal/number/posit/quire.hpp>
#include <universal/number/posit/fdp.hpp>
#include <universal/verification/posit_test_suite.hpp>
#include <universal/verification/quire_test_suite.hpp>
#include <universal/utility/convert_to.hpp>

// if you want to enable ISSUE_45
//#define ISSUE_45_DEBUG
#ifdef ISSUE_45_DEBUG
// forward reference
template<size_t nbits, size_t es, size_t capacity> void Issue45_2();
#endif

template<size_t nbits, size_t es>
void PrintTestVector(std::ostream& ostr, const std::vector< sw::universal::posit<nbits,es> >& pv) {
	std::for_each (begin(pv), end(pv), [&ostr](const sw::universal::posit<nbits,es>& p){
		ostr << p << std::endl;
	});
}

template<size_t nbits, size_t es, size_t capacity>
int GenerateQuireAccumulationTestCase(bool bReportIndividualTestCases, size_t nrOfElements, const sw::universal::posit<nbits,es>& seed) {
	using namespace sw::universal;
	int nrOfFailedTestCases = 0;
	std::stringstream ss;
	ss << "quire<" << nbits << "," << es << "," << capacity << ">";
	std::vector< sw::universal::posit<nbits, es> > t = GenerateVectorForZeroValueFDP(nrOfElements, seed);
	nrOfFailedTestCases += ReportTestResult(ValidateQuireAccumulation<nbits, es, capacity>(bReportIndividualTestCases, t), ss.str(), "accumulation");
	return nrOfFailedTestCases;
}

// initialize a vector
template<typename Vector, typename Scalar>
void init(Vector& x, const Scalar& value) {
	for (size_t i = 0; i < x.size(); ++i) x[i] = value;
}

// regular dot product
template<typename Vector>
typename Vector::value_type dot(const Vector& a, const Vector& b) {
	typename Vector::value_type sum{ 0 };
	if (size(a) != size(b)) {
		std::cerr << "vectors are not the same size\n";
		return sum;
	}
	for (size_t i = 0; i < size(a); ++i) {
		sum += a[i] * b[i];
	}
	return sum;
}

template<size_t nbits, size_t es, size_t nrElements = 16>
int ValidateExactDotProduct() {
	using namespace std;
	using namespace sw::universal;
	int nrOfFailures = 0;
	using Scalar = posit<nbits, es>;
	using Vector = vector<Scalar>;
	Scalar maxpos;
	maxpos.maxpos();
	Vector pv = GenerateVectorForZeroValueFDP(nrElements, maxpos);
	Vector ones(nrElements);

	{
		init(ones, Scalar(1));

		Scalar result = fdp(ones, pv);
		cout << "exact FDP test yields   = " << float(result) << endl;

		if (!result.iszero()) ++nrOfFailures;
	}

	{
		using Vector = vector<float>;
		Vector fv;
		for_each(begin(pv), end(pv), [&fv](const Scalar& p) {
			fv.push_back(float(p));
		});
		Vector fones;
		for_each(begin(pv), end(pv), [&fones](const Scalar& p) {
			fones.push_back(float(p));
		});
		float result = dot(fones, fv);
		cout << "regular DOT test yields = " << result << endl << endl;
	}

	return nrOfFailures;
}

int ValidateQuireMagnitudeComparison() {
	using namespace std;
	using namespace sw::universal;

	quire<16, 1, 2> q;
	internal::value<20> v;
	v = 0xAAAA;
	q += v;
	v = 0xAAAB;
	cout << "quire: " << q << endl;
	cout << "value: " << v.get_fixed_point() << " " << to_triple(v) << endl;
	cout << (q < v ? "correct" : "incorrect") << endl;
	cout << (q > v ? "incorrect" : "correct") << endl;
	v = 0xAAAA;
	cout << "value: " << v.get_fixed_point() << " " << to_triple(v) << endl;
	cout << (q == v ? "correct" : "incorrect") << endl;
	return 0;
}

template<size_t nbits, size_t es, size_t capacity = 2>
int ValidateSignMagnitudeTransitions() {
	using namespace std;
	using namespace sw::universal;

	int nrOfFailedTestCases = 0;
	std::cout << "Quire configuration: quire<" << nbits << ", " << es << ", " << capacity << ">" << std::endl;

	// moving through the four quadrants of a sign/magnitude adder/subtractor
	posit<nbits, es> min1, min2, min3, min4;
	min1.minpos();                              // ...0001
	min2 = min1; min2++;                        // ...0010
	min3 = min2; min3++;                        // ...0011
	min4 = min3; min4++;                        // ...0100
	posit<nbits, es> max1, max2, max3, max4;
	max1.maxpos();                              // 01..111
	max2 = max1; --max2;                        // 01..110
	max3 = max2; --max3;                        // 01..101
	max4 = max3; --max4;                        // 01..100

	cout << endl;
	cout << "Posit range extremes:" << endl;
	cout << "min1 = minpos  " << min1.get() << " " << min1 << endl;
	cout << "min2           " << min2.get() << " " << min2 << endl;
	cout << "min3           " << min3.get() << " " << min3 << endl;
	cout << "min4           " << min4.get() << " " << min4 << endl;
	cout << "..." << endl;
	cout << "max4           " << max4.get() << " " << max4 << endl;
	cout << "max3           " << max3.get() << " " << max3 << endl;
	cout << "max2           " << max2.get() << " " << max2 << endl;
	cout << "max1 = maxpos  " << max1.get() << " " << max1 << endl;

	cout << endl;

	cout << "Quire experiments: sign/magnitude transitions at the range extremes" << endl;

	posit<nbits, es> one{ 1.0f };
	quire<nbits, es, capacity> q;
	internal::value<2 * (nbits - 2 - es)> addend;

		// show the relative positions of maxpos^2, maxpos, minpos, minpos^2
	q = addend = quire_mul(max1, max1);
	cout << q << " q == maxpos^2         = " << to_triple(addend) << endl;
	q = addend = quire_mul(max1, one);  // indicative that the quire 'sits' behind the ALU.
	cout << q << " q == maxpos           = " << to_triple(addend) << endl;
	q = addend = quire_mul(min1, one);  // indicative that the quire 'sits' behind the ALU.
	cout << q << " q == minpos           = " << to_triple(addend) << endl;
	q = addend = quire_mul(min1, min1);
	cout << q << " q == minpos^2         = " << to_triple(addend) << endl;

	// reset to zero
	q.clear();
	cout << q << "                                               <-- start at zero" << endl;
	// start in the positive, SE quadrant with minpos^2
	q += addend = quire_mul(min1, min1);
	cout << q << " q += minpos^2  addend = " << to_triple(addend) << endl;
	// move to the negative SW quadrant by adding negative value that is bigger
	q += addend = quire_mul(min2, -min2);
	cout << q << " q += min2^2    addend = " << to_triple(addend) << endl;
	// remove minpos^2 from the quire by subtracting it
	q -= addend = quire_mul(min1, min1);
	cout << q << " q -= minpos^2  addend = " << to_triple(addend) << endl;
	// move back into posit, SE quadrant by adding the next bigger product
	q += addend = quire_mul(min3, min3);
	cout << q << " q += min3^2    addend = " << to_triple(addend) << endl;
	// remove the min2^2 from the quire by subtracting it
	q -= addend = quire_mul(min2, min2);
	cout << q << " q -= min2^2    addend = " << to_triple(addend) << endl;
	// add a -maxpos^2, to flip it again
	q += addend = quire_mul(max1, -max1);
	cout << q << " q += -maxpos^2 addend = " << to_triple(addend) << endl;
	// subtract min3^2 to propagate the carry
	q -= addend = quire_mul(min3, min3);
	cout << q << " q -= min3^2    addend = " << to_triple(addend) << endl;
	// remove min2^2 remenants
	q += addend = quire_mul(min2, min2);
	cout << q << " q += min2^2    addend = " << to_triple(addend) << endl;
	q += addend = quire_mul(min2, min2);
	cout << q << " q += min2^2    addend = " << to_triple(addend) << endl;
	// borrow propagate
	q += addend = quire_mul(min1, min1);
	cout << q << " q += minpos^2  addend = " << to_triple(addend) << endl;
	// flip the max3 bit
	q += addend = quire_mul(max3, max3);
	cout << q << " q += max3^2    addend = " << to_triple(addend) << endl;
	// add maxpos^2 to be left with max3^2
	q += addend = quire_mul(max1, max1);
	cout << q << " q += maxpos^2  addend = " << to_triple(addend) << endl;;
	// subtract max2^2 to flip the sign again
	q -= addend = quire_mul(max2, max2);
	cout << q << " q -= max2^2    addend = " << to_triple(addend) << endl;
	// remove the max3^2 remenants
	q -= addend = quire_mul(max3, max3);
	cout << q << " q -= max3^2    addend = " << to_triple(addend) << endl;
	// remove the minpos^2 bits
	q -= addend = quire_mul(min1, min1);
	cout << q << " q -= minpos^2  addend = " << to_triple(addend) << endl;
	// add maxpos^2 to be left with max2^2 and flipped back to positive quadrant
	q += addend = quire_mul(max1, max1);
	cout << q << " q += maxpos^2  addend = " << to_triple(addend) << endl;
	// add max2^2 to remove its remenants
	q += addend = quire_mul(max2, max2);
	cout << q << " q += max2^2    addend = " << to_triple(addend) << endl;
	// subtract minpos^2 to propagate the borrow across the quire
	q -= addend = quire_mul(min1, min1);
	cout << q << " q -= minpos^2  addend = " << to_triple(addend) << endl;
	// subtract maxpos^2 to flip the sign and be left with minpos^2
	q -= addend = quire_mul(max1, max1);
	cout << q << " q -= maxpos^2  addend = " << to_triple(addend) << endl;
	// add minpos^2 to get to zero
	q += addend = quire_mul(min1, min1);
	cout << q << " q += minpos^2  addend = " << to_triple(addend) << endl;
	// subtract minpos^2 to go negative
	q += addend = -quire_mul(min1, min1);
	cout << q << " q += -minpos^2 addend = " << to_triple(addend) << endl;
	// add minpos^2 to get to zero
	q += addend = quire_mul(min1, min1);
	cout << q << " q += minpos^2  addend = " << to_triple(addend) << " <-- back to zero" << endl;

	return nrOfFailedTestCases;
}

template<size_t nbits, size_t es, size_t capacity = 2>
int ValidateCarryPropagation(bool bReportIndividualTestCases) {
	using namespace sw::universal;
	int nrOfFailedTests = 0;

	constexpr size_t mbits = 2 * (nbits - 2 - es);
	quire<nbits, es, capacity> q;
	posit<nbits, es> mp(SpecificValue::minpos);
	internal::value<mbits> minpos_square = quire_mul(mp, mp);
	constexpr size_t NR_INCREMENTS_TO_OVERFLOW = (size_t(1) << (q.qbits+1));
	for (size_t i = 0; i < NR_INCREMENTS_TO_OVERFLOW; ++i) {
		q += minpos_square;
	}
	std::cout << q << std::endl;
	nrOfFailedTests = q.iszero() ? 0 : 1;

	return nrOfFailedTests;
}

template<size_t nbits, size_t es, size_t capacity = 2>
int ValidateBorrowPropagation(bool bReportIndividualTestCases) {
	using namespace sw::universal;
	int nrOfFailedTests = 0;

	constexpr size_t mbits = 2 * (nbits - 2 - es);
	quire<nbits, es, capacity> q;
	posit<nbits, es> mp(SpecificValue::minpos);
	internal::value<mbits> minpos_square = quire_mul(mp, mp);
	q -= minpos_square;
	std::cout << q << std::endl;
	constexpr size_t NR_DECREMENTS_TO_OVERFLOW = (size_t(1) << (q.qbits + 1));
	for (size_t i = 0; i < NR_DECREMENTS_TO_OVERFLOW-1; ++i) {
		q -= minpos_square;
	}
	std::cout << q << std::endl;
	nrOfFailedTests = q.iszero() ? 0 : 1;

	return nrOfFailedTests;
}

template<size_t nbits, size_t es, size_t capacity = 2>
int ValidateQuireAccumulation(bool bReportIndividualTestCases) {
	int nrOfFailedTests = 0;

	return nrOfFailedTests;
}

// one of test to check that the quire can deal with 0
void TestCaseForProperZeroHandling() {
	using namespace std;
	using namespace sw::universal;

	quire<8, 1, 2> q;
	posit<8, 1> minpos;
	minpos.minpos();
	q += quire_mul(minpos, minpos);
	internal::value<3> v3 = q.to_value().round_to<3>();
	internal::value<5> v5 = q.to_value().round_to<5>();
	internal::value<7> v7 = q.to_value().round_to<7>();
	cout << to_triple(v3) << endl;
	cout << to_triple(v5) << endl;
	cout << to_triple(v7) << endl;

	// test correct handling of 0
	q = 1;
	cout << q << endl;
	posit<8, 1> one = 1;
	posit<8, 1> aThird = 0.3333333333333333333333333333333333333333333;
	internal::value< posit<8, 1>::mbits > mul = quire_mul(aThird, -one);
	cout << to_triple(mul) << endl;
	q += quire_mul(aThird, -one);
	cout << q << endl;
	internal::value<8> result = q.to_value().round_to<8>();
	cout << result << " " << to_triple(result) << endl;
}

#define MANUAL_TESTING 1
#define STRESS_TESTING 0

int main()
try {
	using namespace std;
	using namespace sw::universal;

	bool bReportIndividualTestCases = false;
	int nrOfFailedTestCases = 0;

	cout << "Quire experiments" << endl;

	std::string tag = "Quire Accumulation failed";

#if MANUAL_TESTING
	cout << "Quire load/store and add/subtract" << endl;
	posit<16, 1> p(1);
	quire<16, 1> q1(p);
	quire<16, 1> q2 = q1;
	cout << q2 << endl;
	q2 += p;
	cout << q2 << endl;
	q2 -= q1;
	cout << q2 << endl;
	q2 -= p;
	cout << q2 << endl;
	q2 -= p;
	cout << q2 << endl;

	cout << endl;

	nrOfFailedTestCases += ValidateExactDotProduct<16, 1>();

	nrOfFailedTestCases += ValidateSignMagnitudeTransitions<8, 1>();

	nrOfFailedTestCases += ValidateSignMagnitudeTransitions<16, 1>();
	
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<8, 1, 2>(bReportIndividualTestCases, 16, posit<8, 1>(SpecificValue::minpos));
	
	cout << "Carry Propagation\n";
	nrOfFailedTestCases += ReportTestResult(ValidateCarryPropagation<4, 1>(bReportIndividualTestCases), "carry propagation", "increment");
	cout << "Borrow Propagation\n";
	nrOfFailedTestCases += ReportTestResult(ValidateBorrowPropagation<4, 1>(bReportIndividualTestCases), "borrow propagation", "increment");

#ifdef ISSUE_45_DEBUG
	{	
		Issue45_2<16, 1, 30>();
	}
#endif

#else

	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<8, 0, 2>(bReportIndividualTestCases, 16, posit<8, 0>(SpecificValue::minpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<8, 1, 2>(bReportIndividualTestCases, 16, posit<8, 1>(SpecificValue::minpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<8, 2, 2>(bReportIndividualTestCases, 16, posit<8, 2>(SpecificValue::minpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<8, 0, 5>(bReportIndividualTestCases, 16, posit<8, 0>(SpecificValue::maxpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<8, 1, 5>(bReportIndividualTestCases, 16, posit<8, 1>(SpecificValue::maxpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<8, 2, 5>(bReportIndividualTestCases, 16, posit<8, 2>(SpecificValue::maxpos));

	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<16, 0, 2>(bReportIndividualTestCases, 256, posit<16, 0>(SpecificValue::minpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<16, 1, 2>(bReportIndividualTestCases, 256, posit<16, 1>(SpecificValue::minpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<16, 2, 2>(bReportIndividualTestCases, 256, posit<16, 2>(SpecificValue::minpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<16, 0, 5>(bReportIndividualTestCases, 16, posit<16, 0>(SpecificValue::maxpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<16, 1, 5>(bReportIndividualTestCases, 16, posit<16, 1>(SpecificValue::maxpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<16, 2, 5>(bReportIndividualTestCases, 16, posit<16, 2>(SpecificValue::maxpos));

	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<24, 0, 2>(bReportIndividualTestCases, 4096, posit<24, 0>(SpecificValue::minpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<24, 1, 2>(bReportIndividualTestCases, 4096, posit<24, 1>(SpecificValue::minpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<24, 2, 2>(bReportIndividualTestCases, 4096, posit<24, 2>(SpecificValue::minpos));

	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<32, 0, 2>(bReportIndividualTestCases, 65536, posit<32, 0>(SpecificValue::minpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<32, 1, 2>(bReportIndividualTestCases, 65536, posit<32, 1>(SpecificValue::minpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<32, 2, 2>(bReportIndividualTestCases, 65536, posit<32, 2>(SpecificValue::minpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<32, 0, 5>(bReportIndividualTestCases, 16, posit<32, 0>(SpecificValue::maxpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<32, 1, 5>(bReportIndividualTestCases, 16, posit<32, 1>(SpecificValue::maxpos));
	nrOfFailedTestCases += GenerateQuireAccumulationTestCase<32, 2, 5>(bReportIndividualTestCases, 16, posit<32, 2>(SpecificValue::maxpos));

#ifdef STRESS_TESTING


#endif // STRESS_TESTING


#endif // MANUAL_TESTING
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


////////////////////////////////////////////////////////////////////////
// specific debug scenarios of note
//
// use forward reference to bring them up to the main body
// template<size_t nbits, size_t es, size_t capacity> void Issue45();
// template<size_t nbits, size_t es, size_t capacity> void Issue45_2();

// test case for github issue #45
template<size_t nbits, size_t es>
void Issue45() {
	using ScalarType = sw::universal::posit<nbits, es>;
	using magnitude = sw::universal::posit<nbits, es>;
	using positX = sw::universal::posit<nbits, es>;
	using quireX = sw::universal::quire<nbits, es, 10>;
	using valueX = sw::universal::value<2 * (nbits - 2 - es)>;

	constexpr int n = 64;
	std::vector<positX> Acoefficients(n);
	for (int i = 0; i < n; ++i) {
		Acoefficients[i] = posit<nbits, es>(sw::universal::SpecificValue::minpos);
	}
	std::vector<positX> xcoefficients(n);
	for (int i = 0; i < n; ++i) {
		xcoefficients[i] = 1.0f;
	}
	std::vector<positX> ycoefficients(n);
	//const LocalOrdinalType* Arowoffsets = &A.row_offsets[0];
	const ScalarType* Acoefs = &Acoefficients[0];
	const ScalarType* xcoefs = &xcoefficients[0];
	ScalarType* ycoefs = &ycoefficients[0];
	ScalarType beta = 0;

	magnitude result;
	quireX resultAsQuire;
	valueX resultValueX(0.0f);
	resultAsQuire = resultValueX;

	for (int row = 0; row < 1; ++row) {
		quireX sum;
		valueX sumVal(0.0f);
		sum = sumVal;

		for (int i = 0; i < n; ++i) {
			valueX addend = sw::universal::quire_mul(Acoefficients[i], xcoefficients[i]);
			sum += addend;
			std::cout << components(addend) << "\n" << sum << std::endl;
		}
		positX tempSum;
		tempSum.convert(sum.to_value());
		ycoefs[row] = tempSum;
		valueX resultValueX = sw::universal::quire_mul(xcoefs[row], tempSum);
		resultAsQuire += resultValueX;

	}
	result.convert(resultAsQuire.to_value());
	std::cout << "result: " << result << std::endl;
}

/*
taking 5.05447e-05 += quire_mul(-0.0165405, 0.000999451) (which equals -1.65314e-05)
(-,-16,00010101010110100000000000)
1: 000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000000100100000100000001100000000000000000000000
Row = 266, i = 5338, tempValue after += 3.43323e-05

taking 3.43323e-05 += quire_mul(-0.00828552, 0.000999451) (which equals -8.28097e-06)
(-,-17,00010101110111010000000000)
1: 000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000000011011011000010011000000000000000000000000
Row = 266, i = 5339, tempValue after += 2.6226e-05

----------------------------------------------------------------------------------------------------------------------

taking 2.6226e-05 += quire_mul(-0.016571, 0.000999451) (which equals -1.65619e-05)
(-,-16,00010101110111010000000000)
-1: 111111111111111111111111111111_111111111111111111111111111111111111111111111111111111111.11111111111111110101111111001010000000000000000000000000
Row = 266, i = 5340, tempValue after += -2.68435e+08
----------------------------------------------------------------------------------------------------------------------

Row = 266, i = 5341, tempValue = -2.68435e+08
taking -2.68435e+08 += quire_mul(-0.00828552, 0.000999451) (which equals -8.28097e-06)
(-,-17,00010101110111010000000000)
-1: 111111111111111111111111111111_111111111111111111111111111111111111111111111111111111111.11111111111111111110101010111000100000000000000000000000
*/

// step by step testing to find where the failure occurred
template<size_t nbits, size_t es, size_t capacity = 30>
void Issue45_2() {
	using namespace std;
	using namespace sw::universal;

	cout << "Debug of issue #45\n";

	constexpr size_t mbits = 2 * (nbits - 2 - es);
	sw::universal::quire<nbits, es, capacity> q, q_base;
	sw::universal::value<mbits> unrounded, q_value;
	sw::universal::bitblock<mbits> fraction;

	//  quire_bits = "+:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000000100100000100000001100000000000000000000000";
	//	quire_bits = "+:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000000011011011000010011000000000000000000000000";
	//	quire_bits = "-:111111111111111111111111111111_111111111111111111111111111111111111111111111111111111111.11111111111111110101111111001010000000000000000000000000";
	//	quire_bits = "-:111111111111111111111111111111_111111111111111111111111111111111111111111111111111111111.11111111111111111110101010111000100000000000000000000000";

	fraction.load_bits("00010101010110100000000000");
	//unrounded.set(true, -16, fraction, false, false, false);  // (-,-16,00010101010110100000000000)

	std::string quire_bits;
	quire_bits = "+:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000000100100000100000001100000000000000000000000";
	q.load_bits(quire_bits);
	//std::cout << quire_bits << std::endl;
	//std::cout << q << std::endl;
	fraction.load_bits("00010101110111010000000000");
	unrounded.set(true, -17, fraction, false, false, false);  // (-, -17, 00010101110111010000000000)
	q += unrounded; q_base += unrounded;
	std::cout << q_base << " <--- q_base" << std::endl;
	std::cout << q << std::endl;
	quire_bits = "+:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000000011011011000010011000000000000000000000000";
	std::cout << quire_bits << " <--- debug reference" << std::endl;

	q_base.clear();
	fraction.load_bits("00010101110111010000000000");
	unrounded.set(true, -16, fraction, false, false, false);  // (-,-16,00010101110111010000000000)

	q += unrounded; q_base += unrounded;
	std::cout << q_base << " <--- q_base" << std::endl;
	std::cout << q << std::endl;
	quire_bits = "-:111111111111111111111111111111_111111111111111111111111111111111111111111111111111111111.11111111111111110101111111001010000000000000000000000000";
	std::cout << quire_bits << " <--- debug reference" << std::endl;

	q_base.clear();
	fraction.load_bits("00010101110111010000000000");
	unrounded.set(true, -17, fraction, false, false, false);  // (-,-17,00010101110111010000000000)
	q += unrounded; q_base += unrounded;
	std::cout << q_base << " <--- q_base" << std::endl;
	std::cout << q << std::endl;
	quire_bits = "-:111111111111111111111111111111_111111111111111111111111111111111111111111111111111111111.11111111111111111110101010111000100000000000000000000000";
	std::cout << quire_bits << " <--- debug reference" << std::endl;


	quire_bits = "+:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000000011011011000010011000000000000000000000000";
	quire_bits = "-:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000000010001010111011101000000000000000000000000";

	std::cout << std::endl << std::endl;
	q_base.clear();
	quire_bits = "+:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000000001000000000010000000000000000000000000000";
	q.load_bits(quire_bits);
	std::cout << q << " <---- starting value" << std::endl;
	fraction.load_bits("00000000000111000000000000");
	unrounded.set(true, -17, fraction, false, false, false);  // (-, -17, 00000000000111000000000000)
	q += unrounded; q_base += unrounded;
	std::cout << q_base << " <--- q_base" << std::endl;
	std::cout << q << std::endl;

	std::cout << std::endl << std::endl;
	q_base.clear();
	quire_bits = "+:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.01000000000000000000000000000000000000000000000000000000";
	q.load_bits(quire_bits);
	std::cout << q << " <---- starting value" << std::endl;
	fraction.load_bits("11000000000000000000000000");
	unrounded.set(true, -3, fraction, false, false, false);
	q += unrounded; q_base += unrounded;
	std::cout << q_base << " <--- q_base" << std::endl;
	std::cout << q << std::endl;

	std::cout << std::endl << std::endl;
	q_base.clear();
	quire_bits = "+:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000010000000000000000000000000000000000000000000000000";
	q.load_bits(quire_bits);
	std::cout << q << " <---- starting value" << std::endl;
	fraction.load_bits("11000000000000000000000000");
	unrounded.set(true, -8, fraction, false, false, false);
	q += unrounded; q_base += unrounded;
	std::cout << q_base << " <--- q_base" << std::endl;
	std::cout << q << std::endl;

	std::cout << std::endl << std::endl;
	q_base.clear();
	quire_bits = "+:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000100000000000000000000000000000000000000000000";
	q.load_bits(quire_bits);
	std::cout << q << " <---- starting value" << std::endl;
	fraction.load_bits("11000000000000000000000000");
	unrounded.set(true, -13, fraction, false, false, false);
	q += unrounded; q_base += unrounded;
	std::cout << q_base << " <--- q_base" << std::endl;
	std::cout << q << std::endl;

	std::cout << std::endl << std::endl;
	q_base.clear();
	quire_bits = "+:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000000001000000000000000000000000000000000000000";
	q.load_bits(quire_bits);
	std::cout << q << " <---- starting value" << std::endl;
	fraction.load_bits("11000000000000000000000000");
	unrounded.set(true, -18, fraction, false, false, false);
	q += unrounded; q_base += unrounded;
	std::cout << q_base << " <--- q_base" << std::endl;
	std::cout << q << std::endl;

	std::cout << std::endl << std::endl;
	q_base.clear();
	quire_bits = "+:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000000000000010000000000000000000000000000000000";
	q.load_bits(quire_bits);
	std::cout << q << " <---- starting value" << std::endl;
	fraction.load_bits("11000000000000000000000000");
	unrounded.set(true, -23, fraction, false, false, false);
	q += unrounded; q_base += unrounded;
	std::cout << q_base << " <--- q_base" << std::endl;
	std::cout << q << std::endl;

	std::cout << std::endl << std::endl;
	q_base.clear();
	quire_bits = "+:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000000000000000000100000000000000000000000000000";
	q.load_bits(quire_bits);
	std::cout << q << " <---- starting value" << std::endl;
	fraction.load_bits("11000000000000000000000000");
	unrounded.set(true, -28, fraction, false, false, false);
	q += unrounded; q_base += unrounded;
	std::cout << q_base << " <--- q_base" << std::endl;
	std::cout << q << std::endl;

	std::cout << std::endl << std::endl;
	q_base.clear();
	quire_bits = "+:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000000000000000000010000000000000000000000000000";
	q.load_bits(quire_bits);
	std::cout << q << " <---- starting value" << std::endl;
	fraction.load_bits("11000000000000000000000000");
	unrounded.set(true, -29, fraction, false, false, false);
	q += unrounded; q_base += unrounded;
	std::cout << q_base << " <--- q_base" << std::endl;
	std::cout << q << std::endl;

	std::cout << std::endl << std::endl;
	q_base.clear();
	quire_bits = "+:000000000000000000000000000000_000000000000000000000000000000000000000000000000000000000.00000000000000000000000000001000000000000000000000000000";
	q.load_bits(quire_bits);
	std::cout << q << " <---- starting value" << std::endl;
	fraction.load_bits("11000000000000000000000000");
	unrounded.set(true, -30, fraction, false, false, false);
	q += unrounded; q_base += unrounded;
	std::cout << q_base << " <--- q_base" << std::endl;
	std::cout << q << std::endl;

	{
		constexpr size_t mbits = 2 * (nbits - 2 - es);
		sw::universal::quire<nbits, es, capacity> q, q_base;

		// inefficient as we are copying a whole quire just to reset the sign bit, but we are leveraging the comparison logic
		//quire<nbits, es, capacity> absq = abs(*this);
		//constexpr size_t qbits = (size_t(1) << es) * (4 * nbits - 8) + capacity;
		//constexpr size_t fbits = nbits - 3 - es;
		//value<qbits> absq = abs(q);
		quire <nbits, es, capacity> absq = abs(q);
		internal::value<mbits> absv = abs(unrounded);
		if (absq < absv) {
			cout << "q < v" << endl;
		}
		else if (absq > absv) {
			cout << "q > v" << endl;
		}
		else {
			cout << "q == v" << endl;
		}
	}
}
