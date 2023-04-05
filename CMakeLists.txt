cmake_minimum_required(VERSION 3.24)
project(vk_framework)

set(CMAKE_CXX_STANDARD 17)

set(LOG_SRC ${CMAKE_SOURCE_DIR}/src/platform/${CMAKE_HOST_SYSTEM_NAME}/Log.cpp inc/utils/vk_swapchain_utils.h inc/utils/vk_command_X_utils.h inc/utils/vk_render_X_utils.h inc/utils/vk_shader_utils.h)

# Include Directories
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/inc)
set(VK_LOADER_DIR ${CMAKE_SOURCE_DIR}/vk_loader/inc)
set(LOG_INC_DIR ${CMAKE_SOURCE_DIR}/inc/Log)
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third-party)

# platform specific source loading
configure_file(/home/jae/CLionProjects/vk-framework/inc/includes.h.in
                /home/jae/CLionProjects/vk-framework/inc/includes.h)

# add shaderc lib
add_library(shaderc STATIC IMPORTED)
set_target_properties(shaderc PROPERTIES IMPORTED_LOCATION
                        ${THIRD_PARTY_DIR}/shaderc/lib/libshaderc.a)

# compile glfw
add_subdirectory(${THIRD_PARTY_DIR}/glfw-3.3.8)

# Log library
add_library(log ${LOG_SRC})
target_include_directories(log PUBLIC ${LOG_INC_DIR})

#vk loader
add_library(vk_loader   ${CMAKE_SOURCE_DIR}/vk_loader/vk_functions.cpp
                        ${CMAKE_SOURCE_DIR}/vk_loader/vk_loader.cpp)

target_include_directories(vk_loader PUBLIC VK_LOADER_DIR)

target_link_libraries(vk_loader PUBLIC log)

# vk framework
add_library(vk_framework ${CMAKE_SOURCE_DIR}/src/vk_framework.cpp
                         ${CMAKE_SOURCE_DIR}/src/utils/vk_instance_and_device_utils.cpp
                         ${CMAKE_SOURCE_DIR}/src/utils/vk_layer_and_extension_utils.cpp
                         ${CMAKE_SOURCE_DIR}/src/platform/${CMAKE_HOST_SYSTEM_NAME}/vk_render_surface.cpp
                         ${CMAKE_SOURCE_DIR}/src/utils/vk_swapchain_utils.cpp
                         ${CMAKE_SOURCE_DIR}/src/utils/vk_command_X_utils.cpp
                         ${CMAKE_SOURCE_DIR}/src/utils/vk_render_X_utils.cpp
                         ${CMAKE_SOURCE_DIR}/src/utils/vk_shader_utils.cpp)

target_include_directories(vk_framework PUBLIC ${INCLUDE_DIR})

target_link_libraries(vk_framework PUBLIC vk_loader glfw shaderc)

# main
add_executable(main main.cpp)

# Link libs
target_link_libraries(main vk_framework)
