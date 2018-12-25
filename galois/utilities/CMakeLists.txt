cmake_minimum_required(VERSION 3.6)


# # depends on boost::thread, boost::system, boost::atomic, boost::date_time, boost::chrono
# set(Boost_USE_STATIC_LIBS        OFF)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

set(source_dir ${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES( ${source_dir}/../.. )

set(headers 
	${source_dir}/core.hpp
	${source_dir}/singleton.hpp
	${source_dir}/singleton_ptr.hpp
	${source_dir}/spinlockmutex.h
    ${source_dir}/highlight.h
	
	${source_dir}/friend_close_helper.h
    ${source_dir}/trading_day.h
    ${source_dir}/trading_time_parser.h
	
	${source_dir}/trading_time_helper.h	 # # depends on trading_time_parser, local_time_helper
	${source_dir}/local_time_helper.h   # # depends on singleton, spinlockmutex
)

set(sources 
    ${source_dir}/highlight.cpp
    ${source_dir}/friend_close_helper.cpp
	${source_dir}/trading_time_parser.cpp
	${source_dir}/trading_day.cpp
	${source_dir}/trading_time_helper.cpp
	${source_dir}/local_time_helper.cpp
)


add_library(galois_utilities ${headers} ${sources})
TARGET_LINK_LIBRARIES(galois_utilities ${Boost_LIBRARIES}) 


add_executable(galois_utilities_test ${source_dir}/tests.cpp)
TARGET_LINK_LIBRARIES(galois_utilities_test galois_utilities ${Boost_LIBRARIES}) 

