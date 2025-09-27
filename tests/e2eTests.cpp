#include <gtest/gtest.h>

#include "../src/App.hpp"

TEST(E2E, Positive) {
    char* argv[3] = {"app", "--processor", "square"};
    App app(3, argv);

    EXPECT_EQ(EXIT_SUCCESS, app.run());
}

TEST(E2E, RunWithoutProcessor) {
    char* argv[1] = {"app"};
    App app(1, argv);

    EXPECT_NE(EXIT_SUCCESS, app.run());
}

TEST(E2E, Negative) {
    char* argv[3] = {"app", "--processor", "notExistedProcessor"};
    App app(3, argv);

    EXPECT_EQ(EXIT_FAILURE, app.run());
}
