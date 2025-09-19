#include "../src/CubeProcessor.hpp"
#include "../src/Processor.hpp"
#include "../src/ProcessorFactory.hpp"
#include "../src/SquareProcessor.hpp"
#include <gtest/gtest.h>

TEST(ProcessorFactoryTest, CreateAndProcess) {
  auto cube = ProcessorFactory::instance().create(cubeFactoryName);
  auto square = ProcessorFactory::instance().create(squareFactoryName);

  ASSERT_NE(cube, nullptr);
  ASSERT_NE(square, nullptr);

  double val = 3.0;

  EXPECT_DOUBLE_EQ(cube->process(val), 27.0);  // 3^3
  EXPECT_DOUBLE_EQ(square->process(val), 9.0); // 3^2
}
