cmake_minimum_required(VERSION 2.8)

project(serial_tbb)

set(TBB_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/tbb EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME} "src/main.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE ${PYTHON_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/tbb/include)
target_link_libraries(${PROJECT_NAME} tbb)

