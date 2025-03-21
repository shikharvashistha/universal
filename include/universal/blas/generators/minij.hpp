#pragma once
// minij.hpp: uniform random matrix generator
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.

namespace sw::universal::blas {

    // MATLAB-style minij returns the n-by-n symmetric positive definite matrix with entries A(i,j) = min(i,j) with i,j range 1..N
    template<typename Scalar>
    matrix<Scalar> minij(size_t N) {
        matrix<Scalar> A(N, N);
        return minij(A);
    }

    // minij returns the n-by-n symmetric positive definite matrix with entries A(i,j) = min(i+1,j+1) as C++ is zero indexed
    template<typename Scalar>
    matrix<Scalar>& minij(matrix<Scalar>& A) {
        for (size_t i = 0; i < num_rows(A); ++i) {
            for (size_t j = 0; j < num_cols(A); ++j) {
                A(i, j) = Scalar(std::min(i+1, j+1));
            }
        }
        return A;
    }

} // namespace sw::universal::blas