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


#ifndef __Types_h
#define __Types_h


/**
********************************************************************************
*
*   @file       Types.h
*
*   @brief      Type declarations.
*
*   @version    1.0
*
*   @date       13/01/2017
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
*   http://www.fpvidal.net/, Jan 2017, 2017, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <limits>

#ifndef __Vec3_h
#include "gVirtualXRay/Vec3.h"
#endif

#ifndef __Vec2_h
#include "gVirtualXRay/Vec2.h"
#endif

#ifndef __Matrix4x4_h
#include "gVirtualXRay/Matrix4x4.h"
#endif


//******************************************************************************
//  define
//******************************************************************************


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  type declaration
//******************************************************************************
/// Type of data used to store sinograms
typedef float                    SinogramType;


/// Type of data used to store real numbers
typedef float                    RATIONAL_NUMBER;


/// Type of data used to store 2D vectors
typedef Vec2<RATIONAL_NUMBER>      VEC2;


/// Type of data used to store 3D vectors
typedef Vec3<RATIONAL_NUMBER>      VEC3;


/// Type of data used to store 4x4 matrices
typedef Matrix4x4<RATIONAL_NUMBER> MATRIX4;


//******************************************************************************
//  class declaration
//******************************************************************************
//==============================================================================
/**
 *  @typedef Vec2d
 *  @brief  Vec2d is a class to handle a 2D vector using double precision
 *          floating point numbers.
 */
//==============================================================================
typedef Vec2<double> Vec2d;


//==============================================================================
/**
 *  @typedef Vec2f
 *  @brief  Vec2f is a class to handle a 2D vector using double precision
 *          floating point numbers.
 */
//==============================================================================
typedef Vec2<float> Vec2f;


//==============================================================================
/**
 *  @typedef Vec2i
 *  @brief  Vec2i is a class to handle a 2D vector using 32 bits integers.
 */
//==============================================================================
typedef Vec2<int> Vec2i;


//==============================================================================
/**
 *  @typedef Vec2ui
 *  @brief  Vec2ui is a class to handle a 2D vector using
 *          unsigned 32 bits integers.
 */
//==============================================================================
typedef Vec2<unsigned int> Vec2ui;


//==============================================================================
/**
 *  @typedef Vec3d
 *  @brief  Vec3d is a class to handle a 3D vector using double precision
 *          floating point numbers.
 */
//==============================================================================
typedef Vec3<double> Vec3d;


//==============================================================================
/**
 *  @typedef Vec3f
 *  @brief  Vec3f is a class to handle a 3D vector using double precision
 *          floating point numbers.
 */
//==============================================================================
typedef Vec3<float> Vec3f;


//==============================================================================
/**
 *  @typedef Vec3i
 *  @brief  Vec3i is a class to handle a 3D vector using
 *          signed 32 bits integers.
 */
//==============================================================================
typedef Vec3<int> Vec3i;


//==============================================================================
/**
 *  @typedef Vec3ui
 *  @brief  Vec3ui is a class to handle a 3D vector using
 *          unsigned 32 bits integers.
 */
//==============================================================================
typedef Vec3<unsigned int> Vec3ui;


//==============================================================================
/**
 *  @typedef Matrix4x4d
 *  @brief  Matrix4x4d is a class to handle a 4x4 matrix
 *          using double precision floating point numbers.
 */
//==============================================================================
typedef Matrix4x4<double> Matrix4x4d;


//==============================================================================
/**
 *  @typedef Matrix4x4f
 *  @brief  Matrix4x4f is a class to handle a 4x4 matrix
 *          using single precision floating point numbers.
 */
//==============================================================================
typedef Matrix4x4<float> Matrix4x4f;


//******************************************************************************
//  function declaration
//******************************************************************************


} // namespace gVirtualXRay


#endif // __Types_h
