find_package( Boost 1.59 )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

cmake_minimum_required(VERSION 2.8.11)

project(stdcell_city)

add_executable(stdcell_city main.cpp)

target_link_libraries(stdcell_city timing-driven_placement)
