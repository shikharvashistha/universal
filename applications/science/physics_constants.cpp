﻿// physics_constants.cpp: experiments with posit representations of important constants in physics 
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the UNIVERSAL project, which is released under an MIT Open Source license.
#include <universal/utility/number_system_properties.hpp> //minmax_range etc. for native types
#include <universal/number/posit/posit.hpp>
#include <universal/number/integer/integer.hpp>

/*
The 2019 redefinition of the SI base units came into force on 20 May 2019,[1][2] the 144th anniversary 
of the Metre Convention. In the redefinition, four of the seven SI base units 
           – the kilogram, ampere, kelvin, and mole –
were redefined by setting exact numerical values for 
   - the Planck constant (h), 
   - the elementary electric charge (e), 
   - the Boltzmann constant (k), and 
   - the Avogadro constant (NA), 
 respectively. 
 
 The second, metre, and candela were already defined by physical constants and were subject to correction 
 to their definitions. The new definitions aimed to improve the SI without changing the value of any units, 
 ensuring continuity with existing measurements.[3][4] In November 2018, the 26th General Conference on 
 Weights and Measures (CGPM) unanimously approved these changes,[5][6] which the International Committee 
 for Weights and Measures (CIPM) had proposed earlier that year after determining that previously agreed 
 conditions for the change had been met.[7]:23 These conditions were satisfied by a series of experiments 
 that measured the constants to high accuracy relative to the old SI definitions, and were the culmination 
 of decades of research.

 Following the successful 1983 redefinition of the metre in terms of an exact numerical value for the 
 speed of light, the BIPM's Consultative Committee for Units (CCU) recommended and the BIPM proposed that 
 four further constants of nature should be defined to have exact values. These are:

The Planck constant h is exactly 6.62607015×10^−34 joule-second (J⋅s).
The elementary charge e is exactly 1.602176634×10^−19 coulomb (C).
The Boltzmann constant k is exactly 1.380649×10^−23 joule per kelvin (J⋅K−1).
The Avogadro constant NA is exactly 6.02214076×10^23 reciprocal mole (mol−1).

These constants are described in the 2006 version of the SI manual but in that version, the latter three 
are defined as "constants to be obtained by experiment" rather than as "defining constants". The redefinition 
retains unchanged the numerical values associated with the following constants of nature:

The speed of light c is exactly 299792458 metres per second (m⋅s−1);
The ground state hyperfine structure transition frequency of the caesium-133 atom ΔνCs is exactly 9192631770 hertz (Hz);
The luminous efficacy Kcd of monochromatic radiation of frequency 540×1012 Hz (540 THz) – a frequency 
of green-colored light at approximately the peak sensitivity of the human eye – is exactly 683 lumens per watt (lm⋅W−1).
The seven definitions above are rewritten below with the derived units (joule, coulomb, hertz, lumen, and watt) expressed 
in terms of the seven base units; second, metre, kilogram, ampere, kelvin, mole, and candela, according to the 9th SI Brochure. 
In the list that follows, the symbol sr stands for the dimensionless unit steradian.

ΔνCs = Δν(133Cs)hfs = 9192631770 s−1
c = 299792458 m⋅s−1
h = 6.62607015×10−34 kg⋅m2⋅s−1
e = 1.602176634×10−19 A⋅s
k = 1.380649×10−23 kg⋅m2⋅K−1⋅s−2
NA = 6.02214076×1023 mol−1
Kcd = 683 cd⋅sr⋅s3⋅kg−1⋅m−2
As part of the redefinition, the international prototype kilogram was retired and definitions of the kilogram, 
the ampere, and the kelvin were replaced. The definition of the mole was revised. These changes have the effect 
of redefining the SI base units, though the definitions of the SI derived units in terms of the base units remain the same.
 */

std::string version_string(int a, int b, int c) {
	std::ostringstream ss;
	ss << a << '.' << b << '.' << c;
	return ss.str();
}

std::string report_compiler_version() {
#if defined(__clang__)
	/* Clang/LLVM. ---------------------------------------------- */
	return version_string(__clang_major__, __clang_minor__, __clang_patchlevel__);

#elif defined(__ICC) || defined(__INTEL_COMPILER)
	/* Intel ICC/ICPC. ------------------------------------------ */
	return std::string("Intel Compiler");

#elif defined(__GNUC__) || defined(__GNUG__)
	/* GNU GCC/G++. --------------------------------------------- */
	return version_string(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);

#elif defined(__HP_cc) || defined(__HP_aCC)
	/* Hewlett-Packard C/C++. ---------------------------------- */
	return std::string("Hewlett-Packard C/C++ compiler");

#elif defined(__IBMC__) || defined(__IBMCPP__)
	/* IBM XL C/C++. -------------------------------------------- */
	return std::string("IBM XL C/C++");

#elif defined(_MSC_VER)
	/* Microsoft Visual Studio. --------------------------------- */
	// Visual C++ compiler is 15.00.20706.01, the _MSC_FULL_VER will be 15002070601
	char version[16];
	sprintf(version, "%d", _MSC_FULL_VER);
	return std::string("Microsoft Visual Studio C++ compiler version ") + std::string(version);

#elif defined(__PGI)
	/* Portland Group PGCC/PGCPP. ------------------------------- */
	return std::string("Portland Group PGCC/PGCPP");

#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
	/* Oracle Solaris Studio. ----------------------------------- */
	return std::string("Oracle Solaris Studio");
#endif
}

template<typename Scalar>
void Represent(std::ostream& ostr, Scalar s, std::streamsize precision = 17, bool hexFormat = false) {
	using namespace std;
	// preserve the existing ostream precision
	auto old_precision = cout.precision();
	ostr << setprecision(precision);

	ostr << setw(15) << s;
	if (hexFormat) {
		ostr << " : " << setw(70) << color_print(s) << " : " << hex_format(s);
	}
	ostr << endl;

	// restore the previous ostream precision
	ostr << setprecision(old_precision);
}

void Sample(std::ostream& ostr, long double constant) {
	using namespace sw::universal;
	Represent(ostr << minmax_range< long double  >() << " : ", constant, 23);
	Represent(ostr << minmax_range< double       >() << " : ", double(constant), 15);
	Represent(ostr << minmax_range< float        >() << " : ", float(constant), 6);
	Represent(ostr << minmax_range< posit<32, 2> >() << " : ", posit<32, 2>(constant), 4, true);
	Represent(ostr << minmax_range< posit<32, 3> >() << " : ", posit<32, 3>(constant), 6, true);
	Represent(ostr << minmax_range< posit<40, 3> >() << " : ", posit<40, 3>(constant), 8, true);
	Represent(ostr << minmax_range< posit<48, 3> >() << " : ", posit<48, 3>(constant), 10, true);
	Represent(ostr << minmax_range< posit<56, 3> >() << " : ", posit<56, 3>(constant), 12, true);
	Represent(ostr << minmax_range< posit<64, 3> >() << " : ", posit<64, 3>(constant), 15, true);
}

void CompareIEEEValues(std::ostream& ostr, long double constant) {
	using namespace std;
	using namespace sw::universal;

	constexpr int f_prec = std::numeric_limits<float>::max_digits10;
	constexpr int d_prec = std::numeric_limits<double>::max_digits10;
	constexpr int q_prec = std::numeric_limits<long double>::max_digits10;

	constexpr int f_fbits = std::numeric_limits<float>::digits - 1;
	constexpr int d_fbits = std::numeric_limits<double>::digits - 1;
	constexpr int q_fbits = std::numeric_limits<long double>::digits - 1;

	float f = float(constant);
	double d = double(constant);
	long double q = constant;

	internal::value<f_fbits> vf(f);
	internal::value<d_fbits> vd(d);
	internal::value<q_fbits> vq(q);

	int width = q_prec + 5;

	std::streamsize old_precision = cout.precision();

	ostr << report_compiler_version() << endl;
	ostr << "float precision       : " << f_fbits << " bits\n";
	ostr << "double precision      : " << d_fbits << " bits\n";
	ostr << "long double precision : " << q_fbits << " bits\n";

	cout << endl;

//	ostr << "input value: " << setprecision(f_prec) << setw(width) << constant << endl;
	ostr << "      float: " << setprecision(f_prec) << setw(width) << f << " " << to_triple(vf) << endl;
	ostr << "     double: " << setprecision(d_prec) << setw(width) << d << " " << to_triple(vd) << endl;
	ostr << "long double: " << setprecision(q_prec) << setw(width) << q << " " << to_triple(vq) << endl;

	cout << setprecision(old_precision);
}

int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::universal;

	//constexpr size_t nbits = 32;
	//constexpr size_t es = 2;
	//using Posit = posit<nbits,es>;

	// print detailed bit-level computational intermediate results
	// bool verbose = false;

	long double h = 6.62607015e-34;  // (J⋅s)
	long double e = 1.602176634e-19; // (C)
	long double k = 1.380649e-23;    // (J⋅K−1)
	long double NA = 6.02214076e23;  // (mol−1)


	cout << "The Planck constant h is exactly 6.62607015*10^-34 joule - second.\n";
	Sample(cout, h);
	cout << endl;

	cout << "The elementary charge e is exactly 1.602176634*10^-19 coulomb.\n";
	Sample(cout, e);
	cout << endl;

	cout << "The Boltzmann constant k is exactly 1.380649*10^-23 joule per kelvin.\n";
	Sample(cout, k);
	cout << endl;

	cout << "The Avogadro constant NA is exactly 6.02214076*10^+23 reciprocal mole.\n";
	Sample(cout, NA);
	cout << endl;


	cout << "----\n\n";
	CompareIEEEValues(cout, h);

	integer<128> i;
	if (parse("66260701500000000000000000000000000", i)) {
		cout << "h = " << i << endl;
	}
	else {
		cerr << "error parsing h" << endl;
	}

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
catch (std::runtime_error& err) {
	std::cerr << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (...) {
	std::cerr << "Caught unknown exception" << std::endl;
	return EXIT_FAILURE;
}

/*
The Planck constant h is exactly 6.62607015*10^-34 joule - second.
				   long double min  2.22507e-308     max  1.79769e+308      : 6.6260701499999998297249e-34
						double min  2.22507e-308     max  1.79769e+308      : 6.62607015e-34
						 float min   1.17549e-38     max   3.40282e+38      : 6.62607e-34
   class sw::universal::posit<32,2> min   7.52316e-37     max   1.32923e+36      : 7.7037197775489434e-34
   class sw::universal::posit<32,3> min    5.6598e-73     max   1.76685e+72      : 6.6260265567150702e-34
   class sw::universal::posit<40,3> min   3.06818e-92     max   3.25926e+91      : 6.6260706377532261e-34
   class sw::universal::posit<48,3> min  1.66327e-111     max  6.01227e+110      : 6.6260701498771527e-34
   class sw::universal::posit<64,3> min   4.8879e-150     max  2.04587e+149      : 6.6260701499999853e-34

The elementary charge e is exactly 1.602176634*10^-19 coulomb.
				   long double min  2.22507e-308     max  1.79769e+308      : 1.6021766339999998937562e-19
						double min  2.22507e-308     max  1.79769e+308      : 1.602176634e-19
						 float min   1.17549e-38     max   3.40282e+38      : 1.60218e-19
   class sw::universal::posit<32,2> min   7.52316e-37     max   1.32923e+36      : 1.6022157592907125e-19
   class sw::universal::posit<32,3> min    5.6598e-73     max   1.76685e+72      : 1.6021764682116162e-19
   class sw::universal::posit<40,3> min   3.06818e-92     max   3.25926e+91      : 1.6021766378482653e-19
   class sw::universal::posit<48,3> min  1.66327e-111     max  6.01227e+110      : 1.6021766339986241e-19
   class sw::universal::posit<64,3> min   4.8879e-150     max  2.04587e+149      : 1.6021766340000001e-19

The Boltzmann constant k is exactly 1.380649*10^-23 joule per kelvin.
				   long double min  2.22507e-308     max  1.79769e+308      : 1.3806490000000000921522e-23
						double min  2.22507e-308     max  1.79769e+308      : 1.380649e-23
						 float min   1.17549e-38     max   3.40282e+38      : 1.38065e-23
   class sw::universal::posit<32,2> min   7.52316e-37     max   1.32923e+36      : 1.3803576471978649e-23
   class sw::universal::posit<32,3> min    5.6598e-73     max   1.76685e+72      : 1.380650472365883e-23
   class sw::universal::posit<40,3> min   3.06818e-92     max   3.25926e+91      : 1.3806490129732083e-23
   class sw::universal::posit<48,3> min  1.66327e-111     max  6.01227e+110      : 1.3806490000309591e-23
   class sw::universal::posit<64,3> min   4.8879e-150     max  2.04587e+149      : 1.3806490000000013e-23

The Avogadro constant NA is exactly 6.02214076*10^+23 reciprocal mole.
				   long double min  2.22507e-308     max  1.79769e+308      : 6.0221407599999998702387e+23
						double min  2.22507e-308     max  1.79769e+308      : 6.02214076e+23
						 float min   1.17549e-38     max   3.40282e+38      : 6.02214e+23
   class sw::universal::posit<32,2> min   7.52316e-37     max   1.32923e+36      : 6.0210172656587976e+23
   class sw::universal::posit<32,3> min    5.6598e-73     max   1.76685e+72      : 6.0221471287333124e+23
   class sw::universal::posit<40,3> min   3.06818e-92     max   3.25926e+91      : 6.0221407336218415e+23
   class sw::universal::posit<48,3> min  1.66327e-111     max  6.01227e+110      : 6.0221407600101206e+23
   class sw::universal::posit<64,3> min   4.8879e-150     max  2.04587e+149      : 6.0221407600000005e+23
 */
