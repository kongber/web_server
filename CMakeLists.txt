cmake_minimum_required(VERSION 3.25)
project(cyber)
set(CMAKE_CXX_STANDARD 17)

# Server
include_directories(dependence/ares/include)
include_directories(src)
aux_source_directory(src WEB_SERVER_SRC)
link_directories(dependence/ares/lib)
add_executable(cyber main.cpp ${WEB_SERVER_SRC})
target_link_libraries(cyber -lcares)