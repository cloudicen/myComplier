cmake_minimum_required(VERSION 3.17)
project(my_compiler)

set(CMAKE_CXX_STANDARD 20)
add_definitions(-DUSE_LOG4CPP)

include_directories("${PROJECT_SOURCE_DIR}" lib/LOG4CPP/include utils)

link_directories(lib/LOG4CPP/lib)

add_executable(my_compiler main.cpp regEngine/RegTree.cpp utils/logger/LoggerFactory.cpp utils/logger/LoggerFactory.h utils/logger/logger.cpp utils/logger/logger.h)

target_link_libraries(my_compiler log4cppD.lib)