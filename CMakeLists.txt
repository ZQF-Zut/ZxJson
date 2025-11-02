cmake_minimum_required(VERSION 3.31)

# VCPKG
if(ZQF_VCPKG)
  include("cmake/vcpkg/fetch.cmake")
endif()

# Project
project(zqf_zut_zxjson_main)

# Modules
find_package(zqf_cmake_modules CONFIG REQUIRED)

# Config
if(ZQF_DEVELOP)
  include("zqf_settings")
  include("zqf_profiles")
endif()

# Source
add_subdirectory("src")

# Dev
if(ZQF_DEVELOP)
  add_subdirectory("tryout")
endif()

# Testing
if(ZQF_TESTING)
  enable_testing()
  add_subdirectory("test")
endif()
