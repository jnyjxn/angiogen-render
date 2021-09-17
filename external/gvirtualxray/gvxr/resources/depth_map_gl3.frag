/**
********************************************************************************
*
*	@file		depth_map_gl3.frag
*
*	@brief      fragment shader to create a depth map.
*
*	@version	1.0
*
*	@date		20/07/2016
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
//	Output
//******************************************************************************
out float frag_depth;

//-------------
void main(void)
//-------------
{
	//output: the distance of the fragment
	frag_depth  = gl_FragCoord.z;
}
