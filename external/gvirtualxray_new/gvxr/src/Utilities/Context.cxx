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
*   @file       Context.cxx
*
*   @brief      Create an OpenGL context using EGL or GLFW.
*
*   @version    1.0
*
*   @date       10/09/2020
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
*               http://www.fpvidal.net/, Sept 2020, 2020, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************

#include <iostream>

#include <algorithm> // For capitalising a string

#ifndef __OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#define GL_GLEXT_PROTOTYPES 1

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#ifndef __Context_h
#include "gVirtualXRay/Context.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;


//--------------------------------------------
Context::Context(const std::string& aRenderer,
				 int aRendererMajorVersion,
				 int aRendererMinorVersion):
//--------------------------------------------
		m_renderer(""),
		m_major_version(0),
		m_minor_version(0),

		m_p_window(0),

#ifdef HAS_EGL
		m_display(0),
		m_config(0),
		m_context(0),
		m_surface(0),
#endif

		m_ready(false)
//--------------------------------------------
{
	create(aRenderer, aRendererMajorVersion, aRendererMinorVersion);
}


//-----------------
Context::~Context()
//-----------------
{
	release();
}


//------------------------------------------------
void Context::create(const std::string& aRenderer,
					 int aRendererMajorVersion,
					 int aRendererMinorVersion)
//------------------------------------------------
{
	// The renderer has changed
	if (m_renderer != aRenderer ||
			m_major_version != aRendererMajorVersion ||
			m_minor_version != aRendererMinorVersion)
	{
		// Release the previous context if applicable
		release();

		// Store the context details
		m_renderer = aRenderer;
		m_major_version = aRendererMajorVersion;
		m_minor_version = aRendererMinorVersion;

		// Create the context if possible
		std::string renderer = m_renderer;
		std::transform(renderer.begin(), renderer.end(), renderer.begin(), ::toupper);

		if (renderer == "OPENGL") createGLWithGLFW();
		else if (renderer == "VULKAN") createVulkanWithGLFW();
		else if (renderer == "EGL") createGLWithEGL();
		else std::cerr << "WARNING: Invalaid context: " << aRenderer <<
				" (" << aRendererMajorVersion << "." << aRendererMinorVersion << ")" <<
				std::endl;
	}
}


//---------------------
void Context::release()
//---------------------
{
#ifdef HAS_EGL
	if (m_display && m_context)
		{
		eglDestroyContext(m_display, m_context);
		m_context = 0;
		}

	if (m_display)
		{
		eglTerminate(m_display);
		m_display = 0;
		}
#endif // HAS_EGL

	if (m_p_window)
		{
		glfwDestroyWindow(m_p_window);
		m_p_window = 0;
		}

	m_ready = false;
}


//---------------------
void Context::destroy()
//---------------------
{
	release();
}


//---------------------------
bool Context::isReady() const
//---------------------------
{
	return m_ready;
}


//------------------------------------
GLFWwindow* Context::getWindowHandle()
//------------------------------------
{
	return m_p_window;
}


//------------------------------------------------
const GLFWwindow* Context::getWindowHandle() const
//------------------------------------------------
{
	return m_p_window;
}


//-----------------------------
void Context::createGLWithEGL()
//-----------------------------
{
	// Release the previous context if applicable
	release();

#ifdef HAS_EGL

	// Context attributes
	constexpr EGLint attrib_list[] =
		{
		EGL_RED_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_LUMINANCE_SIZE, 0,
		EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_NONE, EGL_NONE
		};

	// Get the display
	m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if(m_display == EGL_NO_DISPLAY)
		{
		std::cerr << "WARNING: Failed to eglGetDisplay(EGL_DEFAULT_DISPLAY)" << std::endl;
		m_ready = false;
		return;
		}

	// Initialise EGL
	EGLint egl_major_ver;
	EGLint egl_minor_ver;
	if(eglInitialize(m_display, &egl_major_ver, &egl_minor_ver) == EGL_FALSE)
		{
		std::cerr << "WARNING: Failed to eglInitialize" << std::endl;
		EGLint error =  eglGetError();
		switch(error)
			{
			case EGL_BAD_DISPLAY:
				std::cerr << "WARNING: display is not an EGL display connection" << std::endl;
				break;
			case EGL_NOT_INITIALIZED:
				std::cerr << "WARNING: display cannot be initialized" << std::endl;
				break;
			default:
				break;
			}
		m_ready = false;
		return;
		}
	std::cout << "EGL version: " << egl_major_ver << "." << egl_minor_ver << std::endl;

	// Choose the config supporting the attributes set above
	EGLint    num_config;
	if(eglChooseConfig(m_display, attrib_list, &m_config, 1, &num_config) == EGL_FALSE)
		{
		std::cerr << "WARNING: Failed to eglChooseConfig" << std::endl;
		m_ready = false;
		return;
		}

	if(num_config < 1)
		{
		std::cerr << "WARNING: No matching EGL frame buffer configuration" << std::endl;
		m_ready = false;
		return;
		}

	// Bind the OpenGL API to the context
	if(eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
		{
		std::cerr << "WARNING: Failed to eglBindAPI(EGL_OPENGL_API)" << std::endl;
		m_ready = false;
		return;
		}

	// OpenGL context
	const EGLint context_attrib[] =
		{
		EGL_CONTEXT_MAJOR_VERSION_KHR,  m_major_version,
		EGL_CONTEXT_MINOR_VERSION_KHR,  m_minor_version,
		EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR,    EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
		EGL_CONTEXT_FLAGS_KHR,          EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR
#ifndef NDEBUG
		| EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR
#endif
		,
		EGL_NONE
		};

	// Create the context
	m_context = eglCreateContext(m_display, m_config, EGL_NO_CONTEXT, context_attrib);
	if(m_context == EGL_NO_CONTEXT)
		{
		m_context = eglCreateContext(m_display, m_config, EGL_NO_CONTEXT, nullptr);
		}

	// The context was not created
	if(m_context == EGL_NO_CONTEXT)
		{
		std::cerr << "WARNING: Failed to eglCreateContext" << std::endl;
		EGLint error =  eglGetError();
		switch(error)
			{
			case EGL_BAD_CONFIG:
				std::cerr << "WARNING: config is not an EGL frame buffer configuration, or does not support the current rendering API" << std::endl;
				break;
			case EGL_BAD_ATTRIBUTE:
				std::cerr << "WARNING: attrib_list contains an invalid context attribute or if an attribute is not recognized or out of range" << std::endl;
				break;
			default:
				std::cerr << "WARNING: Unknown error" << std::endl;
				break;
			}
		m_ready = false;
		return;
		}

	// Create the surface
	m_surface = eglCreatePbufferSurface(m_display,
			m_config,
			nullptr);

	// The surface was not created
	if (!m_surface)
		{
		std::cerr << "WARNING: Error: eglCreatePbufferSurface failed Error" << std::endl;
		return;
		}

	// Make current
	if (eglMakeCurrent(m_display,
			m_surface,
			m_surface,
			m_context) == EGL_FALSE)
		{
		std::cerr << "WARNING: Failed to eglMakeCurrent" << std::endl;
		m_ready = false;
		return;
		}

	// Initialise GLEW
	gVirtualXRay::initialiseGLEW();

	// Still no error?
	if(EGL_SUCCESS != eglGetError())
		{
		m_ready = false;
		return;
		}

	std::cout << "OpenGL version supported by this platform " << glGetString(GL_VERSION) << std::endl;

	// Get rid of any OpenGL error at this stage
	glGetError();

	m_ready = true;
#endif // HAS_EGL
}


//------------------------------
void Context::createGLWithGLFW()
//------------------------------
{
	// Release the previous context if applicable
	release();

    std::string window_title = "gVirtualXRay -- OpenGL Window (GLFW)";

    // Set the version of OpenGL that is required
	glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_minor_version);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Enable anti-aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a windowed mode window and its OpenGL context
    m_p_window = glfwCreateWindow(1, 1, window_title.data(), 0, 0);

    // Window has be created
    if (m_p_window)
    {
        // Make the window's context current
        glfwMakeContextCurrent(m_p_window);
        glfwSetWindowUserPointer(m_p_window, this);

        // Initialise GLEW
        gVirtualXRay::initialiseGLEW();

        m_ready = true;
    }
}


//----------------------------------
void Context::createVulkanWithGLFW()
//----------------------------------
{
	// Release the previous context if applicable
	release();

    throw gVirtualXRay::Exception(
        __FILE__,
        __FUNCTION__,
        __LINE__,
        "ERROR: not implemented, contact Franck if you need this functionality.");
}
