cmake_minimum_required(VERSION 3.0.0)
project(RTS-Monitor VERSION 0.1.0)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

include(CTest)
enable_testing()

add_executable(RTS-Monitor main.cpp
               haarcascade_frontalface_alt.xml
               TrackingMenager.hpp
               TrackingMenager.cpp
               FaceTracker.hpp
               FaceTracker.cpp
               FaceMonitorFunc.hpp
               FaceMonitorFunc.cpp)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
target_link_libraries( RTS-Monitor ${OpenCV_LIBS} pthread rt)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
