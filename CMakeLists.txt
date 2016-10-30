cmake_minimum_required (VERSION 2.8.8)

project (local-derived)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC)

# Set compiler flags and options. 

# General
#   Debug information format      - Program Database for Edit And Continue (/ZI)
#   Warning level                 - Level4 (/W4)   
#   SDL Checks                    - Yes    (/sdl)
# Language
#   Remove unreferenced code and data - Yes (/Zc:inline)
#   Enforce type conversion rules - Yes    (/Zc:rvalueCast)

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI /W4 /sdl /Zc:inline /Zc:rvalueCast ")

# General
#   Debug information format      - Program Database (/Zi)
# Optimization                                            
#   Enable Intrinsic Functions    - Yes (/Oi)
#   Whole Program Optimization    - Yes (/GL)
# Code Generation
#   Enable Function-Level Linking - Yes (/Gy)  
# Language
#   Enforce type conversion rules - Yes (/Zc:rvalueCast)

set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi /Oi /GL /Gy /Zc:rvalueCast ")

else()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -std=c++14 ")

endif()

add_subdirectory (src)
add_subdirectory (test)

enable_testing()

add_test (RunTestExecutable test/Test)