################################################################################
cmake_minimum_required(VERSION 3.20)
################################################################################
enable_language(C CXX)
################################################################################
project(fs_test)
################################################################################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
################################################################################
add_library(littlefs STATIC EXCLUDE_FROM_ALL)
target_sources(littlefs PRIVATE ${PROJECT_SOURCE_DIR}/littlefs/lfs.c)
target_sources(littlefs PRIVATE ${PROJECT_SOURCE_DIR}/littlefs/lfs_util.c)
target_include_directories(littlefs PUBLIC ${PROJECT_SOURCE_DIR}/littlefs)
# target_compile_definitions(littlefs PUBLIC LFS_YES_TRACE)
################################################################################
add_library(littlefs_rambd STATIC EXCLUDE_FROM_ALL)
target_sources(littlefs_rambd PRIVATE ${PROJECT_SOURCE_DIR}/littlefs/bd/lfs_rambd.c)
target_include_directories(littlefs_rambd PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(littlefs_rambd PUBLIC littlefs)
# target_compile_definitions(littlefs_rambd PUBLIC LFS_RAMBD_YES_TRACE)
################################################################################
add_library(littlefs_filebd STATIC EXCLUDE_FROM_ALL)
target_sources(littlefs_filebd PRIVATE ${PROJECT_SOURCE_DIR}/littlefs/bd/lfs_filebd.c)
target_include_directories(littlefs_filebd PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(littlefs_filebd PUBLIC littlefs)
################################################################################
add_executable(${PROJECT_NAME}_ram)
target_sources(${PROJECT_NAME}_ram PRIVATE ${PROJECT_SOURCE_DIR}/main_ram.c)
target_link_libraries(${PROJECT_NAME}_ram PRIVATE littlefs)
target_link_libraries(${PROJECT_NAME}_ram PRIVATE littlefs_rambd)
################################################################################
add_executable(${PROJECT_NAME}_file)
target_sources(${PROJECT_NAME}_file PRIVATE ${PROJECT_SOURCE_DIR}/main_file.c)
target_link_libraries(${PROJECT_NAME}_file PRIVATE littlefs)
target_link_libraries(${PROJECT_NAME}_file PRIVATE littlefs_filebd)
################################################################################
