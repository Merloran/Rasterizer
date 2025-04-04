cmake_minimum_required(VERSION 3.26)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

set(targetName Rasterizer)
project(${targetName})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(spdlog CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_image.h" REQUIRED)

add_executable(${targetName} "Code/main.cpp")

file(GLOB_RECURSE sourceFiles
     "${CMAKE_SOURCE_DIR}/Code/*.cpp"
     "${CMAKE_SOURCE_DIR}/Code/*.h"
     "${CMAKE_SOURCE_DIR}/Code/*.hpp"
     "${CMAKE_SOURCE_DIR}/Code/*.inl")

list(REMOVE_ITEM sourceFiles "${CMAKE_SOURCE_DIR}/Code/main.cpp")

target_sources(${targetName} PRIVATE ${sourceFiles})

source_group("Project" FILES "Code/main.cpp")
foreach(sourceFile IN ITEMS ${sourceFiles})
    get_filename_component(sourcePath "${sourceFile}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}/Code" "" groupPath "${sourcePath}")
    string(REPLACE "/" "\\" groupPath "${groupPath}")
    source_group("Project\\${groupPath}" FILES "${sourceFile}")
endforeach()

target_compile_options(${targetName} PRIVATE /utf-8)

target_include_directories(${targetName} PRIVATE ${STB_INCLUDE_DIRS})
target_link_libraries(${targetName} PRIVATE spdlog::spdlog)
target_link_libraries(${targetName} PRIVATE magic_enum::magic_enum)
  
# Precompiled headers and force include for pch
target_precompile_headers(${targetName} PRIVATE Code/Core/Utilities/pch.hpp)
if (MSVC)
    target_compile_options(${targetName} PRIVATE /FI"${CMAKE_BINARY_DIR}/CMakeFiles/${targetName}.dir/$<CONFIG>/cmake_pch.hxx")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${targetName} PRIVATE -include "$(IntDir)cmake_pch.hxx")
endif()

target_include_directories(${targetName} PRIVATE
    ${CMAKE_SOURCE_DIR}/Code
    ${CMAKE_SOURCE_DIR}/Code/Core)