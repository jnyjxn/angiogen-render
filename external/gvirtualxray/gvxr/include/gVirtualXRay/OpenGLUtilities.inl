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
*	@file		OpenGLUtilities.inl
*
*	@brief		Some utility functions about OpenGL.
*
*	@version	1.0
*
*	@date		07/11/2013
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
//	Include
//******************************************************************************
#include <cmath>

#ifndef GVXR_OPENGL_EXCEPTION_H
#include "gVirtualXRay/OpenGLException.h"
#endif

#ifndef GVXR_OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef GVXR_FBO_EXCEPTION_H
#include "gVirtualXRay/FBOException.h"
#endif

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_CONSTANT_VALUES_H
#include "gVirtualXRay/ConstantValues.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//--------------------------------
inline void pushProjectionMatrix()
//--------------------------------
{
	g_p_projection_matrix_stack.push_back(g_current_projection_matrix);
}


//-------------------------------
inline void pushModelViewMatrix()
//-------------------------------
{
	g_p_modelview_matrix_stack.push_back(g_current_modelview_matrix);
}


//-------------------------------
inline void popProjectionMatrix()
//-------------------------------
{
	// Make sure the stack is not empty
	if (g_p_projection_matrix_stack.size() == 0)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	// Restore the matrix
	g_current_projection_matrix = g_p_projection_matrix_stack.back();

	// Remove the last matrix
	g_p_projection_matrix_stack.pop_back();
}


//------------------------------
inline void popModelViewMatrix()
//------------------------------
{
	// Make sure the stack is not empty
	if (g_p_modelview_matrix_stack.size() == 0)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	// Restore the matrix
	g_current_modelview_matrix = g_p_modelview_matrix_stack.back();
	applyModelViewMatrix();

	// Remove the last matrix
	g_p_modelview_matrix_stack.pop_back();
}


//----------------------------------------
inline void loadIdentityProjectionMatrix()
//----------------------------------------
{
	g_current_projection_matrix.loadIdentity();
}


//---------------------------------------
inline void loadIdentityModelViewMatrix()
//---------------------------------------
{
	g_current_modelview_matrix.loadIdentity();
}


//---------------------------------------------------------
inline MATRIX4 buildOrthoProjectionMatrix(double left,
										  double right,
										  double bottom,
										  double top,
										  double nearPlane,
										  double farPlane)
//---------------------------------------------------------
{
	double t_x(-(right + left)   / (right - left));
	double t_y(-(top   + bottom) / (top   - bottom));
	double t_z(-(farPlane   + nearPlane)   / (farPlane   - nearPlane));

	return (MATRIX4(			
		2.0 / (right - left),
		0.0,
		0.0,
		0.0,
		
		0.0,
		2.0 / (top   - bottom),
		0.0,
		0.0,

		0.0,
		0.0,
		-2.0 / (farPlane   - nearPlane),
		0.0,

		t_x,
		t_y,
		t_z,
		1.0)
	);
}

//-----------------------------------------------------
inline void loadOrthoProjectionMatrix(double left,
									  double right,
									  double bottom,
									  double top,
									  double nearPlane,
									  double farPlane)
//-----------------------------------------------------
{
	g_current_projection_matrix = buildOrthoProjectionMatrix(left, right, bottom, top, nearPlane, farPlane);
}


//-------------------------------------------------------
inline void loadFrustumProjectionMatrix(double left,
										double right,
										double bottom,
										double top,
										double nearPlane,
										double farPlane)
//-------------------------------------------------------
{
	g_current_projection_matrix = buildFrustumProjectionMatrix(left, right, bottom, top, nearPlane, farPlane);
}


//-----------------------------------------------------------
inline MATRIX4 buildFrustumProjectionMatrix(double left,
										    double right,
										    double bottom,
										    double top,
										    double nearPlane,
										    double farPlane)
//-----------------------------------------------------------
{
	double a( (right    + left)        / (right    - left));
	double b( (top      + bottom)      / (top      - bottom));
	double c(-(farPlane + nearPlane)   / (farPlane - nearPlane));
	double d(-(2.0 * farPlane * nearPlane)   / (farPlane - nearPlane));

	return (MATRIX4(			
		2.0 * nearPlane / (right - left),
		0.0,
		0.0,
		0.0,
		
		0.0,
		2.0 * nearPlane / (top   - bottom),
		0.0,
		0.0,

		a,
		b,
		c,
		-1.0,

		0.0,
		0.0,
		d,
		0.0)
	);
}


//---------------------------------------------------------
inline void multiplyOrthoProjectionMatrix(double left,
										  double right,
										  double bottom,
										  double top,
										  double nearPlane,
										  double farPlane)
//---------------------------------------------------------
{
	g_current_projection_matrix *= buildOrthoProjectionMatrix(left, right, bottom, top, nearPlane, farPlane);
}


//------------------------------------------------------------
inline MATRIX4 buildPerspectiveProjectionMatrix(double fovy,
												double aspect,
												double zNear,
												double zFar)
//------------------------------------------------------------
{
	double f(tan(gVirtualXRay::PI_2 - (((fovy * gVirtualXRay::PI) / 180.0) / 2.0)));

	return (MATRIX4(			
		f / aspect,
		0.0,
		0.0,
		0.0,
			
		0.0,
		f,
		0.0,
		0.0,

		0.0,
		0.0,
		(zFar + zNear) / (zNear - zFar),
		-1.0,

		0.0,
		0.0,
		(2.0 * zFar * zNear) / (zNear - zFar),
		0.0)
	);
}


//--------------------------------------------------------
inline void loadPerspectiveProjectionMatrix(double fovy,
											double aspect,
											double zNear,
											double zFar)
//--------------------------------------------------------
{
	g_current_projection_matrix = buildPerspectiveProjectionMatrix(fovy, aspect, zNear, zFar);
}


//------------------------------------------------------------
inline void multiplyPerspectiveProjectionMatrix(double fovy,
												double aspect,
												double zNear,
												double zFar)
//------------------------------------------------------------
{	
	g_current_projection_matrix *= buildPerspectiveProjectionMatrix(fovy, aspect, zNear, zFar);
}


//-----------------------------------------------------------
inline MATRIX4 buildLookAtModelViewMatrix(const VEC3& eye,
										  const VEC3& center,
										  const VEC3& up)
//-----------------------------------------------------------
{
	// Forward vector: centre - eye
	VEC3 forward(center - eye);
	forward.normalize();

	// Up vector
	VEC3 up1(up.normal());

	// Side vector: forward x up
	VEC3 side(forward ^ up1);
	
	// New up vector: side x forward
	VEC3 u(side.normal() ^ forward);
		

	MATRIX4 matrix( 	
		side.getX(),
		u.getX(),
		-forward.getX(),
		0.0,
	
		side.getY(),
		u.getY(),
		-forward.getY(),
		0.0,
	
		side.getZ(),
		u.getZ(),
		-forward.getZ(),
		0.0,
	
		0.0,
		0.0,
		0.0,
		1.0);
	
	matrix.translate(-eye.getX(), -eye.getY(), -eye.getZ());
	
	return (matrix);
}


//-------------------------------------------------------
inline MATRIX4 buildLookAtModelViewMatrix(double eyeX,
										  double eyeY,
										  double eyeZ,
										  double centerX,
										  double centerY,
										  double centerZ,
										  double upX,
										  double upY,
										  double upZ)
//-------------------------------------------------------
{
	return (buildLookAtModelViewMatrix(VEC3(eyeX, eyeY, eyeZ), VEC3(centerX, centerY, centerZ), VEC3(upX, upY, upZ)));
}


//-------------------------------------------------------
inline void loadLookAtModelViewMatrix(const VEC3& eye,
									  const VEC3& center,
									  const VEC3& up)
//-------------------------------------------------------
{	
	g_current_modelview_matrix = buildLookAtModelViewMatrix(eye, center, up);
}


//---------------------------------------------------
inline void loadLookAtModelViewMatrix(double eyeX,
									  double eyeY,
									  double eyeZ,
									  double centerX,
									  double centerY,
									  double centerZ,
									  double upX,
									  double upY,
									  double upZ)
//---------------------------------------------------
{	
	g_current_modelview_matrix = buildLookAtModelViewMatrix(
			eyeX, eyeY, eyeZ,
			centerX, centerY, centerZ,
			upX, upY, upZ);
}


//---------------------
inline VBO* createVBO()
//---------------------
{
    if (useOpenGL3_2OrAbove())
    {
        return (createOpenGL3VBO());
    }
    else
    {
        return (createOpenGL2VBO());
    }
}


//-----------------------------------
inline OpenGL2VBO* createOpenGL2VBO()
//-----------------------------------
{
    return (new OpenGL2VBO);
}


//-----------------------------------
inline OpenGL3VBO* createOpenGL3VBO()
//-----------------------------------
{
    return (new OpenGL3VBO);
}


} // namespace gVirtualXRay
