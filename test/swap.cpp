#include <string>
#include <utility>
#include "catch.hpp"
#include "local_derived.h"
#include "simple_hierarchy.h"

TEST_CASE("test swap")
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

	SECTION("member swap some objects")
	{
		// initially b is derived22, d22 is base etc.
		auto b = local_derived<base, S>(emplace_tag_t<derived22>(), tag_d22);
		auto d1 = local_derived<base, S>(emplace_tag_t<derived21>(), tag_d21);
		auto d11 = local_derived<base, S>(emplace_tag_t<derived2>(), tag_d2);
		auto d12 = local_derived<base, S>(emplace_tag_t<derived12>(), tag_d12);
		auto d2 = local_derived<base, S>(emplace_tag_t<derived11>(), tag_d11);
		auto d21 = local_derived<base, S>(emplace_tag_t<derived1>(), tag_d1);
		auto d22 = local_derived<base, S>(emplace_tag_t<base>(), tag_base);

		// "fix" the order
		b.swap(d22);
		d1.swap(d21);
		d11.swap(d2);
		d12.swap(d12);

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

	SECTION("ADL swap some objects")
	{
		// initially b is derived22, d22 is base etc.
		auto b = local_derived<base, S>(emplace_tag_t<derived22>(), tag_d22);
		auto d1 = local_derived<base, S>(emplace_tag_t<derived21>(), tag_d21);
		auto d11 = local_derived<base, S>(emplace_tag_t<derived2>(), tag_d2);
		auto d12 = local_derived<base, S>(emplace_tag_t<derived12>(), tag_d12);
		auto d2 = local_derived<base, S>(emplace_tag_t<derived11>(), tag_d11);
		auto d21 = local_derived<base, S>(emplace_tag_t<derived1>(), tag_d1);
		auto d22 = local_derived<base, S>(emplace_tag_t<base>(), tag_base);

		using std::swap;

		// "fix" the order
		swap(b, d22);
		swap(d1, d21);
		swap(d11, d2);
		swap(d12, d12);

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