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

find_package(Boost 1.74.0 REQUIRED )

# Sources common to all drones
set(rrobot_SOURCES
        src/rrobot/main.cpp
        src/rrobot/environment/environmentProcessor.cpp
        src/rrobot/state/rrqueue_manager.cpp
        src/rrobot/state/state_manager.cpp
        src/rrobot/protocols/curators/rrp/msp_ident_curator.cpp
        src/rrobot/protocols/curators/rrp/msp_mode_curator.cpp
        src/rrobot/protocols/curators/multiwii/rmultiwii.cpp
        src/rrobot/protocols/curators/multiwii/rmultiwii_curator.cpp
        src/rrobot/protocols/curators/multiwii/rmmsp_sensor_curator.cpp
        src/rrobot/protocols/curators/multiwii/rmmsp_sonic_curator.cpp
        src/rrobot/protocols/curators/encoder.cpp
        src/rrobot/events/eventrunner.cpp
        src/rrobot/events/serializers/jserializer.cpp
        src/rrobot/com/server.cpp
        src/rrobot/com/rrserialmc.cpp
        src/rrobot/handlers/uihandler.cpp
        src/rrobot/handlers/threadmanager.cpp
        src/rrobot/handlers/statushandler.cpp
        src/rrobot/handlers/identhandler.cpp
        src/rrobot/handlers/aihandler.cpp
        src/rrobot/handlers/currentstatehandler.cpp
        src/rrobot/ai/greedyai.cpp
)

add_library(rrobot SHARED  ${rrobot_SOURCES})
target_link_libraries(rrobot pthread m  ${LIBSERIAL_LIBRARIES} ${EXTRA_LIBS}  ${Boost_LIBRARIES})

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/rrobot.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/rrobot.pc
  @ONLY
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/rrobot.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)
include(Dart)

include_directories(${json_SOURCE_DIR}/include src ${LIBSERIAL_INCLUDE_DIRS} ${PROJECT_BINARY_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/test)

add_executable(test_environmentprocessor test/test_environmentprocessor.cpp)
set_target_properties(test_environmentprocessor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_environmentprocessor GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_environmentprocessor  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/resources")

add_executable(test_rrqueue_manager test/test_rrqueue_manager.cpp)
set_target_properties(test_rrqueue_manager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_rrqueue_manager GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_rrqueue_manager  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/resources")

add_executable(test_state_manager test/test_state_manager.cpp)
set_target_properties(test_state_manager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_state_manager GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_state_manager  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/resources")

add_executable(test_eventrunner test/test_eventrunner.cpp)
set_target_properties(test_eventrunner PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_eventrunner GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_eventrunner  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/resources")

add_executable(test_rrpcurator test/test_rrpcurator.cpp)
set_target_properties(test_rrpcurator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_rrpcurator GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_rrpcurator  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_executable(test_jserializer test/test_jserializer.cpp)
set_target_properties(test_jserializer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_jserializer GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_jserializer  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/resources")

add_executable(test_uihandler test/test_uihandler.cpp)
set_target_properties(test_uihandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_uihandler GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_uihandler  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/resources")

add_executable(test_threadmanager test/test_threadmanager.cpp)
set_target_properties(test_threadmanager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_threadmanager GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_threadmanager  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/resources")

add_executable(test_statushandler test/test_statushandler.cpp)
set_target_properties(test_statushandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_statushandler GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_statushandler  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/resources")

add_executable(test_identhandler test/test_identhandler.cpp)
set_target_properties(test_identhandler PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_identhandler GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_identhandler  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/resources")

add_executable(test_rmultiwii test/test_rmultiwii.cpp)
set_target_properties(test_rmultiwii PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_rmultiwii GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_rmultiwii  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/resources")

add_executable(test_rmmsp_sensor_curator test/test_rmmsp_sensor_curator.cpp)
set_target_properties(test_rmmsp_sensor_curator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_rmmsp_sensor_curator GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_rmmsp_sensor_curator  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/resources")

add_executable(test_encoder test/test_encoder.cpp)
set_target_properties(test_encoder PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_encoder GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_encoder  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/resources")

add_executable(test_greedyai test/test_greedyai.cpp)
set_target_properties(test_greedyai PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(test_greedyai GTest::gtest_main rrobot dlib gmock)
gtest_discover_tests(test_greedyai  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/resources")