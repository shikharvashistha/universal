#pragma once
// numeric_limits.hpp: definition of numeric_limits for classic cfloat types
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <universal/number/cfloat/cfloat_impl.hpp>
namespace std {

template <size_t nbits, size_t es, typename bt, bool hasSubnormals, bool hasSupernormals, bool isSaturating> 
class numeric_limits< sw::universal::cfloat<nbits, es, bt, hasSubnormals, hasSupernormals, isSaturating> > {
public:
	using Cfloat = sw::universal::cfloat<nbits, es, bt, hasSubnormals, hasSupernormals, isSaturating>;
	static constexpr bool is_specialized = true;
	static constexpr Cfloat min() { // return minimum value
		return Cfloat(sw::universal::SpecificValue::minpos);
	} 
	static constexpr Cfloat max() { // return maximum value
		return Cfloat(sw::universal::SpecificValue::maxpos);
	} 
	static constexpr Cfloat lowest() { // return most negative value
		return Cfloat(sw::universal::SpecificValue::maxneg);
	} 
	static constexpr Cfloat epsilon() { // return smallest effective increment from 1.0
		Cfloat one{ 1.0f }, incr{ 1.0f };
		return ++incr - one;
	}
	static constexpr Cfloat round_error() { // return largest rounding error
		return Cfloat(0.5f);
	}
	static constexpr Cfloat denorm_min() {  // return minimum denormalized value
		return Cfloat(1.0f);
	}
	static constexpr Cfloat infinity() { // return positive infinity
		return Cfloat(INFINITY);
	}
	static constexpr Cfloat quiet_NaN() { // return non-signaling NaN
		return Cfloat(NAN);
	}
	static constexpr Cfloat signaling_NaN() { // return signaling NaN
		return Cfloat(NAN);
	}

	static constexpr int digits       = nbits - 1 - es + 1;
	static constexpr int digits10     = int(digits / 3.3);
	static constexpr int max_digits10 = digits10;
	static constexpr bool is_signed   = true;
	static constexpr bool is_integer  = false;
	static constexpr bool is_exact    = false;
	static constexpr int radix        = 2;

	static constexpr int min_exponent   = -int(1 << (es - 1));
	static constexpr int min_exponent10 = int(min_exponent / 3.3);
	static constexpr int max_exponent   = int(1 << (es - 1));
	static constexpr int max_exponent10 = int(max_exponent / 3.3);
	static constexpr bool has_infinity  = true;
	static constexpr bool has_quiet_NaN = true;
	static constexpr bool has_signaling_NaN = true;
	static constexpr float_denorm_style has_denorm = denorm_absent;
	static constexpr bool has_denorm_loss = false;

	static constexpr bool is_iec559 = false;
	static constexpr bool is_bounded = false;
	static constexpr bool is_modulo = false;
	static constexpr bool traps = false;
	static constexpr bool tinyness_before = false;
	static constexpr float_round_style round_style = round_toward_zero;
};

}
