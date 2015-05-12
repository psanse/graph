#include <iostream>

#include "google/gtest/gtest.h"
#include "../simple_ugraph.h"
#include "../kcore.h"


GTEST_API_ int main(int argc, char **argv) {
  std::cout << "Running main() from gtest_main.cc\n";

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



