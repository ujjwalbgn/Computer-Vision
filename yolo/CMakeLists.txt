project (yolo)
cmake_minimum_required(VERSION 3.15)

# configure OpenCV
find_package(OpenCV REQUIRED)

# create create individual projects
add_executable(yolo yolo.cpp)
target_link_libraries(yolo ${OpenCV_LIBS})
