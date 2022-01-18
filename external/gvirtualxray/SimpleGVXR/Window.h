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


#ifndef __Window_h
#define __Window_h


/**
********************************************************************************
*
*   @file       Window.h
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
#include <string>
#include <list>

// To create the OpenGL or Vulkan context
#ifndef __Context_h
#include "gVirtualXRay/Context.h"
#endif


//******************************************************************************
//  Function declarations
//******************************************************************************

namespace SimpleGVXR {
class Window
{
public:


    //------------------------------------------------------------------------------
    /// Create a window
    /**
     *  @param aVisibilityFlag: flag controling if the window should be visible (1)
     *                          or hidden (0). (default value: 0)
     */
    //------------------------------------------------------------------------------
    Window(int aVisibilityFlag = 0);


    ~Window();


    //------------------------------------------------------------------------------
    /// Create an OpenGL context and display it in a window
    /**
     *  @param aWidth:      the number of pixels along the X-axis
     *  @param aHeight:     the number of pixels along the Y-axis
     */
    //------------------------------------------------------------------------------
    virtual void setWindowSize(int aWidth, int aHeight) = 0;


    void initViewer();


    virtual void displayScene(bool aSceneRotationFlag) = 0;


    virtual void takeScreenshot() = 0;
    

    void renderLoop();


    void showWindow();


    void hideWindow();
    
    
    //--------------------------------------------------------------------------
    /// Set window background colour.
    /**
     *  @param R: the red colour [0, 1]
     *  @param G: the green colour [0, 1]
     *  @param B: the blue colour [0, 1]
     */
    //--------------------------------------------------------------------------    
    virtual void setWindowBackGroundColour(float R, float G, float B);


    //--------------------------------------------------------------------------
    /// Set window background colour.
    /**
     *  @param R: the red colour [0, 1]
     *  @param G: the green colour [0, 1]
     *  @param B: the blue colour [0, 1]
     */
    //--------------------------------------------------------------------------    
    virtual void setWindowBackGroundColor(float R, float G, float B);


    void destroyWindow();


    static Window* getWindow(GLFWwindow* apGLFWwindow);

protected:
    // Initialise GLEW
    virtual void createWindow(bool aVisibilityFlag) = 0;

    // Initialise OpenGL
    virtual void initShader() = 0;

    virtual void render(bool aSceneRotationFlag) = 0;


    virtual void registerCallbacks();


    static std::list<Window*> m_p_window_set;

    gVirtualXRay::Context m_context;

    std::string m_title;
    bool m_visible;
    int m_width;
    int m_height;

    bool m_viewer_initialised;
    bool m_callback_registered;
    
    float m_background_colour[3];
};
};

#endif
