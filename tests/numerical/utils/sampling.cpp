// sampling.cpp: sample a range of encodings to investigate rounding dynamics
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <iostream>
#include <universal/native/ieee754.hpp>
#include <universal/number/cfloat/cfloat.hpp>
#include <universal/number/cfloat/table.hpp>

template<typename cfloatConfiguration, typename Ty>
void GenerateTestCase(Ty _a, Ty _b) {
	constexpr size_t nbits = cfloatConfiguration::nbits;
	cfloatConfiguration a, b, sum, ref;
	a = _a;
	b = _b;
	sum = a + b;
	// generate the reference
	Ty reference = _a + _b;
	ref = reference;

	std::cout << std::setprecision(10);
	//	std::cout << "native: " << std::setw(nbits) << _a << " + " << std::setw(nbits) << _b << " = " << std::setw(nbits) << reference << std::endl;
	Ty _c{ reference };
	std::cout << sw::universal::to_binary(_a) << " : " << _a << '\n';
	std::cout << sw::universal::to_binary(_b) << " : " << _b << '\n';
	std::cout << sw::universal::to_binary(_c) << " : " << _c << '\n';
	std::cout << a << " + " << b << " = " << sum << " (reference: " << ref << ")   ";
	std::cout << to_binary(a, true) << " + " << to_binary(b, true) << " = " << to_binary(sum, true) << " (reference: " << to_binary(ref, true) << ")   ";
	std::cout << (ref == sum ? "PASS" : "FAIL") << std::endl << std::endl;
	std::cout << std::setprecision(5);
}

template<typename Real, typename cfloatConfiguration>
void Sample(cfloatConfiguration start, cfloatConfiguration stop, Real sample) {
	using namespace sw::universal;
	constexpr size_t nbits = cfloatConfiguration::nbits;
	constexpr size_t es = cfloatConfiguration::es;
	cfloatConfiguration a;
	sw::universal::cfloat<nbits+1, es> b;
	a = start;
	if (a < 0.0f) {
		while (a > stop) {
			std::cout << to_binary(a) << "   " << a << '\n';
			b = float(a);
			--b; // intermediate sample
			if (Real(a) > sample && sample > Real(b)) {
				std::cout << "                  <------- sample " << sample << ' ' << to_binary(sample) << '\n';
			}
			std::cout << to_binary(b) << "  " << b << '\n';
			--a;
			if (Real(b) > sample && sample > Real(a)) {
				std::cout << "                  <------- sample " << sample << ' ' << to_binary(sample) << '\n';
			}
		}
	}
	else {
		while (a < stop) {
			std::cout << to_binary(a) << "   " << a << '\n';
			b = float(a);
			++b; // intermediate sample
			if (Real(a) < sample && sample < Real(b)) {
				std::cout << "                  <------- sample " << sample << ' ' << to_binary(sample) << '\n';
			}
			std::cout << to_binary(b) << "  " << b << '\n';
			++a;
			if (Real(b) < sample && sample < Real(a)) {
				std::cout << "                  <------- sample " << sample << ' ' << to_binary(sample) << '\n';
			}

		}
	}
}

int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::universal;


	/*
 168:     0b1.0101.000       1      -2           b0101            b000                         -0.25       8.4x0xA8r
 169:     0b1.0101.001       1      -2           b0101            b001                      -0.28125       8.4x0xA9r
 170:     0b1.0101.010       1      -2           b0101            b010                       -0.3125       8.4x0xAAr
 171:     0b1.0101.011       1      -2           b0101            b011                      -0.34375       8.4x0xABr
 172:     0b1.0101.100       1      -2           b0101            b100                        -0.375       8.4x0xACr
 173:     0b1.0101.101       1      -2           b0101            b101                      -0.40625       8.4x0xADr
 174:     0b1.0101.110       1      -2           b0101            b110                       -0.4375       8.4x0xAEr
 175:     0b1.0101.111       1      -2           b0101            b111                      -0.46875       8.4x0xAFr
 176:     0b1.0110.000       1      -1           b0110            b000                          -0.5       8.4x0xB0r
 177:     0b1.0110.001       1      -1           b0110            b001                       -0.5625       8.4x0xB1r
 178:     0b1.0110.010       1      -1           b0110            b010                        -0.625       8.4x0xB2r
 179:     0b1.0110.011       1      -1           b0110            b011                       -0.6875       8.4x0xB3r
 180:     0b1.0110.100       1      -1           b0110            b100                         -0.75       8.4x0xB4r
 181:     0b1.0110.101       1      -1           b0110            b101                       -0.8125       8.4x0xB5r
 182:     0b1.0110.110       1      -1           b0110            b110                        -0.875       8.4x0xB6r
 183:     0b1.0110.111       1      -1           b0110            b111                       -0.9375       8.4x0xB7r
 184:     0b1.0111.000       1       0           b0111            b000                            -1       8.4x0xB8r
	*/
	//GenerateTable< cfloat<8, 4> >(std::cout, false);


	/*
   8:     0b0.0001.000       0      -6           b0001            b000                      0.015625       8.4x0x08r
   9:     0b0.0001.001       0      -6           b0001            b001                     0.0175781       8.4x0x09r
  10:     0b0.0001.010       0      -6           b0001            b010                     0.0195312       8.4x0x0Ar
  11:     0b0.0001.011       0      -6           b0001            b011                     0.0214844       8.4x0x0Br
  12:     0b0.0001.100       0      -6           b0001            b100                     0.0234375       8.4x0x0Cr
  13:     0b0.0001.101       0      -6           b0001            b101                     0.0253906       8.4x0x0Dr
  14:     0b0.0001.110       0      -6           b0001            b110                     0.0273438       8.4x0x0Er
  15:     0b0.0001.111       0      -6           b0001            b111                     0.0292969       8.4x0x0Fr
  16:     0b0.0010.000       0      -5           b0010            b000                       0.03125       8.4x0x10r

	*/
	float fa = 0.017578125;
	float fb = -0.5f;
	float fc = fa + fb;
	cfloat<8, 4> a;
	a = -0.40625f;
	Sample(a, cfloat<8, 4>{-1.0f}, fc);

	{
		cfloat < 8, 4, uint8_t > a, b, c;

		a = fa;
		b = fb;
		c = a + b;
		std::cout << a << " + " << b << " = " << c << '\n';
		std::cout << to_binary(a) << " + " << to_binary(b) << " = " << to_binary(c) << '\n';
	}
	GenerateTestCase< cfloat<8, 4, uint8_t>, float>(fa, fb);

	return EXIT_SUCCESS;
}
catch (const char* const msg) {
	std::cerr << msg << std::endl;
	return EXIT_FAILURE;
}
catch (...) {
	std::cerr << "caught unknown exception" << std::endl;
	return EXIT_FAILURE;
}
