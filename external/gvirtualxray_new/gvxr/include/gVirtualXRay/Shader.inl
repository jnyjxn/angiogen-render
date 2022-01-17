/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*	@file		Shader.inl
*
*	@brief		Class to handle GLSL shaders.
*
*	@version	1.0
*
*	@date		29/11/2013
*
*	@author		Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net), 
*   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//---------------------------------------
inline Shader::Shader():
//---------------------------------------
	m_vertex_source(0),
	m_vertex_source_number_of_lines(0),
	
	m_fragment_source(0),
	m_fragment_source_number_of_lines(0),

	m_valid(false),

	m_shader_program_id(0),
	m_vertex_shader_id(0),
	m_fragment_shader_id(0)
//---------------------------------------
{
}


//----------------------
inline Shader::~Shader()
//----------------------
{
	release();
}


//--------------------------------------------------------------------
inline void Shader::setLabels(const std::string& aVertexShaderLabel,
							  const std::string& aFragmentShaderLabel)
//--------------------------------------------------------------------
{
	m_vertex_shader_label   = aVertexShaderLabel;
	m_fragment_shader_label = aFragmentShaderLabel;
}


//--------------------------------------------------
inline unsigned int Shader::getProgramHandle() const
//--------------------------------------------------
{
	return (m_shader_program_id);
}


//---------------------------------
inline bool Shader::isValid() const
//---------------------------------
{
	return (m_valid);
}


//---------------------------------------------
inline void Shader::releaseVertexSourceMemory()
//---------------------------------------------
{
	releaseSourceMemory(m_vertex_source_number_of_lines, m_vertex_source);
}


//-----------------------------------------------
inline void Shader::releaseFragmentSourceMemory()
//-----------------------------------------------
{
	releaseSourceMemory(m_fragment_source_number_of_lines, m_fragment_source);
}


} // namespace gVirtualXRay
