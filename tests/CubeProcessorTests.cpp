#include <limits>
#include <cmath>

#include <gtest/gtest.h>

#include "../src/CubeProcessor.hpp"

TEST(CubeProcessor, Positive) {
    double value = 2.;
    double expected = value * value * value;
    CubeProcessor cubeProcessor;
    EXPECT_DOUBLE_EQ(cubeProcessor.process(value), expected);
}

TEST(CubeProcessor, Overflow) {
    double value = std::numeric_limits<double>::max();
    CubeProcessor cubeProcessor;
    EXPECT_TRUE(std::isinf(cubeProcessor.process(value)));
}
