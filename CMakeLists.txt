cmake_minimum_required(VERSION 3.9)
project(qudot)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
list(APPEND CMAKE_CXX_FLAGS "-Wall -O3 -march=native -DBOOST_LOG_DYN_LINK")

find_package(OpenMP REQUIRED) # Find the package

#Bring the headers, into the project
include_directories(include $ENV{MKLROOT}/include $ENV{TBBROOT}/include)
link_directories($ENV{MKLROOT}/lib/intel64 $ENV{TBBROOT}/lib/intel64/gcc4.7)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
#file(GLOB SOURCES "src/*.cpp")
message("C++ compiler flags: ${CMAKE_CXX_FLAGS_RELEASE}")
set(QUDOTVM_SRC 
  ${PROJECT_SOURCE_DIR}/src/common.cpp
  ${PROJECT_SOURCE_DIR}/include/qudot/fenwicktree.hpp
  ${PROJECT_SOURCE_DIR}/src/gates/qugate.cpp
  ${PROJECT_SOURCE_DIR}/src/qumvn.cpp
  ${PROJECT_SOURCE_DIR}/src/quworld.cpp
  ${PROJECT_SOURCE_DIR}/src/bytecodes.cpp
  ${PROJECT_SOURCE_DIR}/src/qudotconfig.cpp
  ${PROJECT_SOURCE_DIR}/src/qureg.cpp
  ${PROJECT_SOURCE_DIR}/src/gatestackframe.cpp
  ${PROJECT_SOURCE_DIR}/src/qudotvm.cpp
  ${PROJECT_SOURCE_DIR}/src/qufrequency.cpp
  ${PROJECT_SOURCE_DIR}/src/gates/X.cpp
  ${PROJECT_SOURCE_DIR}/src/gates/Y.cpp
  ${PROJECT_SOURCE_DIR}/src/gates/H.cpp
  ${PROJECT_SOURCE_DIR}/src/gates/R.cpp
  ${PROJECT_SOURCE_DIR}/src/intrinsics/qft.cpp
  ${PROJECT_SOURCE_DIR}/src/intrinsics/qftinv.cpp
  ${PROJECT_SOURCE_DIR}/src/intrinsics/fulladder.cpp
  ${PROJECT_SOURCE_DIR}/src/intrinsics/comparator.cpp
  ${PROJECT_SOURCE_DIR}/src/components/feynmanunit.cpp 
  ${PROJECT_SOURCE_DIR}/src/components/heisenbergunit.cpp
  ${PROJECT_SOURCE_DIR}/src/components/mwlu.cpp
  ${PROJECT_SOURCE_DIR}/src/components/eprunit.cpp
  ${PROJECT_SOURCE_DIR}/src/scalemem.cpp
)

add_library(qudot SHARED ${QUDOTVM_SRC})
add_executable(qudotvm ${PROJECT_SOURCE_DIR}/src/qudotapp.cpp ${QUDOTVM_SRC})

target_link_libraries(qudotvm mkl_intel_ilp64 mkl_tbb_thread mkl_core tbb tbbmalloc stdc++ pthread boost_log m dl ${OpenMP_CXX_LIBRARIES})
# -Wl,--start-group $(MKLROOT)/lib/intel64/libmkl_intel_ilp64.a $(MKLROOT)/lib/intel64/libmkl_sequential.a $(MKLROOT)/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm -ldl
target_link_libraries(qudot mkl_intel_ilp64 mkl_tbb_thread mkl_core tbb tbbmalloc stdc++ pthread boost_log m dl ${OpenMP_CXX_LIBRARIES})