cmake_minimum_required(VERSION 3.16)

project(Tensat
        LANGUAGES C CXX)

set(BUILD_TESTING ${MyLib_Test} CACHE BOOL "Build testing" FORCE)

include(cmake/configure-conan.cmake)
include(cmake/conan.cmake)

if (NOT CONAN_EXPORTED)
    conan_check()
endif ()

if (CONAN_EXPORTED)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else ()
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake")
        file(DOWNLOAD "https://raw.githubusercontent.com/akemimadoka/cmake-conan/develop/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake" SHOW_PROGRESS
                STATUS _download_status)
        list(GET _download_status 0 _download_status_code)
        list(GET _download_status 1 _download_status_msg)
        if(NOT _download_status_code EQUAL 0)
            file(REMOVE "${CMAKE_BINARY_DIR}/conan.cmake")
            message(FATAL_ERROR "Failed to download conan.cmake, status code is ${_download_status_code}, msg is ${_download_status_msg}")
        endif()
    endif()

    include(${CMAKE_SOURCE_DIR}/cmake/conan.cmake)
    conan_cmake_run(CONANFILE conanfile.py
            BASIC_SETUP CMAKE_TARGETS
            OPTIONS ${CONAN_OPTS}
            SETTINGS ${CONAN_SETTINGS}
            BUILD missing)
endif ()


add_subdirectory(src)
add_subdirectory(test)