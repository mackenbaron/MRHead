set(ProjName MRCrossPlatform)
project(${ProjName})
cmake_minimum_required(VERSION 2.8.11)

if(WIN32)
	AUX_SOURCE_DIRECTORY(. SRC_LIST)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	add_executable(${ProjName} ${SRC_LIST})
else()
	find_package(OpenCV REQUIRED)
	include_directories(${OPENCV_DIRS})
	AUX_SOURCE_DIRECTORY(. SRC_LIST)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	add_executable(${ProjName} ${SRC_LIST})
	target_link_libraries (${ProjName} ${OpenCV_LIBS})
endif()
install(TARGETS ${ProjName}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../mropencv.h" DESTINATION "include")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../tickmeter.hpp" DESTINATION "include")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../mrdir.h"  DESTINATION "include")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../mrutil.h" DESTINATION "include")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/opencv.png" DESTINATION "./")	