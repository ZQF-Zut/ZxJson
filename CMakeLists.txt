cmake_minimum_required(VERSION 3.31)

# VCPKG
if(ZQF_VCPKG)
  include("cmake/ZQF_VCPKG.cmake")
  zqf_vcpkg_startup()
endif()

# Project
project(zqf_zut_zxjson_main)

# Modules
find_package(zqf_cmake_modules CONFIG REQUIRED)

# Config
if(ZQF_DEVMODE)
  include("ZQF_Settings")
endif()

# Source
add_subdirectory("src")

# Dev
if(ZQF_DEVMODE)
  enable_testing()
  add_subdirectory("test")
  add_subdirectory("tryout")
endif()
