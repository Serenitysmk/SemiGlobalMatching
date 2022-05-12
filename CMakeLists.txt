cmake_minimum_required(VERSION 3.10)

project(SemiGlobalMatching)

################################
# Find packages
################################

# OpenCV
find_package(OpenCV 4 REQUIRED)

# COLMAP
find_package(COLMAP REQUIRED)


# Common include directories
set(COMMON_INCLUDE_DIRS
    ${OpenCV_INCLUDE_DIRS}
    ${COLMAP_INCLUDE_DIRS}
)

set(COMMON_LINK_LIBRARIES
    ${OpenCV_LIBRARIES}
    ${COLMAP_LIBRARIES}
)

add_subdirectory(src)