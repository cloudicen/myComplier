cmake_minimum_required(VERSION 3.16)
project(my_compiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++2a -pthread")


include_directories(lib/log4cpp/include utils)

link_directories(lib/log4cpp/lib)

add_executable(my_compiler main.cpp regEngine/RegTree.cpp utils/logger/LoggerFactory.cpp utils/logger/LoggerFactory.h utils/logger/logger.cpp utils/logger/logger.h regEngine/NFA_Graph.cpp regEngine/NFA_Graph.h regEngine/NFA.cpp regEngine/NFA.h)

target_link_libraries(my_compiler log4cpp)