#pragma once
#include <string>
#include <sstream>

// Templates for testing any depth of inheritance.
namespace simple_hierarchy
{

// A base class for the derived<...> objects
class base
{
public:
	using tag_t = int;

	base()
	{
	}

	base(tag_t t) : tag(t)
	{
	}

	virtual ~base()
	{
	}

	static std::string expected_message(int t)
	{
		auto m = std::ostringstream();
		m << "base: " << t << '\n';
		return m.str();
	}

	virtual std::string message() const
	{
		return expected_message(tag);
	}

private:
	tag_t tag;
};

// Encodes a string as a type.
template <char... All>
struct name_tag;

template <char Head, char... Rest>
struct name_tag<Head, Rest...>
{
	name_tag()
	{
		str.push_back(Head);
		str.append(name_tag<Rest...>().str);
	}
	std::string str;
};
template <>
struct name_tag<>
{
	name_tag()
	{
	}
	std::string str;
};

// Padding for testing base class subobject offsets.
class Padding
{
public:
	virtual ~Padding(){}
private:
	uint8_t a;
};

// A derived class. Can inherit from base, or other derived<>
template <class BaseClass, class NameTag, class TagType>
class derived : public Padding, public BaseClass
{
public:
	using tag_t = TagType;

	derived()
	{
	}

	derived(tag_t t) : tag(t)
	{
	}

	static std::string expected_message(tag_t t)
	{
		auto m = std::ostringstream();
		m << NameTag().str << ": " << t;
		return m.str();
	}

	std::string message() const override
	{
		return expected_message(tag);
	}

private:
	tag_t tag;
};

/* Simple hierarchy example */

// tag: string
using derived1 = derived<base,
                         name_tag<'D', 'e', 'r', 'i', 'v', 'e', 'd', '1'>,
                         std::string>;

// tag: float
using derived11 = derived<derived1,
                          name_tag<'D', 'e', 'r', 'i', 'v', 'e', 'd', '1', '1'>,
                          float>;

// tag: short
using derived12 = derived<derived1,
                          name_tag<'D', 'e', 'r', 'i', 'v', 'e', 'd', '1', '2'>,
                          short>;

// tag: double
using derived2 =
    derived<base, name_tag<'D', 'e', 'r', 'i', 'v', 'e', 'd', '2'>, double>;

// tag: string
using derived21 = derived<derived2,
                          name_tag<'D', 'e', 'r', 'i', 'v', 'e', 'd', '2', '1'>,
                          std::string>;

// tag: double
using derived22 = derived<derived2,
                          name_tag<'D', 'e', 'r', 'i', 'v', 'e', 'd', '2', '2'>,
                          double>;
}