cmake_minimum_required(VERSION 3.31)

# VCPKG
if(ZQF_VCPKG)
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/.vcpkg/vcpkg-fetch.cmake")
    file(DOWNLOAD https://raw.githubusercontent.com/Dir-A/Registry/5a4db340dd77316e86180ae22a690b4b99db54f7/cmake/vcpkg_fetch.cmake "${CMAKE_SOURCE_DIR}/.vcpkg/vcpkg-fetch.cmake")
  endif()

  set(ZQF_VCPKG_VERSION "2026-05-27")
  include("${CMAKE_SOURCE_DIR}/.vcpkg/vcpkg-fetch.cmake")
endif()

# Project
project(zqf_zut_zxjson_main)

# Modules
find_package(zqf_cmake_modules CONFIG REQUIRED)

# Config
if(ZQF_DEVELOP)
  include("zqf_profiles")
  include("zqf_optimizer")
  include("zqf_settings")
  zqf_set_output_dir()
  zqf_optimizer_enable_lto()
  zqf_optimizer_enable_gc_sections()
  zqf_profiles_fetch(CLANGD EDITOR_CONFIG)
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
