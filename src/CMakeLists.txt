# Project Name
project(zqf_zut_zxjson VERSION 1.0 LANGUAGES C CXX)

# Set Library
add_library(${PROJECT_NAME}
  "src/Zut/ZxJson/JDoc.cpp"
  "src/Zut/ZxJson/JIO.cpp"
  "src/Zut/ZxJson/JParser.cpp"
  "src/Zut/ZxJson/JValue.cpp"
  "src/Zut/ZxJson/Plat.cpp"
)
add_library(ZQF::Zut::ZxJson ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/> $<INSTALL_INTERFACE:include>)

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

# Properties
set_target_properties(${PROJECT_NAME} PROPERTIES EXPORT_NAME ZxJson)

# Install
include("ZQF_Installer")
zqf_installer(
  ${PROJECT_NAME}
  NAMESPACE "ZQF::Zut"
  HEADER_FILE_DIR "${CMAKE_CURRENT_LIST_DIR}/src/"
  CONFIG_TEMPLATE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Config.cmake.in"
)

# Doc
if(ZQF_DOXYGEN)
  include("ZQF_Doxygen")
  zqf_doxygen(${PROJECT_NAME})
endif()
