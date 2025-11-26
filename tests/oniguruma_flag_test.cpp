// oniguruma_flag_test.cpp --- Tests for oniguruma flag
// Author: katahiromz
// License: BSD-2-Clause
//
// These tests verify that the oniguruma flag enables Oniguruma's native syntax
// and behavior, specifically testing:
// - Numeric backreferences (\1, \2, ...)
// - Named backreferences (\k<name>, \k'name')

#include "tests.h"

#ifndef USE_STD_FOR_TESTS

using sregex = myns::basic_regex<char>;
using smatch = myns::match_results<std::string::const_iterator>;

// Helper macros for test cases
#define TEST_CASE(name) \
	std::cout << "\n--- " << (name) << " ---\n"; \
	try {

#define TEST_CASE_END(name) \
	std::cout << "✅ " << (name) << " PASSED.\n"; \
	} catch (const myns::regex_error& e) { \
		std::cout << "❌ " << (name) << " FAILED with regex_error: " << e.what() << "\n"; \
		assert(false); \
	} catch (const std::exception& e) { \
		std::cout << "❌ " << (name) << " FAILED with std::exception: " << e.what() << "\n"; \
		assert(false); \
	} catch (...) { \
		std::cout << "❌ " << (name) << " FAILED with unknown exception.\n"; \
		assert(false); \
	}

// Test numeric backreference \1 with oniguruma flag
void TestOnigurumaNumericBackref() {
	TEST_CASE("TestOnigurumaNumericBackref")
	
	// Pattern with \1 backreference using oniguruma flag
	sregex re(std::string("(.+)\\1"), myns::regex_constants::oniguruma);
	smatch m;
	
	// "abab" should match: "ab" captured by group 1, then repeated
	std::string input = "abab";
	bool found = myns::regex_search(input, m, re);
	assert(found);
	assert(m.size() >= 2);
	assert(m[0].str() == "abab");
	assert(m[1].str() == "ab");
	std::cout << "  Matched: " << m[0].str() << ", group 1: " << m[1].str() << std::endl;
	
	// "abc" should not match (no repetition)
	std::string input2 = "abc";
	smatch m2;
	bool found2 = myns::regex_search(input2, m2, re);
	assert(!found2);
	std::cout << "  Non-matching input correctly rejected" << std::endl;
	
	TEST_CASE_END("TestOnigurumaNumericBackref")
}

// Test named backreference \k<name> with oniguruma flag
void TestOnigurumaNamedBackrefAngle() {
	TEST_CASE("TestOnigurumaNamedBackrefAngle")
	
	// Pattern with \k<name> backreference using oniguruma flag
	sregex re(std::string("(?<word>\\w+)\\s+\\k<word>"), myns::regex_constants::oniguruma);
	smatch m;
	
	// "hello hello" should match: "hello" captured, then repeated
	std::string input = "hello hello";
	bool found = myns::regex_search(input, m, re);
	assert(found);
	assert(m.size() >= 2);
	assert(m[0].str() == "hello hello");
	assert(m[1].str() == "hello");
	std::cout << "  Matched: " << m[0].str() << ", group 1: " << m[1].str() << std::endl;
	
	// "hello world" should not match (different words)
	std::string input2 = "hello world";
	smatch m2;
	bool found2 = myns::regex_search(input2, m2, re);
	assert(!found2);
	std::cout << "  Non-matching input correctly rejected" << std::endl;
	
	TEST_CASE_END("TestOnigurumaNamedBackrefAngle")
}

// Test named backreference \k'name' with oniguruma flag
void TestOnigurumaNamedBackrefQuote() {
	TEST_CASE("TestOnigurumaNamedBackrefQuote")
	
	// Pattern with \k'name' backreference using oniguruma flag
	sregex re(std::string("(?<word>\\w+)\\s+\\k'word'"), myns::regex_constants::oniguruma);
	smatch m;
	
	// "world world" should match
	std::string input = "world world";
	bool found = myns::regex_search(input, m, re);
	assert(found);
	assert(m.size() >= 2);
	assert(m[0].str() == "world world");
	assert(m[1].str() == "world");
	std::cout << "  Matched: " << m[0].str() << ", group 1: " << m[1].str() << std::endl;
	
	TEST_CASE_END("TestOnigurumaNamedBackrefQuote")
}

// Test multiple numeric backreferences
void TestOnigurumaMultipleBackrefs() {
	TEST_CASE("TestOnigurumaMultipleBackrefs")
	
	// Pattern for palindrome-like matching
	sregex re(std::string("(.)(.)(.)(.)(.)\\5\\4\\3\\2\\1"), myns::regex_constants::oniguruma);
	smatch m;
	
	// "abcdeedcba" should match
	std::string input = "abcdeedcba";
	bool found = myns::regex_match(input, m, re);
	assert(found);
	assert(m[0].str() == "abcdeedcba");
	assert(m[1].str() == "a");
	assert(m[2].str() == "b");
	assert(m[3].str() == "c");
	assert(m[4].str() == "d");
	assert(m[5].str() == "e");
	std::cout << "  Matched palindrome: " << m[0].str() << std::endl;
	
	TEST_CASE_END("TestOnigurumaMultipleBackrefs")
}

// Test multi-digit backreference \10 with oniguruma flag
void TestOnigurumaMultiDigitBackref() {
	TEST_CASE("TestOnigurumaMultiDigitBackref")
	
	// Define 10 groups and use \10 to reference group 10
	sregex re(std::string("(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)\\10"), myns::regex_constants::oniguruma);
	smatch m;
	
	// "abcdefghijj" should match: \10 refers to group 10 ("j")
	std::string input = "abcdefghijj";
	bool found = myns::regex_match(input, m, re);
	assert(found);
	assert(m.size() == 11);  // 10 groups + full match
	assert(m[10].str() == "j");
	std::cout << "  Matched with \\10: " << m[0].str() << std::endl;
	
	TEST_CASE_END("TestOnigurumaMultiDigitBackref")
}

// Compare behavior with and without oniguruma flag
void TestOnigurumaFlagVsDefault() {
	TEST_CASE("TestOnigurumaFlagVsDefault")
	
	// With oniguruma flag: native Oniguruma behavior
	sregex re_onig(std::string("(.+)\\1"), myns::regex_constants::oniguruma);
	smatch m1;
	std::string input = "abab";
	bool found1 = myns::regex_search(input, m1, re_onig);
	assert(found1);
	std::cout << "  With oniguruma flag: matched '" << m1[0].str() << "'" << std::endl;
	
	// Without oniguruma flag (default ECMAScript): should also work for simple backrefs
	sregex re_default(std::string("(.+)\\1"));  // Default is ECMAScript
	smatch m2;
	bool found2 = myns::regex_search(input, m2, re_default);
	assert(found2);
	std::cout << "  Without oniguruma flag: matched '" << m2[0].str() << "'" << std::endl;
	
	// Both should produce the same result for simple patterns
	assert(m1[0].str() == m2[0].str());
	assert(m1[1].str() == m2[1].str());
	
	TEST_CASE_END("TestOnigurumaFlagVsDefault")
}

// Test that oniguruma flag works with case-insensitive matching
void TestOnigurumaFlagWithIcase() {
	TEST_CASE("TestOnigurumaFlagWithIcase")
	
	// Combine oniguruma and icase flags
	sregex re(std::string("(\\w+)\\s+\\1"), myns::regex_constants::oniguruma | myns::regex_constants::icase);
	smatch m;
	
	// "Hello HELLO" should match with case-insensitive backreference
	std::string input = "Hello HELLO";
	bool found = myns::regex_search(input, m, re);
	assert(found);
	assert(m[0].str() == "Hello HELLO");
	assert(m[1].str() == "Hello");
	std::cout << "  Case-insensitive match: " << m[0].str() << std::endl;
	
	TEST_CASE_END("TestOnigurumaFlagWithIcase")
}

// Test that oniguruma flag works with multiline option
void TestOnigurumaFlagWithMultiline() {
	TEST_CASE("TestOnigurumaFlagWithMultiline")
	
	// Combine oniguruma and multiline flags
	// In native Oniguruma mode, dot should match newline with multiline
	sregex re(std::string("^(.+)$"), myns::regex_constants::oniguruma | myns::regex_constants::multiline);
	smatch m;
	
	std::string input = "line1\nline2";
	bool found = myns::regex_search(input, m, re);
	assert(found);
	std::cout << "  Multiline match: " << m[0].str() << std::endl;
	
	TEST_CASE_END("TestOnigurumaFlagWithMultiline")
}

int main() {
	TESTS_OUTPUT_INIT();
	ONIGPP_TEST_INIT;
	
	std::cout << "========================================" << std::endl;
	std::cout << "oniguruma Flag Tests" << std::endl;
	std::cout << "========================================" << std::endl;
	
	TestOnigurumaNumericBackref();
	TestOnigurumaNamedBackrefAngle();
	TestOnigurumaNamedBackrefQuote();
	TestOnigurumaMultipleBackrefs();
	TestOnigurumaMultiDigitBackref();
	TestOnigurumaFlagVsDefault();
	TestOnigurumaFlagWithIcase();
	TestOnigurumaFlagWithMultiline();
	
	std::cout << "\n========================================" << std::endl;
	std::cout << "All oniguruma flag tests PASSED!" << std::endl;
	std::cout << "========================================" << std::endl;
	
	return 0;
}

#else
// When USE_STD_FOR_TESTS is defined, skip these tests as std::regex doesn't have oniguruma flag
int main() {
	std::cout << "oniguruma flag tests skipped (USE_STD_FOR_TESTS mode)" << std::endl;
	return 0;
}
#endif
