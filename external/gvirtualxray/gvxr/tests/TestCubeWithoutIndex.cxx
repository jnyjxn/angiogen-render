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
*   @file       TestCube.cxx
*
*   @brief      Unit testing: Test the implementation of CubeMesh,
*   			last date of check: Passed test on 01 Apr 2019.
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
#include <cmath>
#include <iomanip>
#include <memory>
#include <cstdlib>

#include "gtest/gtest.h"

#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

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

#ifndef __Context_h
#include "gVirtualXRay/Context.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
//using namespace std;


//******************************************************************************
//  Global variables
//******************************************************************************
Context g_GL_context;

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_detector_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;

GLfloat g_incident_energy(80.0 * keV);
const GLfloat g_scaling_factor(2.0);
GLfloat g_resolution(0.15 * mm * g_scaling_factor); // 1mm per pixel
Vec2ui g_number_of_pixels(600 / g_scaling_factor, 600 / g_scaling_factor);
VEC3 g_source_position(   0.0, 0.0, -10.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

RATIONAL_NUMBER g_cube_side_in_cm(3.0);

SceneGraphNode g_scene_graph("root", NULL);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_half_precision_renderer;
XRayRenderer g_xray_full_precision_renderer;


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


//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer1)
//--------------------------------------
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

    // Register the exit callback
    atexit(quit);

    // Create an OpenGL context
#ifdef HAS_EGL
    g_GL_context.create("EGL", 3, 2);
#else
    g_GL_context.create("OpenGL", 3, 2);
#endif

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
    g_xray_half_precision_renderer.addOuterSurface(&g_scene_graph.getChild("outside"));
    g_xray_full_precision_renderer.addOuterSurface(&g_scene_graph.getChild("outside"));

    // Compute the X-ray image
    g_xray_half_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    g_xray_half_precision_renderer.printLBuffer("cube1_half_precision_renderer.mha");
    g_xray_full_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);
    g_xray_full_precision_renderer.printLBuffer("cube1_full_precision_renderer.mha");


    RATIONAL_NUMBER central_pixel(g_xray_half_precision_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2));

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);
    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);

    std::cout << "On GPU, with half precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" << std::endl;
    std::cout << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_half_precision| cm" << std::endl;
    std::cout << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;

    central_pixel = g_xray_full_precision_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2);

    simulated_result = central_pixel;
    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);

    std::cout << "On GPU, with full precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    std::cout << "\t\t           = " << simulated_result * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_full_precision| cm" << std::endl;
    std::cout << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;
}


//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer2)
//--------------------------------------
{
    // Rotate by 90 deg
    g_sample_rotation_matrix = Matrix4x4<GLfloat>();
    g_sample_rotation_matrix.rotate(90.0, VEC3(1, 0, 0));

    // Compute the X-ray image
    g_xray_half_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    g_xray_full_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);


    RATIONAL_NUMBER central_pixel(g_xray_half_precision_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2));

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);
    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);

    std::cout << "On GPU, with half precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" << std::endl;
    std::cout << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_half_precision| cm" << std::endl;
    std::cout << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;

    central_pixel = g_xray_full_precision_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2);

    simulated_result = central_pixel;
    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);


    std::cout << "On GPU, with full precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    std::cout << "\t\t           = " << simulated_result * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_full_precision| cm" << std::endl;
    std::cout << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;
}


//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer3)
//--------------------------------------
{
    // Rotate by 90 deg
    g_sample_rotation_matrix = Matrix4x4<GLfloat>();
    g_sample_rotation_matrix.rotate(90.0, VEC3(-1, 0, 0));

    // Compute the X-ray image
    g_xray_half_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    g_xray_full_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);


    RATIONAL_NUMBER central_pixel(g_xray_half_precision_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2));

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);
    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);

    std::cout << "On GPU, with half precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" << std::endl;
    std::cout << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_half_precision| cm" << std::endl;
    std::cout << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;

    central_pixel = g_xray_full_precision_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2);

    simulated_result = central_pixel;
    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);


    std::cout << "On GPU, with full precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    std::cout << "\t\t           = " << simulated_result * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_full_precision| cm" << std::endl;
    std::cout << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;
}


//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer4)
//--------------------------------------
{
    // Rotate by 90 deg
    g_sample_rotation_matrix = Matrix4x4<GLfloat>();
    g_sample_rotation_matrix.rotate(90.0, VEC3(0, 1, 0));

    // Compute the X-ray image
    g_xray_half_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    g_xray_full_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);


    RATIONAL_NUMBER central_pixel(g_xray_half_precision_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2));

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);
    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);

    std::cout << "On GPU, with half precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" << std::endl;
    std::cout << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_half_precision| cm" << std::endl;
    std::cout << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;

    central_pixel = g_xray_full_precision_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2);

    simulated_result = central_pixel;
    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);


    std::cout << "On GPU, with full precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    std::cout << "\t\t           = " << simulated_result * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_full_precision| cm" << std::endl;
    std::cout << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;
}


//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer5)
//--------------------------------------
{
    // Rotate by 90 deg
    g_sample_rotation_matrix = Matrix4x4<GLfloat>();
    g_sample_rotation_matrix.rotate(90.0, VEC3(0, -1, 0));

    // Compute the X-ray image
    g_xray_half_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    g_xray_full_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);


    RATIONAL_NUMBER central_pixel(g_xray_half_precision_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2));

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);
    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);

    std::cout << "On GPU, with half precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" << std::endl;
    std::cout << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_half_precision| cm" << std::endl;
    std::cout << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;

    central_pixel = g_xray_full_precision_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2);

    simulated_result = central_pixel;
    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);


    std::cout << "On GPU, with full precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    std::cout << "\t\t           = " << simulated_result * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_full_precision| cm" << std::endl;
    std::cout << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;
}

//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer6)
//--------------------------------------
{
    // Rotate by 90 deg
    g_sample_rotation_matrix = Matrix4x4<GLfloat>();
    g_sample_rotation_matrix.rotate(90.0, VEC3(0, 0, 1));

    // Compute the X-ray image
    g_xray_half_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    g_xray_full_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);


    RATIONAL_NUMBER central_pixel(g_xray_half_precision_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2));

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);
    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);

    std::cout << "On GPU, with half precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" << std::endl;
    std::cout << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_half_precision| cm" << std::endl;
    std::cout << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;

    central_pixel = g_xray_full_precision_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2);

    simulated_result = central_pixel;
    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);


    std::cout << "On GPU, with full precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    std::cout << "\t\t           = " << simulated_result * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_full_precision| cm" << std::endl;
    std::cout << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;
}


//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer7)
//--------------------------------------
{
    // Rotate by 90 deg
    g_sample_rotation_matrix = Matrix4x4<GLfloat>();
    g_sample_rotation_matrix.rotate(90.0, VEC3(0, 0, -1));

    // Compute the X-ray image
    g_xray_half_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    g_xray_full_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);


    RATIONAL_NUMBER central_pixel(g_xray_half_precision_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2));

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);
    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);

    std::cout << "On GPU, with half precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" << std::endl;
    std::cout << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_half_precision| cm" << std::endl;
    std::cout << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;

    central_pixel = g_xray_full_precision_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2);

    simulated_result = central_pixel;
    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);


    std::cout << "On GPU, with full precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    std::cout << "\t\t           = " << simulated_result * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_full_precision| cm" << std::endl;
    std::cout << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;
}


//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer8)
//--------------------------------------
{
  // Close the window and shut EGL
  quit();
}


//---------
void quit()
//---------
{
    g_GL_context.release();
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
    g_scene_graph.addChild("outside", CubeMesh<GLdouble>(g_cube_side_in_cm * cm));


    // HU values
    RATIONAL_NUMBER HU_water(0.0);
    RATIONAL_NUMBER HU_soft_tissue(51.714752);

    g_scene_graph.getChild("outside").getPolygonMesh().setHounsfieldValue(HU_soft_tissue);

    std::cout << "We run the simulation as follows:" << std::endl;
    std::cout << "- A cube, which has edge length of 3 cm, is made of soft tissue (HU = " << std::floor(HU_soft_tissue + 0.5) << "). " << std::endl;
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

    // Use a parallel source
    g_xray_detector.setParallelBeam();
}


//----------------------
void loadXRaySimulator()
//----------------------
{
    // Initialise the X-ray renderer
    g_xray_half_precision_renderer.initialise(XRayRenderer::OPENGL,
        GL_RGB16F,
        &g_xray_detector,
        &g_xray_beam);

    g_xray_full_precision_renderer.initialise(XRayRenderer::OPENGL,
        GL_RGB32F,
        &g_xray_detector,
        &g_xray_beam);
}
