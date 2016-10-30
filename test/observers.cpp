#include <string>
#include "catch.hpp"
#include "local_derived.h"
#include "simple_hierarchy.h"

TEST_CASE("test observers")
{
	using namespace simple_hierarchy;

	// pick the biggest object (you could use a compile-time max)

	const auto S = sizeof(derived21);

	// prepare tags for objects

	auto tag_base = 9;
	auto tag_d1 = std::string("derived1");
	auto tag_d11 = 6.1f;
	auto tag_d12 = short(11);
	auto tag_d2 = 3.14;
	auto tag_d21 = std::string("derived21");
	auto tag_d22 = 6.99;

	SECTION("get or dereference the pointer to wrapped object")
	{
		auto b = local_derived<base, S>(emplace_tag_t<base>(), tag_base);
		auto d1 = local_derived<base, S>(emplace_tag_t<derived1>(), tag_d1);
		auto d11 = local_derived<base, S>(emplace_tag_t<derived11>(), tag_d11);
		auto d12 = local_derived<base, S>(emplace_tag_t<derived12>(), tag_d12);
		auto d2 = local_derived<base, S>(emplace_tag_t<derived2>(), tag_d2);
		auto d21 = local_derived<base, S>(emplace_tag_t<derived21>(), tag_d21);
		auto d22 = local_derived<base, S>(emplace_tag_t<derived22>(), tag_d22);

		SECTION("test if all getters return the same address")
		{
			REQUIRE(b.get() == b.operator->());
			REQUIRE(b.get() == &(*b));

			REQUIRE(d1.get() == d1.operator->());
			REQUIRE(d1.get() == &(*d1));

			REQUIRE(d11.get() == d11.operator->());
			REQUIRE(d11.get() == &(*d11));

			REQUIRE(d12.get() == d12.operator->());
			REQUIRE(d12.get() == &(*d12));

			REQUIRE(d2.get() == d2.operator->());
			REQUIRE(d2.get() == &(*d2));

			REQUIRE(d21.get() == d21.operator->());
			REQUIRE(d21.get() == &(*d21));

			REQUIRE(d22.get() == d22.operator->());
			REQUIRE(d22.get() == &(*d22));
		}

		SECTION("test if the proper tags are retrieved by a virtual call")
		{
			REQUIRE(b->message() == base::expected_message(tag_base));
			REQUIRE(d1->message() == derived1::expected_message(tag_d1));
			REQUIRE(d11->message() == derived11::expected_message(tag_d11));
			REQUIRE(d12->message() == derived12::expected_message(tag_d12));
			REQUIRE(d2->message() == derived2::expected_message(tag_d2));
			REQUIRE(d21->message() == derived21::expected_message(tag_d21));
			REQUIRE(d22->message() == derived22::expected_message(tag_d22));
		}
	}
}