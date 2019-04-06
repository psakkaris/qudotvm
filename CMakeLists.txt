cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
project(kratos)

#Bring the headers, into the project
include_directories(include)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

ADD_LIBRARY(qudot STATIC ${SOURCES})
 
add_executable(kratosvm ${SOURCES})
