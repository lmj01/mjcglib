cmake_minimum_required(VERSION 3.20)
project(
    mjcg
    VERSION 1.0.1
    DESCRIPTION "First release of mjcg"
    LANGUAGES C CXX
)

# set the install folder
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT) 
    message(STATUS "Setting default CMAKE_INSTALL_PREFIX path to ${CMAKE_BINARY_DIR}/install")
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE STRING "the path to use make install" FORCE)
endif()


# Enable linking of this library on windows
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

# Setting of directory variables for convenience
set(SUBMODULE_DIR "${PROJECT_SOURCE_DIR}/third")

# -- fmt --
if(NOT EXISTS "${SUBMODULE_DIR}/fmt/CMakeLists.txt")
    message(FATAL_ERROR "Submodule 'fmt' was not downloaded! Please update submodules and try again.")
endif()

# problem with shared library can't link static library
if (CMAKE_HOST_UNIX)
    # relocation R_X86_64_PC32 against symbol can not be used when making a shared object; recompile with -fPIC
    # set (CMAKE_POSITION_INDEPENDENT_CODE ON)
    set(CMAKE_CXX_FLAGS "-fPIC")
endif()

# Disable targets that are not needed
set(FMT_DOC OFF CACHE BOOL "" FORCE)
set(FMT_TEST OFF CACHE BOOL "" FORCE)
set(FMT_FUZZ OFF CACHE BOOL "" FORCE)
set(FMT_CUDA_TEST OFF CACHE BOOL "" FORCE)
set(FMT_OS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory("${SUBMODULE_DIR}/fmt")

# -- fmt --

# -- GLFW --
if(NOT EXISTS "${SUBMODULE_DIR}/glfw/CMakeLists.txt")
    message(FATAL_ERROR "Submodule 'GLFW' was not downloaded! Please update submodules and try again.")
endif()

# Disable targets that are not needed
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
add_subdirectory("${SUBMODULE_DIR}/glfw")

# -- GLFW --

# -- GLM --
if(NOT EXISTS "${SUBMODULE_DIR}/glm/CMakeLists.txt")
    message(FATAL_ERROR "Submodule 'GLM' was not downloaded! Please update submodules and try again.")
endif() 

add_subdirectory("${SUBMODULE_DIR}/glm")

# -- GLM --

# -- glad --
# set(GLAD_GEN_DIR "${PROJECT_SOURCE_DIR}/third/glad/generated")
set(GLAD_GEN_DIR "${PROJECT_SOURCE_DIR}/third/gladGen")

# if (NOT EXISTS "${GLAD_GEN_DIR}/args.txt")
    set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/third/glad")
    add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)
    glad_add_library(glad REPRODUCIBLE LOCATION "${GLAD_GEN_DIR}" API gl:core=4.6)
# else()

# endif()
set(GLAD_INCLUDE_DIR "${GLAD_GEN_DIR}/include")
# -- glad --

# -- lnlib --

set(ENABLE_UNIT_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory("${SUBMODULE_DIR}/lnlib")

# -- lnlib --


# Found all source files
file(GLOB_RECURSE SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.*")
list(LENGTH SOURCE_FILES SRC_FILES_SIZE)
message(STATUS "Found ${SRC_FILES_SIZE} source files of ${PROJECT_NAME}")

# Define a shared library target named 
add_library(${PROJECT_NAME} 
    # SHARED
    STATIC
)

if (CMAKE_HOST_UNIX)
    # A workaround is to turn of position independent execution (PIE) 
    target_link_options(${PROJECT_NAME} PRIVATE "-no-pie")
endif()

# Specify source files for target
target_sources(${PROJECT_NAME} 
    PRIVATE 
        ${SOURCE_FILES}
)

include_directories("${GLAD_INCLUDE_DIR}") # glad
include_directories("${SUBMODULE_DIR}/glm") # glm
include_directories("${SUBMODULE_DIR}/glfw/include") # glm
# Specify the include directories for the target
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # $<BUILD_INTERFACE:${GLAD_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Specify the link directories for the target named `mjcg`
target_link_libraries(${PROJECT_NAME} 
    PUBLIC glad
    PUBLIC $<BUILD_INTERFACE:fmt>
    PUBLIC $<BUILD_INTERFACE:glfw>
    PUBLIC $<BUILD_INTERFACE:glm>
)

# Request compile features for target named `mjcg`
target_compile_features(mjcg PUBLIC cxx_std_20)

# --- app0 ---
# Found all source files
file(GLOB_RECURSE SOURCE_APP0 "${PROJECT_SOURCE_DIR}/app/app0.cpp")

add_executable(app0 ${SOURCE_APP0})
target_link_libraries(app0
    PUBLIC
        mjcg
)
# --- app0 ---

# --- app1 ---
# Found all source files
# set(app1)
# file(GLOB_RECURSE SOURCE_APP1 "${PROJECT_SOURCE_DIR}/app/app1.cpp")
# add_executable(app1 ${SOURCE_APP1})
# target_include_directories (app1 
#     PRIVATE 
#         ${GLAD_INCLUDE_DIR}
# )
# target_link_libraries(app1
#     PUBLIC
#         glad
#         glfw
# )
# --- app1 ---

# Defines the ${CMAKE_INSTALL_INCLUDEDIR} and ${CMAKE_INSTALL_LIBDIR} variable.
include(GNUInstallDirs)

# Make executable target `mylib` installable
install(TARGETS mjcg
        EXPORT mjcg-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install the header files
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)