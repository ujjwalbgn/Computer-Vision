project (tracking)
cmake_minimum_required(VERSION 3.15)

# configure OpenCV
find_package(OpenCV REQUIRED)

# create create individual projects
add_executable(ballTracker ballTracker.cpp)
target_link_libraries(ballTracker ${OpenCV_LIBS})