project(zqf_zut_zxjson_tryout VERSION 1.0 LANGUAGES C CXX)

# Test Project
add_executable(${PROJECT_NAME} "main.cpp")

# Compiler
zqf_set_encoding(${PROJECT_NAME} PRIVATE UTF8)
zqf_set_warning(${PROJECT_NAME} PRIVATE AllExtra)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# Add Library
target_link_libraries("${PROJECT_NAME}" PRIVATE ZQF::Zut::ZxJson)
