cmake_minimum_required(VERSION 3.5.0)
project(ps_ray_trace VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_BUILD_TYPE "Release")

if(WIN32)
    add_compile_definitions(WIN32)
    if(MSVC)
        add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:/EHsc>)
    endif()
endif()

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sources)
set(ISPC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ispc_shaders)

# ---------------------------------------------------------------------------------------------------------------------------------------------
# -- Files

set(HEADER_FILES
    ${INCLUDE_DIR}/image_writer.h 
    ${INCLUDE_DIR}/stb_image_write.h    
    ${INCLUDE_DIR}/utility.h
)

set(ISPC_FILES
    ${ISPC_DIR}/tracer.ispc
    ${ISPC_DIR}/sphere.ispc
    ${ISPC_DIR}/hittable.ispc
)

set(CPP_FILES
    ${SOURCE_DIR}/image_writer.cpp
    ${SOURCE_DIR}/utility.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(SRC_FILES 
    ${ISPC_FILES}
    ${HEADER_FILES}
    ${CPP_FILES}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/fmt/include/fmt
    ${CMAKE_CURRENT_SOURCE_DIR}/ispc_shaders
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

add_subdirectory(third-party/fmt)

# ---------------------------------------------------------------------------------------------------------------------------------------------
# -- Executable Creation

if(WIN32 OR LINUX)
    find_program(ISPC_EXECUTABLE ispc PATHS ${CMAKE_CURRENT_SOURCE_DIR}/third-party/ispc/bin)
    if (NOT ISPC_EXECUTABLE)
        message(FATAL_ERROR "Failed to find ispc")
    else()
        message(STATUS "ispc is found")
        set(CMAKE_ISPC_COMPILER ${ISPC_EXECUTABLE})
    endif()
endif()

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.19.2 AND (("${CMAKE_GENERATOR}" MATCHES "Make") OR ("${CMAKE_GENERATOR}" MATCHES "Ninja")) )
    enable_language(ISPC)

    add_executable(${PROJECT_NAME})
    target_sources(${PROJECT_NAME} PRIVATE
        ${SRC_FILES}
    )
else()
    message(FATAL_ERROR "GOD NOOOOOOOOOO!!!")
endif()

# ---------------------------------------------------------------------------------------------------------------------------------------------
# -- Executable Configurations

target_link_libraries(${PROJECT_NAME} fmt::fmt)

target_precompile_headers(
    ${PROJECT_NAME} PRIVATE
    ${HEADER_FILES}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

if (NOT WIN32)
    set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(ISPC_OPT_FLAG "-g -O0")
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:ISPC>:-g>)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:ISPC>:-O0>)
elseif(${CMAKE_BUILD_TYPE} MATCHES Release)
    set(ISPC_OPT_FLAG -O3)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:ISPC>:-O3>)
endif()

foreach(ISPC_FILE ${ISPC_FILES})
    get_filename_component(ISPC_FILENAME ${ISPC_FILE} NAME_WE)

    add_custom_target(
        ${ISPC_FILENAME}_ISPC_TARGET
        COMMAND ${CMAKE_ISPC_COMPILER} ${ISPC_FILE} -h ${ISPC_DIR}/${ISPC_FILENAME}.h -o ${ISPC_FILENAME}.obj ${ISPC_OPT_FLAG} -I ${ISPC_DIR}/ 
    )

    add_dependencies(${PROJECT_NAME} ${ISPC_FILENAME}_ISPC_TARGET)
    
endforeach(ISPC_FILE)


# ---------------------------------------------------------------------------------------------------------------------------------------------

if(MSVC)
    # Group ISPC files inside Visual Studio
    source_group("ISPC" FILES ${ISPC_FILES})
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
