cmake_minimum_required(VERSION 3.24)
project(vk_framework)

set(CMAKE_CXX_STANDARD 17)

# Include Directories
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/inc)
set(VK_LOADER_DIR ${CMAKE_SOURCE_DIR}/vk_loader/inc)
set(LOG_INC_DIR ${CMAKE_SOURCE_DIR}/inc/Log)
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third-party)
set(ASSET_FOLDER_DIR ${CMAKE_SOURCE_DIR}/assets)

# platform specific source loading
configure_file(${CMAKE_SOURCE_DIR}/inc/config.h.in
        ${CMAKE_SOURCE_DIR}/inc/config.h)

# add vma
#add_library(vma ${THIRD_PARTY_DIR}/vma/vk_memory_alloc.cpp)

# add stb lib
add_library(stb ${THIRD_PARTY_DIR}/stb/stb_loader.cpp)

# add shaderc lib
add_library(shaderc STATIC IMPORTED)
set_target_properties(shaderc PROPERTIES IMPORTED_LOCATION
        ${THIRD_PARTY_DIR}/shaderc/lib/libshaderc_combined.a)

# compile glfw
add_subdirectory(${THIRD_PARTY_DIR}/glfw-3.3.8)

# Log library
add_library(log ${CMAKE_SOURCE_DIR}/src/platform/${CMAKE_HOST_SYSTEM_NAME}/Log.cpp)
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
                         ${CMAKE_SOURCE_DIR}/src/utils/vk_shader_utils.cpp
                         ${CMAKE_SOURCE_DIR}/src/platform/${CMAKE_HOST_SYSTEM_NAME}/asset_utils.cpp
                         ${CMAKE_SOURCE_DIR}/src/utils/resources/buffer/vk_buffer_util.cpp
                         ${CMAKE_SOURCE_DIR}/src/utils/sync/fences/vk_fence_utils.cpp
                         ${CMAKE_SOURCE_DIR}/src/utils/sync/semaphores/vk_semaphore_utils.cpp)

target_include_directories(vk_framework PUBLIC ${INCLUDE_DIR})

target_link_libraries(vk_framework PUBLIC vk_loader glfw shaderc stb)

# main
add_executable(main main.cpp)

# Link libs
target_link_libraries(main vk_framework)
