/*

Copyright (c) 2017, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


#ifndef __SoftTissueMesh_h
#define __SoftTissueMesh_h


/**
********************************************************************************
*
*   @file       SoftTissueMesh.h
*
*   @brief      Class to handle a polygon mesh for an internal organ.
*
*   @version    1.0
*
*   @date       13/01/2017
*
*   @author     Andreas Meuleman
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __InternalOrganMesh_h
#include "gVirtualXRay/InternalOrganMesh.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  SoftTissueMesh
*   @brief  SoftTissueMesh is a class to handle a polygon mesh for
*           an internal organ. It uses procedural texturing.
*/
//==============================================================================
class SoftTissueMesh: public InternalOrganMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
     *  @param texture_size:                    size of the side of the buffer
     *                                          (default: 1024)
     *  @param texture_3D_size:                 size of the side of the buffer
     *                                          of the 3D texture (default: 64)
     *  @param optimize_texture_coordinates:    To know if the texture
     *                                          coordinates need to be
     *                                          optimized (default: true)
     */
    //--------------------------------------------------------------------------
    SoftTissueMesh(bool optimize_texture_coordinates = true,
                   unsigned int texture_size = 1024,
                   unsigned int texture_3D_size = 64);

    //--------------------------------------------------------------------------
    /// Destructor
    //--------------------------------------------------------------------------
    virtual ~SoftTissueMesh();

    //--------------------------------------------------------------------------
    /// return true if the mesh represent a soft tissue
    //--------------------------------------------------------------------------
    virtual bool isSoftTissue() const;

    //--------------------------------------------------------------------------
    /// Update the VBO.
    /**
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     */
    //--------------------------------------------------------------------------
    virtual void updateVBO(int aTypeOfVBO, int aTypeOfPrimitive);

    
    //--------------------------------------------------------------------------
    /// Link the attributes to their id in the shader
    /*
    *  @param aShaderID: The GLSL shader used to display the anatomical mesh
    */
    //--------------------------------------------------------------------------
    virtual void defineDisplayShaderInput(int aShaderID);

    
    //--------------------------------------------------------------------------
    /// initialize the shaders, the buffers, the normal vectors and the textures
    //--------------------------------------------------------------------------
    virtual void initialize();


//******************************************************************************
protected:

    //--------------------------------------------------------------------------
    /// generates the textures
    //--------------------------------------------------------------------------
    virtual void generateTextures();

    
    //--------------------------------------------------------------------------
    /// Link the attributes to their id in the shader bump map generation
    /*
     *  @param shader_id: The GLSL shader
     */
    //--------------------------------------------------------------------------
    virtual void defineBumpMapShaderInput(int shader_id);

    
    //--------------------------------------------------------------------------
    /// Link the attributes to their id in the shader for colour texture
    /// generation
    /*
    *  @param shader_id: The GLSL shader
    */
    //--------------------------------------------------------------------------
    virtual void defineColourTextureShaderInput(int shader_id);

    
    // Buffers for the colour texture
    unsigned int m_colour_texture;
    
    
    unsigned int m_colour_texture_frame_buffer;

    
    // To know if we use a 3D colour texture
    int m_use_3D_colour_texture;

    
    // GLSL shaders to create the colour texture
    Shader m_colour_texture_shader;
};

    
} // namespace gVirtualXRay


#endif // __SoftTissueMesh_h
