cmake_minimum_required(VERSION 3.2)

project(print_ip VERSION 0.0.1)

add_executable(print_ip_cli print_ip.cpp)

install(TARGETS print_ip_cli RUNTIME DESTINATION bin)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")