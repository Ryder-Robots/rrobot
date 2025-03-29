cmake_minimum_required(VERSION 3.25)
project(fatcnt DESCRIPTION "MVP for Ryder Robots" LANGUAGES CXX)

include(FetchContent)
include(FindPkgConfig)
include(CMakeDependentOption)
include(GNUInstallDirs)

set(CPACK_GENERATOR "DEB")    
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CPACK_PACKAGE_NAME "rrobot")
set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libserial-dev")
set(CPACK_SOURCE_IGNORE_FILES "/gtest/;/googletest/")
set(CPACK_PACKAGE_CONTACT "Aaron Spiteri")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

include(CPack)

set(VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(JSON_VERSION v3.11.3)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/arc)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_TEST ${CMAKE_BINARY_DIR}/test)

add_subdirectory(include)
install(DIRECTORY ${PROJECT_BINARY_DIR}/include/rrobot
        DESTINATION /usr/local/include
)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/${JSON_VERSION}/json.tar.xz)
FetchContent_MakeAvailable(json)
list(APPEND CMAKE_MODULE_PATH ${nlohmann_json})

pkg_check_modules(LIBSERIAL REQUIRED libserial)

include_directories(${json_SOURCE_DIR}/include src ${LIBSERIAL_INCLUDE_DIRS} ${PROJECT_BINARY_DIR}/include)
pkg_search_module(DLIB REQUIRED dlib-1)

find_package(Boost 1.74.0 COMPONENTS program_options REQUIRED )

# Sources common to all drones
set(rrobot_SOURCES
        src/rrobot/main.cpp
)

add_library(rrobot SHARED  ${rrobot_SOURCES})
target_link_libraries(rrobot pthread  ${LIBSERIAL_LIBRARIES} ${EXTRA_LIBS}  ${Boost_LIBRARIES})

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/rrobot.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/rrobot.pc
  @ONLY
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/rrobot.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
