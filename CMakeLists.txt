cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED ENV{TRAVIS_BUILD_NUMBER})
    message("TRAVIS_BUILD_NUMBER environment variable set to default")
    set(ENV{TRAVIS_BUILD_NUMBER} "1")
endif()

project(join_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

# libboost-system-dev libboost-doc
find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads)

configure_file(version.h.in version.h)

add_library(parser_lib parser.h parser.cpp)
add_library(table_lib table.h table.cpp)

add_executable(join_server main.cpp)

set_target_properties(join_server parser_lib table_lib
        PROPERTIES
                CXX_STANDARD 14
                CXX_STANDARD_REQUIRED ON
                CMAKE_CXX_EXTENSIONS OFF
                #COMPILE_DEFINITIONS BOOST_ASIO_ENABLE_HANDLER_TRACKING
                COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(join_server parser_lib table_lib
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS join_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT edkolotilov@gmail.com)

include (CPack)
