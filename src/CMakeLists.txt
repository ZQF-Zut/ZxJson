# ZxJson
cmake_minimum_required(VERSION 3.25)

# Project Name
project(Zut_ZxJson)

# Set Source File Path
list(APPEND "${PROJECT_NAME}_SRC_FILE" 
    "src/Zut/ZxJson/JDoc.cpp" 
    "src/Zut/ZxJson/JIO.cpp"
    "src/Zut/ZxJson/JParser.cpp"
    "src/Zut/ZxJson/JValue.cpp"
    "src/Zut/ZxJson/Plat.cpp")

# Set Library
add_library(${PROJECT_NAME} "${${PROJECT_NAME}_SRC_FILE}")
add_library(Zut::ZxJson ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES EXPORT_NAME ZxJson)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/> $<INSTALL_INTERFACE:include>)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)

# Warning
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

# Install
include(GNUInstallDirs)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT headers
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
install(
  EXPORT ${PROJECT_NAME}-targets
  FILE ${PROJECT_NAME}-config.cmake
  DESTINATION lib/cmake/${PROJECT_NAME}
  NAMESPACE Zut::
)