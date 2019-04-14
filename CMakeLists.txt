cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
project(kratos)

#Bring the headers, into the project
include_directories("include" "/opt/intel/mkl/include")
link_directories(/opt/intel/mkl/lib/intel64)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_library(qudot STATIC ${SOURCES})
add_executable(kratosvm ${SOURCES})

target_link_libraries(kratosvm mkl_intel_ilp64 mkl_sequential mkl_core pthread m dl)
target_link_libraries(qudot mkl_intel_ilp64 mkl_sequential mkl_core pthread m dl)