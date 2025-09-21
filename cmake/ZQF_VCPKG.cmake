function(zqf_vcpkg_startup)
    string(FIND "${CMAKE_TOOLCHAIN_FILE}" "vcpkg.cmake" found_pos)

    if(found_pos GREATER -1)
        message(STATUS "Found 'vcpkg.cmake' in toolchain file. VCPKG is active.")
        return()
    endif()

    set(options)
    set(one_value_keywords)
    set(multi_value_keywords BASE_URL)
    cmake_parse_arguments(ARG "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

    if(NOT ARG_BASE_URL)
        set(ARG_BASE_URL "https://github.com/microsoft/vcpkg-tool/releases/download/2025-09-03")
    endif()

    set(ENV{VCPKG_ROOT} "${CMAKE_BINARY_DIR}/vcpkg")

    if(WIN32)
        file(DOWNLOAD "${ARG_BASE_URL}/vcpkg-init.cmd" "$ENV{VCPKG_ROOT}/vcpkg-init.cmd")
        execute_process(COMMAND cmd.exe /c "$ENV{VCPKG_ROOT}/vcpkg-init.cmd")
    else()
        file(DOWNLOAD "${ARG_BASE_URL}/vcpkg-init" "$ENV{VCPKG_ROOT}/vcpkg-init")
        execute_process(COMMAND "$ENV{VCPKG_ROOT}/vcpkg-init")
    endif()

    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_TOOLCHAIN_FILE} "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" PARENT_SCOPE)
endfunction()
