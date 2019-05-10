cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall" )
project(kratos)

#Bring the headers, into the project
include_directories(include $ENV{MKLROOT}/include $ENV{TBBROOT}/include)
link_directories($ENV{MKLROOT}/lib/intel64 $ENV{TBBROOT}/lib/intel64/gcc4.7)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
#file(GLOB SOURCES "src/*.cpp")
set(KRATOS_SRC 
  ${PROJECT_SOURCE_DIR}/src/common.cpp
  ${PROJECT_SOURCE_DIR}/src/quworld.cpp
  ${PROJECT_SOURCE_DIR}/src/kratosapp.cpp
  ${PROJECT_SOURCE_DIR}/src/bytecodes.cpp
  ${PROJECT_SOURCE_DIR}/src/qudotconfig.cpp
  ${PROJECT_SOURCE_DIR}/src/qureg.cpp
  ${PROJECT_SOURCE_DIR}/src/gatestackframe.cpp
  ${PROJECT_SOURCE_DIR}/src/kratosvm.cpp
  ${PROJECT_SOURCE_DIR}/src/qufrequency.cpp
  ${PROJECT_SOURCE_DIR}/src/gates/X.cpp
  ${PROJECT_SOURCE_DIR}/src/gates/Y.cpp
  ${PROJECT_SOURCE_DIR}/src/gates/H.cpp
  ${PROJECT_SOURCE_DIR}/src/gates/R.cpp
  ${PROJECT_SOURCE_DIR}/src/components/feynmanunit.cpp 
  ${PROJECT_SOURCE_DIR}/src/components/heisenbergunit.cpp
)

# removes the kratosapp.cpp file which has a main function
set(QUDOT_LIB_SRC 
  ${PROJECT_SOURCE_DIR}/src/common.cpp
  ${PROJECT_SOURCE_DIR}/src/quworld.cpp
  ${PROJECT_SOURCE_DIR}/src/bytecodes.cpp
  ${PROJECT_SOURCE_DIR}/src/qudotconfig.cpp
  ${PROJECT_SOURCE_DIR}/src/qureg.cpp
  ${PROJECT_SOURCE_DIR}/src/gatestackframe.cpp
  ${PROJECT_SOURCE_DIR}/src/kratosvm.cpp
  ${PROJECT_SOURCE_DIR}/src/qufrequency.cpp
  ${PROJECT_SOURCE_DIR}/src/gates/X.cpp
  ${PROJECT_SOURCE_DIR}/src/gates/Y.cpp
  ${PROJECT_SOURCE_DIR}/src/gates/H.cpp
  ${PROJECT_SOURCE_DIR}/src/gates/R.cpp
  ${PROJECT_SOURCE_DIR}/src/components/feynmanunit.cpp 
  ${PROJECT_SOURCE_DIR}/src/components/heisenbergunit.cpp
)


add_library(qudot SHARED ${QUDOT_LIB_SRC})
add_executable(kratosapp ${KRATOS_SRC})

target_link_libraries(kratosapp mkl_intel_ilp64 mkl_sequential mkl_core tbb pthread m dl)
# -Wl,--start-group $(MKLROOT)/lib/intel64/libmkl_intel_ilp64.a $(MKLROOT)/lib/intel64/libmkl_sequential.a $(MKLROOT)/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm -ldl
target_link_libraries(qudot mkl_intel_ilp64 mkl_tbb_thread mkl_core tbb stdc++ pthread m dl)