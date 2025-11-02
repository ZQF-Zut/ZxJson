cmake_minimum_required(VERSION 3.31)

# VCPKG
if(ZQF_VCPKG)
 include("cmake/modules/zqf_vcpkg.cmake")
endif()

# Project
project(zqf_zut_zxjson_main)

# Modules
find_package(zqf_cmake_modules CONFIG REQUIRED)

# Config
if(ZQF_DEVMODE)
  include("zqf_settings")
  include("zqf_profiles")
endif()

# Source
add_subdirectory("src")

# Dev
if(ZQF_DEVMODE)
  add_subdirectory("tryout")
endif()

# Testing
if(ZQF_TESTING)
  enable_testing()
  add_subdirectory("test")
endif()
