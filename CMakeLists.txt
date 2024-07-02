cmake_minimum_required(VERSION 3.5.0)
project(ps_ray_trace VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

if(WIN32)
    add_compile_definitions(WIN32)
    if(MSVC)
        add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:/EHsc>)
    endif()
endif()

# ---------------------------------------------------------------------------------------------------------------------------------------------
# -- Files

file(GLOB HEADER_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/ispc_shaders/*.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/*.h
)

file(GLOB ISPC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/ispc_shaders/*.ispc)

file(GLOB CPP_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp 
    main.cpp
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

if(WIN32)
    find_program(ISPC_EXECUTABLE ispc PATHS ${CMAKE_CURRENT_SOURCE_DIR}/third-party/ispc/bin)
    if (NOT ISPC_EXECUTABLE)
        message(FATAL_ERROR "Failed to find ispc")
    else()
        message(STATUS "ispc is found")
    endif()

    set(CMAKE_ISPC_COMPILER ${ISPC_EXECUTABLE})
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

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY ISPC_INSTRUCTION_SETS "avx2")

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(ISPC_OPT_FLAG -g)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:ISPC>:-g>)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:ISPC>:-O0>)
elseif(${CMAKE_BUILD_TYPE} MATCHES Release)
    set(ISPC_OPT_FLAG -O2)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:ISPC>:-O2>)
endif()

foreach(ISPC_FILE ${ISPC_FILES})
    get_filename_component(ISPC_FILENAME ${ISPC_FILE} NAME_WE)

    add_custom_target(
        ${ISPC_FILENAME}_ISPC_TARGET
        COMMAND ${CMAKE_ISPC_COMPILER} ${ISPC_FILE} -o ${CMAKE_CURRENT_SOURCE_DIR}/ispc_shaders/${ISPC_FILENAME}.h ${ISPC_OPT_FLAG} -I ispc_shaders/ --target avx2 
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
