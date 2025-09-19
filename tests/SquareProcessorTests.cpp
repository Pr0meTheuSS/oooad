#include <limits>
#include <cmath>

#include <gtest/gtest.h>

#include "../src/SquareProcessor.hpp"

TEST(SquareProcessor, Positive) {
    double value = 2.;
    double expected = 4.;
    SquareProcessor squareProcessor;
    EXPECT_DOUBLE_EQ(squareProcessor.process(value), expected);
}

TEST(SquareProcessor, Overflow) {
    double value = std::numeric_limits<double>::max();
    SquareProcessor squareProcessor;
    EXPECT_TRUE(std::isinf(squareProcessor.process(value)));
}
