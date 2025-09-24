file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ../src/*.cpp)
file(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS *.cpp)

foreach(test_src ${TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src} ${SRC_FILES})
    target_link_libraries(${test_name}
        PRIVATE
            fmt::fmt
            GTest::gtest
            GTest::gtest_main
            CLI11::CLI11
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()