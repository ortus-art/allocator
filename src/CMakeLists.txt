cmake_minimum_required(VERSION 3.2)

configure_file(version.h.in version.h)

set(PROJECT_LIB ${PROJECT_NAME}_lib)

set(allocator_lib_src
    common.h
    common.cpp
)

#Create a static library that is shared between main application and tests
add_library(${PROJECT_LIB} STATIC ${allocator_lib_src})
target_include_directories(${PROJECT_LIB} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} )

add_executable(${PROJECT_NAME} main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(${PROJECT_NAME} ${PROJECT_LIB})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)