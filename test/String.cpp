// Copyright (C) 2020 Maxim, 2dev2fun@gmail.com. All rights reserved.

#include "String.h"

#include <catch2/catch.hpp>

#include <cstring>
#include <string>
#include <utility>

namespace simple {

TEST_CASE("String", "[String]") {
	char const* a_ref = "Small text";
	char const* b_ref = "Very very very very very very very very very very long text";

	String a = a_ref;
	String b = b_ref;

	REQUIRE(a == a_ref);
	REQUIRE(b == b_ref);

	REQUIRE(a.size() == std::strlen(a_ref));
	REQUIRE(b.size() == std::strlen(b_ref));

	const size_t capacity = 300;

	a.reserve(capacity);
	b.reserve(capacity);

	REQUIRE(a == a_ref);
	REQUIRE(b == b_ref);

	REQUIRE(a.size() == std::strlen(a_ref));
	REQUIRE(b.size() == std::strlen(b_ref));

	REQUIRE(a.capacity() == capacity);
	REQUIRE(b.capacity() == capacity);

	String a_assignment = a;
	String b_assignment = b;

	REQUIRE(a_assignment == a);
	REQUIRE(b_assignment == b);

	REQUIRE(a_assignment == a_ref);
	REQUIRE(b_assignment == b_ref);

	String* a0_heap = new String(a_ref);
	String* b0_heap = new String(b_ref);

	String* a1_heap = new String(a);
	String* b1_heap = new String(b);

	REQUIRE(*a0_heap == a_ref);
	REQUIRE(*b0_heap == b_ref);

	REQUIRE(*a1_heap == a_ref);
	REQUIRE(*b1_heap == b_ref);

	delete a0_heap;
	delete b0_heap;

	delete a1_heap;
	delete b1_heap;

	REQUIRE(a == a_ref);
	REQUIRE(b == b_ref);

	String aa = std::move(a);
	String bb = std::move(b);

	REQUIRE(aa == a_ref);
	REQUIRE(bb == b_ref);

	char const* append = " append";

	aa.append(append);
	bb.append(append);

	REQUIRE(aa == (std::string(a_ref) + append).c_str());
	REQUIRE(bb == (std::string(b_ref) + append).c_str());

	REQUIRE(aa.size() == std::strlen(a_ref) + std::strlen(append));
	REQUIRE(bb.size() == std::strlen(b_ref) + std::strlen(append));

	String a_concatenate = String(a_ref) + String(a_ref);
	String b_concatenate = String(b_ref) + String(b_ref);

	REQUIRE(a_concatenate == (std::string(a_ref) + std::string(a_ref)).c_str());
	REQUIRE(b_concatenate == (std::string(b_ref) + std::string(b_ref)).c_str());
}

} // namespace simple
