# local-derived
std::unique_ptr-like type erasure with local storage.

## Premise
`local_derived<Base,...>` allows you to take advantage of pointer-to-base polymorphism,
while allocating the derived object in a member buffer. In doing so, it effectively
lets you store polymorphic values in containers and pass them around, without the need for heap allocation or custom memory pools.

Semantics are similar to `std::unique_ptr`, i.e. no copy construction/assignment.

## Template parameters

    local_derived<Base,size,align = /* max */,Offset = uint8_t>

 - `Base` - the base class of objects to wrap
 - `size` - maximum allowed object size (fixed buffer size)
 - `align` - minimum object alignment, defaults to `alignof(max_align_t)`
 - `Offset` - small integer for offset within buffer, defaults to `uint8_t`

## Requirements

 - Base must have a virtual destructor
 - For an object U to be compatible:
   - it must either be Base, or inherit from Base in public or protected mode
   - it must have a move constructor
   - `sizeof(U) <= size`
   - `alignof(U) <= alignment`
 - other:
   - `0 < size <= std::numeric_limits<Offset>::max`

## Install

Download and include the header: `src/include/local_derived.h`

## Sample code and unit tests

For usage example, see `src/example.cpp`.

The project uses Catch framework for unit testing.
You can build the sample code, as well as tests using CMake.

    mkdir build && cd build
    cmake ..
    make && make install && make test

Tested on Visual Studio 14 on Windows, and GCC 6.1.1 on Linux.

Note: CMake treats the Test target as a single test, so for
more verbose and colorful output, run the Test exec directly.

## License

Copyright (c) 2016 Matt Garstka, MIT License
