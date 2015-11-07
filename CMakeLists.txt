##############################
# General Settings
##############################

cmake_minimum_required(VERSION 2.8)
project(libespm2)

option(PROJECT_STATIC_RUNTIME "Build with static runtime libs (/MT)" ON)

set (Boost_USE_STATIC_LIBS ${PROJECT_STATIC_RUNTIME})
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_RUNTIME ${PROJECT_STATIC_RUNTIME})

find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)

set(PROJECT_HEADERS "${CMAKE_SOURCE_DIR}/include/libespm2/FormId.h"
                    "${CMAKE_SOURCE_DIR}/include/libespm2/Group.h"
                    "${CMAKE_SOURCE_DIR}/include/libespm2/SkyrimPlugin.h"
                    "${CMAKE_SOURCE_DIR}/include/libespm2/SkyrimRecord.h")

set(TESTER_SRC "${CMAKE_SOURCE_DIR}/src/tests/main.cpp")

set(TESTER_HEADERS "${CMAKE_SOURCE_DIR}/src/tests/SkyrimPluginTest.h"
                   "${CMAKE_SOURCE_DIR}/src/tests/FormIdTest.h")

source_group("Header Files" FILES ${PROJECT_HEADERS} ${TESTER_HEADERS})

# Include source and library directories.
include_directories("${CMAKE_SOURCE_DIR}/src"
                    "${CMAKE_SOURCE_DIR}/include"
                     ${Boost_INCLUDE_DIRS}
                     ${GTEST_INCLUDE_DIRS})

##############################
# Platform-Specific Settings
##############################

# GCC and MinGW settings.
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "-O3 -std=c++11")
    set(CMAKE_CXX_FLAGS "-O3 -std=c++11")
    IF (PROJECT_STATIC_RUNTIME)
        set (CMAKE_EXE_LINKER_FLAGS "-static-libstdc++ -static-libgcc")
        set (CMAKE_SHARED_LINKER_FLAGS "-static-libstdc++ -static-libgcc")
        set (CMAKE_MODULE_LINKER_FLAGS "-static-libstdc++ -static-libgcc")
    ENDIF ()

    set(PROJECT_LIBRARIES "pthread")
else (MSVC)
    IF (PROJECT_STATIC_RUNTIME)
        FOREACH(flag
            CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_DEBUG_INIT
            CMAKE_CXX_FLAGS_RELEASE  CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_DEBUG  CMAKE_CXX_FLAGS_DEBUG_INIT)
            STRING(REPLACE "/MD"  "/MT" "${flag}" "${${flag}}")
            SET("${flag}" "${${flag}} /EHsc")
        ENDFOREACH()
    ENDIF ()
endif ()



##############################
# Define Targets
##############################

add_executable        (tests ${TESTER_SRC} ${PROJECT_HEADERS} ${TESTER_HEADERS})
target_link_libraries (tests ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES} ${PROJECT_LIBRARIES})
