#pragma once
//  test_status.cpp : functions for test status reporting
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <iostream>
#include <iomanip>

namespace sw::universal {

// test reporting helper
// takes an int reporting the number of test failures and prints a PASS/FAIL designation
int ReportTestResult(int nrOfFailedTests, const std::string& description, const std::string& test_operation) {
	constexpr int TEST_TAG_WIDTH = 25;
	if (nrOfFailedTests > 0) {
		std::cout << description << " " << std::setw(TEST_TAG_WIDTH) << test_operation << " FAIL " << nrOfFailedTests << " failed test cases\n";
	}
	else {
		std::cout << description << " " << std::setw(TEST_TAG_WIDTH) << test_operation << " PASS\n";
	}
	return nrOfFailedTests;
}

// simple checker
int ReportCheck(const std::string& tag, const std::string& test, bool success) {
	constexpr int TEST_TAG_WIDTH = 25;
	int nrOfFailedTestCases = 0;
	if (success) {
		std::cout << tag << " " << std::left << std::setw(TEST_TAG_WIDTH) << test << " PASS\n";
	}
	else {
		++nrOfFailedTestCases;
		std::cout << tag << " " << std::left << std::setw(TEST_TAG_WIDTH) << test << " FAIL\n";
	}
	return nrOfFailedTestCases;
}

} // namespace sw::universal
