
#
# local_derived demo
#

set  (EXAMPLE_FILES
      "example.cpp")
      
set  (MAIN_FILES
      "include/local_derived.h")

source_group("Source Files\\" FILES ${EXAMPLE_FILES})
source_group("Header Files\\" FILES ${MAIN_FILES})

include_directories (./include)

add_executable (example ${EXAMPLE_FILES} ${MAIN_FILES})

#
# install
#

install (TARGETS example RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)
