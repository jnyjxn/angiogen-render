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
*   @file       TestBeerLambertLawMonoChromatic.cxx
*
*   @brief      Unit testing: Beer-Lambert Law, Monochromatism Case,
*   			the unit test corresponds to validation_03,
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

#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

#ifndef __Units_h
#include "gVirtualXRay/Units.h"
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
Vec2ui g_number_of_pixels(600 / g_scaling_factor, 1);
VEC3 g_source_position(   0.0, 0.0, -10.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

SceneGraphNode g_scene_graph("root", NULL);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_half_precision_renderer;
XRayRenderer g_xray_full_precision_renderer;

RATIONAL_NUMBER g_literature_result(0.0);
RATIONAL_NUMBER g_expected_result(0.0);

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


//--------------------------------------------------------------------
TEST(TestBeerLambertLawMonoChromatic, TestBeerLambertLawPolyChromatic)
//--------------------------------------------------------------------
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
    g_xray_half_precision_renderer.addInnerSurface(&g_scene_graph.getChild("inside"));
    g_xray_half_precision_renderer.addOuterSurface(&g_scene_graph.getChild("outside"));
    g_xray_full_precision_renderer.addInnerSurface(&g_scene_graph.getChild("inside"));
    g_xray_full_precision_renderer.addOuterSurface(&g_scene_graph.getChild("outside"));

    // Compute the X-ray image
    g_xray_half_precision_renderer.computeImage(g_sample_rotation_matrix);
    g_xray_full_precision_renderer.computeImage(g_sample_rotation_matrix);

    RATIONAL_NUMBER* energy_fluence(
        g_xray_half_precision_renderer.getEnergyFluence().getRawData());

    RATIONAL_NUMBER central_pixel(energy_fluence[g_number_of_pixels.getX() / 2]);

    RATIONAL_NUMBER simulated_result(central_pixel);
    ASSERT_NEAR(g_expected_result, simulated_result, 0.01 / g_expected_result);

    std::cout << "On GPU, with half precision, the energy (in keV) transmitted orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    std::cout << "\t\t           = " << /*std::scientific <<*/ simulated_result / keV << " keV" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_half_precision| keV" << std::endl;
    std::cout << "                     : |" << /*std::scientific <<*/ g_expected_result / keV << " - " << simulated_result / keV << "| keV" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(g_expected_result - simulated_result) / keV << " keV" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(g_expected_result - simulated_result) / g_expected_result << " \%" << std::endl;

    energy_fluence =
            g_xray_full_precision_renderer.getEnergyFluence().getRawData();

    central_pixel = energy_fluence[g_number_of_pixels.getX() / 2];

    simulated_result = central_pixel;
    ASSERT_FLOAT_EQ(g_expected_result, simulated_result);

    // Close the window and shut EGL
    quit();

    std::cout << "On GPU, with full precision, the energy (in keV) transmitted orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    std::cout << "\t\t           = " << /*std::scientific <<*/ simulated_result / keV << " keV" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_full_precision| keV" << std::endl;
    std::cout << "                     : |" << /*std::scientific <<*/ g_expected_result / keV << " - " << simulated_result / keV << "| keV" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(g_expected_result - simulated_result) / keV << " keV" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(g_expected_result - simulated_result) / g_expected_result << " \%" << std::endl;
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
    g_scene_graph.addChild( "inside", CylinderMesh<GLdouble>(20, 3.0 * cm, 1.0 * cm));
    g_scene_graph.addChild("outside", CubeMesh<GLdouble>(3.0 * cm));

    // HU values
    RATIONAL_NUMBER HU_water(0.0);
    RATIONAL_NUMBER HU_bone(1329.885683);
    RATIONAL_NUMBER HU_soft_tissue(51.714752);

    g_scene_graph.getChild( "inside").getPolygonMesh().setHounsfieldValue(HU_bone);
    g_scene_graph.getChild("outside").getPolygonMesh().setHounsfieldValue(HU_soft_tissue);

    std::cout << "We run the simulation as follows:" << std::endl;
    std::cout << "- A cube, which has edge length of 3 cm, is made of soft tissue (HU = " << std::floor(HU_soft_tissue + 0.5) << "). " << std::endl;
    std::cout << "- Inside, a cylinder is inserted. " <<
            "It is made of bone (HU = " << std::floor(HU_bone + 0.5) << "), its height is 3 cm and its diameter is 2 cm." << std::endl;
    std::cout << "- We consider the energy of the incident beam is " << g_incident_energy / keV << " keV (i.e. " << g_incident_energy / keV << " keV). " << std::endl << std::endl;

    std::cout << "From the literature, we know:" << std::endl;
    std::cout.precision(3);
    std::cout <<
            "HU =    "     << std::fixed << HU_water << "\t" <<
            "rho = "    << std::scientific << 1.0 << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << 1.837E-01 << " cm2/g,\t" <<
            "mu = "     << std::scientific << 1.837E-01 << " cm-1" << std::endl;

    std::cout <<
            "HU = "     << std::fixed << HU_bone << "\t" <<
            "rho = "    << std::scientific << 1.920E+00 << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << 2.229E-01 << " cm2/g,\t" <<
            "mu = "     << std::scientific << 4.280E-01 << " cm-1" << std::endl;

    std::cout <<
            "HU =   "     << std::fixed << HU_soft_tissue << "\t" <<
            "rho = "    << std::scientific << 1.060E+00 << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << 1.823E-01 << " cm2/g,\t" <<
            "mu = "     << std::scientific << 1.932E-01 << " cm-1" << std::endl << std::endl;

    g_literature_result = g_incident_energy * exp(-(4.280e-01 * 2 + 1.932e-01 * 1));
    std::cout << "The energy (in keV) transmitted orthogonally throw the middle of cube and cylinder should be:" << std::endl;
    std::cout << "\t\tI_reference = I_0 * exp(-Sum(mu * x))" << std::endl;
    std::cout << "\t\t            = " << std::fixed << g_incident_energy / keV << " * exp(-(mu_bone * 2 + mu_soft_tissue * 1))" << std::endl;
    std::cout << "\t\t            = " << g_incident_energy / keV << " * exp(-(4.280e-01 * 2 + 1.932e-01 * 1))" << std::endl;
    std::cout << "\t\t            = " << /*std::scientific <<*/ g_literature_result / keV << " keV" << std::endl << std::endl;

    std::cout << "To obtain linear attenuation coefficients (mu values) in our implementation, densities (rho values) and mass attenuation coefficients (mu/rho values) are estimated as follows:" << std::endl;
    std::cout <<
            "HU =    "     << std::fixed << HU_water << "\t" <<
            "rho = "    << std::scientific << g_xray_half_precision_renderer.getDensityFromHU(HU_water)  / (g/cm3) << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << g_xray_half_precision_renderer.getMassAttenuationFromHU(HU_water, g_incident_energy)  / (cm2/g) << " cm2/g,\t" <<
            "mu = "     << std::scientific << g_xray_half_precision_renderer.getMuFromHU(HU_water, g_incident_energy) / (1.0 / cm) << " cm-1" << std::endl;

    std::cout <<
            "HU = "     << std::fixed << HU_bone << "\t" <<
            "rho = "    << std::scientific << g_xray_half_precision_renderer.getDensityFromHU(HU_bone)  / (g/cm3) << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << g_xray_half_precision_renderer.getMassAttenuationFromHU(HU_bone, g_incident_energy)  / (cm2/g) << " cm2/g,\t" <<
            "mu = "     << std::scientific << g_xray_half_precision_renderer.getMuFromHU(HU_bone, g_incident_energy) / (1.0 / cm) << " cm-1" << std::endl;

    std::cout <<
            "HU =   "     << std::fixed << HU_soft_tissue << "\t" <<
            "rho = "    << std::scientific << g_xray_half_precision_renderer.getDensityFromHU(HU_soft_tissue)  / (g/cm3) << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << g_xray_half_precision_renderer.getMassAttenuationFromHU(HU_soft_tissue, g_incident_energy)  / (cm2/g) << " cm2/g,\t" <<
            "mu = "     << std::scientific << g_xray_half_precision_renderer.getMuFromHU(HU_soft_tissue, g_incident_energy) / (1.0 / cm) << " cm-1" << std::endl << std::endl;

    g_expected_result = g_incident_energy * exp(-(g_xray_half_precision_renderer.getMuFromHU(HU_bone, g_incident_energy) / (1.0 / cm) * 2 + g_xray_half_precision_renderer.getMuFromHU(HU_soft_tissue, g_incident_energy) / (1.0 / cm) * 1));
    std::cout << "The energy (in keV) transmitted orthogonally throw the middle of cube and cylinder should be:" << std::endl;
    std::cout << "\t\tI_expected = I_0 * exp(-Sum(mu * x))" << std::endl;
    std::cout << "\t\t           = " << std::fixed << g_incident_energy / keV << " * exp(-(mu_bone * 2 + mu_soft_tissue * 1))" << std::endl;
    std::cout << "\t\t           = " << std::fixed << g_incident_energy / keV << " * exp(-(" << std::scientific << g_xray_half_precision_renderer.getMuFromHU(HU_bone, g_incident_energy) / (1.0 / cm) << " * 2 + " << g_xray_half_precision_renderer.getMuFromHU(HU_soft_tissue, g_incident_energy) / (1.0 / cm) << " * 1))" << std::endl;
    std::cout << "\t\t           = " << /*std::scientific*/ std::fixed << g_expected_result / keV << " keV" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_reference| keV" << std::endl;
    std::cout << "                     : |" << /*std::scientific <<*/ g_expected_result / keV << " - " << g_literature_result / keV << "| keV" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(g_expected_result - g_literature_result) / keV << " keV" << std::endl << std::endl;

    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
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
