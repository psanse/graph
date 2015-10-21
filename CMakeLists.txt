
#Include the main biicode macros and functions
include(${CMAKE_HOME_DIRECTORY}/biicode.cmake)

# Define block variables
INIT_BIICODE_BLOCK() 

LIST(REMOVE_ITEM BII_LIB_SRC 
tests/test_graph.cpp
tests/test_kcore.cpp
tests/test_kcore_in_folder.cpp
tests/test_random_graph.cpp
tests/test_sparse_graph.cpp
tests/test_format.cpp
tests/test_graph_properties_in_folder.cpp
tests/test_degree.cpp
tests/test_color.cpp

)

IF(BII_tests_gtest_main_SRC)

LIST(APPEND BII_tests_gtest_main_SRC 
tests/test_graph.cpp
tests/test_degree.cpp
tests/test_color.cpp
#tests/test_kcore.cpp
#tests/test_kcore_in_folder.cpp
#tests/test_random_graph.cpp
#tests/test_sparse_graph.cpp
#tests/test_format.cpp
#tests/test_graph_properties_in_folder.cpp
)

ENDIF(BII_tests_gtest_main_SRC)

# Including tools.cmake from biicode/cmake user block
INCLUDE(biicode/cmake/tools)

# Actually create targets: EXEcutables, STATIC libraries.
ADD_BIICODE_TARGETS()


# Calling specific macro to activate c++11 flags
ACTIVATE_CPP11(INTERFACE ${BII_BLOCK_TARGET})