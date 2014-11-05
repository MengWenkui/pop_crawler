#CMake最低版本
cmake_minimum_required(VERSION 2.8)
#项目信息
project(crawler)
#查找当前目录下的所有源文件
#并将名称保存到 DIR_SRCS 变量中
aux_source_directory(. DIR_SRCS)
#指定生成目标
add_executable(crawler ${DIR_SRCS})
#添加链接库
target_link_libraries(crawler curl)
