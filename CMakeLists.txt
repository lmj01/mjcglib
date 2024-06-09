cmake_minimum_required(VERSION 3.20)
project(
    mjcg
    VERSION 1.0.1
    DESCRIPTION "First release of mjcg"
    LANGUAGES C CXX
)

# Enable linking of this library on windows
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

# Setting of directory variables for convenience
set(SUBMODULE_DIR "${PROJECT_SOURCE_DIR}/third")

# -- fmt --
if(NOT EXISTS "${SUBMODULE_DIR}/fmt/CMakeLists.txt")
    message(FATAL_ERROR "Submodule 'fmt' was not downloaded! Please update submodules and try again.")
endif()

# problem with shared library can't link static library
# relocation R_X86_64_PC32 against symbol can not be used when making a shared object; recompile with -fPIC
# set (CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "-fPIC")

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

set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/third/glad")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)

glad_add_library(glad_gl_core_mx_46 REPRODUCIBLE MX API gl:core=4.6)

# -- glad --


# Found all source files
file(GLOB_RECURSE SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cc")
list(LENGTH SOURCE_FILES SRC_FILES_SIZE)
message(STATUS "Found ${SRC_FILES_SIZE} source files of mjcg")

# Define a shared library target named `mjcg`
add_library(mjcg SHARED)

# A workaround is to turn of position independent execution (PIE) 
target_link_options(mjcg PRIVATE "-no-pie")

# Specify source files for target named `mjcg`
target_sources(mjcg PRIVATE ${SOURCE_FILES})

# Specify the include directories for the target named `mjcg`
target_include_directories(mjcg PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Specify the link directories for the target named `mjcg`
# target_link_libraries(mjcg PUBLIC   
#     glad_gl_core_mx_46
# )
target_link_libraries(${PROJECT_NAME} $<BUILD_INTERFACE:fmt>)
target_link_libraries(${PROJECT_NAME} $<BUILD_INTERFACE:glfw>)
target_link_libraries(${PROJECT_NAME} $<BUILD_INTERFACE:glm>)

# Request compile features for target named `mjcg`
target_compile_features(mjcg PUBLIC cxx_std_20)

# --- app0 ---
# Found all source files
file(GLOB_RECURSE SOURCE_APP0 "${PROJECT_SOURCE_DIR}/src/app/app0.cpp")

add_executable(app0 ${SOURCE_APP0})
target_link_libraries(app0  PUBLIC
    mjcg
)
# --- app0 ---

# --- app1 ---
# Found all source files
file(GLOB_RECURSE SOURCE_APP1 "${PROJECT_SOURCE_DIR}/src/app/app1.cpp")

add_executable(app1 ${SOURCE_APP1})
target_link_libraries(app1  PUBLIC
    glad_gl_core_mx_46
    glfw
)
# --- app1 ---
