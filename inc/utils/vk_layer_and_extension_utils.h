//
// Created by jae on 31/03/23.
//

#ifndef VK_FRAMEWORK_VK_LAYER_AND_EXTENSION_UTILS_H
#define VK_FRAMEWORK_VK_LAYER_AND_EXTENSION_UTILS_H

bool H_checkValidationLayerSupport();
bool H_areDeviceExtensionsSupported(VkPhysicalDevice&);
bool H_areInstanceExtensionsSupported();

#endif //VK_FRAMEWORK_VK_LAYER_AND_EXTENSION_UTILS_H
