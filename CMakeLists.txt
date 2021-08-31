cmake_minimum_required(VERSION 3.17)
project(my_compiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++20 -pthread")
add_definitions(-DUSE_LOG4CPP)

include_directories("${PROJECT_SOURCE_DIR}" lib/log4cpp/include utils)

link_directories(lib/log4cpp/lib)

add_executable(my_compiler main.cpp regEngine/RegTree.cpp utils/logger/LoggerFactory.cpp utils/logger/LoggerFactory.h utils/logger/logger.cpp utils/logger/logger.h)

target_link_libraries(my_compiler log4cpp)