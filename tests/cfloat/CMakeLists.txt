file (GLOB API_SRC "api/*.cpp")
file (GLOB LOGIC_SRC "logic/*.cpp")
file (GLOB CONVERSION_SRC "conversion/*.cpp")
file (GLOB ARITHMETIC_SRC "arithmetic/*.cpp")
file (GLOB STANDARD_SRC "standard/*.cpp")
file (GLOB MATH_SRC "./math/*.cpp")

compile_all("true" "cfloat" "Number Systems/fixed size/floating-point/linear/cfloat/api" "${API_SRC}")
compile_all("true" "cfloat" "Number Systems/fixed size/floating-point/linear/cfloat/logic" "${LOGIC_SRC}")
compile_all("true" "cfloat" "Number Systems/fixed size/floating-point/linear/cfloat/conversion" "${CONVERSION_SRC}")
compile_all("true" "cfloat" "Number Systems/fixed size/floating-point/linear/cfloat/arithmetic" "${ARITHMETIC_SRC}")
compile_all("true" "cfloat" "Number Systems/fixed size/floating-point/linear/cfloat/standard" "${STANDARD_SRC}")
compile_all("true" "cfloat" "Number Systems/fixed size/floating-point/linear/cfloat/math" "${MATH_SRC}")
