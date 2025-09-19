#include <gtest/gtest.h>

#include "../src/SquareProcessor.hpp"

TEST(SquareProcessor, Positive) {
    double value = 2.;
    double expected = 4.;
    EXPECT_DOUBLE_EQ(SquareProcessor::calcSquare(value), expected);
}
