
cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
    set(VERSION_BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(VERSION_BUILD 1)
endif()

project(allocator VERSION 1.0.${VERSION_BUILD})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

add_subdirectory(src)

# Unit test are enabled only for Debug or Coverage builds
if( CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Coverage")

    #include(CodeCoverage)
    #setup_target_for_coverage(${PROJECT_NAME}_coverage ${PROJECT_NAME}_test coverage)

    include(CTest)

    enable_testing()

    add_subdirectory(tests)

endif()

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_NAME, ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_DEBIAN_PACKAGE_SECTION "contrib/misc")

set(CPACK_PACKAGE_CONTACT otus.art@gmail.com)
include (CPack)


