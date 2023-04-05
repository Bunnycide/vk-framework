//
// Created by jae on 31/03/23.
//
#include "includes.h"

bool H_checkValidationLayerSupport(){
    std::set<std::string> layers(Consts::VALIDATION_LAYERS, Consts::VALIDATION_LAYERS + Consts::NUM_VALIDATION_LAYERS);
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (VkLayerProperties& props : availableLayers){
        layers.erase(props.layerName);
    }

    return layers.empty();
}

bool H_areDeviceExtensionsSupported(VkPhysicalDevice& physicalDevice){
    uint32_t extensionCount = 0;

    VK_CHECK_RESULT(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount,
                                                         nullptr))

    std::vector<VkExtensionProperties> deviceExtensionProperties;
    if(extensionCount > 0){
        deviceExtensionProperties.resize(extensionCount);
        VK_CHECK_RESULT(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount,
                                                             deviceExtensionProperties.data()))
    }

    std::set<std::string> desired_extensions(Consts::DEVICE_EXTENSIONS, Consts::DEVICE_EXTENSIONS + Consts::NUM_DEVICE_EXTENSIONS);

    for(VkExtensionProperties& prop : deviceExtensionProperties){
        desired_extensions.erase(prop.extensionName);
    }

    if(! desired_extensions.empty()){
        for(const auto& ext : desired_extensions){
            Log::error("Could not find %s", ext.c_str());
        }
    }

    return desired_extensions.empty();
}

bool H_areInstanceExtensionsSupported(){
    uint32_t instanceExtensionNumbers = 0;
    VK_CHECK_RESULT(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionNumbers, nullptr))

    if(instanceExtensionNumbers <= 0) return false;
    std::vector<VkExtensionProperties> instanceExtensionProperties;
    instanceExtensionProperties.resize(instanceExtensionNumbers);

    VK_CHECK_RESULT(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionNumbers, instanceExtensionProperties.data()))

    std::set<std::string> desired_extensions(Consts::INSTANCE_EXTENSIONS, Consts::INSTANCE_EXTENSIONS + Consts::NUM_INSTANCE_EXTENSIONS);

    for(VkExtensionProperties& extensionProperty : instanceExtensionProperties){
        desired_extensions.erase(extensionProperty.extensionName);
    }

    if(! desired_extensions.empty()){
        for(const auto& ext : desired_extensions){
            Log::error("Could not find %s", ext.c_str());
        }
    }

    return desired_extensions.empty();
}