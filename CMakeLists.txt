
cmake_minimum_required(VERSION 3.13)
project(safe_load_xtensa_elf CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

option(BUILD_32BIT "Compile the plugin as a 32-bit binary" OFF)
if(BUILD_32BIT)
  set(IdaSdk_COMPILE_32BIT ON)
  set(IdaSdk_LEGACY_FILE_EXTENSIONS ON)
else()
  set(_safe_load_xtensa_elf_ida64 IDA64)
endif()

find_package(IdaSdk REQUIRED)

if(MSVC)
  # Use the static runtime
  foreach(flag_var CMAKE_CXX_FLAGS
                   CMAKE_CXX_FLAGS_DEBUG
                   CMAKE_CXX_FLAGS_RELEASE
                   CMAKE_CXX_FLAGS_MINSIZEREL
                   CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif()
    if(${flag_var} MATCHES "/MDd")
      string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endif()
  endforeach()
endif()

add_ida_plugin(safe_load_xtensa_elf EA64 safe_load_xtensa_elf.cpp)
foreach(target safe_load_xtensa_elf${_plx} safe_load_xtensa_elf${_plx64})
  set_property(TARGET ${target} PROPERTY CXX_STANDARD 11)
  target_include_directories(${target} PRIVATE ${IdaSdk_INCLUDE_DIRS})
  target_compile_definitions(${target} PRIVATE ${_safe_load_xtensa_elf_ida64})
endforeach()
