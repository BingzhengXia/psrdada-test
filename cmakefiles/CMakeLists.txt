# 设置cmake的最低版本
cmake_minimum_required(VERSION 3.13)

# 设置项目名
project(demo_test)

add_definitions(-std=c++11)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_CXX_STANDARD 11)

set(CUDA_NVCC_PLAGS ${CUDA_NVCC_PLAGS};-std=c++11;-g;-G;-gencode)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -Wfatal-errors -D_MWAITXINTRIN_H_INCLUDED -g")

find_package(CUDA REQUIRED)
set(CMAKE_BUILD_TYPE debug)

# 添加头文件路径
include_directories(
    # ${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Eigen/
    # ${CMAKE_CURRENT_SOURCE_DIR}/../include
    # ${CMAKE_CURRENT_SOURCE_DIR}/
    /workspace/psrdadaput/include

)

# 添加依赖库路径
link_directories(
    /usr/local/lib/
    # /usr/local/cuda-11.8/targets/x86_64-linux/lib
    # ${CMAKE_CURRENT_SOURCE_DIR}/../lib

    /workspace/psrdadaput/lib
    /usr/local/cuda-11.8/targets/x86_64-linux/lib
    
)

# 添加源文件
#set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/testBuff.cpp)

# 添加可执行目标
#add_executable(demo_test ${SOURCE_FILES})
#CUDA_ADD_EXECUTABLE(demo_test ./testBuff.cu)
CUDA_ADD_EXECUTABLE(ipcbuf_rwg  /workspace/psrdada_test/src/ipcbuf_rw.cu)
# target_link_libraries(ipcbuf_rwg cudart cublas cufft cusolver matio)
# target_link_libraries(demo_test ZJLab_AlgRFIS)
target_link_libraries(ipcbuf_rwg psrdada cuda)
