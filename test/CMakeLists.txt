# Project
project("zqf_zxjson_test" VERSION 1.0 LANGUAGES C CXX)

# Executable
add_executable(${PROJECT_NAME} "main.cpp")

# Compiler
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)

# Warning
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra>)

# External Libraries
find_package(Catch2 CONFIG)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain ZQF::Zut::ZxJson)

# Testing
include(Catch)
catch_discover_tests("zqf_zxjson_test")
