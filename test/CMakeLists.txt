cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(kratostest)

#Bring the headers, into the project
include_directories(${CMAKE_SOURCE_DIR}/../include)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "*.cpp")
 
add_executable(testmain ${SOURCES})

# link the libraries to the executable
target_link_libraries(testmain gtest ${CMAKE_SOURCE_DIR}/../build/libqudot.a)