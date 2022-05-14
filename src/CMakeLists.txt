set(SGM_SRCS
    semi_global_matching.h semi_global_matching.cc
)

add_library(SGM ${SGM_SRCS})
target_include_directories(SGM PUBLIC ${COMMON_INCLUDE_DIRS})
target_link_directories(SGM PUBLIC ${COLMAP_LINK_DIRS})
target_link_libraries(SGM PUBLIC ${COMMON_LINK_LIBRARIES})

add_executable(main main.cc)
target_link_libraries(main PRIVATE SGM)

add_executable(general_test general_test.cc)
target_link_libraries(general_test PRIVATE SGM)