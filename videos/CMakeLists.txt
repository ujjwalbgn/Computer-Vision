project (video)
cmake_minimum_required(VERSION 3.15)

# configure OpenCV
find_package(OpenCV REQUIRED)

# create create individual projects
add_executable(tracking tracking.cpp)
target_link_libraries(tracking ${OpenCV_LIBS})

add_executable(background_subtraction background_subtraction.cpp)
target_link_libraries(background_subtraction ${OpenCV_LIBS})

add_executable(program3 assignment3.cpp)
target_link_libraries(program3 ${OpenCV_LIBS})

