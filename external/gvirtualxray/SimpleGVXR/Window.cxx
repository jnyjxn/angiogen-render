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
*   @file       Window.cxx
*
*   @brief      Window is a virtual class to handle windows. The plan is to use
*               GLFW to support any OpenGL version as well as Vulkan.
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
#include <cmath>
#include <algorithm>

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

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

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif

// To perform the computations of the X-ray simulation
#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef __Window_h
#include "Window.h"
#endif


//******************************************************************************
//  Namespaces
//******************************************************************************
using namespace std;


//******************************************************************************
//  Function declarations
//******************************************************************************
void keyCallback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods);

void mouseButtonCallback(GLFWwindow* apWindow,
    int aButton,
    int aButtonState,
    int aModifierKey);

void cursorPosCallback(GLFWwindow* apWindow, double x, double y);

void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);


//******************************************************************************
//  Static class members
//******************************************************************************
std::list<SimpleGVXR::Window*> SimpleGVXR::Window::m_p_window_set;


//******************************************************************************
//  Global variables
//******************************************************************************
extern double g_initial_fovy;
extern double g_initial_near;
extern double g_initial_far;
extern GLfloat g_zoom;

extern bool g_run_loop;
extern bool g_display_beam;
extern bool g_display_wireframe;
extern bool g_use_lighing;
extern bool g_display_detector;
extern bool g_display_normal_vectors;

extern gVirtualXRay::XRayBeam g_xray_beam;         // X-ray beam
extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine

extern gVirtualXRay::Shader g_display_shader;

extern std::shared_ptr<gVirtualXRay::SceneGraphBinder> g_p_scene_graph;


//******************************************************************************
//  Function definitions
//******************************************************************************


//----------------------------------
SimpleGVXR::Window::Window(int aVisibilityFlag):
//----------------------------------
    m_title("gVirtualXRay -- Window"),
    m_visible(aVisibilityFlag),
    m_width(500),
    m_height(500),
    m_viewer_initialised(false),
	  m_callback_registered(false)
//----------------------------------
{
    cout << "gvxrStatus:\tCreate window " << endl;

    m_background_colour[0] = 0.5;
    m_background_colour[1] = 0.5;
    m_background_colour[2] = 0.5;

	m_p_window_set.push_back(this);
}


SimpleGVXR::Window::~Window()
{
	std::list<SimpleGVXR::Window*>::iterator ite = std::find(m_p_window_set.begin(), m_p_window_set.end(), this);

	if (ite != m_p_window_set.end())
	{
		m_p_window_set.erase(ite);
	}
}


//-------------------------------------------------
void SimpleGVXR::Window::setWindowSize(int aWidth, int aHeight)
//-------------------------------------------------
{
    // The size has changed
    if (m_width != aWidth || m_height != aHeight)
    {
        m_width  = aWidth;
        m_height = aHeight;

        if (m_context.getWindowHandle())
        	{
        	glfwSetWindowSize(m_context.getWindowHandle(), aWidth, aHeight);
        	}
    }
}


//-----------------------
void SimpleGVXR::Window::renderLoop()
//-----------------------
{
    g_run_loop = true;

    // Launch the event loop
    if (m_context.getWindowHandle())
		{
		while (!glfwWindowShouldClose(m_context.getWindowHandle()) &&
				g_run_loop)
			{
			// Process the event
			glfwPollEvents();

			// Render here
			displayScene(true);
			}
		}
}


//-----------------------
void SimpleGVXR::Window::showWindow()
//-----------------------
{
	if (m_context.getWindowHandle())
		{
		glfwShowWindow(m_context.getWindowHandle());
		}
}


//-----------------------
void SimpleGVXR::Window::hideWindow()
//-----------------------
{
	if (m_context.getWindowHandle())
		{
		glfwHideWindow(m_context.getWindowHandle());
		}
}


//---------------------------------------------------------------------------
void SimpleGVXR::Window::setWindowBackGroundColour(float R, float G, float B)
//---------------------------------------------------------------------------
{
    m_background_colour[0] = R;
    m_background_colour[1] = G;
    m_background_colour[2] = B;
}


//--------------------------------------------------------------------------
void SimpleGVXR::Window::setWindowBackGroundColor(float R, float G, float B)
//--------------------------------------------------------------------------
{
    setWindowBackGroundColour(R, G, B);
}


//--------------------------
void SimpleGVXR::Window::destroyWindow()
//--------------------------
{
	m_context.release();
}


//-----------------------
void SimpleGVXR::Window::initViewer()
//-----------------------
{
    if (!m_viewer_initialised && g_xray_detector.getSourcePositionSet().size())
    {
        float distance_source_object =
            g_xray_detector.getSourcePositionSet()[0].length();

        float distance_detector_object =
            g_xray_detector.getDetectorPosition().length();

        float distance = std::max(distance_detector_object,
                                  distance_source_object);

        float half_fovy = (M_PI * g_initial_fovy / 2.0) / 180.0;

        g_zoom = atan(half_fovy) * distance;

        if (g_p_scene_graph.get())
        {
            g_initial_near = (g_p_scene_graph->getBoundingBox().second -
                g_p_scene_graph->getBoundingBox().first).length() / 5.0;

            m_viewer_initialised = true;
        }

        g_initial_far = g_zoom * 50.0;
    }
}


SimpleGVXR::Window* SimpleGVXR::Window::getWindow(GLFWwindow* apGLFWwindow)
{
	for (auto& ite: m_p_window_set)
	{
		if (ite->m_context.getWindowHandle() == apGLFWwindow)
			return ite;
	}

	return 0;
}

//----------------------------------
void keyCallback(GLFWwindow* window,
                 int key,
                 int scancode,
                 int action,
                 int mods)
//----------------------------------
{
    if (action == GLFW_PRESS)
    {
		switch (key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:
			g_run_loop = false;
			break;

		case GLFW_KEY_B:
			g_display_beam = !g_display_beam;
			break;

		case GLFW_KEY_W:
			g_display_wireframe = !g_display_wireframe;
			break;

		case GLFW_KEY_N:
			g_xray_renderer.useNegativeFilteringFlag(
					!g_xray_renderer.getNegativeFilteringFlag());
			break;

		case GLFW_KEY_L:
			g_use_lighing = !g_use_lighing;
			break;

		case GLFW_KEY_D:
			g_display_detector = !g_display_detector;
			break;

		case GLFW_KEY_V:
			g_display_normal_vectors = !g_display_normal_vectors;
			break;

		case GLFW_KEY_T:
		    SimpleGVXR::Window* p_window = SimpleGVXR::Window::getWindow(window);
		    p_window->takeScreenshot();
		    break;
		}
    }
}


//------------------------------
void SimpleGVXR::Window::registerCallbacks()
//------------------------------
{
	if (m_context.getWindowHandle() && !m_callback_registered)
	{
		m_callback_registered = true;

        glfwSetKeyCallback(m_context.getWindowHandle(), keyCallback);
        glfwSetMouseButtonCallback(m_context.getWindowHandle(), mouseButtonCallback);
        glfwSetCursorPosCallback(m_context.getWindowHandle(), cursorPosCallback);
        glfwSetScrollCallback(m_context.getWindowHandle(), scrollCallback);
	}
}
