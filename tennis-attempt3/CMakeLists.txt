cmake_minimum_required(VERSION 3.13)
project(
    catch2-template LANGUAGES CXX)
find_package(
    Catch2 REQUIRED)
add_executable(
    ${PROJECT_NAME} main.cpp test.cpp)
target_compile_features(
    ${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE Catch2::Catch2)
