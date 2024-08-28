# 设置cmake的最低版本
cmake_minimum_required(VERSION 3.13)

# 设置项目名
project(demo_test)

# set(CMAKE_CXX_COMPILER nvcc)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++14)
set(CMAKE_BUILD_TYPE release)

# 添加头文件路径
include_directories(
    # ${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Psrdada/include/
    # ${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Eigen/
    # ${CMAKE_CURRENT_SOURCE_DIR}/../include
    # ${CMAKE_CURRENT_SOURCE_DIR}/
    # /opt/dtk-23.10/cuda/include/
    # /opt/dtk-23.10/cuda/samples/Common/
    /workspace/psrdadaput/include
)

# 添加依赖库路径
set(LIBRARY_PATH 
                # $ENV{ROCM_PATH}/lib/
                # ${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/Psrdada/lib
                # ${CMAKE_CURRENT_SOURCE_DIR}/../lib
                /workspace/psrdadaput/lib
                )
link_directories(
    ${LIBRARY_PATH}
    
)

# 添加源文件
set(SOURCE_FILES 
    /workspace/psrdada_test/src/test_ipcio.c
    #/workspace/psrdada_test/src/writertest.c
    )

#set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/testBuff.cpp)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")

# 添加可执行目标
# add_executable(demo_test ${SOURCE_FILES})
# target_link_libraries(demo_test psrdada)
# add_executable(test_ipcbuf /workspace/psrdada_test/src/test_ipcbuf.c)
# target_link_libraries(test_ipcbuf psrdada)
# add_executable(writertest /workspace/psrdada_test/src/writertest.c)
# target_link_libraries(writertest psrdada)
# add_executable(ipcio_writer /workspace/psrdada_test/src/ipcio_writer.c)
# target_link_libraries(ipcio_writer psrdada)
# add_executable(readertest /workspace/psrdada_test/src/readertest.c)
# target_link_libraries(readertest psrdada)
# add_executable(ipcio_rw /workspace/psrdada_test/src/ipcio_rw.c)
# target_link_libraries(ipcio_rw psrdada)
add_executable(ipcbuf_manage /workspace/psrdada_test/src/ipcbuf_manage.cpp)
target_link_libraries(ipcbuf_manage psrdada)