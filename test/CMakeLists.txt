cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall" )
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(kratostest)

#Bring the headers, into the project
include_directories(${CMAKE_SOURCE_DIR}/../include $ENV{MKLROOT}/include $ENV{TBBROOT}/include)
link_directories(${CMAKE_SOURCE_DIR}/../build $ENV{MKLROOT}/lib/intel64 $ENV{TBBROOT}/lib/intel64/gcc4.7)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "*.cpp")
 
add_executable(testmain ${SOURCES})

# link the libraries to the executable
target_link_libraries(testmain gtest qudot mkl_intel_ilp64 mkl_tbb_thread mkl_core tbb stdc++ pthread m dl)