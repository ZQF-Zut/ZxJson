project(zqf_zut_zxjson_tryout VERSION 1.0 LANGUAGES C CXX)

# Test Project
add_executable(${PROJECT_NAME} "main.cpp")

# Compiler
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8 /W4> # msvc
  $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra> # gcc
  $<$<OR:$<C_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:Clang>>: # clang
    $<$<OR:$<C_COMPILER_FRONTEND_VARIANT:GNU>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>:-Wall -Wextra> # gnu frontend
    $<$<OR:$<C_COMPILER_FRONTEND_VARIANT:MSVC>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>:/utf-8 /W4> # msvc frontend
  >
)

# Add Library
target_link_libraries("${PROJECT_NAME}" PRIVATE ZQF::Zut::ZxJson)
