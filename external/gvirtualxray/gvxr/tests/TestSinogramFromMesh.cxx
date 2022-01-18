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
*   @file       TestSinogramFromMesh.cxx
*
*   @brief      Unit testing: Sinogram,
*   			the unit test corresponds to validation_08,
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
//  Constant variables
//******************************************************************************
const float g_incident_energy(0.08 * MeV);
const VEC2 g_resolution(0.5 * mm, 1.0 * mm);
const Vec2ui g_number_of_pixels(367, 128);
const VEC3 g_source_position(   0.0,  10.0 * cm, 0.0);
const VEC3 g_detector_position( 0.0, -10.0 * cm, 0.0);
const VEC3 g_detector_up_vector(0.0, 0.0, 1.0);

const unsigned int g_angle_number(360);
const RATIONAL_NUMBER g_angle_step(0.5);


//******************************************************************************
//  Global variables
//******************************************************************************
Context g_GL_context;

Matrix4x4<GLfloat> g_sample_rotation_matrix;

std::vector<float> g_p_vertex_data;
std::vector<unsigned int> g_p_index_data;

SceneGraphNode g_polygon_mesh;

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;
Shader g_display_shader;


//******************************************************************************
//  Function declaration
//******************************************************************************
void createPhantom();

void quit();

void loadSTLFile();

void loadDetector();

void loadSource();

void loadXRaySimulator();

Vec3<GLfloat> getArcballVector(int x, int y);

void computeRotation(MATRIX4& aRotationMatrix);

void updateXRayImage();

void errorCallback(int error, const char* description);

void draw();


//----------------------------------------------
TEST(TestSinogramFromMesh, TestSinogramFromMesh)
//----------------------------------------------
{
    // Initialize random seed
    srand(time(0));

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

    // Check the OpenGL version
    std::cout << "GL:\t" << glGetString(GL_VERSION) << std::endl;

    // Check the GLSL version
    std::cout << "GLSL:\t" <<
        glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Check the current FBO
    checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Create the phantom
    createPhantom();

    // Display the mu coefficients
    std::cout << "Linear attenuation coefficient at:\t" << g_incident_energy / keV << " keV (" << g_incident_energy / MeV << "MeV)" << std::endl;
    std::cout << "Mu(HU(0):     \t" << g_xray_renderer.getMuFromHU(0, g_incident_energy) / (1.0 / cm)      << " cm-1" << std::endl;
    std::cout << "Mu(HU(-1000): \t" << g_xray_renderer.getMuFromHU(-1000, g_incident_energy) / (1.0 / cm)      << " cm-1" << std::endl;

    // Load the data
    loadDetector();
    loadSource();
    loadXRaySimulator();

    // Add the geometry to the X-ray renderer
    loadSTLFile();
    g_xray_renderer.addInnerSurface(&g_polygon_mesh);

    std::cout << "Angular step: " << g_angle_step << " degrees" << std::endl;
    std::cout << "Angular span: [0, " <<
        g_angle_step * (g_angle_number - 1) <<
        "] degrees" << std::endl;

    // Compute the sinogram
    g_xray_renderer.useArtefactFilteringOnGPU(true);
    g_xray_renderer.computeSinogram(g_sample_rotation_matrix,
        VEC3(),
        g_angle_number,
        g_angle_step);

    // Save the sinogram
    g_xray_renderer.printSinogram("TestSinogram_simulation_sinogram.mha");

    // Compute the projection set
    g_xray_renderer.computeProjectionSet(
        g_sample_rotation_matrix,
        VEC3(),
        g_angle_number,
        g_angle_step);

    // Save the projection set
    g_xray_renderer.printProjectionSet("TestSinogram_simulation_projection_set.mha");

    // Close the window and shut EGL
    quit();

    // Load reference sinogram (computed using GNU octave)
    Image<XRayRenderer::PixelType> reference_sinogram("data/phantom_sinogram.mhd");

    // Compare reference sinogram and simulated sinogram using NCC
    ASSERT_NEAR(reference_sinogram.computeNCC(g_xray_renderer.getSinogram()), 1.0, 0.00001);

    // Compare reference sinogram and simulated sinogram using RMSE
    float RMSE = reference_sinogram.computeRMSE(g_xray_renderer.getSinogram());
    float range = (reference_sinogram.getMaxValue() - reference_sinogram.getMinValue());
    ASSERT_TRUE(RMSE < 0.01f * std::min(range, g_xray_renderer.getSinogram().getMaxValue() - g_xray_renderer.getSinogram().getMinValue()));
}


//------------------
void createPhantom()
//------------------
{
    Image<short> phantom(256, 256, 128, -1000);
    phantom.setSpacing(0.5, 0.5, 1.0);

    // Set pixels in a square to one value
    unsigned int slice_size(phantom.getDepth() / 4);
    unsigned int start_slice((phantom.getDepth() - slice_size) / 2);

    unsigned int step_number(6);


    unsigned int step_height(((phantom.getWidth() - (phantom.getWidth() / 2) / 2)) / step_number);

    for(unsigned int s = start_slice; s < start_slice + slice_size; s++)
        {
        for (unsigned int step(0); step < step_number; ++step)
            {
            unsigned int row_size(phantom.getHeight() / 2);
            unsigned int start_row((phantom.getHeight()-row_size) / 4);
            start_row += step * step_height;

            for (unsigned int r(start_row);
                r < (start_row + step_height);
                ++r)
                {
                unsigned int column_size(phantom.getWidth() / 2);
                unsigned int start_column((phantom.getWidth()-column_size) / 4);

                unsigned int step_width(step_height * (1 + step));
                for (unsigned int c(start_column);
                    c < (start_column + step_width);
                    ++c)
                    {
                    phantom.setPixel(r, c, s, 0);
                    }
                }
            }
        }

    phantom.save("TestSinogram_phantom.mha");

    // Extract the isosurface
    g_polygon_mesh = SceneGraphNode("root", phantom.marchingCubes(-500), VEC3(), NULL);
    g_polygon_mesh.getPolygonMesh().setFileName("step_wedge.stl");
    g_polygon_mesh.getPolygonMesh().writeSTLFile();
}


//---------
void quit()
//---------
{
	g_GL_context.release();
}


//----------------
void loadSTLFile()
//----------------
{
    // Set geometry
    g_polygon_mesh.getPolygonMesh().setHounsfieldValue(0);

    float red((float(rand()) / (RAND_MAX)) + 1);
    float green((float(rand()) / (RAND_MAX)) + 1);
    float blue((float(rand()) / (RAND_MAX)) + 1);
    float alpha(1.0);

    VEC3 base_colour(red, green, blue);
    base_colour.normalise();

    g_polygon_mesh.getPolygonMesh().getMaterial().setAmbientColour(base_colour.getX() * 0.19225,
        base_colour.getY() * 0.19225,
        base_colour.getZ() * 0.19225,
        alpha);

    g_polygon_mesh.getPolygonMesh().getMaterial().setDiffuseColour(base_colour.getX() * 0.50754,
        base_colour.getY() * 0.50754,
        base_colour.getZ() * 0.50754,
        alpha);

    g_polygon_mesh.getPolygonMesh().getMaterial().setSpecularColour(base_colour.getX() * 0.50827,
        base_colour.getY() * 0.50827,
        base_colour.getZ() * 0.50827,
        alpha);

    g_polygon_mesh.getPolygonMesh().getMaterial().setShininess(50);

    // Compute the translation vector
    VEC3 translation_vector(g_polygon_mesh.getNodeAndChildrenWorldMinCorner() +
        (g_polygon_mesh.getNodeAndChildrenWorldMaxCorner() - g_polygon_mesh.getNodeAndChildrenWorldMinCorner()) / 2.0);

    g_sample_rotation_matrix =
        MATRIX4::buildTranslationMatrix(-translation_vector);
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
    g_xray_renderer.initialise(XRayRenderer::OPENGL,
        GL_RGB32F,
        &g_xray_detector,
        &g_xray_beam);
}
