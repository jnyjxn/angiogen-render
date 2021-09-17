/**
********************************************************************************
*
*	@file		bump_map_generation_gl3.vert
*
*	@brief      vertex shader to create a bump map.
*
*	@version	1.0
*
*	@date		01/08/2016
*
*	@author		Andréas Meuleman
*
********************************************************************************
*/



//******************************************************************************
//	OpenGL 3.2
//******************************************************************************
#version 150


//******************************************************************************
//	Input
//******************************************************************************
in vec3 in_Vertex;
in vec2 in_TexCoord;
in vec3 in_Normal;

//******************************************************************************
//	Output
//******************************************************************************
smooth out vec3 g_object_position;

//-------------
void main(void)
//-------------
{	
	g_object_position = in_Vertex;
		
	gl_Position = vec4(in_Vertex,1.)*2-1;
}


