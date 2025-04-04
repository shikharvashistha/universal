// CHEBFUN - constructs an object representing the function F on the interval [-1,1]
// Copyright (C) 2017-2020 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.

/* Class for approximating functions defined on finite, semi-infinite, or
    doubly-infinite intervals [a,b]. Functions may be smooth, piecewise smooth,
     weakly singular, or blow up on the interval.
*/ 

// Dependencies
#include<vector>
#include<cmath> /* cos */




template<typename Scalar>
class chebfun { 

    public:             
        typedef Scalar              value_type;
        typedef size_type           size_t;
        // constexpr Scalar pi = 3.14159265358979323846;
    
    // Class Constructor:     
        chebfun():domain(0){}
        chebfun(size_type n):domain(n){-cos(k*pi/6)}

        Scalar operator[](size_type i){return domain[i];}

   

    private:
        std::vector<Scalar> domain;     // 1 x (n+1)  vector of doubles or posits




}; // End CHEBFUN