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
zqf_set_encoding(${PROJECT_NAME} PRIVATE UTF8)
zqf_set_warning(${PROJECT_NAME} PRIVATE AllExtra)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# Properties
set_target_properties(${PROJECT_NAME} PROPERTIES EXPORT_NAME ZxJson)

# Install
include("zqf_installer")
zqf_installer(
  ${PROJECT_NAME}
  NAMESPACE "ZQF::Zut"
  HEADER_FILE_DIR "${CMAKE_CURRENT_LIST_DIR}/src/"
  CONFIG_TEMPLATE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Config.cmake.in"
)

# Doc
if(ZQF_DOXYGEN)
  include("zqf_doxygen")
  zqf_doxygen(${PROJECT_NAME})
endif()
