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
*   @file       Context.h
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


#ifndef __Context_h
#define __Context_h


//******************************************************************************
//  defines
//******************************************************************************
#ifndef EGL_NO_X11
#define EGL_NO_X11
#endif

//#ifndef MESA_EGL_NO_X11_HEADERS
//#define MESA_EGL_NO_X11_HEADERS
//#endif


//******************************************************************************
//  Include
//******************************************************************************
#include <string>

#ifdef HAS_EGL
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  Context
*   @brief  Context is a class to manage an OpenGL or Vulkan context.
*/
//==============================================================================
class Context
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
	*  @param aRenderer:   the type of context. Accepted values are "OpenGL", "Vulkan", and "EGL" (default: "OpenGL")
	*  @param aRendererMajorVersion: Select the major version of the renderer.
	*                    (default value: 3)
	*  @param aRendererMajorVersion: Select the minor version of the renderer.
	*                    (default value: 2)
	*/
    //--------------------------------------------------------------------------
	Context(const std::string& aRenderer = "OpenGL",
			int aRendererMajorVersion = 3,
			int aRendererMinorVersion = 2);


    //--------------------------------------------------------------------------
    /// Destructor
    //--------------------------------------------------------------------------
	~Context();


    //--------------------------------------------------------------------------
    /// Create a new context
    /*
	*  @param aRenderer:   the type of context. Accepted values are "OpenGL", "Vulkan", and "EGL" (default: "OpenGL")
	*  @param aRendererMajorVersion: Select the major version of the renderer.
	*                    (default value: 3)
	*  @param aRendererMajorVersion: Select the minor version of the renderer.
	*                    (default value: 2)
	*/
    //--------------------------------------------------------------------------
	void create(const std::string& aRenderer = "OpenGL",
			int aRendererMajorVersion = 3,
			int aRendererMinorVersion = 2);


	//--------------------------------------------------------------------------
    /// Destroy the context
    //--------------------------------------------------------------------------
	void release();


    //--------------------------------------------------------------------------
    /// Destroy the context
    //--------------------------------------------------------------------------
	void destroy();


	bool isReady() const;


	GLFWwindow* getWindowHandle();
	const GLFWwindow* getWindowHandle() const;
//******************************************************************************
protected:
    //--------------------------------------------------------------------------
    /// Create an OpenGL context using EGL
    //--------------------------------------------------------------------------
	void createGLWithEGL();


    //--------------------------------------------------------------------------
    /// Create an OpenGL context using GLFW
    //--------------------------------------------------------------------------
	void createGLWithGLFW();


	//--------------------------------------------------------------------------
    /// Create a Vulkan context using GLFW
    //--------------------------------------------------------------------------
	void createVulkanWithGLFW();


	std::string m_renderer;
	int m_major_version;
	int m_minor_version;

    GLFWwindow* m_p_window;

#ifdef HAS_EGL
	EGLDisplay m_display;
    EGLConfig m_config;
    EGLContext m_context;
    EGLSurface m_surface;
#endif

    bool m_ready;
};


} // namespace gVirtualXRay



#endif
