cmake_minimum_required(VERSION 4.2.3)

project(
  InariKonKonGameFramework
  VERSION 0.0.2
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 26)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(BUILD_EXAMPLES "Build example applications" ON)

# set(BUILD_SHARED_LIBS ON CACHE BOOL "Build using shared libraries" FORCE)
# set(BUILD_EXAMPLES OFF CACHE BOOL "Build example applications" FORCE)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utility/ProjectVersion.cppm.temp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utility/ProjectVersion.cppm @ONLY)

add_subdirectory(src)

add_library(InariKonKon)
add_library(InariKonKon::InariKonKon ALIAS InariKonKon)
target_sources(InariKonKon PUBLIC FILE_SET CXX_MODULES TYPE CXX_MODULES FILES
                                  ${INARIKONKON_MODULES})

add_subdirectory(dependencies)
target_link_libraries(InariKonKon PRIVATE Dependencies)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(
    InariKonKon
    PRIVATE -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wsign-conversion
            -Wnull-dereference
            -Wdouble-promotion
            -Wformat=2)
endif()

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
