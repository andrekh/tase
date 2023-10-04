cmake_minimum_required(VERSION 3.0.0)
project(TASE VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_library(
    json
    lib/json.hpp)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/SQLiteCpp)

add_executable(TASE 
src/main.cpp
src/serverhandler.cpp
src/valvehandler.cpp
src/geodatabase.cpp
src/geolocater.cpp
src/ipapi.cpp
src/geoinfo.cpp
src/mohaahandler.cpp)
target_link_libraries(TASE PRIVATE 
                json
                sqlite3
                SQLiteCpp)

target_include_directories(TASE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set_target_properties(json PROPERTIES LINKER_LANGUAGE CXX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
