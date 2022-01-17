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
*   @file       SimpleGVXR.cxx
*
*   @brief      SimpleGVXR is an API to the bulk functionalities and classes of
*               gVirtualXRay. SimpleGVXR is used as the basis for
*               the language bindings generated using SWIG.
*
*   @version    1.0
*
*   @date       26/04/2019
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
*   http://www.fpvidal.net/, Apr 2019, 2019, version 1.1, BSD 3-Clause License
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

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype> // Needed for std::toupper
#include <cmath> // Header file for abs

#define GLFW_INCLUDE_GLCOREARB 1

#ifdef HAS_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif

#include <GLFW/glfw3.h>

#ifdef HAS_CUDA
#include <cuda_gl_interop.h>
#include <thrust/device_vector.h>
#endif

// To perform the computations of the X-ray simulation
#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

// For polygon meshes
#ifndef __PolygonMesh_h
#include "gVirtualXRay/PolygonMesh.h"
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

#ifndef __CubeMesh_h
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef __CylinderMesh_h
#include "gVirtualXRay/CylinderMesh.h"
#endif

#ifndef __OutOfBoundsException_h
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef __ElementSet_h
#include "gVirtualXRay/ElementSet.h"
#endif

#ifndef __PhotonCrossSection_h
#include "gVirtualXRay/PhotonCrossSection.h"
#endif

#ifndef __SimpleGVXR_h
#include "SimpleGVXR.h"
#endif

#ifndef __Window_h
#include "Window.h"
#endif

#ifndef __OpenGLWindow_h
#include "OpenGLWindow.h"
#endif

#ifndef __VulkanWindow_h
#include "VulkanWindow.h"
#endif

#ifndef __EGLWindow_h
#include "EGLWindow.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************
using namespace std;


//******************************************************************************
//  Function declarations
//******************************************************************************
GLFWwindow* initGLFW(int aWindowID, int aVisibilityFlag);


void errorCallback(int error, const char* description);

void quit();


void initialiseXRayRenderer();


void computeRotation(gVirtualXRay::MATRIX4& aRotationMatrix,
    int aViewportWidth,
    int aViewportHeight);

gVirtualXRay::Vec3<GLfloat> getArcballVector(int x,
    int y,
    int aViewportWidth,
    int aViewportHeight);

float radian2degree(float anAngle);

std::vector<std::vector<float> > getVectorImage2D(
    const gVirtualXRay::Image<float>& anImage);

std::vector<std::vector<std::vector<float> > > getVectorImage3D(
    const gVirtualXRay::Image<float>& anImage);

gVirtualXRay::Image<float> getImage(const std::vector<std::vector<float> >& anImage);

gVirtualXRay::Image<float> getImage(const std::vector<std::vector<std::vector<float> > >& anImage);

gVirtualXRay::MATRIX4 Vector2gvxrMatrix(
    const std::vector<std::vector<float> >& aNumpyMatrix);

std::vector<std::vector<float> > gvxrMatrix2Vector(
    const gVirtualXRay::MATRIX4& aMatrix4);


//******************************************************************************
//  Global variables
//******************************************************************************
const GLfloat ROTATION_SPEED(2.0);

std::map<int, SimpleGVXR::Window*> g_p_window_set;      // The set of windows
std::vector<int> g_p_window_id_stack;           // The stack of window IDs

double g_initial_fovy(45);                // Field of view along
                                                // the y-axis
double g_initial_near(5.0 * gVirtualXRay::cm);  // Near clipping plane
double g_initial_far(500.0 * gVirtualXRay::cm); // Far clipping plane
GLfloat g_zoom(120.0 * gVirtualXRay::cm);       // How far is the camera from
                                                // the look at point
// State variables
bool g_run_loop = false;
bool g_display_beam(true);
bool g_display_wireframe(false);
bool g_use_lighing(true);
bool g_display_detector(true);
bool g_display_normal_vectors(false);

bool g_beam_shape_initialised = false;
bool g_beam_energy_initialised = false;
bool g_source_position_initialised = false;
bool g_detector_position_initialised = false;
bool g_detector_up_vector_initialised = false;
bool g_detector_size_initialised = false;
bool g_detector_pixel_size_initialised = false;
bool g_renderer_initialised = false;

int g_button(-1);
int g_button_state(-1);
bool g_use_arc_ball(false);
GLint g_last_x_position(0);
GLint g_last_y_position(0);
GLint g_current_x_position(0);
GLint g_current_y_position(0);

gVirtualXRay::XRayBeam g_xray_beam;         // X-ray beam
gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine

gVirtualXRay::Shader g_display_shader;      // Visualisation shader

// Scenegraph of scanned objects
std::shared_ptr<gVirtualXRay::SceneGraphBinder> g_p_scene_graph = 0;

// Rotate the whole 3-D view
gVirtualXRay::Matrix4x4<GLfloat> g_scene_rotation_matrix;

// Rotate the scanned objects
gVirtualXRay::Matrix4x4<GLfloat> g_sample_rotation_matrix;

// Bias/gain of the shift/scale filter for the display of the X-ray image
double g_shift_filter = 0;
double g_scale_filter = 1;

bool g_has_own_gl_context = false;


std::vector<std::vector<std::vector< float> > > lastest_screenshot;


//******************************************************************************
//  Function definitions
//******************************************************************************

//-----------------------------
int getMajorVersionOfCoreGVXR()
//-----------------------------
{
    return (atoi(gVirtualXRay_VERSION_MAJOR));
}

//-----------------------------
int getMinorVersionOfCoreGVXR()
//-----------------------------
{
    return (atoi(gVirtualXRay_VERSION_MINOR));
}

//-----------------------------
int getPatchVersionOfCoreGVXR()
//-----------------------------
{
    return (atoi(gVirtualXRay_VERSION_PATCH));
}

//--------------------------------
std::string getVersionOfCoreGVXR()
//--------------------------------
{
    return (std::string(gvxr_VERSION));
}

//-------------------------------
int getMajorVersionOfSimpleGVXR()
//-------------------------------
{
    return (SimpleGVXR_VERSION_MAJOR);
}

//-------------------------------
int getMinorVersionOfSimpleGVXR()
//-------------------------------
{
    return (SimpleGVXR_VERSION_MINOR);
}

//-------------------------------
int getPatchVersionOfSimpleGVXR()
//-------------------------------
{
    return (SimpleGVXR_VERSION_PATCH);
}

//----------------------------------
std::string getVersionOfSimpleGVXR()
//----------------------------------
{
    return (std::string(SimpleGVXR_VERSION_STRING));
}


void displayBeam(bool aState)
{
    g_display_beam = aState;
}


void displayDetector(bool aState)
{
    g_display_detector = aState;
}


void displayNormalVectors(bool aState)
{
    g_display_normal_vectors = aState;
}


void useWireframe(bool aState)
{
    g_display_wireframe = aState;
}


void useLighing(bool aState)
{
    g_use_lighing = aState;
}


void useNegative(bool aState)
{
    g_xray_renderer.useNegativeFilteringFlag(aState);
}


//--------------------------------------
void autoCreateOpenGLContext(bool aFlag)
//--------------------------------------
{
    g_has_own_gl_context = !aFlag;
}

//-------------------
void initialiseGLEW()
//-------------------
{
    gVirtualXRay::initialiseGLEW();
}

//-------------------
void initializeGLEW()
//-------------------
{
    gVirtualXRay::initialiseGLEW();
}

//------------------------------------------------------
void setSourcePosition(double x,
                       double y,
                       double z,
                       const std::string& aUnitOfLength)
//------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    g_xray_detector.setXrayPointSource(
            gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length,
                    z * unit_of_length));
    g_source_position_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}

//---------------------------------------------------------------------
std::vector<double> getSourcePosition(const std::string& aUnitOfLength)
//---------------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    std::vector<double> position(3);

    position[0] = g_xray_detector.getXraySourceCentre()[0] / unit_of_length;
    position[1] = g_xray_detector.getXraySourceCentre()[1] / unit_of_length;
    position[2] = g_xray_detector.getXraySourceCentre()[2] / unit_of_length;

    return (position);
}

//--------------------------------------------------------
void setDetectorPosition(double x,
                         double y,
                         double z,
                         const std::string& aUnitOfLength)
//--------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    g_xray_detector.setDetectorPosition(
            gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length,
                    z * unit_of_length));
    g_detector_position_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}

//-----------------------------------------------------------------------
std::vector<double> getDetectorPosition(const std::string& aUnitOfLength)
//-----------------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    std::vector<double> position(3);

    position[0] = g_xray_detector.getDetectorPosition()[0] / unit_of_length;
    position[1] = g_xray_detector.getDetectorPosition()[1] / unit_of_length;
    position[2] = g_xray_detector.getDetectorPosition()[2] / unit_of_length;

    return (position);
}

//----------------------------------------------------
void setDetectorUpVector(double x, double y, double z)
//----------------------------------------------------
{
    g_xray_detector.setUpVector(gVirtualXRay::VEC3(x, y, z).normal());
    g_detector_up_vector_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}

//---------------------------------------
std::vector<double> getDetectorUpVector()
//---------------------------------------
{
    std::vector<double> up_vector(3);

    up_vector[0] = g_xray_detector.getUpVector()[0];
    up_vector[1] = g_xray_detector.getUpVector()[1];
    up_vector[2] = g_xray_detector.getUpVector()[2];

    return (up_vector);
}

std::vector<double> getDetectorRightVector()
{
    std::vector<double> right_vector(3);

    right_vector[0] = g_xray_detector.getRightVector()[0];
    right_vector[1] = g_xray_detector.getRightVector()[1];
    right_vector[2] = g_xray_detector.getRightVector()[2];

    return (right_vector);
}


//-----------------------------------------------------
void setDetectorNumberOfPixels(int aWidth, int aHeight)
//-----------------------------------------------------
{
    g_xray_detector.setNumberOfPixels(gVirtualXRay::Vec2ui(aWidth, aHeight));
    g_detector_size_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    g_renderer_initialised = false;
    initialiseXRayRenderer();
}

//------------------------------------------
std::vector<int> getDetectorNumberOfPixels()
//------------------------------------------
{
    std::vector<int> number_of_pixels(2);

    number_of_pixels[0] = g_xray_detector.getNumberOfPixels()[0];
    number_of_pixels[1] = g_xray_detector.getNumberOfPixels()[1];

    return (number_of_pixels);
}

//---------------------------------------------------------
void setDetectorPixelSize(double aWidth,
                          double aHeight,
                          const std::string& aUnitOfLength)
//---------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    g_xray_detector.setResolutionInUnitsOfLengthPerPixel(
        gVirtualXRay::VEC2(aWidth * unit_of_length,
                           aHeight * unit_of_length));

    g_detector_pixel_size_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}


//-------------------------------------------------------------------
std::vector<double> getDetectorSize(const std::string& aUnitOfLength)
//-------------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    std::vector<double> detector_size(2);

    detector_size[0] =
        g_xray_detector.getSizeInUnitOfLength()[0] / unit_of_length;

    detector_size[1] =
        g_xray_detector.getSizeInUnitOfLength()[1] / unit_of_length;

    return (detector_size);
}


//--------------------------------
void clearDetectorEnergyResponse()
//--------------------------------
{
    g_xray_detector.clearEnergyResponse();
}


//---------------------------------------------------------------
void loadDetectorEnergyResponse(const std::string& aFileName,
                                const std::string& aUnitOfEnergy)
//---------------------------------------------------------------
{
    g_xray_detector.loadEnergyResponse(aFileName, getUnitOfEnergy(aUnitOfEnergy));
}


//-------------------------------------------------
void loadMeshFile(const std::string& aLabel,
                  const std::string& aFileName,
                  const std::string& aUnitOfLength,
				  bool addToRendererAsInnerSurface)
//-------------------------------------------------
{
    // Initialise the X-ray renderer if needed and if possible
    //initialiseXRayRenderer();

    gVirtualXRay::SceneGraphBinder* p_temp = g_p_scene_graph.get();
    if (!p_temp)
        {
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Add a new node to the scenegraph
    g_p_scene_graph->getNode("root")->addChild(aLabel);

    // Set the file name
    g_p_scene_graph->getNode(aLabel)->getPolygonMesh().setFileName(aFileName);

    // Load the file
    g_p_scene_graph->getNode(aLabel)->getPolygonMesh().loadSTLFile(false, true,
    true, true, getUnitOfLength(aUnitOfLength), GL_STATIC_DRAW);

    // Add the geometry to the X-ray renderer
    if (addToRendererAsInnerSurface)
    	g_xray_renderer.addInnerSurface(g_p_scene_graph->getNode(aLabel));
}


//---------------------------------------------------
void loadSceneGraph(const std::string& aFileName,
                    const std::string& aUnitOfLength)
//---------------------------------------------------
{
    bool scene_graph_loaded = false;

#ifdef HAS_ASSIMP
    if (!scene_graph_loaded)
        {
        g_p_scene_graph.reset(new gVirtualXRay::AssimpSceneGraphBinder(
            aFileName,
            getUnitOfLength(aUnitOfLength)));

        scene_graph_loaded = true;
        }
#endif

#ifdef HAS_OPENSCENEGRAPH
    if (!scene_graph_loaded)
        {
        g_p_scene_graph.reset(new gVirtualXRay::OpenSceneGraphBinder(aFileName,
            getUnitOfLength(aUnitOfLength)));

        scene_graph_loaded = true;
        }
#endif

    if (!scene_graph_loaded)
        {
        cerr << "gvxrWarning:\tGraphScene " <<
            aFileName <<
            " not loaded." <<
            endl;
        }
    // Add the geometry to the X-ray renderer
    else
        {
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
            if (p_node->getPolygonMesh().getVertexNumber())
                {
                g_xray_renderer.addInnerSurface(p_node);
                }
            }
        }
}


unsigned int getNumberOfPrimitives(const std::string& aLabel)
{
  // The scenegraph does not exist
  if (!g_p_scene_graph.get())
      {
      return 0;
      }

  // Get the node
  gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

  // The node exists
  if (p_node)
      {
      p_node->getPolygonMesh().getTriangleNumber();
      }
  // The node does not exist
  else
      {
      return 0;
      }
}

void emptyMesh(const std::string& aLabel, const std::string& aParent)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(gVirtualXRay::PolygonMesh());
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(aLabel, gVirtualXRay::PolygonMesh(),
        gVirtualXRay::VEC3(), aParent);
        }
}


void makeCube(const std::string& aLabel,
              double aLength,
              const std::string& aUnitOfLength,
              const std::string& aParent)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(gVirtualXRay::CubeMesh<double>(
            aLength * getUnitOfLength(aUnitOfLength),
            GL_UNSIGNED_INT));
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(
            aLabel,
            gVirtualXRay::CubeMesh<double>(
                aLength * getUnitOfLength(aUnitOfLength),
                GL_UNSIGNED_INT),
            gVirtualXRay::VEC3(),
            aParent);
        }
}


void makeCylinder(const std::string& aLabel,
                  unsigned int aNumberOfSectors,
                  double aHeight,
                  double aRadius,
                  const std::string& aUnitOfLength,
                  const std::string& aParent)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(gVirtualXRay::CylinderMesh<double>(
            aNumberOfSectors, aHeight * getUnitOfLength(aUnitOfLength),
            aRadius * getUnitOfLength(aUnitOfLength),
            GL_UNSIGNED_INT));
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(
            aLabel,
            gVirtualXRay::CylinderMesh<double>(aNumberOfSectors,
                aHeight * getUnitOfLength(aUnitOfLength),
                aRadius * getUnitOfLength(aUnitOfLength),
                GL_UNSIGNED_INT),
            gVirtualXRay::VEC3(),
            aParent);
        }
}


//----------------------------------------------------------
void makeIsoSurface(const std::string& aLabel,
                    short anIsoValue,
                    const std::vector<short>& aVoxelDataSet,
                    unsigned int aWidth,
                    unsigned int aHeight,
                    unsigned int aDepth,
                    double aSpacingX,
                    double aSpacingY,
                    double aSpacingZ,
                    const std::string& aUnitOfLength,
                    const std::string& aParent)
                    //----------------------------------------------------------
{
    // Extract the isosurface
    gVirtualXRay::Image<short> voxel_data_set(
        &aVoxelDataSet[0],
        aWidth, aHeight, aDepth,
        gVirtualXRay::VEC3(aSpacingX, aSpacingY, aSpacingZ)
    );

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(voxel_data_set.marchingCubes(anIsoValue));
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(
            aLabel,
            voxel_data_set.marchingCubes(anIsoValue),
            gVirtualXRay::VEC3(),
            aParent);
        }
}


//-----------------------------------------------------------
void makeTriangularMesh(const std::string& aLabel,
		                const std::vector<float>& aVertexSet,
						const std::string& aUnitOfLength,
						const std::string& aParent)
//-----------------------------------------------------------
{
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(gVirtualXRay::PolygonMesh());
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(
            aLabel,
			gVirtualXRay::PolygonMesh(),
            gVirtualXRay::VEC3(),
            aParent);

        p_node = g_p_scene_graph->getNode(aLabel);
        }

    p_node->getPolygonMesh().setInternalData(GL_TRIANGLES,
    		&aVertexSet,
             false,
			 GL_STATIC_DRAW);

    // Change scale and compute bounding box
    p_node->getPolygonMesh().applyScale(getUnitOfLength(aUnitOfLength));
}


//-------------------------------------------------------------------------
void makeTriangularMesh(const std::string& aLabel,
						const std::vector<float>& aVertexSet,
						const std::vector<int>& aTriangleIndexSet,
						const std::string& aUnitOfLength,
						const std::string& aParent)
//-------------------------------------------------------------------------
{
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(gVirtualXRay::PolygonMesh());
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(
            aLabel,
			gVirtualXRay::PolygonMesh(),
            gVirtualXRay::VEC3(),
            aParent);

        p_node = g_p_scene_graph->getNode(aLabel);
        }

    std::vector<unsigned int> p_temp;

    for (std::vector<int>::const_iterator ite(aTriangleIndexSet.begin());
    		ite != aTriangleIndexSet.end();
    		++ite)
    {
    	if (*ite < 0)
    	{
            throw gVirtualXRay::Exception(__FILE__,
                                          __FUNCTION__,
                                          __LINE__,
                                          "The index is negative.");
    	}

    	p_temp.push_back(*ite);
    }

    p_node->getPolygonMesh().setInternalData(GL_TRIANGLES,
			&aVertexSet,
			&p_temp,
			false,
			GL_STATIC_DRAW);

    // Change scale and compute bounding box
    p_node->getPolygonMesh().applyScale(getUnitOfLength(aUnitOfLength));
}


void addMesh(const std::string& aDestination, const std::string& aSource)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
    {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
    }

    // Find the source. It must exist
    const gVirtualXRay::SceneGraphNode* p_source_node =
        g_p_scene_graph->getNode(aSource);

    // Find the destination. It must exist
    gVirtualXRay::SceneGraphNode* p_destination_node =
        g_p_scene_graph->getNode(aDestination);

    // The source does not exist
    if (!p_source_node)
    {
        cerr <<
            "gvxrWarning:\tPolygonMesh " <<
            aSource <<
            " not found in g_p_polygon_mesh_set." <<
            endl;

        return;
    }

	// The destination does not exist
	if (!p_destination_node)
	{
		cerr <<
			"gvxrWarning:\tPolygonMesh " <<
			aDestination <<
			" not found in g_p_polygon_mesh_set." <<
			endl;

		return;
	}

	// Add the source to the destination
	p_destination_node->getPolygonMesh() += p_source_node->getPolygonMesh();
}


void subtractMesh(const std::string& aDestination, const std::string& aSource) {
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
    {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
    }

    // Find the source. It must exist
    const gVirtualXRay::SceneGraphNode* p_source_node =
        g_p_scene_graph->getNode(aSource);

    // Find the destination. It must exist
    gVirtualXRay::SceneGraphNode* p_destination_node =
        g_p_scene_graph->getNode(aDestination);

    // The source does not exist
    if (!p_source_node)
    {
        cerr <<
            "gvxrWarning:\tPolygonMesh " <<
            aSource <<
            " not found in g_p_polygon_mesh_set." <<
            endl;

        return;
    }

	// The destination does not exist
	if (!p_destination_node)
	{
		cerr <<
			"gvxrWarning:\tPolygonMesh " <<
			aDestination <<
			" not found in g_p_polygon_mesh_set." <<
			endl;

		return;
	}

	// Subtract the source to the destination
	p_destination_node->getPolygonMesh() -= p_source_node->getPolygonMesh();
}


unsigned int getNumberOfChildren(const std::string& aLabel)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    const gVirtualXRay::SceneGraphNode* p_node =
        g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        return p_node->getNumberOfChildren();
        }
    else
        {
        // Show warning message
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }

    return 0;
}

std::string getChildLabel(const std::string& aLabel, unsigned int i)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    const gVirtualXRay::SceneGraphNode* p_node =
        g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        if (p_node->getNumberOfChildren())
            {
            return p_node->getChild(i).getLabel();
            }
        }

    return std::string();
}


//-------------------------------------------------------------------------
std::vector<double> getLocalTransformationMatrix(const std::string& aLabel)
//-------------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    const gVirtualXRay::SceneGraphNode* p_node =
        g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
        {
        // Return its local transformation matrix
        return p_node->getLocalTransformationMatrix().getAsVector();
        }
    // The node does not exist
    else
        {
        // Show warning message
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }

    // Return the identity matrix
    return gVirtualXRay::MATRIX4().getAsVector();
}

//-------------------------------------------------------------------
void setLocalTransformationMatrix(const std::string& aLabel,
    const std::vector<double>& aMatrix)
//-------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
        {
        // Set its local transformation matrix
        p_node->setLocalTransformation(gVirtualXRay::MATRIX4(aMatrix));
        }
    // The node does not exist
    else
        {
        // Show warning message
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}


void applyCurrentLocalTransformation(const std::string& aLabel)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
        {
    	p_node->getPolygonMesh().applyTransform(p_node->getLocalTransformationMatrix());

        // Reset its local transformation matrix
        p_node->setLocalTransformation(gVirtualXRay::MATRIX4());
        }
    // The node does not exist
    else
        {
        // Show warning message
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}


std::vector<double> getNodeAndChildrenBoundingBox(const std::string& aLabel,
    const std::string& aUnitOfLength)
{
    std::vector<double> bbox;

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
        {
        double unit_of_length = getUnitOfLength(aUnitOfLength);

        // Get its bounding box in world transformation
        gVirtualXRay::VEC3 min_corner =
        p_node->getNodeAndChildrenWorldMinCorner() / unit_of_length;
        gVirtualXRay::VEC3 max_corner =
        p_node->getNodeAndChildrenWorldMaxCorner() / unit_of_length;

        bbox.push_back(std::min(min_corner[0], max_corner[0]));
        bbox.push_back(std::min(min_corner[1], max_corner[1]));
        bbox.push_back(std::min(min_corner[2], max_corner[2]));

        bbox.push_back(std::max(min_corner[0], max_corner[0]));
        bbox.push_back(std::max(min_corner[1], max_corner[1]));
        bbox.push_back(std::max(min_corner[2], max_corner[2]));
        }
    // The node does not exist
    else
        {
        // Show warning message
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }

    return (bbox);
}

std::vector<double> getNodeOnlyBoundingBox(const std::string& aLabel,
                                           const std::string& aUnitOfLength)
{
    std::vector<double> bbox;

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
        {
        double unit_of_length = getUnitOfLength(aUnitOfLength);

        // Get its bounding box in world transformation
        gVirtualXRay::VEC3 min_corner =
            p_node->getNodeOnlyWorldMinCorner() / unit_of_length;

        gVirtualXRay::VEC3 max_corner =
            p_node->getNodeOnlyWorldMaxCorner() / unit_of_length;

        bbox.push_back(std::min(min_corner[0], max_corner[0]));
        bbox.push_back(std::min(min_corner[1], max_corner[1]));
        bbox.push_back(std::min(min_corner[2], max_corner[2]));

        bbox.push_back(std::max(min_corner[0], max_corner[0]));
        bbox.push_back(std::max(min_corner[1], max_corner[1]));
        bbox.push_back(std::max(min_corner[2], max_corner[2]));
        }
    // The node does not exist
    else
        {
        // Show warning message
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }

    return (bbox);
}

//-----------------------------------------
void displayNode(const std::string& aLabel)
//-----------------------------------------
{
    std::vector<float> colour(4, 1.0);

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        p_node->display();
        }
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

//-------------------------------------------------
void invertNormalVectors(const std::string& aLabel)
//-------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Invert the normal vectors
        p_node->getPolygonMesh().invertNormalVectors();
        }
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

//-------------------------------------------------------------------------
void setColor(const std::string& aLabel, float R, float G, float B, float A)
//-------------------------------------------------------------------------
{
    setColour(aLabel, R, G, B, A);
}

//---------------------------------------------------------------------------
void setColour(const std::string& aLabel, float R, float G, float B, float A)
//---------------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set the colour
        p_node->getPolygonMesh().getMaterial().setAmbientColour(R, G, B, A);
        p_node->getPolygonMesh().getMaterial().setDiffuseColour(R, G, B, A);
        p_node->getPolygonMesh().getMaterial().setShininess(50.0);
        p_node->getPolygonMesh().getMaterial().setSpecularColour(
            1.0, 1.0, 1.0, 1.0);
        }
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

std::vector<float> getAmbientColour(const std::string& aLabel)
{
    std::vector<float> colour(4, 1.0);

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Get the colour
        colour[0] = p_node->getPolygonMesh().getMaterial().
            getAmbientColour()[0];

        colour[1] = p_node->getPolygonMesh().getMaterial().
            getAmbientColour()[1];

        colour[2] = p_node->getPolygonMesh().getMaterial().
            getAmbientColour()[2];

        colour[3] = p_node->getPolygonMesh().getMaterial().
            getAmbientColour()[3];
        }
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }

    return colour;
}

std::vector<float> getDiffuseColour(const std::string& aLabel)
{
    std::vector<float> colour(4, 1.0);

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Get the colour
        colour[0] = p_node->getPolygonMesh().getMaterial().
            getDiffuseColour()[0];

        colour[1] = p_node->getPolygonMesh().getMaterial().
            getDiffuseColour()[1];

        colour[2] = p_node->getPolygonMesh().getMaterial().
            getDiffuseColour()[2];

        colour[3] = p_node->getPolygonMesh().getMaterial().
            getDiffuseColour()[3];
        }
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }

    return colour;
}

std::vector<float> getSpecularColour(const std::string& aLabel)
{
    std::vector<float> colour(4, 1.0);

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Get the colour
        colour[0] = p_node->getPolygonMesh().getMaterial().
            getSpecularColour()[0];

        colour[1] = p_node->getPolygonMesh().getMaterial().
            getSpecularColour()[1];

        colour[2] = p_node->getPolygonMesh().getMaterial().
            getSpecularColour()[2];

        colour[3] = p_node->getPolygonMesh().getMaterial().
            getSpecularColour()[3];
        }
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }

    return colour;
}


float getShininess(const std::string& aLabel)
{
    float shininess = 0.0;

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Get the colour
        shininess = p_node->getPolygonMesh().getMaterial().getShininess();
        }
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }

    return shininess;
}


//----------------------------------------------------------
void addPolygonMeshAsInnerSurface(const std::string& aLabel)
//----------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Add the polygon mesh to the X-ray renderer
        g_xray_renderer.addInnerSurface(p_node);
        }
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

//---------------------------------------------
void addPolygonMeshAsOuterSurface(const std::string& aLabel)
//---------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Add the polygon mesh to the X-ray renderer
        g_xray_renderer.addOuterSurface(p_node);
        }
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

//--------------------------------------
void removePolygonMeshesFromSceneGraph()
//--------------------------------------
{
    // The scenegraph exists
    if (g_p_scene_graph.get())
        {
        // Empty it
        g_p_scene_graph->clear();
        }

    removePolygonMeshesFromXRayRenderer();
}

//----------------------------------------
void removePolygonMeshesFromXRayRenderer()
//----------------------------------------
{
    g_xray_renderer.removeInnerSurfaces();
    g_xray_renderer.removeOuterSurface();
}

//-----------------
void moveToCentre()
//-----------------
{
    if (g_p_scene_graph.get())
        {
        g_p_scene_graph->moveToCentre();
        }
    else
        {
        cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
        }
}

//-----------------
void moveToCenter()
//-----------------
{
    moveToCentre();
}

//-----------------------------------
void moveToCentre(const std::string& aLabel)
//-----------------------------------
{
    if (g_p_scene_graph.get())
        {
        g_p_scene_graph->moveToCentre(aLabel);
        }
    else
        {
        cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
        }
}

//-----------------------------------
void moveToCenter(const std::string& aLabel)
//-----------------------------------
{
    moveToCentre(aLabel);
}

void scaleNode(const std::string& aLabel,
               double x,
               double y,
               double z)
{
    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            gVirtualXRay::MATRIX4 scaling_matrix =
            gVirtualXRay::MATRIX4::buildScaleMatrix(x, y, z);

            // Root node
            if (p_node->getLabel() == "root")
                {
                p_node->setLocalTransformation(
                scaling_matrix
                * p_node->getLocalTransformationMatrix());
                }
            else
                {
                p_node->setLocalTransformation(
                p_node->getLocalTransformationMatrix()
                * scaling_matrix);
                }
            }
        // It has not been found
        else
            {
            cerr << "gvxrWarning:\tPolygonMesh " << aLabel
            << " not found in g_p_polygon_mesh_set." << endl;
            }
        }
    else
        {
        cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
        }
}


void rotateNode(const std::string& aLabel,
                double angle,
                double x,
                double y,
                double z)
{
    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            gVirtualXRay::MATRIX4 rotation_matrix =
            gVirtualXRay::MATRIX4::buildRotationMatrix(angle, x, y, z);

            // Root node
            if (p_node->getLabel() == "root")
                {
                p_node->setLocalTransformation(
                rotation_matrix
                * p_node->getLocalTransformationMatrix());
                }
            else
                {
                p_node->setLocalTransformation(
                    p_node->getLocalTransformationMatrix() * rotation_matrix);
                }
            }
        // It has not been found
        else
            {
            cerr << "gvxrWarning:\tPolygonMesh " <<
                aLabel <<
                " not found in g_p_polygon_mesh_set." <<
                endl;
            }
        }
    else
        {
        cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
        }
}


void translateNode(const std::string& aLabel,
                   double x,
                   double y,
                   double z,
                   const std::string& aUnitOfLength)
{
    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            double unit_of_length = getUnitOfLength(aUnitOfLength);
            gVirtualXRay::MATRIX4 translation_matrix =
            gVirtualXRay::MATRIX4::buildTranslationMatrix(
            x * unit_of_length, y * unit_of_length,
            z * unit_of_length);

            // Root node
            if (p_node->getLabel() == "root")
                {
                p_node->setLocalTransformation(
                translation_matrix
                * p_node->getLocalTransformationMatrix());
                }
            else
                {
                p_node->setLocalTransformation(
                p_node->getLocalTransformationMatrix()
                * translation_matrix);
                }
            }
        // It has not been found
        else
            {
            cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
            }
        }
    else
        {
        cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
        }
}


void shearNode(const std::string& aLabel,
               double yx,
               double zx,
               double xy,
               double zy,
               double xz,
               double yz)
{
    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            gVirtualXRay::MATRIX4 shear_matrix(
                     1, yx, zx, 0,
					xy,  1, zy, 0,
					xz, yz,  1, 0,
					 0,  0,  0, 1);

            // Root node
            if (p_node->getLabel() == "root")
                {
                p_node->setLocalTransformation(
                        shear_matrix *
                        p_node->getLocalTransformationMatrix());
                }
            else
                {
                p_node->setLocalTransformation(
                        p_node->getLocalTransformationMatrix() *
                        shear_matrix);
                }
            }
        // It has not been found
        else
            {
            cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
            }
        }
    else
        {
        cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
        }
}


void resetSceneTransformation()
{
    g_sample_rotation_matrix = gVirtualXRay::MATRIX4();
}


void scaleScene(double x,
                double y,
                double z)
{
    g_sample_rotation_matrix *= gVirtualXRay::MATRIX4::buildScaleMatrix(
        x, y, z);
}


void scaleScene(double x,
                double y,
                double z,
                const std::string& aUnitOfLength)
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);
    scaleScene(x * unit_of_length,
               y * unit_of_length,
               z * unit_of_length);
}


void rotateScene(double angle, double x, double y, double z)
{
    // Move the scenegraph
    g_sample_rotation_matrix *= gVirtualXRay::MATRIX4::buildRotationMatrix(
        angle, x, y, z);
}


void translateScene(double x,
                    double y,
                    double z,
                    const std::string& aUnitOfLength)
{
    // Move the scenegraph
    double unit_of_length = getUnitOfLength(aUnitOfLength);
    g_sample_rotation_matrix *= gVirtualXRay::MATRIX4::buildTranslationMatrix(
        x * unit_of_length,
        y * unit_of_length,
        z * unit_of_length);
}


std::vector<std::vector<float> > getSceneTransformationMatrix()
{
    return gvxrMatrix2Vector(g_sample_rotation_matrix);
}


std::vector<std::vector<float> > getRootTransformationMatrix()
{
    if (!g_p_scene_graph.get())
        {
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "The scenegraph is empty.");
        }

    return (gvxrMatrix2Vector(
        g_p_scene_graph->getRootNodeTransformationMatrix()));
}


std::vector<std::vector<float> > getNodeLocalTransformationMatrix(
        const std::string& aLabel)
{
    gVirtualXRay::MATRIX4 transformation;

    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            transformation = p_node->getLocalTransformationMatrix();
            }
        // It has not been found
        else
            {
            cerr << "gvxrWarning:\tPolygonMesh " <<
                aLabel <<
                " not found in g_p_polygon_mesh_set." <<
                endl;
            }
        }
    else
        {
        cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
        }

    return (gvxrMatrix2Vector(transformation));
}


std::vector<std::vector<float> > getNodeWorldTransformationMatrix(
        const std::string& aLabel)
{
    gVirtualXRay::MATRIX4 transformation;

    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            transformation = p_node->getWorldTransformationMatrix();
            }
        // It has not been found
        else
            {
            cerr << "gvxrWarning:\tPolygonMesh " <<
                aLabel <<
                " not found in g_p_polygon_mesh_set." <<
                endl;
            }
        }
    else
        {
        cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
        }

    return (gvxrMatrix2Vector(transformation));
}


void setSceneTransformationMatrix(
        const std::vector<std::vector<float> >& aMatrix)
{
    g_sample_rotation_matrix = Vector2gvxrMatrix(aMatrix);
}

void setRootTransformationMatrix(
        const std::vector<std::vector<float> >& aMatrix)
{
    if (g_p_scene_graph.get())
        {
        g_p_scene_graph->setRootNodeTransformationMatrix(
        Vector2gvxrMatrix(aMatrix));
        }
    else
        {
        cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
        }
}

void setNodeTransformationMatrix(const std::string& aLabel,
        const std::vector<std::vector<float> >& aMatrix)
{
    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            p_node->setLocalTransformation(Vector2gvxrMatrix(aMatrix));
            }
        // It has not been found
        else
            {
            cerr << "gvxrWarning:\tPolygonMesh " <<
                aLabel <<
                " not found in g_p_polygon_mesh_set." <<
                endl;
            }
        }
    else
        {
        cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
        }
}

//---------------------------------------------
void setHU(const std::string& aLabel, short HU)
//---------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setHounsfieldValue(HU);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

//---------------------------------------------------------
void setHounsfieldUnit(const std::string& aLabel, short HU)
//---------------------------------------------------------
{
    setHU(aLabel, HU);
}

//----------------------------------------------------------
void setHounsfieldValue(const std::string& aLabel, short HU)
//----------------------------------------------------------
{
    setHU(aLabel, HU);
}

//----------------------------------------------------------
void setElement(const std::string& aLabel, unsigned short Z)
//----------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setElement(Z);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

//------------------------------------------------------------------
void setElement(const std::string& aLabel, const std::string& aName)
//------------------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setElement(aName);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

//------------------------------------------------------------------
void setMixture(const std::string& aLabel, const std::string& aName)
//------------------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setMixture(aName);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}


//----------------------------------------------------
void setMixture(const std::string& aLabel,
                const std::vector<int>& aZNumberSet,
                const std::vector<double>& aWeightSet)
//----------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        std::map<int, double> mixture;

        if (aZNumberSet.size() != aWeightSet.size())
            {
            throw gVirtualXRay::Exception(
                __FILE__,
                __FUNCTION__,
                __LINE__,
                "ERROR: The Z number set and weight set don't have the same size.");
            }

        for (unsigned int i = 0; i != aZNumberSet.size(); ++i)
            {
            mixture[aZNumberSet[i]] = aWeightSet[i];
            }

        p_node->getPolygonMesh().setMixture(mixture);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

//-------------------------------------------------------------------
void setCompound(const std::string& aLabel, const std::string& aName)
//-------------------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setCompound(aName);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

//-----------------------------------------------------------
void setMassAttenuationCoefficient(const std::string& aLabel,
                                   double aCoefficient,
                                   const std::string& aUnit)
//-----------------------------------------------------------
{
    if (aUnit != "cm2/g" && aUnit != "cm2.g-1")
        {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "ERROR: Unknown unit of mass attenuation coefficient.");
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setMassAttenuationCoefficient(
        aCoefficient * gVirtualXRay::cm2 / gVirtualXRay::g);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

//-------------------------------------------------------------
void setLinearAttenuationCoefficient(const std::string& aLabel,
                                     double aCoefficient,
                                     const std::string& aUnit)
//-------------------------------------------------------------
{
    if (aUnit != "cm-1")
        {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "ERROR: Unknown unit of linear attenuation coefficient.");
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setLinearAttenuationCoefficient(
        aCoefficient / gVirtualXRay::cm);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

//-----------------------------------
void setMu(const std::string& aLabel,
           double aCoefficient,
           const std::string& aUnit)
//-----------------------------------
{
    setLinearAttenuationCoefficient(aLabel, aCoefficient, aUnit);
}

//----------------------------------------
void setDensity(const std::string& aLabel,
                double aDensity,
                const std::string& aUnit)
//----------------------------------------
{
    if (aUnit != "g/cm3" && aUnit != "g.cm-3")
    {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "ERROR: Unknown unit of linear attenuation coefficient.");
    }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setDensity(
        aDensity * gVirtualXRay::g / gVirtualXRay::cm3);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

//------------------------------------------
double getDensity(const std::string& aLabel)
//------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        return p_node->getPolygonMesh().getDensity()
        / (gVirtualXRay::g / gVirtualXRay::cm3);
        }
    // It has not been found
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}


//--------------------------------------------------------------------
double getMassAttenuationCoefficient(const std::string& aLabel,
                                     double anEnergy,
                                     const std::string& aUnitOfEnergy)
//--------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        return (p_node->getPolygonMesh().getPhotonCrossSection().
                getMassAttenuationCoefficient(anEnergy *
                        getUnitOfEnergy(aUnitOfEnergy)) /
                (gVirtualXRay::cm2 / gVirtualXRay::g));
        }
    // It has not been found
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}


//----------------------------------------------------------------------
double getLinearAttenuationCoefficient(const std::string& aLabel,
                                       double anEnergy,
                                       const std::string& aUnitOfEnergy)
//----------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        return (p_node->getPolygonMesh().getPhotonCrossSection().
                getLinearAttenuationCoefficient(anEnergy *
                    getUnitOfEnergy(aUnitOfEnergy)) /
                (1.0 / gVirtualXRay::cm));
        }
    // It has not been found
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }
}

//----------------------------------------------
std::string getMaterialLabel(const std::string& aLabel)
//----------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        return p_node->getPolygonMesh().getMaterialLabel();
        }
    // It has not been found
    else
        {
        cerr << "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
        }

    return std::string();
}

//-------------------------------------------------
void createOpenGLContext(int aWindowID,
		                 int aRendererMajorVersion,
		                 int aRendererMinorVersion)
//-------------------------------------------------
{
    createWindow(aWindowID,
        false,
        "OpenGL",
        aRendererMajorVersion,
        aRendererMinorVersion);
        cout << "Created EGL Window" << endl;
}

//---------------------------------------------
void createWindow(int aWindowID,
		          int aVisibilityFlag,
				  const std::string& aRenderer,
	              int aRendererMajorVersion,
	              int aRendererMinorVersion)
//---------------------------------------------
{
    if (!g_p_window_set.size())
        {
        // Make sure all the windows are destroyed
        atexit(quit);

        if (aRenderer != "EGL")
        	{
			// Set an error callback
			glfwSetErrorCallback(errorCallback);

			// Initialize GLFW
			if (!glfwInit())
				{
				throw gVirtualXRay::Exception(
					__FILE__,
					__FUNCTION__,
					__LINE__,
					"ERROR: cannot initialise GLFW.");
				}
			}
        }

    if (aWindowID == -1)
        {
        aWindowID = g_p_window_set.size();
        }

    cout << "gvxrStatus:\tCreate window " << aWindowID << endl;

    // The window does not exist
    if (!g_p_window_set[aWindowID])
        {
    	SimpleGVXR::Window* p_window = 0;

    	// Create the window
    	std::string renderer = aRenderer;
    	for (auto& c: renderer) c = std::toupper(c);

    	if (aRenderer == "OPENGL")
    	{
    		p_window = new SimpleGVXR::OpenGLWindow(aVisibilityFlag, aRendererMajorVersion, aRendererMinorVersion);
    	}
    	else if (aRenderer == "VULKAN")
        {
        	p_window = new SimpleGVXR::VulkanWindow(aVisibilityFlag);
        }
#ifdef HAS_EGL
    	else if (aRenderer == "EGL")
        {
        	p_window = new SimpleGVXR::EGLWindow();
        }
#endif // HAS_EGL
        else
    	{
    		std::string error_message = "ERROR: Unknown renderer (";
    		error_message += aRenderer;
    		error_message += ")";

    		throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    				error_message);
    	}

        // The window has been created
        if (p_window)
            {
            // Add the window
            g_p_window_set[aWindowID] = p_window;

            // Register its ID
            g_p_window_id_stack.push_back(aWindowID);

            // Initialise the X-ray renderer if needed and if possible
            initialiseXRayRenderer();
            }
        // The window has not been created
        else
            {
            // Display a warning
            cerr << "gvxrWarning:\tWindow " <<
                aWindowID <<
                " cannot be created." <<
                endl;

            // Delete the empty record that may have been created
            // in g_p_window_set
            std::map<int, SimpleGVXR::Window*>::iterator it;
            it = g_p_window_set.find(aWindowID);
            if (it != g_p_window_set.end())
                {
                g_p_window_set.erase(it);
                }
            }
        }
    // The window already exist
    // Do nothing
    else
        {
        cerr << "gvxrWarning:\tWindow " <<
            aWindowID <<
            " already exists." <<
            endl;
        }
}

//--------------------------------------------------------
void setWindowSize(int aWidth, int aHeight, int aWindowID)
//--------------------------------------------------------
{
    // Resize the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            cerr << "gvxrWarning:\tNo active window to resize." << endl;
            }
        // Resize the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to resize
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                search->second->setWindowSize(aWidth, aHeight);
                }
            }
        else
            {
            cerr << "gvxrWarning:\tWindow " <<
                aWindowID <<
                " not found." <<
                endl;
            }
        }
}

//-------------------------------------------------------
void displayScene(bool aSceneRotationFlag, int aWindowID)
//-------------------------------------------------------
{
    // Display in the last window that has been created
    if (aWindowID == -1 && !g_has_own_gl_context)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            cerr << "gvxrWarning:\tNo active window to use." << endl;
            }
        // Destroy the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to use
    if (aWindowID != -1 || g_has_own_gl_context)
        {
        // Find the window
        auto search_window = g_p_window_set.find(aWindowID);

        // It has been found
        if (search_window != g_p_window_set.end() || g_has_own_gl_context)
            {
            if (search_window->second || g_has_own_gl_context)
                {
                search_window->second->displayScene(aSceneRotationFlag);
                }

            // Poll for and process events
            if (!g_has_own_gl_context)
                {
                glfwPollEvents();
                }
            }
        else
            {
            cerr << "gvxrWarning:\tWindow " <<
                aWindowID <<
                " not found in g_p_window_set." <<
                endl;
            }
        }
}

//----------------------------
void renderLoop(int aWindowID)
//----------------------------
{
    // Display in the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            cerr << "gvxrWarning:\tNo active window to destroy." << endl;
            }
        // Destroy the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to use
    if (aWindowID != -1)
        {
        // Find the window
        auto search_window = g_p_window_set.find(aWindowID);

        // It has been found
        if (search_window != g_p_window_set.end())
            {
            if (search_window->second)
                {
                search_window->second->renderLoop();
                }
            }
        else
            {
            cerr << "gvxrWarning:\tWindow " <<
                aWindowID <<
                " not found in g_p_window_set." <<
                endl;
            }
        }
}

//----------------------------
void showWindow(int aWindowID)
//----------------------------
{
    // Resize the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            cerr << "gvxrWarning:\tNo active window to show." << endl;
            }
        // Resize the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to resize
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                search->second->showWindow();
                }
            }
        else
            {
            cerr << "gvxrWarning:\tWindow " << aWindowID <<
                " not found." << endl;
            }
        }
}

//----------------------------
void hideWindow(int aWindowID)
//----------------------------
{
    // Resize the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            cerr << "gvxrWarning:\tNo active window to hide." << endl;
            }
        // Resize the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to resize
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                search->second->hideWindow();
                }
            }
        else
            {
            cerr << "gvxrWarning:\tWindow " << aWindowID <<
                " not found." << endl;
            }
        }
}


//----------------------------------------------------------------------
void setWindowBackGroundColour(float R, float G, float B, int aWindowID)
//----------------------------------------------------------------------
{
    // Change the colour of the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            cerr << "gvxrWarning:\tNo active window to hide." << endl;
            }
        // Change the colour of the last window that has been created
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to change
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                search->second->setWindowBackGroundColour(R, G, B);
                }
            }
        else
            {
            cerr << "gvxrWarning:\tWindow " << aWindowID <<
                " not found." << endl;
            }
        }
}

//---------------------------------------------------------------------
void setWindowBackGroundColor(float R, float G, float B, int aWindowID)
//---------------------------------------------------------------------
{
    setWindowBackGroundColour(R, G, B, aWindowID);
}


//----------------------------------------------------------------------------------
const std::vector<std::vector<std::vector< float> > >& takeScreenshot(int aWindowID)
//----------------------------------------------------------------------------------
{
    // Retrieve the window
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            cerr << "gvxrWarning:\tNo active window to hide." << endl;
            }
        // Change the colour of the last window that has been created
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to capture
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                search->second->takeScreenshot();
                }
            }
        else
            {
            cerr << "gvxrWarning:\tWindow " << aWindowID <<
                " not found." << endl;
            }
        }

    // Return the image
    return lastest_screenshot;
}


//--------------------------------------------------------------------------
const std::vector<std::vector<std::vector< float> > >& getLatestScreenshot()
//--------------------------------------------------------------------------
{
    return lastest_screenshot;
}

//-------------------------------
void destroyWindow(int aWindowID)
//-------------------------------
{
    // Destroy the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            cerr << "gvxrWarning:\tNo active window to destroy." << endl;
            }
        // Destroy the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to destroy
    if (aWindowID != -1)
        {
        // Find the window
        auto search_window = g_p_window_set.find(aWindowID);

        // It has been found
        if (search_window != g_p_window_set.end())
            {
            cout << "gvxrStatus:\tDestroy window " << aWindowID <<
                "(" << search_window->second << ")" << endl;

            if (search_window->second) search_window->second->destroyWindow();

            g_p_window_set.erase(search_window);
            }
        else
            {
            cerr << "gvxrWarning:\tWindow " << aWindowID <<
                " not found in g_p_window_set." << endl;
            }

        // Unregister its ID from the stack of IDs
        // Find the ID
        std::vector<int>::iterator search_id = find(
            g_p_window_id_stack.begin(),
            g_p_window_id_stack.end(),
            aWindowID);

        // It has been found
        if (search_id != g_p_window_id_stack.end())
            {
            g_p_window_id_stack.erase(search_id);
            }
        else
            {
            cerr << "gvxrWarning:\tWindow " << aWindowID <<
                " not found in g_p_window_id_stack." << endl;
            }
        }
}


//----------------------
void destroyAllWindows()
//----------------------
{
    cout << "gvxrStatus:\tDestroy all the windows" << endl;

    // Process all the windows
    while (g_p_window_set.size())
        {
        // Destroy the window if possible
        destroyWindow(g_p_window_set.begin()->first);
        }
}


//-------------------
void usePointSource()
//-------------------
{
    g_xray_detector.setXrayPointSource();
    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}

//--------------------
void useParallelBeam()
//--------------------
{
    // Use a parallel source
    g_xray_detector.setParallelBeam();
    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}

//----------------------
void useParallelSource()
//----------------------
{
    useParallelBeam();
}

//----------------------
void resetBeamSpectrum()
//----------------------
{
    g_xray_beam.clear();
    g_beam_energy_initialised = false;
}

//-----------------------------------------------------
void setMonoChromatic(double anEnergy,
                      const std::string& aUnitOfEnergy,
                      double aNumberOfPhotons)
//-----------------------------------------------------
{
    g_xray_beam.clear();
    addEnergyBinToSpectrum(anEnergy, aUnitOfEnergy, aNumberOfPhotons);
}

//----------------------------------------------------
void addEnergyBinToSpectrum(double anEnergy, const std::string& aUnitOfEnergy,
    double aNumberOfPhotons)
//----------------------------------------------------
{
    g_xray_beam.addChannel(aNumberOfPhotons,
    anEnergy * getUnitOfEnergy(aUnitOfEnergy));
    g_beam_energy_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}

//--------------------------------------------------------
void loadSpectrumFromTSV(const std::string& aFileName,
    const std::string& aUnitOfEnergy, bool aNormalisationFlag)
//--------------------------------------------------------
{
    g_xray_beam.clear();

    g_xray_beam.initialise(aFileName.c_str(), getUnitOfEnergy(aUnitOfEnergy));

    if (aNormalisationFlag) {
    g_xray_beam.normalise();
    }

    g_beam_energy_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer();
}

//-----------------------------------------------------------------
std::vector<double> getEnergyBins(const std::string& aUnitOfEnergy)
//-----------------------------------------------------------------
{
    std::vector<double> p_temp;

    for (unsigned int i = 0; i < g_xray_beam.getEnergyChannelNumber(); ++i) {
    p_temp.push_back(
    g_xray_beam.getEnergyChannel(i).getPhotonEnergy()
    / getUnitOfEnergy(aUnitOfEnergy));
    }

    return (p_temp);
}


//--------------------------------------------
void saveSTLfile(const std::string& aLabel,
                 const std::string& aFileName)
//--------------------------------------------
{
	// Create the file name
	std::string file_name;
	if (aFileName.size() == 0)
	{
		file_name = aLabel + ".stl";
	}
	else
	{
		file_name = aFileName;
	}

	// Get the scenegraph
    gVirtualXRay::SceneGraphBinder* p_temp = g_p_scene_graph.get();
    if (!p_temp)
        {
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Get the node
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
        {
        // Set the file name
    	p_node->getPolygonMesh().setFileName(file_name);

        // save the file
    	p_node->getPolygonMesh().writeSTLFile();
        }
    // The node does not exist
    else
        {
		cerr <<
			"gvxrWarning:\tPolygonMesh " <<
			aLabel <<
			" not found in g_p_polygon_mesh_set." <<
			endl;
        }
}

//--------------------------------------------
std::vector<double> getPhotonCountEnergyBins()
//--------------------------------------------
{
    std::vector<double> p_temp;

    for (unsigned int i = 0; i < g_xray_beam.getEnergyChannelNumber(); ++i) {
    p_temp.push_back(g_xray_beam.getEnergyChannel(i).getPhotonNumber());
    }

    return (p_temp);
}


//-----------------------------------------
double getTotalEnergyWithDetectorResponse()
//-----------------------------------------
{
    return g_xray_renderer.getTotalEnergyWithDetectorResponse();
}


//--------------------------------------------------
void saveLastXRayImage(const std::string& aFileName,
                       bool useCompression)
//--------------------------------------------------
{
    static int file_counter = 0;
    string file_name;

    if (aFileName.size())
    file_name = aFileName;
    else
    file_name = "xray_image-" + to_string(file_counter++) + ".mha";

    g_xray_renderer.getEnergyFluence().save(
        file_name, false, "X-ray projection", useCompression);
}

//------------------------------------------------
void saveLastLBuffer(const std::string& aFileName,
                     bool useCompression)
//------------------------------------------------
{
    static int file_counter = 0;
    string file_name;

    if (aFileName.size())
    file_name = aFileName;
    else
    file_name = "lbuffer-" + to_string(file_counter++) + ".mha";

    g_xray_renderer.getLBuffer().save(
        file_name, false, "L-buffer", useCompression);
}


//---------------------------------------------------------
void saveLastCumulatedLBuffer(const std::string& aFileName,
                              bool useCompression)
//---------------------------------------------------------
{
    static int file_counter = 0;
    string file_name;

    if (aFileName.size())
    file_name = aFileName;
    else
    file_name = "cumulated_lbuffer-" + to_string(file_counter++) + ".mha";

    g_xray_renderer.getCumulatedLBuffer().save(
        file_name, false, "Cumulated L-buffer", useCompression);
}


//-------------------------------------------------
void saveLastSinogram(const std::string& aFileName,
                      bool useCompression)
//-------------------------------------------------
{
    static int file_counter = 0;
    string file_name;

    if (aFileName.size())
        file_name = aFileName;
    else
        file_name = "sinogram-" + to_string(file_counter++) + ".mha";

    g_xray_renderer.getSinogram().save(
        file_name, false, "Sinogram", useCompression);
}


//-----------------------------------------------------
void saveLastProjectionSet(const std::string& aFileName,
                           bool useCompression)
//-----------------------------------------------------
{
    static int file_counter = 0;
    string file_name;

    if (aFileName.size())
        file_name = aFileName;
    else
        file_name = "projection_set-" + to_string(file_counter++) + ".mha";

    g_xray_renderer.getProjectionSet().save(
        file_name, false, "ProjectionSet", useCompression);
}


//-------------------------------------
void enableArtefactFilteringOnGPU(void)
//-------------------------------------
{
    g_xray_renderer.enableArtifactFilteringOnGPU();
}

//-------------------------------------
void enableArtifactFilteringOnGPU(void)
//-------------------------------------
{
    enableArtefactFilteringOnGPU();
}

//-------------------------------------
void enableArtefactFilteringOnCPU(void)
//-------------------------------------
{
    g_xray_renderer.enableArtifactFilteringOnCPU();
}

//-------------------------------------
void enableArtifactFilteringOnCPU(void)
//-------------------------------------
{
    enableArtefactFilteringOnCPU();
}

//---------------------------------
void disableArtefactFiltering(void)
//---------------------------------
{
    g_xray_renderer.disableArtifactFilteringOnGPU();
    g_xray_renderer.disableArtifactFilteringOnCPU();
}

//---------------------------------
void disableArtifactFiltering(void)
//---------------------------------
{
    disableArtefactFiltering();
}

//---------------------------------------------------------------------------
std::vector<std::vector<float> > computeXRayImage(bool anIntegrateEnergyFlag)
//---------------------------------------------------------------------------
{
    // There is no OpenGL context
    if (!g_has_own_gl_context
    && (g_p_window_id_stack.empty() || g_p_window_set.empty())) {
    // Create a window
    createWindow();
    }

    // The renderer is ready
    if (g_xray_renderer.isReady()) {
    // Compute the X-ray image
    g_xray_renderer.computeImage(g_sample_rotation_matrix, anIntegrateEnergyFlag);
    }
    // The renderer is not ready
    else {
    cerr << "gvxrWarning:\tX-ray renderer not ready." << endl;
    }

    return (getVectorImage2D(g_xray_renderer.getEnergyFluence()));
}


//--------------------------------------------
std::vector<std::vector<float> > computeLBuffer(const std::string& aLabel)
//--------------------------------------------
{
    // There is no OpenGL context
    if (!g_has_own_gl_context
    && (g_p_window_id_stack.empty() || g_p_window_set.empty())) {
    // Create a window
    createWindow();
    }

    // The renderer is ready
    if (g_xray_renderer.isReady()) {
    if (g_p_scene_graph.get()) {
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(
    aLabel);

    // It has been found
    if (p_node) {
    // Compute the L-buffer
    g_xray_renderer.computeLBuffer(*p_node, gVirtualXRay::VEC3(),
    g_sample_rotation_matrix, true);
    }
    // It has not been found
    else {
    cerr << "gvxrWarning:\tPolygonMesh " << aLabel
    << " not found in g_p_polygon_mesh_set." << endl;
    }
    } else {
    cerr << "gvxrWarning:\tThe scenegraph is empty." << endl;
    }
    }
    // The renderer is not ready
    else {
    cerr << "gvxrWarning:\tX-ray renderer not ready." << endl;
    }

    // Compute its L-buffer
    return (getVectorImage2D(g_xray_renderer.getLBuffer()));
}


std::vector<std::vector<std::vector<float> > > computeSinogram(double x, double y, double z,
    const std::string& aUnitOfLength, unsigned int aNumberOfAngle,
    double anAngleOffset) {
    // There is no OpenGL context
    if (!g_has_own_gl_context
    && (g_p_window_id_stack.empty() || g_p_window_set.empty())) {
    // Create a window
    createWindow();
    }

    // The renderer is ready
    if (g_xray_renderer.isReady()) {
    // Compute the X-ray image

    double unit_of_length = getUnitOfLength(aUnitOfLength);
    gVirtualXRay::VEC3 rotation_centre(x * unit_of_length,
    y * unit_of_length, z * unit_of_length);

    g_xray_renderer.computeSinogram(g_sample_rotation_matrix,
    rotation_centre, aNumberOfAngle, anAngleOffset);
    }
    // The renderer is not ready
    else {
    cerr << "gvxrWarning:\tX-ray renderer not ready." << endl;
    }

    return (getVectorImage3D(g_xray_renderer.getSinogram()));
}


std::vector<std::vector<std::vector<float> > > computeProjectionSet(double x, double y, double z,
    const std::string& aUnitOfLength, unsigned int aNumberOfAngle,
    double anAngleOffset) {
    // There is no OpenGL context
    if (!g_has_own_gl_context
    && (g_p_window_id_stack.empty() || g_p_window_set.empty())) {
    // Create a window
    createWindow();
    }

    // The renderer is ready
    if (g_xray_renderer.isReady()) {
    // Compute the X-ray image

    double unit_of_length = getUnitOfLength(aUnitOfLength);
    gVirtualXRay::VEC3 rotation_centre(x * unit_of_length,
    y * unit_of_length, z * unit_of_length);

    g_xray_renderer.computeProjectionSet(g_sample_rotation_matrix,
    rotation_centre, aNumberOfAngle, anAngleOffset);
    }
    // The renderer is not ready
    else {
    cerr << "gvxrWarning:\tX-ray renderer not ready." << endl;
    }

    return (getVectorImage3D(g_xray_renderer.getProjectionSet()));
}


std::vector<std::vector<float> > getLastXRayImage() {
    return (getVectorImage2D(g_xray_renderer.getEnergyFluence()));
}

std::vector<std::vector<float> > getLastLBuffer() {
    return (getVectorImage2D(g_xray_renderer.getLBuffer()));
}

std::vector<std::vector<std::vector<float> > >  getLastSinogram() {
    return (getVectorImage3D(g_xray_renderer.getSinogram()));
}


std::vector<std::vector<std::vector<float> > > getLastProjectionSet() {
    return (getVectorImage3D(g_xray_renderer.getProjectionSet()));
}


int getImageWidth(const std::vector<std::vector<float> >& anImage) {
    int image_width = 0;

    if (getImageHeght(anImage)) {
    image_width = anImage[0].size();
    }

    return (image_width);
}

int getImageHeght(const std::vector<std::vector<float> >& anImage) {
    return (anImage.size());
}

std::vector<float> getImageRow(const std::vector<std::vector<float> >& anImage,
    int j) {
    if (j >= getImageHeght(anImage)) {
    throw gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__,
    __LINE__);
    }

    return (anImage[j]);
}



void startArcBallRotation(double x, double y) {
    g_last_x_position = x;
    g_last_y_position = y;

    g_current_x_position = x;
    g_current_y_position = y;

    g_use_arc_ball = true;
}

void stopArcBallRotation() {
    g_use_arc_ball = false;
}

bool usingArcBallRotation() {
    return (g_use_arc_ball);
}

void cursorPositionCallback(double x, double y, int aViewportWidth,
    int aViewportHeight) {
    g_current_x_position = x;
    g_current_y_position = y;

    computeRotation(g_scene_rotation_matrix, aViewportWidth, aViewportHeight);
}

void scrollCallback(double xoffset, double yoffset) {
    // Scrolling along the Y-axis
    if (fabs(yoffset) > EPSILON) {
    g_use_arc_ball = false;
    g_zoom += yoffset * gVirtualXRay::cm;

    gVirtualXRay::loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
                                            0.0, 0.0, 0.0,
                                            0.0, 1.0, 0.0);
    }
}


void setZoom(float aZoomValue)
{
    g_zoom = aZoomValue;

    gVirtualXRay::loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
                                            0.0, 0.0, 0.0,
                                            0.0, 1.0, 0.0);
}


float getZoom()
{
    return g_zoom;
}


void setSceneRotationMatrix(const std::vector<double>& aRotationMatrix)
{
    g_scene_rotation_matrix = gVirtualXRay::Matrix4x4<GLfloat>(aRotationMatrix);
}

std::vector<double> getSceneRotationMatrix()
{
    return g_scene_rotation_matrix.getAsVector();
}

//--------------------------------------------
void mouseButtonCallback(GLFWwindow* apWindow, int aButton, int aButtonState,
    int aModifierKey)
//--------------------------------------------
{
    g_button = aButton;
    g_button_state = aButtonState;

    // Use the arc ball
    if (g_button_state == GLFW_PRESS) {
    double xpos(0);
    double ypos(0);
    glfwGetCursorPos(apWindow, &xpos, &ypos);

    startArcBallRotation(xpos, ypos);
    }
    // Stop using the arc ball
    else {
    stopArcBallRotation();
    }
}

//--------------------------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
//--------------------------------------------------------------
{
    int viewport_width  = 0;
    int viewport_height = 0;

    glfwGetWindowSize(apWindow, &viewport_width, &viewport_height);

    // Compute the rotation
    cursorPositionCallback(x, y, viewport_width, viewport_height);
}

//-----------------------------------------------------------------------
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
//-----------------------------------------------------------------------
{
    scrollCallback(xoffset, yoffset);
}

void pushModelViewMatrix() {
    // Store the current transformation matrix
    gVirtualXRay::pushModelViewMatrix();
}

void popModelViewMatrix() {
    // Store the current transformation matrix
    gVirtualXRay::popModelViewMatrix();
}

//----------------------------------------------------------------
void rotateModelView(double anAngle, double x, double y, double z)
//----------------------------------------------------------------
{
    gVirtualXRay::Matrix4x4 < GLfloat > rotation_matrix;
    rotation_matrix.rotate(anAngle, gVirtualXRay::Vec3 < GLfloat > (x, y, z));

    gVirtualXRay::g_current_modelview_matrix *= rotation_matrix;
}


//---------
void quit()
//---------
{
    cout << "gvxrStatus:\tQuit and destroy all the windows" << endl;
    destroyAllWindows();
}

//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
    // Display the error in the terminal
    std::cerr << "GLFW error: " << description << std::endl;
}


//------------------------------
void orthographic(double left,
                  double right,
                  double bottom,
                  double top,
                  double nearZ,
                  double farZ)
//------------------------------
{
    gVirtualXRay::loadOrthoProjectionMatrix(
        left, right,
        bottom, top,
        nearZ, farZ);
}

//------------------------------------------
void perspective(double initial_fovy,
                 double screen_aspect_ratio,
                 double initial_near,
                 double initial_far)
//------------------------------------------
{
    gVirtualXRay::loadPerspectiveProjectionMatrix(initial_fovy,
    screen_aspect_ratio, initial_near, initial_far);
}

//-------------------------
void lookAt(double eye_x,
            double eye_y,
            double eye_z,
            double lookAtX,
            double lookAtY,
            double lookAtZ,
            double upX,
            double upY,
            double upZ)
//-------------------------
{
    gVirtualXRay::loadLookAtModelViewMatrix(eye_x, eye_y, eye_z, lookAtX,
    lookAtY, lookAtZ, upX, upY, upZ);
}


//---------------------------
void initialiseXRayRenderer()
//---------------------------
{
    if (g_beam_shape_initialised &&
        g_beam_energy_initialised &&
        g_source_position_initialised &&
        g_detector_position_initialised &&
        g_detector_up_vector_initialised &&
        g_detector_size_initialised &&
        g_detector_pixel_size_initialised)
    {
        // The renderer is not initialised
        if (!g_renderer_initialised)
        {
            cout << "INIT RENDER" << endl;
            // Initialise the renderer
            g_xray_renderer.initialise(gVirtualXRay::XRayRenderer::OPENGL,
                GL_RGB32F,
                &g_xray_detector,
                &g_xray_beam);

            g_xray_renderer.disableArtifactFilteringOnCPU();
            g_xray_renderer.enableArtifactFilteringOnGPU();
            g_xray_renderer.useNegativeFilteringFlag(false);
            g_renderer_initialised = true;
        }
    }
}


//----------------------------------------------------------
void computeRotation(gVirtualXRay::MATRIX4& aRotationMatrix,
                     int aViewportWidth,
                     int aViewportHeight)
//----------------------------------------------------------
{
    if (g_use_arc_ball)
    {
        if (g_current_x_position != g_last_x_position ||
            g_current_y_position != g_last_y_position)
        {
            gVirtualXRay::Vec3<GLfloat> va(
                getArcballVector(g_last_x_position,
                                 g_last_y_position,
                                 aViewportWidth,
                                 aViewportHeight));

            gVirtualXRay::Vec3<GLfloat> vb(
                getArcballVector(g_current_x_position,
                                 g_current_y_position,
                                 aViewportWidth,
                                 aViewportHeight));

            float angle(ROTATION_SPEED *
                radian2degree(acos(std::min(1.0, va.dotProduct(vb)))));

            gVirtualXRay::Vec3 < GLfloat > axis_in_camera_coord(va ^ vb);
            //axis_in_camera_coord.normalize();

            gVirtualXRay::Matrix4x4<GLfloat> camera2object(
                aRotationMatrix.getInverse());

            gVirtualXRay::Vec3<GLfloat> axis_in_object_coord =
            camera2object * axis_in_camera_coord;
            //axis_in_object_coord.normalize();

            gVirtualXRay::Matrix4x4 < GLfloat > rotation_matrix;
            rotation_matrix.rotate(angle, axis_in_object_coord);
            aRotationMatrix = aRotationMatrix * rotation_matrix;

            g_last_x_position = g_current_x_position;
            g_last_y_position = g_current_y_position;
        }
    }
}

//---------------------------------------------------------------
gVirtualXRay::Vec3<GLfloat> getArcballVector(int x,
                                             int y,
                                             int aViewportWidth,
                                             int aViewportHeight)
//---------------------------------------------------------------
{
    gVirtualXRay::Vec3 < GLfloat
    > P(2.0 * float(x) / float(aViewportWidth) - 1.0,
    2.0 * float(y) / float(aViewportHeight) - 1.0, 0);

    P.setY(-P.getY());

    float OP_squared = P.getX() * P.getX() + P.getY() * P.getY();
    if (OP_squared <= 1.0) {
    P.setZ(sqrt(1.0 - OP_squared));  // Pythagore
    } else {
    P.normalise();  // nearest point
    }

    return (P);
}

//--------------------------------
float radian2degree(float anAngle)
//--------------------------------
{
    return (180.0 * anAngle / gVirtualXRay::PI);
}

//-----------------------------------------------
double getUnitOfEnergy(const std::string& aUnitOfEnergy)
//-----------------------------------------------
{
    double unit_of_energy = 0;

    if (aUnitOfEnergy == "megaelectronvolt" || aUnitOfEnergy == "MeV") {
    unit_of_energy = gVirtualXRay::MeV;
    } else if (aUnitOfEnergy == "kiloelectronvolt" || aUnitOfEnergy == "keV") {
    unit_of_energy = gVirtualXRay::keV;
    } else if (aUnitOfEnergy == "electronvolt" || aUnitOfEnergy == "eV") {
    unit_of_energy = gVirtualXRay::eV;
    } else {
    throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    "ERROR: Unknown unit of energy.");
    }

    return (unit_of_energy);
}

//-----------------------------------------------
double getUnitOfLength(const std::string& aUnitOfLength)
//-----------------------------------------------
{
    double unit_of_length = 0;

    if ((aUnitOfLength == "kilometer") || (aUnitOfLength == "kilometre")
    || (aUnitOfLength == "km")) {
    unit_of_length = gVirtualXRay::km;
    } else if ((aUnitOfLength == "hectometer")
    || (aUnitOfLength == "hectometre") || (aUnitOfLength == "hm")) {
    unit_of_length = gVirtualXRay::hm;
    } else if ((aUnitOfLength == "decameter") || (aUnitOfLength == "decametre")
    || (aUnitOfLength == "dam")) {
    unit_of_length = gVirtualXRay::dam;
    } else if ((aUnitOfLength == "meter") || (aUnitOfLength == "metre")
    || (aUnitOfLength == "m")) {
    unit_of_length = gVirtualXRay::m;
    } else if ((aUnitOfLength == "decimeter") || (aUnitOfLength == "decimetre")
    || (aUnitOfLength == "dm")) {
    unit_of_length = gVirtualXRay::dm;
    } else if ((aUnitOfLength == "centimeter")
    || (aUnitOfLength == "centimetre") || (aUnitOfLength == "cm")) {
    unit_of_length = gVirtualXRay::cm;
    } else if ((aUnitOfLength == "millimeter")
    || (aUnitOfLength == "millimetre") || (aUnitOfLength == "mm")) {
    unit_of_length = gVirtualXRay::mm;
    } else if ((aUnitOfLength == "micrometer")
    || (aUnitOfLength == "micrometre") || (aUnitOfLength == "um")) {
    unit_of_length = gVirtualXRay::um;
    } else {
    throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    "ERROR: Unknown unit of length.");
    }

    return (unit_of_length);
}

std::vector<std::vector<float> > getVectorImage2D(
    const gVirtualXRay::Image<float>& anImage) {
    std::vector < std::vector<float> > output;

    for (unsigned int j = 0; j < anImage.getHeight(); ++j) {
    std::vector<float> row;
    row.insert(row.end(), anImage.getRawData() + j * anImage.getWidth(),
    anImage.getRawData() + (j + 1) * anImage.getWidth());

    output.push_back(row);
    }

    return output;
}


std::vector<std::vector<std::vector<float> > > getVectorImage3D(
    const gVirtualXRay::Image<float>& anImage)
{
	std::vector < std::vector < std::vector<float> > > output;

    for (unsigned int k = 0; k < anImage.getDepth(); ++k)
    {
    	std::vector < std::vector<float> > slice;
        for (unsigned int j = 0; j < anImage.getHeight(); ++j)
		{
			std::vector<float> row;
	        for (unsigned int i = 0; i < anImage.getWidth(); ++i)
			{
	        	row.push_back(anImage.getPixel(i, j, k));
			}

			slice.push_back(row);
		}

		output.push_back(slice);
    }

    return output;
}


gVirtualXRay::Image<float> getImage(const std::vector<std::vector<float> >& anImage)
{
    gVirtualXRay::Image<float> output(anImage[0].size(), anImage.size());

    for (unsigned int j = 0; j < anImage.size(); ++j)
    {
        for (unsigned int i = 0; i < anImage[0].size(); ++i)
        {
            output.setPixel(i, j, 0, anImage[j][i]);
        }
    }

    return output;
}


gVirtualXRay::Image<float> getImage(const std::vector<std::vector<std::vector<float> > >& anImage)
{
    gVirtualXRay::Image<float> output(anImage[0][0].size(), anImage[0].size(), anImage.size());

    for (unsigned int k = 0; k < anImage.size(); ++k)
    {
        for (unsigned int j = 0; j < anImage[0].size(); ++j)
        {
			for (unsigned int i = 0; i < anImage[0][0].size(); ++i)
			{
				output.setPixel(i, j, k, anImage[k][j][i]);
			}
		}
    }

    return output;
}



gVirtualXRay::MATRIX4 Vector2gvxrMatrix(
    const std::vector<std::vector<float> >& aSTLMatrix) {
    if (aSTLMatrix.size() == 4) {
    if (aSTLMatrix[0].size() == 4 && aSTLMatrix[1].size() == 4
    && aSTLMatrix[2].size() == 4 && aSTLMatrix[3].size() == 4) {
    return gVirtualXRay::MATRIX4(aSTLMatrix[0][0], aSTLMatrix[0][1],
    aSTLMatrix[0][2], aSTLMatrix[0][3], aSTLMatrix[1][0],
    aSTLMatrix[1][1], aSTLMatrix[1][2], aSTLMatrix[1][3],
    aSTLMatrix[2][0], aSTLMatrix[2][1], aSTLMatrix[2][2],
    aSTLMatrix[2][3], aSTLMatrix[3][0], aSTLMatrix[3][1],
    aSTLMatrix[3][2], aSTLMatrix[3][3]);
    }
    }

    throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    "Wrong matrix size");
}

std::vector<std::vector<float> > gvxrMatrix2Vector(
    const gVirtualXRay::MATRIX4& aMatrix4) {
    std::vector < std::vector<float> > output;
    output.push_back(std::vector<float>(4));
    output.push_back(std::vector<float>(4));
    output.push_back(std::vector<float>(4));
    output.push_back(std::vector<float>(4));

    output[0][0] = aMatrix4.get4x4()[0];
    output[0][1] = aMatrix4.get4x4()[1];
    output[0][2] = aMatrix4.get4x4()[2];
    output[0][3] = aMatrix4.get4x4()[3];
    output[1][0] = aMatrix4.get4x4()[4];
    output[1][1] = aMatrix4.get4x4()[5];
    output[1][2] = aMatrix4.get4x4()[6];
    output[1][3] = aMatrix4.get4x4()[7];
    output[2][0] = aMatrix4.get4x4()[8];
    output[2][1] = aMatrix4.get4x4()[9];
    output[2][2] = aMatrix4.get4x4()[10];
    output[2][3] = aMatrix4.get4x4()[11];
    output[3][0] = aMatrix4.get4x4()[12];
    output[3][1] = aMatrix4.get4x4()[13];
    output[3][2] = aMatrix4.get4x4()[14];
    output[3][3] = aMatrix4.get4x4()[15];

    return output;
}

//------------------------------------------
std::string getElementName(unsigned short Z)
//------------------------------------------
{
    return (gVirtualXRay::ElementSet::getInstance().getElement(Z).getName());
}

//--------------------------------------------
std::string getElementSymbol(unsigned short Z)
//--------------------------------------------
{
    return (gVirtualXRay::ElementSet::getInstance().getElement(Z).getSymbol());
}

//-----------------------------------------------------------------
unsigned short getElementAtomicNumber(const std::string& anElement)
//-----------------------------------------------------------------
{
    return (gVirtualXRay::ElementSet::getInstance().getElement(anElement).
        getZ());
}

double getDensityFromElement(const std::string& anElement) {
    unsigned short Z = getElementAtomicNumber(anElement);
    return (getDensityFromElement(Z));
}

double getDensityFromElement(unsigned short Z) {
    return (gVirtualXRay::ElementSet::getInstance().getElement(Z).getDensity()
    / (gVirtualXRay::g / gVirtualXRay::cm3));
}


//-------------------------------
double getDensityFromHU(double HU)
//-------------------------------
{
    return (gVirtualXRay::XRayRenderer::getDensityFromHU(HU)
    / (gVirtualXRay::g / gVirtualXRay::cm3));
}


//------------------------------------------------------------
double getMassAttenuationFromElement(const std::string& anElement,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//------------------------------------------------------------
{
    unsigned short Z = getElementAtomicNumber(anElement);
    return getMassAttenuationFromElement(Z, anEnergy, aUnitOfEnergy);
}


//------------------------------------------------
double getMassAttenuationFromElement(unsigned short Z,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//------------------------------------------------
{
    return (gVirtualXRay::ElementSet::getInstance().getElement(Z).getMassAttenuationTotal(anEnergy * getUnitOfEnergy(aUnitOfEnergy))
    / (gVirtualXRay::cm2 / gVirtualXRay::g));
}


//-------------------------------------------
double getMassAttenuationFromHU(double HU,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//-------------------------------------------
{
    // Initialise the photon cross-section
    gVirtualXRay::PhotonCrossSection photon_cross_section;
    photon_cross_section.useHU(HU);

	return (photon_cross_section.getMassAttenuationCoefficient(
			anEnergy * getUnitOfEnergy(aUnitOfEnergy)) /
				(gVirtualXRay::cm2 / gVirtualXRay::g));
}


//-----------------------------------------------------------
double getMassAttenuationFromMixture(const std::string& aMixture,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//-----------------------------------------------------------
{
    // Initialise the photon cross-section
    gVirtualXRay::PhotonCrossSection photon_cross_section;
    photon_cross_section.useMixture(aMixture);

	return (photon_cross_section.getMassAttenuationCoefficient(
			anEnergy * getUnitOfEnergy(aUnitOfEnergy)) /
				(gVirtualXRay::cm2 / gVirtualXRay::g));
}


//-------------------------------------------------------------
double getMassAttenuationFromCompound(const std::string& aCompound,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//-------------------------------------------------------------
{
    // Initialise the photon cross-section
    gVirtualXRay::PhotonCrossSection photon_cross_section;
    photon_cross_section.useCompound(aCompound);

	return (photon_cross_section.getMassAttenuationCoefficient(
			anEnergy * getUnitOfEnergy(aUnitOfEnergy)) /
				(gVirtualXRay::cm2 / gVirtualXRay::g));
}


double getMuFromHU(double HU,
		double anEnergy,
		const std::string& aUnitOfEnergy)
{
	return (gVirtualXRay::XRayRenderer::getMuFromHU(HU, anEnergy * getUnitOfEnergy(aUnitOfEnergy)) / (1.0 / gVirtualXRay::cm));
}



std::vector<float> rayIntersect(const std::string& aLabel,
    double aRayOriginX, double aRayOriginY, double aRayOriginZ,
    double aRayDirectionX, double aRayDirectionY, double aRayDirectionZ)
{
    std::vector<float> p_intersection_set;
    gVirtualXRay::SceneGraphNode* p_source_node = 0;

    // The scenegraph does not exist
    if (g_p_scene_graph.get())
    {
        // Find the node. It must exist
        gVirtualXRay::SceneGraphNode* p_source_node =
            g_p_scene_graph->getNode(aLabel);

        // The node exists
        if (p_source_node)
        {
            // Ray tracing
            std::map<float, gVirtualXRay::VEC3> p_intersection_full_set = p_source_node->rayIntersect(
            		gVirtualXRay::VEC3(aRayOriginX, aRayOriginY, aRayOriginZ),
			        gVirtualXRay::VEC3(aRayDirectionX, aRayDirectionY, aRayDirectionZ),
			        gVirtualXRay::MATRIX4());

			// Iterate through the map
			for (std::map<float, gVirtualXRay::VEC3>::const_iterator ite = p_intersection_full_set.begin();
			         ite != p_intersection_full_set.end();
			         ++ite)
			{
    			p_intersection_set.push_back(ite->first);
			}
        }
    }

    // The source does not exist
    if (!p_source_node)
    {
        cerr <<
            "gvxrWarning:\tPolygonMesh " <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            endl;
    }

    return p_intersection_set;
}















void setShiftFilter(double aValue) {
    g_shift_filter = aValue;
}

void setScaleFilter(double aValue) {
    g_scale_filter = aValue;
}

double getShiftFilter() {
    return (g_shift_filter);
}

double getScaleFilter() {
    return (g_scale_filter);
}


std::vector<std::vector<float> > loadImage2D(const std::string& aFileName)
{
    return getVectorImage2D(gVirtualXRay::Image<float>(aFileName));
}


std::vector<std::vector<std::vector<float> > > loadImage3D(const std::string& aFileName)
{
    return getVectorImage3D(gVirtualXRay::Image<float>(aFileName));
}


double computeZNCC(const std::vector<std::vector<std::vector<float> > >& aReferenceImage, const std::vector<std::vector<std::vector<float> > >& aTestImage)
{
    gVirtualXRay::Image<float> ref = getImage(aReferenceImage);
    gVirtualXRay::Image<float> test = getImage(aTestImage);

    return ref.computeNCC(test);
}


double computeRMSE(const std::vector<std::vector<std::vector<float> > >& aReferenceImage, const std::vector<std::vector<std::vector<float> > >& aTestImage)
{
    gVirtualXRay::Image<float> ref = getImage(aReferenceImage);
    gVirtualXRay::Image<float> test = getImage(aTestImage);

    return ref.computeRMSE(test);
}


double getMean(const std::vector<float>& anImage)
{
    double avg = 0.0;

#pragma omp parallel for reduction( + : avg )
    for (int i = 0; i < anImage.size(); ++i)
    {
        avg += anImage[i];
    }

    return avg / anImage.size();
}

double getStddev(const std::vector<float>& anImage)
{
    double avg = getMean(anImage);
    double stddev = 0.0;

#pragma omp parallel for reduction( + : stddev )
    for (unsigned int i = 0; i < anImage.size(); ++i)
    {
        stddev += std::pow(anImage[i] - avg, 2);
    }

    return sqrt(stddev / anImage.size());
}


double getStddev(const std::vector<float>& anImage, double avg)
{
    double stddev = 0.0;

#pragma omp parallel for reduction( + : stddev )
    for (unsigned int i = 0; i < anImage.size(); ++i)
    {
        stddev += std::pow(anImage[i] - avg, 2);
    }

    return sqrt(stddev / anImage.size());
}

double computeZNCC(const std::vector<float>& aReferenceImage, const std::vector<float>& aTestImage)
{
    double avg_ref = getMean(aReferenceImage);
    double avg_test = getMean(aTestImage);

    double stddev_ref = getStddev(aReferenceImage, avg_ref);
    double stddev_test = getStddev(aTestImage, avg_test);

    double zncc = 0.0;
#pragma omp parallel for reduction( + : zncc )
    for (int i = 0; i < aReferenceImage.size(); ++i)
    {
        zncc += (aReferenceImage[i] - avg_ref) * (aTestImage[i] - avg_test);
    }

    return (zncc / (stddev_ref * stddev_test)) / aReferenceImage.size();
}


double computeRMSE(const std::vector<float>& aReferenceImage, const std::vector<float>& aTestImage, bool normalise)
{
    double sse = 0.0;

    if (normalise)
    {
        double avg_ref = getMean(aReferenceImage);
        double avg_test = getMean(aTestImage);

        double stddev_ref = getStddev(aReferenceImage, avg_ref);
        double stddev_test = getStddev(aTestImage, avg_test);

#pragma omp parallel for reduction( + : sse )
        for (int i = 0; i < aReferenceImage.size(); ++i)
        {
            sse += std::pow(((aReferenceImage[i] - avg_ref) / stddev_ref) -
                ((aTestImage[i] - avg_test) / stddev_test), 2);
        }
    }
    else
    {
#pragma omp parallel for reduction( + : sse )
        for (int i = 0; i < aReferenceImage.size(); ++i)
        {
            sse += std::pow(aReferenceImage[i] - aTestImage[i], 2);
        }
    }

    return sqrt(sse / aReferenceImage.size());
}


std::vector<float> g_p_ref_image;
std::vector<float> g_p_test_image;

double computeRMSE(bool normalise)
{
    return computeRMSE(g_p_ref_image, g_p_test_image, normalise);
}


double computeZNCC()
{
    return computeZNCC(g_p_ref_image, g_p_test_image);
}


void loadReference(const std::vector<float>& aReferenceImage)
{
    g_p_ref_image = aReferenceImage;
}


void loadTest(const std::vector<float>& aTestImage)
{
    g_p_test_image = aTestImage;
}


void lineariseTest(float aThreshold, float aScalingFactor)
{
#pragma omp parallel for
    for (int i = 0; i < g_p_test_image.size(); ++i)
    {
        if (g_p_test_image[i] < aThreshold)
        {
            g_p_test_image[i] = aThreshold;
        }

        g_p_test_image[i] = -log(g_p_test_image[i]);
        g_p_test_image[i] *= aScalingFactor;
    }
}


float getMinValue(const std::vector<std::vector<std::vector<float> > >& aImage)
{
    return getImage(aImage).getMinValue();
}


float getMaxValue(const std::vector<std::vector<std::vector<float> > >& aImage)
{
    return getImage(aImage).getMaxValue();
}


void FBO2Thrust(unsigned int aFBO, void* aThrustVector, int anOffset)
{
#ifndef HAS_CUDA
    throw gVirtualXRay::Exception(__FILE__,
                                  __FUNCTION__,
                                  __LINE__,
                                  "CUDA is not supported.");
#else
    cudaGLSetGLDevice(0);
    struct cudaGraphicsResource *fbo_cuda;
    cudaGraphicsGLRegisterBuffer(&fbo_cuda, g_xray_renderer.getFboId(aFBO), cudaGraphicsMapFlagsReadOnly);


    thrust::device_ptr<float4>* p_device_data = static_cast<thrust::device_ptr<float4>*>(aThrustVector);
#endif
}
