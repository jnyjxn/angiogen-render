/*

Copyright (c) 2020, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


#ifndef __VulkanWindow_h
#define __VulkanWindow_h


/**
********************************************************************************
*
*   @file       VulkanWindow.h
*
*   @brief      VulkanWindow is a class to handle windows. It uses
*               GLFW to support Vulkan.
*
*   @version    1.0
*
*   @date       03/03/2020
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Mar 2020, 2020, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cstdint>

/*#ifdef HAS_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif
*/
#include "Window.h"

#ifdef WIN32
typedef __int32 int32_t;
typedef unsigned __int32 u_int32_t; 
#endif


namespace SimpleGVXR {
class VulkanWindow: public Window
{
public:


    //--------------------------------------------------------------------------
    /// Create a window
    /**
    *  @param aVisibilityFlag: flag controling if the window should be visible (1)
    *                          or hidden (0). (default value: 0)
     */
    //--------------------------------------------------------------------------
    VulkanWindow(int aVisibilityFlag = 0);


    //------------------------------------------------------------------------------
    /// Create an OpenGL context and display it in a window
    /**
     *  @param aWidth:      the number of pixels along the X-axis
     *  @param aHeight:     the number of pixels along the Y-axis
     */
    //------------------------------------------------------------------------------
    virtual void setWindowSize(int aWidth, int aHeight);


    virtual void displayScene(bool aSceneRotationFlag);
    
    virtual void takeScreenshot();


protected:
    // Initialise GLEW
    virtual void createWindow(bool aVisibilityFlag);

    // Initialise OpenGL
    virtual void initShader();


    virtual void render(bool aSceneRotationFlag);


    virtual void registerCallbacks();

    static std::vector<const char *> enableValidationLayers();
    static void createInstance(const std::vector<const char *>& anEnabledExtensionSet);
    static uint32_t findPhysicalDevice();
    static void printDeviceProperties();
    static void createDevice();
    //void createBuffer();
    //void createDescriptorSetLayout();
    //void createDescriptorSet();
    //void createComputePipeline();
    //void createCommandBuffer();

    // Finally, run the recorded command buffer.
    //void runCommandBuffer();

    // The former command rendered a mandelbrot set to a buffer.
    // Save that buffer as a png on disk.
    //void saveRenderedImage();

    // Clean up all vulkan resources.
    //void cleanup();
    static bool supportsQueue(u_int32_t aDeviceID, u_int32_t aQueue);
    static uint32_t getComputeQueueFamilyIndex(u_int32_t aQueue);


    /*static void framebufferSizeCallback(GLFWwindow* apWindow, int width, int height);

    static void initVulkan();
    static void createInstance();
    static void checkDevices();
    static void printDeviceProperties();
    static void initDevice();
    static void initCommandBuffer();

    static bool m_is_instance_ready;
    static bool m_is_device_ready;*/
    static uint32_t m_number_of_devices;
    //static uint32_t m_queue_family_property_count;
/*
#ifdef HAS_VULKAN
    static VkInstance m_instance;

#endif*/
};
};


#endif
