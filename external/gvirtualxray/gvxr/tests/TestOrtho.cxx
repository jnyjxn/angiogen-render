/*

Copyright (c) 2019, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       TestOrtho.cxx
*
*   @brief      Unit testing: test orthographic projection.
*   			Last date of check: Passed test on 01 Apr 2019.
*
*   @version    1.0
*
*   @date       01/04/2019
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Apr 2019, 2019, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <cmath>
#include <iomanip>
#include <memory>
#include <cstdlib>

#include "gtest/gtest.h"

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

#ifndef __Units_h
#include "gVirtualXRay/Units.h"
#endif

#ifndef __SceneGraphNode_h
#include "gVirtualXRay/SceneGraphNode.h"
#endif

#ifndef __CylinderMesh_h
#include "gVirtualXRay/CylinderMesh.h"
#endif

#ifndef __CubeMesh_h
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef __XRayBeam_h
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef __XRayDetector_h
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef __Matrix4x4_h
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef __Shader_h
#include "gVirtualXRay/Shader.h"
#endif

#ifndef __Utilities_h
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
//using namespace std;


//******************************************************************************
//  Constant variables
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************
//******************************************************************************
//  Global variables
//******************************************************************************
GLsizei g_main_window_width(1);
GLsizei g_main_window_height(1);
GLFWwindow* g_p_main_window_id(0);

Matrix4x4<GLfloat> g_sample_rotation_matrix;

GLfloat g_incident_energy(80.0 * keV);
GLfloat g_resolution(1 * mm); // 1mm per pixel
Vec2ui g_number_of_pixels(201, 201);
VEC3 g_source_position(   0.0, 0.0, -10.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

SceneGraphNode g_scene_graph("root", NULL);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;

double g_cube_length = 51.0 * mm;


//******************************************************************************
//  Function declaration
//******************************************************************************
void initGL();
void loadSTLFile();
void loadDetector();
void loadSource();
void loadXRaySimulator();
void errorCallback(int error, const char* description);
void quit();


//------------------------
TEST(TestOrtho, TestOrtho)
//------------------------
{
    // Make sure the detector has an odd number of columns
    if ((g_number_of_pixels.getX() % 2) == 0)
        {
        g_number_of_pixels.setX(g_number_of_pixels.getX() + 1);
        }

    // Make sure the detector has an odd number of rows
    if ((g_number_of_pixels.getY() % 2) == 0)
        {
        g_number_of_pixels.setY(g_number_of_pixels.getY() + 1);
        }

    // Set an error callback
    glfwSetErrorCallback(errorCallback);

    // Initialize GLFW
    ASSERT_EQ(glfwInit(), GLFW_TRUE) << "Cannot initialise GLFW.";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, false);

    // Enable anti-aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a windowed mode window and its OpenGL context
    g_p_main_window_id = glfwCreateWindow(g_main_window_width,
        g_main_window_height,
        "gVirtualXRay -- Unit testing: Point source.",
        0,
        0);

    // Window cannot be created
    if (!g_p_main_window_id)
        {
        glfwTerminate();
        }
    ASSERT_TRUE(g_p_main_window_id) << "Cannot create a GLFW windowed mode window and its OpenGL context.";

    // Make the window's context current
    glfwMakeContextCurrent(g_p_main_window_id);

    // Initialise GLEW
    initialiseGLEW();

    // Initialise OpenGL
    initGL();

    // Check the current FBO
    checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Load the data
    loadDetector();
    loadSource();
    loadXRaySimulator();

    // Add the geometry to the X-ray renderer
    loadSTLFile();
    g_xray_renderer.addInnerSurface(&g_scene_graph.getChild("inside"));

    // Set the projection matrix
    GLint width(0);
    GLint height(0);
    glfwGetFramebufferSize(g_p_main_window_id, &width, &height);

    // Output the detector properties
    std::cout << "Detector size: " << g_number_of_pixels.getX() << "x" << g_number_of_pixels.getY() << " pixels" << std::endl;
    std::cout << std::fixed << "Pixel size:" << g_resolution / mm << "x" << g_resolution / mm << " mm2" << std::endl;
    std::cout << "Detector size: " << g_resolution / cm * g_number_of_pixels.getX() << "x" << g_resolution / cm * g_number_of_pixels.getY() << " cm2" << std::endl;
    std::cout << "Detector position: " << g_detector_position / cm << " (in cm)" << std::endl;
    std::cout << "Source position: "   << g_source_position   / cm << " (in cm)" << std::endl;

    RATIONAL_NUMBER tangent(
        ((g_resolution * g_number_of_pixels.getX()) / 2.0) /
        ((g_detector_position - g_source_position).length()));

    RATIONAL_NUMBER angle_in_radians(2.0 * std::atan(tangent));
    RATIONAL_NUMBER angle_in_degrees(180.0 * angle_in_radians / M_PI);
    std::cout << "Field of view: " <<
        angle_in_degrees <<
        " degrees" << std::endl;

    // Compute the projection set
    g_xray_renderer.disableArtifactFilteringOnCPU();
    g_xray_renderer.disableArtifactFilteringOnGPU();
    g_xray_renderer.computeProjectionSet(g_sample_rotation_matrix,
        VEC3(),
        1,
        0);

    // Save the projection set
    g_xray_renderer.printProjectionSet("TestOrtho_single_projection.mha");

    
    g_xray_renderer.computeLBuffer(g_scene_graph.getChild("inside"),
                                   VEC3(),
                                   g_sample_rotation_matrix,
                                   true);
    
    g_xray_renderer.printLBuffer("TestOrtho_lbuffer.mha");

    
    // Close the window
    glfwSetWindowShouldClose(g_p_main_window_id, GL_TRUE);

    // Close the window and shut GLFW
    quit();

    // Load reference image
    Image<XRayRenderer::PixelType> reference_l_buffer(g_number_of_pixels[0], g_number_of_pixels[1]);
    Image<XRayRenderer::PixelType> gvxr_l_buffer(
    		g_xray_renderer.getLBuffer().getROI(
    				0, 0, 0, g_number_of_pixels[0], g_number_of_pixels[1], 1
					)
			);

    // Compute the theoretical L-buffer
    unsigned cube_pixel = g_cube_length / g_resolution;
    double distance_intersection = -(g_source_position - g_detector_position).length();

    for (unsigned int j = 0; j < g_number_of_pixels[1]; ++j)
    {
        for (unsigned int i = 0; i < g_number_of_pixels[0]; ++i)
        {
        	unsigned int ii = (g_number_of_pixels[0] - cube_pixel) / 2;
        	unsigned int jj = (g_number_of_pixels[1] - cube_pixel) / 2;

        	if ((ii <= i && i < ii + cube_pixel) && (jj <= j && j < jj + cube_pixel))
			{
        		reference_l_buffer.setPixel(i, j, 0, g_cube_length / cm);
			}
        }
    }

    reference_l_buffer.save("reference_l_buffer.mha");

    // Compare reference and simulated projection set using NCC
    double ZNCC = reference_l_buffer.computeNCC(gvxr_l_buffer);
    ASSERT_NEAR(ZNCC, 1, 1e-9);

    // Compare reference projection set and simulated projection set using RMSE
    float RMSE = reference_l_buffer.computeRMSE(gvxr_l_buffer);
    ASSERT_NEAR(RMSE, 0, 1.0 * um);
}


//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
    std::cerr << "GLFW error: " << description << std::endl;
}


//---------
void quit()
//---------
{
    if (g_p_main_window_id)
        {
        glfwDestroyWindow(g_p_main_window_id);
        g_p_main_window_id = 0;
        //glfwTerminate();
        }
}


//-----------
void initGL()
//-----------
{
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable the Z-buffer
    glEnable(GL_DEPTH_TEST);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the background colour
    glClearColor(0.5, 0.5, 0.5, 1.0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glEnable(GL_MULTISAMPLE);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//----------------
void loadSTLFile()
//----------------
{
	// Set the geometry
    g_scene_graph.addChild("inside", CubeMesh<GLdouble>(g_cube_length));

    // HU values
    RATIONAL_NUMBER HU_water(0.0);
    RATIONAL_NUMBER HU_bone(1329.885683);
    RATIONAL_NUMBER HU_soft_tissue(51.714752);

    g_scene_graph.getChild( "inside").getPolygonMesh().setHounsfieldValue(HU_bone);
}


//-----------------
void loadDetector()
//-----------------
{
    g_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
    g_xray_detector.setNumberOfPixels(g_number_of_pixels);
    g_xray_detector.setDetectorPosition(g_detector_position);
    g_xray_detector.setUpVector(g_detector_up_vector);
}


//---------------
void loadSource()
//---------------
{
    // Set the energy
    g_xray_beam.initialise(g_incident_energy);

    // Set the source position
    g_xray_detector.setXrayPointSource(g_source_position);
    g_xray_detector.setParallelBeam();
}


//----------------------
void loadXRaySimulator()
//----------------------
{
    // Initialise the X-ray renderer
    g_xray_renderer.initialise(XRayRenderer::OPENGL,
        GL_RGB32F,
        &g_xray_detector,
        &g_xray_beam);
}