
add_library(snappy_util     
    ${CMAKE_CURRENT_SOURCE_DIR}/snappy_util.h
    ${CMAKE_CURRENT_SOURCE_DIR}/snappy_util.cpp
)

set(google_snappy_build_dir ${PROJECT_BINARY_DIR}/snappy)
IF(IS_DIRECTORY ${google_snappy_build_dir})
    target_include_directories(snappy_util PRIVATE ${google_snappy_build_dir})
else()
    message(STATUS "Failed to find google_snappy_build_dir:" ${google_snappy_build_dir})
ENDIF()
TARGET_LINK_LIBRARIES(snappy_util PRIVATE snappy)


if(Boost_INCLUDE_DIR)
    find_package(Boost 1.57 COMPONENTS system filesystem  program_options  REQUIRED)
    INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR}  )

    add_executable(snappy_archive ${CMAKE_CURRENT_SOURCE_DIR}/snappy_exe.cpp)
    set_target_properties(snappy_archive PROPERTIES OUTPUT_NAME "snappy_morse_key_exe")
    TARGET_LINK_LIBRARIES(snappy_archive snappy_util snappy ${Boost_LIBRARIES})    
endif()