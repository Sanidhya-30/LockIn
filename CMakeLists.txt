cmake_minimum_required(VERSION 3.15)
project(LockIn)
set(CMAKE_CXX_STANDARD 17)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories("/usr/local/include/opencv4/opencv2")

add_executable(main main.cpp)
add_executable(test test.cpp)

target_link_libraries(main ${OpenCV_LIBS})
target_link_libraries(test ${OpenCV_LIBS})

