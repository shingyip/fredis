find_package(Catch2 3 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIREDON)

add_executable(fredis_test monadic_tests.cpp)
target_link_libraries(fredis_test PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(fredis_test)