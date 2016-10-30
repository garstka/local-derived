
#
# local_derived tests
#

set  (TEST_FILES
      "Test.cpp"
      "constructors.cpp"
      "assignment.cpp"
      "observers.cpp"
      "swap.cpp")

set  (TEST_H_FILES
      "simple_hierarchy.h")
      
source_group("Header Files\\" FILES ${TEST_H_FILES})
source_group("Source Files\\" FILES ${TEST_FILES})

include_directories (${PROJECT_SOURCE_DIR}/src/include)
include_directories (./catch)

add_executable (Test ${TEST_FILES} ${TEST_H_FILES})

#
# install
#

install (TARGETS Test RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)
