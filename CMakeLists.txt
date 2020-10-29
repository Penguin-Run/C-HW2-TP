cmake_minimum_required(VERSION 3.6)
project(C_HW2_TP)

set(PROJECT_NAME C_HW2_TP)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -fPIC")

set(ALL_SOURCE_FILES
        main.c
        )

set(UNIT_TESTS
        unit_tests/tests.cpp
        unit_tests/test_main.cpp
        )

#-------------------------static-lib-----------------------
set(STATIC_LIB consecutive_work_lib)

set(SOURCE_FILES_STATIC_LIB
        src/consecutive_work.c
        src/IO_manager.c
        )

set(HEADER_FILES_STATIC_LIB
        include/IO_manager.h
        )

add_library(${STATIC_LIB} STATIC ${SOURCE_FILES_STATIC_LIB})
target_link_libraries(${STATIC_LIB})
target_include_directories(${STATIC_LIB} PUBLIC include)

#------------------------end-static-lib---------------------


#-------------------------dynamic-lib-----------------------
set(DYNAMIC_LIB parallel_work_lib)

set(SOURCE_FILES_DYNAMIC_LIB
        src/parallel_work.c
        src/IO_manager.c
        )

set(HEADER_FILES_DYNAMIC_LIB
        include/IO_manager.h
        )

set ( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
add_library(${DYNAMIC_LIB} SHARED ${SOURCE_FILES_DYNAMIC_LIB})
target_link_libraries(${DYNAMIC_LIB})
target_include_directories(${DYNAMIC_LIB} PUBLIC include)

#------------------------end-dynamic-lib---------------------


#---------------------------google-tests---------------------------

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()


#----------------------------include-test-----------------------------

add_executable(test_consecutive ${UNIT_TESTS})
target_link_libraries(test_consecutive gtest gtest_main -ldl pthread ${STATIC_LIB})

add_executable(test_parallel ${UNIT_TESTS})
target_link_libraries(test_parallel gtest gtest_main -ldl pthread ${DYNAMIC_LIB})

#----------------------------end-include-test-----------------------------

#---------------------------end-google-tests---------------------------

add_executable(${PROJECT_NAME} ${ALL_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} -lm -ldl pthread ${STATIC_LIB})


