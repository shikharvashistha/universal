#pragma once
// sqrt.hpp: templated sqrt function stub for native floating-point
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.

namespace sw::universal {

	template<typename Scalar,
		typename = typename std::enable_if<std::is_floating_point<Scalar>::value>::type>
		Scalar sqrt(Scalar v) {
		return std::sqrt(v);
	}

} // namespace sw::universal
