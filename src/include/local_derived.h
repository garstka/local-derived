#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <limits>

// tag type for an emplace constructor
template <class U>
struct emplace_tag_t
{
};

// forward declarations

namespace local_derived_internal
{
template <class Base, class Derived>
size_t get_offset_of_base_within_derived();

template <class Base, class U>
size_t add_offsets(size_t other_offset);

template <class U>
struct move_wrapper;
}


/*
     A wrapper that, given a base class, allows you to store an instance of
    any derived class in-place.

     Semantics are similar to std::unique_ptr, i.e. no copy
    construction/assignment.

     Params:
      - Base       the base class of objects to wrap
      - size       maximum allowed object size (fixed buffer size)
      - alignment  minimum object alignment
      - Offset     small integer for offset within buffer

     Requirements:
    Base:
     - has a virtual destructor
     - alignof(Base) <= alignment
    Derived:
     - has a move-constructor
     - alignof(Derived) <= alignment
    Other:
     - size <= std::numeric_limits<Offset>::max
*/
template <class Base,
          size_t size,
          size_t alignment = alignof(std::max_align_t),
          class Offset = uint8_t>
class local_derived
{
public:
	using pointer = std::add_pointer_t<Base>;
	using element_type = Base;

	static_assert(
	    size > 0 && size <= std::numeric_limits<Offset>::max(),
	    "Requirement: 0 < size <= std::numeric_limits<Offset>::max()");

	static_assert(std::has_virtual_destructor<Base>::value,
	              "Base must have a virtual destructor.");

	static_assert(alignof(Base) <= alignment,
	              "Base must not have a stricter alignment than specified.");

	static_assert(std::is_unsigned<Offset>::value, "Offset must be a uint.");

	/* constructors */

	// Constructs by copying a derived class instance.
	template <class U,
	          class = std::enable_if_t<std::is_base_of<Base, U>::value &&
	                                   sizeof(U) <= size &&
	                                   alignof(U) <= alignment>>
	local_derived(const U& val) noexcept(
	    std::is_nothrow_copy_constructible<U>::value)
	{
		initialize_construction_from_value<U>();
		new (&data) U(val);
	}

	// Constructs by moving a derived class instance.
	template <class U,
	          class = std::enable_if_t<std::is_base_of<Base, U>::value &&
	                                   sizeof(U) <= size &&
	                                   alignof(U) <= alignment>>
	local_derived(U&& val) noexcept(
	    std::is_nothrow_move_constructible<U>::value)
	{
		initialize_construction_from_value<U>();
		new (&data) U(std::move(val));
	}

	/*
	    Constructs a derived class instance in-place.

	    Use emplace_tag_t<U>() as the first parameter
	    to indicate the derived class to emplace.
	*/
	template <class U,
	          class... Args,
	          class = std::enable_if_t<std::is_base_of<Base, U>::value &&
	                                   sizeof(U) <= size &&
	                                   alignof(U) <= alignment>>
	local_derived(emplace_tag_t<U>, Args&&... args)
	{
		initialize_construction_from_value<U>();
		new (&data) U(std::forward<Args>(args)...);
	}

	// Constructs by moving other.
	local_derived(local_derived&& other)
	  : wrapped_move(other.wrapped_move),
	    offset(other.offset) // same offset and move
	{
		wrapped_move(&other.data, &data); // just move the object
	}

	/*
	    Constructs by moving other, different types.

	    Requirements:
	     - U* must be convertible to Base*
	     - other's storage must be smaller or equal in size
	     - other's alignment must not be stricter
	*/
	template <class U,
	          size_t other_size,
	          size_t other_alignment,
	          class OtherOffset,
	          class = std::enable_if_t<std::is_base_of<Base, U>::value &&
	                                   other_size <= size &&
	                                   other_alignment <= alignment>>
	local_derived(
	    local_derived<U, other_size, other_alignment, OtherOffset>&& other)
	  : wrapped_move(other.wrapped_move)
	{
		// get offset to the Base subobject
		offset = static_cast<Offset>(
		    local_derived_internal::add_offsets<Base, U>(other.offset));
		wrapped_move(&other.data, &data);
	}

	// No copy constructor.
	local_derived(const local_derived&) = delete;

	/* destructor */

	// Destructor calls the virtual destructor of the stored object.
	~local_derived()
	{
		get()->~Base();
	}

	/* assignment */

	// Assigns a derived class instance, by copy.
	template <class U,
	          class = std::enable_if_t<std::is_base_of<Base, U>::value &&
	                                   sizeof(U) <= size &&
	                                   alignof(U) <= alignment>>
	local_derived& operator=(const U& val) noexcept(
	    std::is_nothrow_copy_constructible<U>::value)
	{
		get()->~Base(); // call the stored object's destructor

		return (*this = local_derived(val));
	}

	// Assigns a derived class instance, by move.
	template <class U,
	          class = std::enable_if_t<std::is_base_of<Base, U>::value &&
	                                   sizeof(U) <= size &&
	                                   alignof(U) <= alignment>>
	local_derived& operator=(U&& val) noexcept(
	    std::is_nothrow_move_constructible<U>::value)
	{
		return (*this = local_derived(std::move(val)));
	}

	// Move assignment.
	local_derived& operator=(local_derived&& other)
	{
		if (&other != this) // self-assignment check
		{
			get()->~Base(); // call the stored object's destructor

			offset = other.offset;
			wrapped_move = other.wrapped_move;
			wrapped_move(&other.data, &data); // move the assigned object.
		}
		return *this;
	}

	/*
	    Move assignment, another type.

	    Requirements:
	     - U* must be convertible to Base*
	     - other's storage space must be smaller or equal in size
	     - other's alignment must not be stricter
	*/
	template <class U,
	          size_t other_size,
	          size_t other_alignment,
	          class OtherOffset,
	          class = std::enable_if_t<std::is_base_of<Base, U>::value &&
	                                   other_size <= size &&
	                                   other_alignment <= alignment>>
	local_derived& operator=(
	    local_derived<U, other_size, other_alignment, OtherOffset>&& other)
	{

		// get offset to the Base subobject
		offset = static_cast<Offset>(
		    local_derived_internal::add_offsets<Base, U>(other.offset));
		wrapped_move = other.wrapped_move; // copy the move pointer
		wrapped_move(&other.data, &data);  // move the assigned object.

		return *this;
	}

	// No copy assignment.
	local_derived& operator=(const local_derived&) = delete;

	/* observers */

	// Returns the stored pointer.
	Base* get() const noexcept
	{
		// add the base subobject offset and return the pointer
		return reinterpret_cast<Base*>(reinterpret_cast<uintptr_t>(&data) +
		                               static_cast<size_t>(offset));
	}

	// Dereferences the stored pointer.
	Base& operator*() const noexcept
	{
		return *get();
	}

	// Returns the stored pointer.
	Base* operator->() const noexcept
	{
		return get();
	}

	/* swap */

	// Exchanges contents.
	void swap(local_derived& other)
	{
		std::aligned_storage_t<size, alignment> other_temp; // temporary buffer

		other.wrapped_move(&other.data, &other_temp); // move other to temp
		wrapped_move(&data, &other.data);             // move this to other

		using std::swap;
		swap(wrapped_move, other.wrapped_move); // swap moves
		swap(offset, other.offset);             // swap offsets

		wrapped_move(&other_temp, &data); // move temp to this
	}

private:
	// To be called before placement new: save the move pointer and offset
	template <class U>
	void initialize_construction_from_value()
	{
		// save the move wrapper function
		wrapped_move = &local_derived_internal::move_wrapper<U>::move;

		// get the offset to the base subobject
		offset = static_cast<Offset>(
		    local_derived_internal::get_offset_of_base_within_derived<Base,
		                                                              U>());
	}

	using MovePtr = void (*)(void*, void*);

	std::aligned_storage_t<size, alignment> data; // object data
	Offset offset; // offset to the Base subobject within data

	// pointer to a std::move wrapper for the stored object
	MovePtr wrapped_move;

	template <class, size_t, size_t, class>
	friend class local_derived;
};

template <class Base, size_t size, size_t alignment, class Offset>
inline void swap(local_derived<Base, size, alignment, Offset>& lhs,
                 local_derived<Base, size, alignment, Offset>&
                     rhs) noexcept(noexcept(lhs.swap(rhs)))
{
	lhs.swap(rhs);
}

namespace local_derived_internal
{
/*
     Returns the offset of the base class subobject within a derived
    class object.

    Note: this works with void too, in which case the offset is 0.
*/
template <class Base, class Derived>
size_t get_offset_of_base_within_derived()
{
	static_assert(std::is_convertible<Derived*, Base*>::value,
	              "Derived* must be convertible to Base*.");

	// note: using 1, because nullptr always stays at 0, even after
	// static_cast

	const auto derived_pointer = reinterpret_cast<Derived*>(1);

	// static cast adds the offset
	const auto base_pointer = static_cast<Base*>(derived_pointer);

	// get the offset and return it
	return static_cast<size_t>(reinterpret_cast<uintptr_t>(base_pointer) -
	                           reinterpret_cast<uintptr_t>(derived_pointer));
}

/*
    Equivalent to
        get_offset_of_base_within_derived<Base,Y>()
    where
        get_offset_of_base_within_derived<U,Y>() = other_offset
*/
template <class Base, class U>
size_t add_offsets(size_t other_offset)
{
	static_assert(std::is_base_of<Base, U>::value,
	              "U* must be convertible to Base*.");

	// if U* is convertible to Base*, U has a subobject of the type Base

	auto offset_of_U_subobject_in_Y = other_offset;

	auto offset_of_Base_subobject_in_U =
	    get_offset_of_base_within_derived<Base, U>();

	// Base subobject in Y
	return offset_of_Base_subobject_in_U + offset_of_U_subobject_in_Y;
}

// Wrapper for the move constructor of U.
template <class U>
struct move_wrapper
{
	// memory - beginning of the source object
	// out - beginning of the destination object
	static void move(void* in, void* out)
	{
		new (out) U(std::move(*reinterpret_cast<U*>(in)));
	}
};
}
