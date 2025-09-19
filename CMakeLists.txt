cmake_minimum_required(VERSION 3.22)
project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Conan find_package
find_package(fmt REQUIRED)
find_package(GTest REQUIRED)

# Рекурсивно собираем cpp из src/
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# Линкуем Conan цели
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt GTest::gtest GTest::gtest_main)

enable_testing()

add_subdirectory(tests)

