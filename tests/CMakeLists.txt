# Tests CMake configuration

# Include Catch2's CMake integration
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/src/third_party/Catch2/extras)
include(CTest)
include(Catch)

# Create test executable
add_executable(SampleProjectTests
    test_main.cpp
    test_sample.cpp
)

target_link_libraries(SampleProjectTests PRIVATE
    Catch2::Catch2
    nlohmann_json::nlohmann_json
    easyloggingpp
    sqlite3
)

target_compile_features(SampleProjectTests PRIVATE cxx_std_20)

# Register tests with CTest
catch_discover_tests(SampleProjectTests)
