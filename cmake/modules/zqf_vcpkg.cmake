include_guard(GLOBAL)

if(Z_VCPKG_ROOT_DIR)
  return()
endif()

function(zqf_vcpkg_fetch)
  set(zqf_vcpkg_tool_release_url "https://github.com/microsoft/vcpkg-tool/releases/download/2025-09-03")

  set(zqf_vcpkg_root_dir "${CMAKE_BINARY_DIR}/vcpkg")
  file(WRITE "${zqf_vcpkg_root_dir}/.vcpkg-root")

  if(WIN32)
    file(DOWNLOAD "${zqf_vcpkg_tool_release_url}/vcpkg-init.cmd" "${zqf_vcpkg_root_dir}/vcpkg-init.cmd")
    execute_process(COMMAND cmd.exe /c "${zqf_vcpkg_root_dir}/vcpkg-init.cmd")
  else()
    file(DOWNLOAD "${zqf_vcpkg_tool_release_url}/vcpkg-init" "${zqf_vcpkg_root_dir}/vcpkg-init")
    execute_process(COMMAND chmod +x "${zqf_vcpkg_root_dir}/vcpkg-init")
    execute_process(COMMAND bash -c ". ${zqf_vcpkg_root_dir}/vcpkg-init")
  endif()

  set(zqf_vcpkg_cmake_file_path "${zqf_vcpkg_root_dir}/scripts/buildsystems/vcpkg.cmake" PARENT_SCOPE)
endfunction()

zqf_vcpkg_fetch()
include(${zqf_vcpkg_cmake_file_path})
