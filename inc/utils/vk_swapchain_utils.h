//
// Created by jae on 04/04/23.
//

#ifndef VK_FRAMEWORK_VK_SWAP_CHAIN_UTILS_H
#define VK_FRAMEWORK_VK_SWAP_CHAIN_UTILS_H


/** Find supported image format*/
VkSurfaceFormatKHR
H_findPresentSurfaceImageFormat(VkPhysicalDevice&,
                                VkSurfaceKHR&);

/** Find supported image size extent*/
VkExtent2D
H_findSupportedImageSize(VkSurfaceCapabilitiesKHR&, uint32_t, uint32_t);

/** Find supported image usage */
VkImageUsageFlags
H_findPresentSurfaceImageUsage(VkPhysicalDevice&,
                               VkSurfaceKHR&);

/** Find supported image sharing mode*/
VkSharingMode
H_findPresentSurfaceImageSharingMode(VkPhysicalDevice&,
                                     VkSurfaceKHR&);

/** Find queue family supporting presentation*/
uint32_t
H_findPresentationQueueFamily(VkPhysicalDevice& ,
                              VkSurfaceKHR& ,
                              std::vector<QueueInfo>& );

/** Find supported presentation mode*/
VkPresentModeKHR
H_findPresentationMode(VkPhysicalDevice&,
                       VkSurfaceKHR&);

void H_createSwapChainImageViews(VkDevice&,
                                 VkSurfaceFormatKHR&,
                                 std::vector<VkImage>&,
                                 std::vector<VkImageView>&);

/** Create swap chain image frame buffers */
void H_createSwapChainFrameBuffers(VkDevice&,
                                   ImageInfo&,
                                   VkRenderPass&,
                                   std::vector<VkImageView>&,
                                   std::vector<VkFramebuffer>&);

/** Delete Frame buffers */
void H_destroyFrameBuffers(VkDevice, std::vector<VkFramebuffer>&);

/** Retrieve swap chain images*/
void H_getSwapChainImages(VkDevice&,
                          VkSwapchainKHR&,
                          std::vector<VkImage>&);

/** Create swap chain*/
void
H_createSwapChain( VkPhysicalDevice&,
                   VkDevice&,
                   VkSurfaceKHR&,
                   uint16_t ,
                   uint32_t ,
                   std::vector<QueueInfo>&,
                   VkSwapchainKHR&);

#endif //VK_FRAMEWORK_VK_SWAP_CHAIN_UTILS_H
