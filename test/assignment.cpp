#include <string>
#include "catch.hpp"
#include "local_derived.h"
#include "simple_hierarchy.h"

TEST_CASE("test assignment")
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

	SECTION("assign by copying the wrapped object")
	{
		auto o_b = base(tag_base);
		auto o_d1 = derived1(tag_d1);
		auto o_d11 = derived11(tag_d11);
		auto o_d12 = derived12(tag_d12);
		auto o_d2 = derived2(tag_d2);
		auto o_d21 = derived21(tag_d21);
		auto o_d22 = derived22(tag_d22);

		// initially b is derived22, d22 is base etc.
		auto b = local_derived<base, S>(o_d22);
		auto d1 = local_derived<base, S>(o_d21);
		auto d11 = local_derived<base, S>(o_d2);
		auto d12 = local_derived<base, S>(o_d12);
		auto d2 = local_derived<base, S>(o_d11);
		auto d21 = local_derived<base, S>(o_d1);
		auto d22 = local_derived<base, S>(o_b);

		// "fix" the order
		REQUIRE(&(b = o_b) == &b);
		REQUIRE(&(d1 = o_d1) == &d1);
		REQUIRE(&(d11 = o_d11) == &d11);
		REQUIRE(&(d12 = o_d12) == &d12);
		REQUIRE(&(d2 = o_d2) == &d2);
		REQUIRE(&(d21 = o_d21) == &d21);
		REQUIRE(&(d22 = o_d22) == &d22);

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

	SECTION("assign by moving the wrapped object")
	{
		auto o_b = base(tag_base);
		auto o_d1 = derived1(tag_d1);
		auto o_d11 = derived11(tag_d11);
		auto o_d12 = derived12(tag_d12);
		auto o_d2 = derived2(tag_d2);
		auto o_d21 = derived21(tag_d21);
		auto o_d22 = derived22(tag_d22);

		// initially b is derived22, d22 is base etc.
		auto b = local_derived<base, S>(o_d22);
		auto d1 = local_derived<base, S>(o_d21);
		auto d11 = local_derived<base, S>(o_d2);
		auto d12 = local_derived<base, S>(o_d12);
		auto d2 = local_derived<base, S>(o_d11);
		auto d21 = local_derived<base, S>(o_d1);
		auto d22 = local_derived<base, S>(o_b);

		// "fix" the order
		REQUIRE(&(b = std::move(o_b)) == &b);
		REQUIRE(&(d1 = std::move(o_d1)) == &d1);
		REQUIRE(&(d11 = std::move(o_d11)) == &d11);
		REQUIRE(&(d12 = std::move(o_d12)) == &d12);
		REQUIRE(&(d2 = std::move(o_d2)) == &d2);
		REQUIRE(&(d21 = std::move(o_d21)) == &d21);
		REQUIRE(&(d22 = std::move(o_d22)) == &d22);

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

	SECTION("assign by moving")
	{
		auto f_b = local_derived<base, S>(emplace_tag_t<base>(), tag_base);
		auto f_d1 = local_derived<base, S>(emplace_tag_t<derived1>(), tag_d1);
		auto f_d11 =
		    local_derived<base, S>(emplace_tag_t<derived11>(), tag_d11);
		auto f_d12 =
		    local_derived<base, S>(emplace_tag_t<derived12>(), tag_d12);
		auto f_d2 = local_derived<base, S>(emplace_tag_t<derived2>(), tag_d2);
		auto f_d21 =
		    local_derived<base, S>(emplace_tag_t<derived21>(), tag_d21);
		auto f_d22 =
		    local_derived<base, S>(emplace_tag_t<derived22>(), tag_d22);

		// initially b is derived22, d22 is base etc.
		auto b = local_derived<base, S>(emplace_tag_t<derived22>(), tag_d22);
		auto d1 = local_derived<base, S>(emplace_tag_t<derived21>(), tag_d21);
		auto d11 = local_derived<base, S>(emplace_tag_t<derived2>(), tag_d2);
		auto d12 = local_derived<base, S>(emplace_tag_t<derived12>(), tag_d12);
		auto d2 = local_derived<base, S>(emplace_tag_t<derived11>(), tag_d11);
		auto d21 = local_derived<base, S>(emplace_tag_t<derived1>(), tag_d1);
		auto d22 = local_derived<base, S>(emplace_tag_t<base>(), tag_base);

		// "fix" the order
		REQUIRE(&(b = std::move(f_b)) == &b);
		REQUIRE(&(d1 = std::move(f_d1)) == &d1);
		REQUIRE(&(d11 = std::move(f_d11)) == &d11);
		REQUIRE(&(d12 = std::move(f_d12)) == &d12);
		REQUIRE(&(d2 = std::move(f_d2)) == &d2);
		REQUIRE(&(d21 = std::move(f_d21)) == &d21);
		REQUIRE(&(d22 = std::move(f_d22)) == &d22);

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

	SECTION("assign by moving a different template instantiation")
	{
		const auto s_d1 = sizeof(derived1);
		const auto s_d11 = sizeof(derived11);
		const auto s_d12 = sizeof(derived12);
		const auto s_d2 = sizeof(derived2);
		const auto s_d21 = sizeof(derived21);
		const auto s_d22 = sizeof(derived22);

		auto f_d1 =
		    local_derived<derived1, s_d1>(emplace_tag_t<derived1>(), tag_d1);
		auto f_d11 =
		    local_derived<derived1, s_d11>(emplace_tag_t<derived11>(), tag_d11);
		auto f_d12 =
		    local_derived<derived1, s_d12>(emplace_tag_t<derived12>(), tag_d12);
		auto f_d2 =
		    local_derived<derived2, s_d2>(emplace_tag_t<derived2>(), tag_d2);
		auto f_d21 =
		    local_derived<derived2, s_d21>(emplace_tag_t<derived21>(), tag_d21);
		auto f_d22 =
		    local_derived<derived2, s_d22>(emplace_tag_t<derived22>(), tag_d22);

		// initially d1 is derived22, d22 is derived1 etc.
		auto d1 = local_derived<base, S>(emplace_tag_t<derived22>(), tag_d22);
		auto d11 = local_derived<base, S>(emplace_tag_t<derived21>(), tag_d21);
		auto d12 = local_derived<base, S>(emplace_tag_t<derived2>(), tag_d2);
		auto d2 = local_derived<base, S>(emplace_tag_t<derived12>(), tag_d12);
		auto d21 = local_derived<base, S>(emplace_tag_t<derived11>(), tag_d11);
		auto d22 = local_derived<base, S>(emplace_tag_t<derived1>(), tag_d1);

		// "fix" the order
		REQUIRE(&(d1 = std::move(f_d1)) == &d1);
		REQUIRE(&(d11 = std::move(f_d11)) == &d11);
		REQUIRE(&(d12 = std::move(f_d12)) == &d12);
		REQUIRE(&(d2 = std::move(f_d2)) == &d2);
		REQUIRE(&(d21 = std::move(f_d21)) == &d21);
		REQUIRE(&(d22 = std::move(f_d22)) == &d22);

		SECTION("test if the proper tags are retrieved by a virtual call")
		{
			REQUIRE(d1->message() == derived1::expected_message(tag_d1));
			REQUIRE(d11->message() == derived11::expected_message(tag_d11));
			REQUIRE(d12->message() == derived12::expected_message(tag_d12));
			REQUIRE(d2->message() == derived2::expected_message(tag_d2));
			REQUIRE(d21->message() == derived21::expected_message(tag_d21));
			REQUIRE(d22->message() == derived22::expected_message(tag_d22));
		}
	}
}