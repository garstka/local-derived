#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <type_traits>
#include "local_derived.h"

// base class with a string tag
class Base
{
public:
	Base(std::string t) : tag(t)
	{
	}

	virtual ~Base()
	{
	}

	virtual void message() const
	{
		std::cerr << "Base     - " << tag << '\n';
	}

	std::string get_base_tag() const
	{
		return tag;
	}

private:
	std::string tag;
};

// derived class with a string and int tags
class Derived1 : public Base
{
public:
	Derived1(std::string t1, int t2) : Base(t1), tag(t2)
	{
	}

	void message() const override
	{
		std::cerr << "Derived1 - " << Base::get_base_tag() << ", " << tag
		          << '\n';
	}

private:
	int tag;
};

// derived class with a string and double tags
class Derived2 : public Base
{
public:
	Derived2(std::string t1, double t2) : Base(t1), tag(t2)
	{
	}

	void message() const override
	{
		std::cerr << "Derived2 - " << Base::get_base_tag() << ", " << tag
		          << '\n';
	}

private:
	double tag;
};


// snippet::max_size_of<T,U,...> - get the max size of all args
namespace snippet
{
template <size_t running, class... Args>
struct max_size_r;

template <size_t running>
struct max_size_r<running> : public std::integral_constant<int, running>
{
};

template <size_t running, class T, class... Args>
struct max_size_r<running, T, Args...>
    : public max_size_r<(running > sizeof(T) ? running : sizeof(T)), Args...>
{
};

template <class... Args>
using max_size_of = max_size_r<0, Args...>;
}

// showcases basic functionality
int main()
{
	// find the max object size

	const auto S = snippet::max_size_of<Base, Derived1, Derived2>::value;

	auto v = std::vector<local_derived<Base, S>>();

	// construct from objects to wrap

	auto b1 = Base("the original");
	auto d1 = Derived1("derived class one", 1);
	auto d2 = Derived2("epsilon", 0.000021);

	v.emplace_back(b1);
	v.emplace_back(d1);
	v.emplace_back(std::move(d2));

	// move in from a smaller, but compatible local_derived

	auto b2 = Base("the other one");
	auto smaller = local_derived<Base, sizeof(Base)>(b2);
	v.emplace_back(std::move(smaller));

	// emplace using a tag

	auto emplaced =
	    local_derived<Base, S>(emplace_tag_t<Derived2>(), "theta", 19.1);

	v.emplace_back(std::move(emplaced));

	// call virtual methods

	for (auto& x : v)
		x->message();

	std::rotate(v.begin(), v.begin() + 3, v.end());

	std::cerr << "\nRotate (begin, begin+3, end):\n\n";
	for (auto& x : v)
		x->message();
}