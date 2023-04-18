cmake_minimum_required(VERSION 3.18)
project(vk_framework)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../../../../)

set(INCLUDE_DIR ${SOURCE_DIR}/inc)
set(VK_LOADER_DIR ${SOURCE_DIR}/vk_loader/inc)
set(LOG_INC_DIR ${SOURCE_DIR}/inc/Log)
set(THIRD_PARTY_DIR ${SOURCE_DIR}/third-party)
set(ASSET_FOLDER_DIR ${SOURCE_DIR}/assets)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

# Add Native app glue
add_library(native_app_glue STATIC
        ${THIRD_PARTY_DIR}/native_app_glue/android_native_app_glue.c)

# platform specific source loading
configure_file(${SOURCE_DIR}/inc/config.h.in
        ${SOURCE_DIR}/inc/config.h)

# GLM
add_subdirectory(${THIRD_PARTY_DIR}/glm ${THIRD_PARTY_DIR}/glm)

# spirv reflect
add_library(spirv_reflect ${THIRD_PARTY_DIR}/spirv-reflect/spirv_reflect.cpp)

# add vma
#add_library(vma ${THIRD_PARTY_DIR}/vma/vk_memory_alloc.cpp)

# add stb lib
add_library(stb ${THIRD_PARTY_DIR}/stb/stb_loader.cpp)

# add shaderc lib
add_library(shaderc STATIC IMPORTED)
set_target_properties(shaderc PROPERTIES IMPORTED_LOCATION
        ${THIRD_PARTY_DIR}/shaderc/lib/libshaderc_combined.a)

# compile glfw
#add_subdirectory(${THIRD_PARTY_DIR}/glfw-3.3.8 ${THIRD_PARTY_DIR}/glfw-3.3.8/lib)
                                            
# Log library
add_library(h_log ${SOURCE_DIR}/src/platform/${CMAKE_HOST_SYSTEM_NAME}/Log.cpp)
target_include_directories(h_log PUBLIC ${LOG_INC_DIR})

#vk loader
add_library(vk_loader   ${SOURCE_DIR}/vk_loader/vk_functions.cpp
        ${SOURCE_DIR}/vk_loader/vk_loader.cpp)

target_include_directories(vk_loader PUBLIC VK_LOADER_DIR)

target_link_libraries(vk_loader PUBLIC h_log)

message(${CMAKE_HOST_SYSTEM_NAME})

# vk framework
add_library(vk_framework
        ${SOURCE_DIR}/src/vk_framework.cpp
        ${SOURCE_DIR}/src/utils/vk_instance_and_device_utils.cpp
        ${SOURCE_DIR}/src/utils/vk_layer_and_extension_utils.cpp
        ${SOURCE_DIR}/src/platform/${CMAKE_HOST_SYSTEM_NAME}/vk_render_surface.cpp
        ${SOURCE_DIR}/src/utils/vk_swapchain_utils.cpp
        ${SOURCE_DIR}/src/utils/vk_command_X_utils.cpp
        ${SOURCE_DIR}/src/utils/vk_render_X_utils.cpp
        ${SOURCE_DIR}/src/utils/vk_shader_utils.cpp
        ${SOURCE_DIR}/src/platform/${CMAKE_HOST_SYSTEM_NAME}/asset_utils.cpp
        ${SOURCE_DIR}/src/utils/resources/buffer/vk_buffer_util.cpp
        ${SOURCE_DIR}/src/utils/sync/fences/vk_fence_utils.cpp
        ${SOURCE_DIR}/src/utils/sync/semaphores/vk_semaphore_utils.cpp
        ${SOURCE_DIR}/src/utils/resources/image/vk_image_util.cpp
        ${SOURCE_DIR}/src/utils/resources/image/texture_util.cpp
        ${SOURCE_DIR}/src/utils/resources/image/depth_util.cpp
        ${SOURCE_DIR}/src/utils/spv_reflect_util.cpp
        ${SOURCE_DIR}/src/utils/vk_descriptor_util.cpp
        ${SOURCE_DIR}/src/utils/vk_draw_util.cpp)

target_include_directories(vk_framework PUBLIC ${INCLUDE_DIR}
                        PUBLIC ${ANDROID_NDK}/sources/android/native_app_glue)

target_link_libraries(vk_framework PUBLIC vk_loader shaderc stb spirv_reflect glm)

# main
add_library(vk_main ${CMAKE_SOURCE_DIR}/vk_main.cpp)

# Link libs
target_link_libraries(vk_main vk_framework)
