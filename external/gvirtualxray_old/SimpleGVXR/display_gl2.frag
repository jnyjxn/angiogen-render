/*

Copyright (c) 2016, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
 *  @file       display_gl2.frag
 *
 *  @brief      Fragment shader to display 3D objects with Phong shading.
 *
 *  @version    1.0
 *
 *  @date       31/12/2013
 *
 *  @author     Dr Franck P. Vidal
 *
 *  @section    License
 *              BSD 3-Clause License.
 *
 *              For details on use and redistribution please refer
 *              to http://opensource.org/licenses/BSD-3-Clause
 *
 *  @section    Copyright
 *              (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
 *              http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
 *              BSD 3-Clause License
 *
 ********************************************************************************
 */


//******************************************************************************
//  OpenGL 2.0
//******************************************************************************
#version 110


//******************************************************************************
//  Varying variables
//******************************************************************************
varying vec3 g_texture_coordinates;
varying vec3 g_normal;
varying vec4 g_object_position;


//******************************************************************************
//  Uniform variables
//******************************************************************************
uniform int g_use_lighting;
uniform vec4 light_global_ambient;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform vec4 light_position;
uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;


//-------------
void main(void)
//-------------
{
    if (g_use_lighting  == 1)
        {
        vec3 view_direction  = vec3(g_object_position) / g_object_position.w;
        vec3 normal_vector   = normalize(g_normal);
        vec3 light_direction = normalize(vec3(light_position.xyz));
        
        vec3 half_vector     = normalize(view_direction + light_direction);
    
        vec3 global_ambient = vec3(light_global_ambient * material_ambient);
        vec3 ambient        = vec3(material_ambient  * light_ambient);
        vec3 diffuse        = vec3(material_diffuse  * light_diffuse  *
            max(0.0, dot(light_direction, normal_vector)));
        vec3 specular       = vec3(material_specular * light_specular *
            pow(max(dot(normal_vector, half_vector), 0.0), material_shininess));
    
        gl_FragColor = vec4(global_ambient + ambient + diffuse + specular, 1.0);
        }
    else
        {
        gl_FragColor = material_ambient;
        }
}
