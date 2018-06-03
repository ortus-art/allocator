cmake_minimum_required(VERSION 3.2)


set(PROJECT_LIB ${PROJECT_NAME}_lib)
set(PROJECT_APP_LIB ${PROJECT_NAME}_app_lib)

set(allocator_lib_src
    chunk_allocator.h
    linked_list.h
)

set(allocator_app_lib_src
    app_traits.h
    app_lib.h
)

if( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Coverage")
    list(APPEND allocator_app_lib_src
        mem_debug.h
        mem_debug.cpp
        )
endif()

#Create a static library that is shared between main application and tests
add_library(${PROJECT_LIB} STATIC ${allocator_lib_src})
set_target_properties(${PROJECT_LIB} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_LIB} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} )

add_library(${PROJECT_APP_LIB} STATIC ${allocator_app_lib_src})
set_target_properties(${PROJECT_APP_LIB} PROPERTIES LINKER_LANGUAGE CXX)

if( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Coverage")
    target_compile_definitions(${PROJECT_APP_LIB} PRIVATE APP_DEBUG=1)
endif()



add_executable(${PROJECT_NAME} main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(${PROJECT_NAME} ${PROJECT_LIB} ${PROJECT_APP_LIB})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
