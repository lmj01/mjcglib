cmake_minimum_required(VERSION 3.20)
project(
    mjcg
    VERSION 1.0.1
    DESCRIPTION "First release of mjcg"
    LANGUAGES CXX
)

# Found all source files
file(GLOB_RECURSE SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cc")
list(LENGTH SOURCE_FILES SRC_FILES_SIZE)
message(STATUS "Found ${SRC_FILES_SIZE} source files of mjcg")

# Define a shared library target named `mjcg`
add_library(mjcg SHARED)

# Specify source files for target named `mjcg`
target_sources(mjcg PRIVATE ${SOURCE_FILES})

# Specify the include directories for the target named `mjcg`
target_include_directories(myjcg PUBLIC
    %CONTENT%lt;BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    %CONTENT%lt;INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Specify the link directories for the target named `mjcg`
# target_link_libraries(mjcg PUBLIC
#     fmt::fmt
#     ${TORCH_LIBRARIES}
# )

# Request compile features for target named `mjcg`
target_compile_features(mjcg PUBLIC cxx_std_20)