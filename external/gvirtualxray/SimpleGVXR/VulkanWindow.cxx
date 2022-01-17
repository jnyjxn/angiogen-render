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


/**
********************************************************************************
*
*   @file       VulkanWindow.cxx
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
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>

#ifdef HAS_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif
// To perform the computations of the X-ray simulation
/*#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif
*/
// To initialise GLEW, to define matrix stacks, to define state stacks, etc.
//#ifndef __OpenGLUtilities_h
//#include "gVirtualXRay/OpenGLUtilities.h"
//#endif
/*
#ifndef __Utilities_h
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef __Shader_h
#include "gVirtualXRay/Shader.h"
#endif

// To load a scenegraph
#ifndef __SceneGraphBinder_h
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

// To load a scenegraph using ASSIMP
#ifndef __AssimpSceneGraphBinder_h
#include "gVirtualXRay/AssimpSceneGraphBinder.h"
#endif

// To load a scenegraph using OpenSceneGraph
#ifndef __OpenSceneGraphBinder_h
#include "gVirtualXRay/OpenSceneGraphBinder.h"
#endif

// To perform the computations of the X-ray simulation
#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif
*/

#ifndef __VulkanWindow_h
#include "VulkanWindow.h"
#endif


// Shaders for rendering

//#include "display_gl4.frag.h"
//#include "display_gl4.vert.h"


//******************************************************************************
//  Namespaces
//******************************************************************************
using namespace std;


//******************************************************************************
//  Macros
//******************************************************************************


// Used for validating return values of Vulkan API calls.
#ifdef HAS_VULKAN
#define VK_CHECK_RESULT(f) 												\
{																		\
    VkResult result = (f);												\
    if (result != VK_SUCCESS)											\
    {                                                                   \
        std::stringstream error_message;                                \
        error_message << "Vulkan error: VkResult is " << result <<".";  \
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, \
            error_message.str());                                       \
    }																	\
}
#endif


//******************************************************************************
//  Function declarations
//******************************************************************************
void framebufferSizeCallback(GLFWwindow* apWindow, int aWidth, int aHeight);


//******************************************************************************
//  Static class members
//******************************************************************************
/*bool SimpleGVXR::VulkanWindow::m_is_instance_ready = false;
bool SimpleGVXR::VulkanWindow::m_is_device_ready = false;*/
uint32_t SimpleGVXR::VulkanWindow::m_number_of_devices = 0;
//uint32_t SimpleGVXR::VulkanWindow::m_queue_family_property_count = 0;

/*
#ifdef HAS_VULKAN
VkInstance SimpleGVXR::VulkanWindow::m_instance;
#endif
*/

//******************************************************************************
//  Global variables
//******************************************************************************
extern double g_initial_fovy;
extern double g_initial_near;
extern double g_initial_far;
//extern GLfloat g_zoom;

extern bool g_run_loop;
extern bool g_display_beam;
extern bool g_display_wireframe;
extern bool g_use_lighing;
extern bool g_display_detector;
extern bool g_display_normal_vectors;

//extern gVirtualXRay::XRayBeam g_xray_beam;         // X-ray beam
//extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
//extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine


//extern gVirtualXRay::Shader g_display_shader;
//extern bool g_has_own_gl_context;

//extern std::shared_ptr<gVirtualXRay::SceneGraphBinder> g_p_scene_graph;

//extern gVirtualXRay::Matrix4x4<GLfloat> g_scene_rotation_matrix;

// Rotate the scanned objects
//extern gVirtualXRay::Matrix4x4<GLfloat> g_sample_rotation_matrix;

// Bias/gain of the shift/scale filter for the display of the X-ray image
extern double g_shift_filter;
extern double g_scale_filter;

#ifdef NDEBUG
const bool g_enable_validation_layers = false;
#else
const bool g_enable_validation_layers = true;
#endif


uint32_t g_physical_device_index = 0;

#ifdef HAS_VULKAN
/*
In order to use Vulkan, you must create an instance.
*/
VkInstance instance;

VkDebugReportCallbackEXT debugReportCallback;
/*
The physical device is some device on the system that supports usage of Vulkan.
Often, it is simply a graphics card that supports Vulkan.
*/
std::vector<VkPhysicalDevice> g_device_set;
/*
Then we have the logical device VkDevice, which basically allows
us to interact with the physical device.
*/
VkDevice device;


/*
The pipeline specifies the pipeline that all graphics and compute commands pass though in Vulkan.

We will be creating a simple compute pipeline in this application.
*/
VkPipeline pipeline;
VkPipelineLayout pipelineLayout;
VkShaderModule computeShaderModule;

/*
The command buffer is used to record commands, that will be submitted to a queue.

To allocate such command buffers, we use a command pool.
*/
VkCommandPool commandPool;
VkCommandBuffer commandBuffer;

/*

Descriptors represent resources in shaders. They allow us to use things like
uniform buffers, storage buffers and images in GLSL.

A single descriptor represents a single resource, and several descriptors are organized
into descriptor sets, which are basically just collections of descriptors.
*/
VkDescriptorPool descriptorPool;
VkDescriptorSet descriptorSet;
VkDescriptorSetLayout descriptorSetLayout;

/*
The mandelbrot set will be rendered to this buffer.

The memory that backs the buffer is bufferMemory.
*/
VkBuffer buffer;
VkDeviceMemory bufferMemory;

uint32_t bufferSize; // size of `buffer` in bytes.

std::vector<const char *> enabledLayers;

/*
In order to execute commands on a device(GPU), the commands must be submitted
to a queue. The commands are stored in a command buffer, and this command buffer
is given to the queue.

There will be different kinds of queues on the device. Not all queues support
graphics operations, for instance. For this application, we at least want a queue
that supports compute operations.
*/
VkQueue queue; // a queue supporting compute operations.

/*
Groups of queues that have the same capabilities(for instance, they all supports graphics and computer operations),
are grouped into queue families.

When submitting a command buffer, you must specify to which queue in the family you are submitting to.
This variable keeps track of the index of that queue in its family.
*/
uint32_t queueFamilyIndex;
#endif


//******************************************************************************
//  Function definitions
//******************************************************************************


//----------------------------------------------
SimpleGVXR::VulkanWindow::VulkanWindow(int aVisibilityFlag):
//----------------------------------------------
    Window(aVisibilityFlag)
//----------------------------------------------
{
    // Initialize GLFW
    if (!glfwInit())
        {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "ERROR: cannot initialise GLFW.");
        }

    // Create the window
    createWindow(aVisibilityFlag);

    // Window has not been created
    if (!m_context.isReady())
    {
        // Display a warning
        cerr << "gvxrWarning:\tVulkanWindow " <<
            " cannot be created." <<
            endl;
    }
}


//-------------------------------------------------------
void SimpleGVXR::VulkanWindow::setWindowSize(int aWidth, int aHeight)
//-------------------------------------------------------
{
    Window::setWindowSize(aWidth, aHeight);

    if (m_context.getWindowHandle())
    	framebufferSizeCallback(m_context.getWindowHandle(), aWidth, aHeight);
}


//------------------------------------------------------
void SimpleGVXR::VulkanWindow::displayScene(bool aSceneRotationFlag)
//------------------------------------------------------
{
	// Initialise the viewer if needed
/*	initViewer();

    // Show the window if needed
    if (!g_has_own_gl_context)
        {
        if (!glfwGetWindowAttrib(m_p_window,
                GLFW_VISIBLE))
            {
            glfwShowWindow(m_p_window);
            }

        // Make the window's context current
        glfwMakeContextCurrent(m_p_window);

        // Clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        initViewer();
        }

    // Display the 3D scene
    render(aSceneRotationFlag);

    // Make sure all the Vulkan code is done
    glFinish();

    // Swap front and back buffers
    if (!g_has_own_gl_context)
        {
        glfwSwapBuffers(m_p_window);
        }*/
}


//---------------------------------
void SimpleGVXR::VulkanWindow::takeScreenshot()
//---------------------------------
{
/*    // Update the window size, just in case
    glfwGetWindowSize(m_context.getWindowHandle(), &m_width, &m_height);

    GLfloat* p_pixel_set = new GLfloat[3 * m_width * m_height];
    glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_FLOAT, p_pixel_set);


    lastest_screenshot.clear();

    for (unsigned int j = 0; j < m_height; ++j)
    {
        lastest_screenshot.push_back(std::vector<std::vector< float> >() );

        for (unsigned int i = 0; i < m_width; ++i)
        {
            lastest_screenshot.back().push_back(std::vector< float>());

            lastest_screenshot.back().back().push_back(p_pixel_set[(m_height - 1 - j) * m_width * 3 + i * 3 + 0]);
            lastest_screenshot.back().back().push_back(p_pixel_set[(m_height - 1 - j) * m_width * 3 + i * 3 + 1]);
            lastest_screenshot.back().back().push_back(p_pixel_set[(m_height - 1 - j) * m_width * 3 + i * 3 + 2]);
        }
    }

    delete [] p_pixel_set;*/
}


//---------------------------------------------------
void SimpleGVXR::VulkanWindow::createWindow(bool aVisibilityFlag)
//---------------------------------------------------
{
	/*
    string window_title = "gVirtualXRay -- Window ";

    // Set the version of Vulkan that is required
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    if (aVisibilityFlag)
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    }
    else
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }

    // Create a windowed mode window and its Vulkan context
    m_p_window = glfwCreateWindow(m_width, m_height, window_title.data(), 0, 0);

    // Window has be created
    if (m_p_window)
    {
        glfwSetWindowUserPointer(m_p_window, this);
        framebufferSizeCallback(m_p_window, m_width, m_height);

        // Initialize GLFW callbacks
        registerCallbacks();

        // Initialize Vulkan
        createInstance(enableValidationLayers());
        g_physical_device_index = findPhysicalDevice();
        printDeviceProperties();
        createDevice();
        //createBuffer();
        //createDescriptorSetLayout();
        //createDescriptorSet();
        //createComputePipeline();
        //createCommandBuffer();

        // Finally, run the recorded command buffer.
        //runCommandBuffer();

        // The former command rendered a mandelbrot set to a buffer.
        // Save that buffer as a png on disk.
        //saveRenderedImage();

        // Clean up all vulkan resources.
        //cleanup();


        // Initialise Vulkan
        //initVulkan();
    	//createInstance();
    	//checkDevices();
    	//printDeviceProperties();
    	//initDevice();
    	//std::cout << "VULKN DONE" << std::endl;
    	exit(0);
    }*/
}


//-----------------------------
void SimpleGVXR::VulkanWindow::initShader()
//-----------------------------
{
 /*   std::cout << "OpenGL renderer:   " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version:    " << glGetString(GL_VERSION) << std::endl;
    std::cout << "OpenGL vender:     " << glGetString(GL_VENDOR) << std::endl;

    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable the Z-buffer
    glEnable (GL_DEPTH_TEST);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the background colour
    glClearColor(0.5, 0.5, 0.5, 1.0);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glEnable (GL_MULTISAMPLE);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Initialise the shaders
    char* p_vertex_shader(0);
    char* p_fragment_shader(0);

    int z_lib_return_code_vertex(0);
    int z_lib_return_code_fragment(0);

    std::string vertex_shader;
    std::string fragment_shader;

    // Display shader
    // OpenGL 4.5
    if (gVirtualXRay::useOpenGL45())
    {
        z_lib_return_code_vertex = gVirtualXRay::inflate(
            g_display_gl4_vert,
            sizeof(g_display_gl4_vert),
            &p_vertex_shader);

        z_lib_return_code_fragment = gVirtualXRay::inflate(
            g_display_gl4_frag,
            sizeof(g_display_gl4_frag),
            &p_fragment_shader);

        g_display_shader.setLabels("display_gl4.vert", "display_gl4.frag");
    }
    // OpenGL 3.2
    else if (gVirtualXRay::useOpenGL45())
    {
        z_lib_return_code_vertex = gVirtualXRay::inflate(
            g_display_gl3_vert,
            sizeof(g_display_gl3_vert),
            &p_vertex_shader);

        z_lib_return_code_fragment = gVirtualXRay::inflate(
            g_display_gl3_frag,
            sizeof(g_display_gl3_frag),
            &p_fragment_shader);

        g_display_shader.setLabels("display_gl3.vert", "display_gl3.frag");
    }
    else
    {
        z_lib_return_code_vertex = gVirtualXRay::inflate(
            g_display_gl2_vert,
            sizeof(g_display_gl2_vert),
            &p_vertex_shader);

        z_lib_return_code_fragment = gVirtualXRay::inflate(
            g_display_gl2_frag,
            sizeof(g_display_gl2_frag),
            &p_fragment_shader);

        g_display_shader.setLabels("display_gl2.vert", "display_gl2.frag");
    }

    vertex_shader = p_vertex_shader;
    fragment_shader = p_fragment_shader;

    delete[] p_vertex_shader;
    delete[] p_fragment_shader;

    p_vertex_shader = 0;
    p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 ||
        z_lib_return_code_fragment <= 0 ||
        !vertex_shader.size() ||
        !fragment_shader.size())
    {
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot decode the shader using ZLib.");
    }

    g_display_shader.loadSource(vertex_shader, fragment_shader);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    initViewer();
   */
}


//------------------------------------------------
void SimpleGVXR::VulkanWindow::render(bool aSceneRotationFlag)
//------------------------------------------------
{
/*
    try
    {
        // Enable back face culling
        gVirtualXRay::pushEnableDisableState (GL_CULL_FACE);
        glEnable(GL_CULL_FACE);
        glCullFace (GL_BACK);

        // Enable the shader
        gVirtualXRay::pushShaderProgram();
        g_display_shader.enable();
        GLint shader_id(g_display_shader.getProgramHandle());

        // Check the status of OpenGL and of the current FBO
        gVirtualXRay::checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Store the current transformation matrix
        gVirtualXRay::pushModelViewMatrix();

        // Rotate the sample
        if (aSceneRotationFlag)
        {
            gVirtualXRay::g_current_modelview_matrix *= g_scene_rotation_matrix;
        }

        GLuint handle(0);
        handle = glGetUniformLocation(shader_id, "g_projection_matrix");
        glUniformMatrix4fv(handle, 1, GL_FALSE,
        gVirtualXRay::g_current_projection_matrix.get());
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "g_modelview_matrix");
        glUniformMatrix4fv(handle, 1, GL_FALSE,
        gVirtualXRay::g_current_modelview_matrix.get());
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        gVirtualXRay::MATRIX4 normal_matrix(
        gVirtualXRay::g_current_modelview_matrix);
        handle = glGetUniformLocation(shader_id, "g_normal_matrix");
        glUniformMatrix3fv(handle, 1, GL_FALSE, normal_matrix.get3x3());
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        GLint lighting;
        if (g_use_lighing)
        {
            lighting = 1;
        }
        else
        {
            lighting = 0;
        }
        handle = glGetUniformLocation(shader_id, "g_use_lighting");
        glUniform1iv(handle, 1, &lighting);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Store the current transformation matrix
        gVirtualXRay::pushModelViewMatrix();

        gVirtualXRay::g_current_modelview_matrix *= g_sample_rotation_matrix;

        // Look for all the meshes in the scenegraph
        std::vector<gVirtualXRay::SceneGraphNode*> p_temp_pull;
        p_temp_pull.push_back(g_p_scene_graph->getNode("root"));

        while (p_temp_pull.size())
        {
            // Get the last node
            gVirtualXRay::SceneGraphNode* p_node = p_temp_pull.back();

            // Remove the node
            p_temp_pull.pop_back();

            // Add its children
            for (unsigned int i = 0; i < p_node->getNumberOfChildren(); ++i)
            {
                p_temp_pull.push_back(&p_node->getChild(i));
            }

            // Add its geometry
            gVirtualXRay::PolygonMesh& mesh = p_node->getPolygonMesh();
            if (mesh.getVertexNumber())
            {
                // Store the current transformation matrix
                gVirtualXRay::pushModelViewMatrix();

                gVirtualXRay::g_current_modelview_matrix *=
                p_node->getWorldTransformationMatrix();

                gVirtualXRay::applyModelViewMatrix();

                // Set the colour of the sample
                handle = glGetUniformLocation(shader_id, "material_ambient");
                glUniform4fv(handle, 1, mesh.getMaterial().getAmbientColour());
                gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                     __FUNCTION__,
                                                     __LINE__);

                handle = glGetUniformLocation(shader_id, "material_diffuse");
                glUniform4fv(handle, 1, mesh.getMaterial().getDiffuseColour());
                gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                     __FUNCTION__,
                                                     __LINE__);

                handle = glGetUniformLocation(shader_id, "material_specular");
                glUniform4fv(handle, 1, mesh.getMaterial().getSpecularColour());
                gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                     __FUNCTION__,
                                                     __LINE__);

                handle = glGetUniformLocation(shader_id, "material_shininess");
                GLfloat shininess = mesh.getMaterial().getShininess();
                glUniform1fv(handle, 1, &shininess);
                gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                     __FUNCTION__,
                                                     __LINE__);

                // Display the sample
                if (g_display_wireframe)
                {
                    mesh.displayWireFrame();
                }
                else
                {
                    mesh.display();
                }

                if (g_display_normal_vectors)
                {
                    int line_width;
                    glGetIntegerv(GL_LINE_WIDTH, &line_width);
                    glLineWidth(3);

                    if (mesh.getVertexNormalNumber())
                    {
                        mesh.displayVertexNormal();
                    }

                    if (mesh.getFaceNormalNumber())
                    {
                        mesh.displayFaceNormal();
                    }
                    glLineWidth(line_width);
                }

                // Restore the current transformation matrix
                gVirtualXRay::popModelViewMatrix();
            }
        }

        // Restore the current transformation matrix
        gVirtualXRay::popModelViewMatrix();

        // Set the colour of the source
        {
            const GLfloat material_ambient[] = {0.0, 0.39225, 0.39225, 1.0};
            const GLfloat material_diffuse[] = {0.0, 0.70754, 0.70754, 1.0};
            const GLfloat material_specular[] = {0.0, 0.708273, 0.708273, 1.0};
            const GLfloat material_shininess = 50.2;

            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_diffuse");
            glUniform4fv(handle, 1, &material_diffuse[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_specular");
            glUniform4fv(handle, 1, &material_specular[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_shininess");
            glUniform1fv(handle, 1, &material_shininess);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);
        }

        {
            // Define the light colours here
            GLfloat light_global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
            GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
            GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
            GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

            handle = glGetUniformLocation(shader_id, "light_global_ambient");
            glUniform4fv(handle, 1, &light_global_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "light_ambient");
            glUniform4fv(handle, 1, &light_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "light_diffuse");
            glUniform4fv(handle, 1, &light_diffuse[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "light_specular");
            glUniform4fv(handle, 1, &light_specular[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            gVirtualXRay::VEC3 light_position(0, 0, g_zoom);
            handle = glGetUniformLocation(shader_id, "light_position");
            glUniform4f(handle, light_position.getX(), light_position.getY(),
            light_position.getZ(), 1.0);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);
        }

        // Display the source
        g_xray_detector.displaySource();

        // Disable back face culling
        glDisable(GL_CULL_FACE);

        // Display the X-Ray image
        if (g_display_detector)
        {
            g_xray_renderer.display(true, true, false, g_shift_filter,
            g_scale_filter);
        }

        // Display the beam
        if (g_display_beam)
        {
            const GLfloat material_ambient[] = { 0.75, 0, 0.5, 0.3 };
            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__,
            __LINE__);

            lighting = 0;
            handle = glGetUniformLocation(shader_id, "g_use_lighting");
            glUniform1iv(handle, 1, &lighting);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__,
            __LINE__);

            g_xray_detector.displayBeam();
        }

        // Disable the shader
        gVirtualXRay::popShaderProgram();

        // Restore the current transformation matrix
        gVirtualXRay::popModelViewMatrix();

        // Restore the attributes
        gVirtualXRay::popEnableDisableState();

        // Check the status of OpenGL and of the current FBO
        gVirtualXRay::checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    }
    // Catch exception if any
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
    }*/
}


//------------------------------------
void SimpleGVXR::VulkanWindow::registerCallbacks()
//------------------------------------
{
	if (m_context.getWindowHandle() && !m_callback_registered)
	{
		Window::registerCallbacks();

        // Initialize GLFW callbacks
        glfwSetFramebufferSizeCallback(m_context.getWindowHandle(), framebufferSizeCallback);
	}
}


//--------------------------------------------------------------
//void SimpleGVXR::VulkanWindow::framebufferSizeCallback(GLFWwindow* apWindow,
//                                           int width,
//										   int height)
//--------------------------------------------------------------
//{
//    if (height == 0) {
    // Prevent divide by 0
//    height = 1;
//    }

    //int x(0), y(0), w(width), h(height);
    //std::cout << x << " " << y << " " << w << " " << h << std::endl;
    //double screen_aspect_ratio(double(width) / double(height));
    //glViewport(x, y, w, h);

    //Window* p_window = Window::getWindow(apWindow);
    //if (p_window)
	//{
//    	p_window->initViewer();
//	}

  /*  gVirtualXRay::loadPerspectiveProjectionMatrix(g_initial_fovy,
    screen_aspect_ratio, g_initial_near, g_initial_far);

    gVirtualXRay::loadLookAtModelViewMatrix(0.0, 0.0, g_zoom, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0);
}
*/

#ifdef HAS_VULKAN

static VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallbackFn(
    VkDebugReportFlagsEXT                       flags,
    VkDebugReportObjectTypeEXT                  objectType,
    uint64_t                                    object,
    size_t                                      location,
    int32_t                                     messageCode,
    const char*                                 pLayerPrefix,
    const char*                                 pMessage,
    void*                                       pUserData) {

    std::cerr << "Debug Report: " << pLayerPrefix << ": " << pMessage << std::endl;

    return VK_FALSE;
 }
#endif

//-------------------------------------------
const char* getVendorName(uint32_t aVendorID)
//-------------------------------------------
{
	switch (aVendorID)
	{
	case 4130:
		return "Advanced Micro Devices ";

	case 8086:
		return "Intel Corporation";

	case 4318:
		return "NVidia Corporation";

	default:
		return "Unknown vendor";
	}
}


//---------------------------------------------
const char* getDeviceType(uint32_t aDeviceType)
//---------------------------------------------
{
	switch (aDeviceType)
	{
	case 0:
		return "Other";

	case 1:
		return "Integrated GPU";

	case 2:
		return "Discrete GPU";

	case 3:
		return "Virtual GPU";

	case 4:
		return "CPU";

	default:
		return "Unknown device type";
	}
}


//----------------------------------------
void SimpleGVXR::VulkanWindow::printDeviceProperties()
//----------------------------------------
{
#ifdef HAS_VULKAN

	for (int i = 0; i < m_number_of_devices; ++i)
	{
		std::cout << "Physical Device #" << i << std::endl;

		VkPhysicalDeviceProperties device_properties;

		vkGetPhysicalDeviceProperties(g_device_set[i], &device_properties);

		std::cout << "Use this device: " << ((i == g_physical_device_index) ? "Yes" : "No");
		std::cout << "Vulkan API version:\t" <<
				VK_VERSION_MAJOR(device_properties.apiVersion) << "."  <<
				VK_VERSION_MINOR(device_properties.apiVersion) << "."  <<
				VK_VERSION_PATCH(device_properties.apiVersion) <<
				std::endl;

		std::cout << "Driver version:\t" << device_properties.driverVersion << std::endl;
		std::cout << "Vendor ID:\t" << 		getVendorName(device_properties.vendorID) << std::endl;
		//std::cout << "Device ID:\t" << 		device_properties.deviceID << std::endl;
		std::cout << "Device Type:\t" <<    getDeviceType(device_properties.deviceType) << std::endl;
		std::cout << "Device name:\t" <<    device_properties.deviceName << std::endl << std::endl;
	}
#endif
}


//--------------------------------------------------------------
std::vector<const char *> SimpleGVXR::VulkanWindow::enableValidationLayers()
//--------------------------------------------------------------
{
	// Store here the extensions that are enabled
    std::vector<const char *> enabled_extension_set;

#ifdef HAS_VULKAN
    // By enabling validation layers, Vulkan will emit warnings if the API
    // is used incorrectly. We shall enable the layer VK_LAYER_LUNARG_standard_validation,
    // which is basically a collection of several useful validation layers.
    if (g_enable_validation_layers)
    {
    	// Get the number of supported layers
        uint32_t layer_count;
        vkEnumerateInstanceLayerProperties(&layer_count, NULL);

        // Get all supported layers with vkEnumerateInstanceLayerProperties
        std::vector<VkLayerProperties> layer_property_set(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, layer_property_set.data());

        // Check if VK_LAYER_LUNARG_standard_validation is among the supported layers
        bool lunarg_standard_validation_layer_found = false;
        for (VkLayerProperties property: layer_property_set)
        {
            if (strcmp("VK_LAYER_LUNARG_standard_validation", property.layerName) == 0)
            {
            	lunarg_standard_validation_layer_found = true;
                break;
            }
        }

        // It has not been found
        if (!lunarg_standard_validation_layer_found)
        {
        	// Throw an error
            throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
                "Layer VK_LAYER_LUNARG_standard_validation not supported");
        }

        // Use VK_LAYER_LUNARG_standard_validation layer
        enabledLayers.push_back("VK_LAYER_LUNARG_standard_validation");


        // We need to enable an extension named VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
        // in order to be able to print the warnings emitted by the validation layer.
		// Check if the VK_EXT_DEBUG_REPORT_EXTENSION_NAME extension is among the supported extensions

    	// Get the number of supported extensions
        uint32_t extension_count;
        vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);

        // Get all supported extensions with vkEnumerateInstanceExtensionProperties
        std::vector<VkExtensionProperties> extension_property_set(extension_count);
        vkEnumerateInstanceExtensionProperties(NULL, &extension_count, extension_property_set.data());

        bool found_extension = false;
        for (VkExtensionProperties property: extension_property_set)
        {
            if (strcmp(VK_EXT_DEBUG_REPORT_EXTENSION_NAME, property.extensionName) == 0)
            {
            	found_extension = true;
                break;
            }
        }

        // Use VK_EXT_DEBUG_REPORT_EXTENSION_NAME extension
        if (!found_extension)
        {
        	// Throw an error
            throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
                "Extension VK_EXT_DEBUG_REPORT_EXTENSION_NAME not supported");
        }
        enabled_extension_set.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }
#endif

    return enabled_extension_set;
}


//---------------------------------------------------------------------------------------
void SimpleGVXR::VulkanWindow::createInstance(const std::vector<const char *>& anEnabledExtensionSet)
//---------------------------------------------------------------------------------------
{
#ifdef HAS_VULKAN
	// Create the application info.
    VkApplicationInfo applicationInfo = {};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = "SimpleGVXR";
    applicationInfo.applicationVersion = 0;
    applicationInfo.pEngineName = "gVirtualXRay";
    applicationInfo.engineVersion = 0;
    applicationInfo.apiVersion = VK_API_VERSION_1_0;;

	// Create the instance info.
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.flags = 0;
    createInfo.pApplicationInfo = &applicationInfo;

    // Give our desired layers and extensions to vulkan.
    createInfo.enabledLayerCount = enabledLayers.size();
    createInfo.ppEnabledLayerNames = enabledLayers.data();
    createInfo.enabledExtensionCount = anEnabledExtensionSet.size();
    createInfo.ppEnabledExtensionNames = anEnabledExtensionSet.data();

    // Create the instance
    VK_CHECK_RESULT(vkCreateInstance(
        &createInfo,
        NULL,
        &instance));

    // Register a callback function for the extension VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
    // so that warnings emitted from the validation layer are actually printed
    if (g_enable_validation_layers)
    {
        VkDebugReportCallbackCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        createInfo.pfnCallback = &debugReportCallbackFn;

        // We have to explicitly load this function
        auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
        if (vkCreateDebugReportCallbackEXT == nullptr)
        {
        	// Throw an error
            throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
                "Could not load vkCreateDebugReportCallbackEXT");
        }

        // Create and register callback.
        VK_CHECK_RESULT(vkCreateDebugReportCallbackEXT(instance, &createInfo, NULL, &debugReportCallback));
    }
#endif
}


//-----------------------------------------
uint32_t SimpleGVXR::VulkanWindow::findPhysicalDevice()
//-----------------------------------------
{
#ifdef HAS_VULKAN
    // Find a physical device that can be used with Vulkan

	// Get the number of physical devices on the system with vkEnumeratePhysicalDevices
    vkEnumeratePhysicalDevices(instance, &m_number_of_devices, NULL);

    // There is no device
    if (m_number_of_devices == 0)
    {
    	// Throw an error
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
            "Could not find a device with Vulkan support");
    }

    // List all physical devices on the system with vkEnumeratePhysicalDevices
    g_device_set.resize(m_number_of_devices);
    vkEnumeratePhysicalDevices(instance, &m_number_of_devices, g_device_set.data());

    // Make sure the physical device supports both VK_QUEUE_COMPUTE_BIT and VK_QUEUE_GRAPHICS_BIT
    for (uint32_t i = 0; i < g_device_set.size(); ++i)
    {
        if (supportsQueue(i, VK_QUEUE_COMPUTE_BIT) && supportsQueue(i, VK_QUEUE_GRAPHICS_BIT))
        {
            return i;
        }
    }
#endif
	// Throw an error
    throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
        "Could not find a Vulkan device with both compute and graphics queue support");
}


//---------------------------------------------------------------------
bool SimpleGVXR::VulkanWindow::supportsQueue(u_int32_t aDeviceID, u_int32_t aQueue)
//---------------------------------------------------------------------
{
#ifdef HAS_VULKAN
	// Get the number of queue families
    uint32_t queue_family_count;
    vkGetPhysicalDeviceQueueFamilyProperties(g_device_set[aDeviceID], &queue_family_count, NULL);

    // Retrieve all queue families.
    std::vector<VkQueueFamilyProperties> queue_family_set(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(g_device_set[aDeviceID], &queue_family_count, queue_family_set.data());

    // Find a family that supports the requested queue
    for (uint32_t i = 0; i < queue_family_set.size(); ++i)
    {
        VkQueueFamilyProperties props = queue_family_set[i];

        if (props.queueCount > 0 && (props.queueFlags & aQueue))
        {
            // Found a suitable queue
            return true;
        }
    }
#endif
    // Did not find a suitable queue
    return false;
}



//-----------------------------------------------------------------
uint32_t SimpleGVXR::VulkanWindow::getComputeQueueFamilyIndex(u_int32_t aQueue)
//-----------------------------------------------------------------
{
  #ifdef HAS_VULKAN
	// Get the number of queue families
    uint32_t queue_family_count;
    vkGetPhysicalDeviceQueueFamilyProperties(g_device_set[g_physical_device_index],
    		&queue_family_count,
			NULL);

    // Retrieve all queue families.
    std::vector<VkQueueFamilyProperties> queue_family_set(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(g_device_set[g_physical_device_index],
    		&queue_family_count,
			queue_family_set.data());

    // Find a family that supports the requested queue
    for (uint32_t i = 0; i < queue_family_set.size(); ++i)
    {
        VkQueueFamilyProperties props = queue_family_set[i];

        if (props.queueCount > 0 && (props.queueFlags & aQueue))
        {
            // Found a suitable queue
            return i;
        }
    }
#endif
    // This line should not be reached
	throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
		"Could not find a queue family that supports operations");
}


//-------------------------------
void SimpleGVXR::VulkanWindow::createDevice()
//-------------------------------
{
#ifdef HAS_VULKAN
	uint32_t compute_queue_index  = getComputeQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
	uint32_t graphics_queue_index = getComputeQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);

	if (compute_queue_index != graphics_queue_index)
	{
		throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
			"The compute and graphics queues don't have the same index");
	}

	queueFamilyIndex = compute_queue_index;

	// Create the logical device

	// DeviceQueueCreateInfo
    VkDeviceQueueCreateInfo queue_create_info = {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queueFamilyIndex;
    queue_create_info.queueCount = 1; // create one queue in this family. We don't need more.
    float queuePriorities = 1.0;  // we only have one queue, so this is not that imporant.
    queue_create_info.pQueuePriorities = &queuePriorities;

    // DeviceCreateInf
    VkDeviceCreateInfo device_create_info = {};

    // Specify any desired device features here
    VkPhysicalDeviceFeatures device_features = {};

    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.enabledLayerCount = enabledLayers.size();  // need to specify validation layers here as well.
    device_create_info.ppEnabledLayerNames = enabledLayers.data();
    device_create_info.pQueueCreateInfos = &queue_create_info; // when creating the logical device, we also specify what queues it has.
    device_create_info.queueCreateInfoCount = 1;
    device_create_info.pEnabledFeatures = &device_features;

    VK_CHECK_RESULT(vkCreateDevice(g_device_set[g_physical_device_index], &device_create_info, NULL, &device)); // create logical device.

    // Get a handle to the only member of the queue family.
    vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);
#endif
}


#ifdef HAS_VULKAN
// find memory type with desired properties.
uint32_t findMemoryType(uint32_t memoryTypeBits, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memoryProperties;

    vkGetPhysicalDeviceMemoryProperties(g_device_set[g_physical_device_index], &memoryProperties);

    /*
    How does this search work?
    See the documentation of VkPhysicalDeviceMemoryProperties for a detailed description.
    */
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
        if ((memoryTypeBits & (1 << i)) &&
            ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties))
            return i;
    }
    return -1;
}
#endif
