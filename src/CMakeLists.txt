set(SGM_SRCS)

add_executable(main main.cc)
target_include_directories(main PRIVATE ${COMMON_INCLUDE_DIRS})
target_link_directories(main PRIVATE ${COLMAP_LINK_DIRS})
target_link_libraries(main PRIVATE ${COMMON_LINK_LIBRARIES})