// complns.cpp: components of a logarithmic number: cli to show the sign/scale/fraction components of a logarithmic number 
//
// Copyright (C) 2017-2020 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <universal/lns/lns>
//#include <universal/lns/lns_manipulators.hpp>

namespace sw {
	namespace unum {
		// return in triple form (sign, scale, fraction)
		template<size_t nbits, typename bt>
		inline std::string to_triple(const lns<nbits, bt>& number) {
			std::stringstream ss;

			// print sign bit
			ss << '(' << "tbd" << ',';

			// scale
			ss << "tbd" << ',';

			// print fraction bits
			ss << "tbd";

			ss << ')';
			return ss.str();
		}
	}
}


// receive a float and print its components
int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::unum;

	// float attributes
	constexpr int max_digits10 = std::numeric_limits<double>::max_digits10;
	constexpr int fbits = std::numeric_limits<float>::digits - 1;

	if (argc != 2) {
		cerr << "complns : components of a logarithmic number" << endl;
		cerr << "Show the sign/scale/fraction components of a logarithmic number." << endl;
	    cerr << "Usage: complns float_value" << endl;
		cerr << "Example: complns 0.03124999" << endl;
		cerr << "TBD" << endl;
		return EXIT_SUCCESS;  // signal successful completion for ctest
	}
	string arg = argv[1];
	lns<32, uint32_t> v;

	constexpr size_t columnWidth = 50;
	cout << setw(columnWidth) << left << typeid(v).name() << ": " << setprecision(max_digits10) << right << v << " " << to_triple(v) << endl;

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