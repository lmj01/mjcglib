cmake_minimum_required(VERSION 3.20)
project(
    mjcg
    VERSION 1.0.1
    DESCRIPTION "First release of mjcg"
    LANGUAGES CXX
)

# Enable linking of this library on windows
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

# Setting of directory variables for convenience
set(SUBMODULE_DIR "${PROJECT_SOURCE_DIR}/third")

# -- fmt --
if(NOT EXISTS "${SUBMODULE_DIR}/fmt/CMakeLists.txt")
    message(FATAL_ERROR "Submodule 'fmt' was not downloaded! Please update submodules and try again.")
endif()

# Disable targets that are not needed
set(FMT_DOC OFF CACHE BOOL "" FORCE)
set(FMT_TEST OFF CACHE BOOL "" FORCE)
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
add_subdirectory("${SUBMODULE_DIR}/glfw")

# -- GLFW --

# Found all source files
file(GLOB_RECURSE SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cc")
list(LENGTH SOURCE_FILES SRC_FILES_SIZE)
message(STATUS "Found ${SRC_FILES_SIZE} source files of mjcg")

# Define a shared library target named `mjcg`
add_library(mjcg SHARED)

# Specify source files for target named `mjcg`
target_sources(mjcg PRIVATE ${SOURCE_FILES})

# Specify the include directories for the target named `mjcg`
target_include_directories(mjcg PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Specify the link directories for the target named `mjcg`
# target_link_libraries(mjcg PUBLIC
#     fmt::fmt
#     # ${TORCH_LIBRARIES}
# )
target_link_libraries(${PROJECT_NAME} $<BUILD_INTERFACE:fmt>)
target_link_libraries(${PROJECT_NAME} $<BUILD_INTERFACE:glfw>)

# Request compile features for target named `mjcg`
target_compile_features(mjcg PUBLIC cxx_std_20)