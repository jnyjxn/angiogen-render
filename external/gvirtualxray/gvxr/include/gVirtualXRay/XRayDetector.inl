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
*	@file		XRayDetector.inl
*
*	@brief		Class to handle virtual X-Ray detectors.
*
*	@version	1.0
*
*	@date		30/11/2013
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
#include <fstream>
#include <iostream>


#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//----------------------------------------------
inline XRayDetector::XRayDetector():
//----------------------------------------------
	m_resolution_in_unit_of_length_per_pixel(0),
	m_distance_source_detector(0),
	m_vbo_need_update(true),
	//m_source_mesh(10, 10, 0.5 * cm),
	m_source_mesh(2, 0.5 * cm),
    m_source_shape(UNKNOWN_SHAPE)
//----------------------------------------------
{
}


//----------------------------------
inline XRayDetector::~XRayDetector()
//----------------------------------
{
}


//---------------------------------
inline void XRayDetector::release()
//---------------------------------
{
    m_p_xray_source_position_set.clear();
    m_detector_geometry.reset();
    m_parallel_beam.reset();
    m_cone_beam.reset();
    m_parallel_beam_highlight.reset();
    m_cone_beam_highlight.reset();
    m_source_mesh.reset();
}


//---------------------------------------
inline void XRayDetector::displaySource()
//---------------------------------------
{
    // Display all the sub-sources
    for (std::vector<VEC3>::const_iterator ite(m_p_xray_source_position_set.begin());
            ite != m_p_xray_source_position_set.end();
            ++ite)
    {
        // Store the model/view matrix
        pushModelViewMatrix();

        g_current_modelview_matrix *= MATRIX4::buildTranslationMatrix(ite->getX(), ite->getY(), ite->getZ());
        applyModelViewMatrix();

        // Draw sphere
        m_source_mesh.display();

        // Restore the model/view matrix
        popModelViewMatrix();
    }
}


//------------------------------------------------
inline void XRayDetector::displaySourceWireframe()
//------------------------------------------------
{
    // Display all the sub-sources
    for (std::vector<VEC3>::const_iterator ite(m_p_xray_source_position_set.begin());
            ite != m_p_xray_source_position_set.end();
            ++ite)
    {
        // Store the model/view matrix
        pushModelViewMatrix();

        g_current_modelview_matrix *= MATRIX4::buildTranslationMatrix(ite->getX(), ite->getY(), ite->getZ());
        applyModelViewMatrix();

        // Draw sphere
        m_source_mesh.displayWireFrame();

        // Restore the model/view matrix
        popModelViewMatrix();
    }
}


//------------------------------------------------------------------------------------------------
inline void XRayDetector::setResolutionInUnitsOfLengthPerPixel(const RATIONAL_NUMBER& aResolution)
//------------------------------------------------------------------------------------------------
{
    // The resolution has changed
    m_resolution_in_unit_of_length_per_pixel = VEC2(aResolution, aResolution);

    // Update the size of the detector
    updateSizeInUnitOfLength();

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-------------------------------------------------------------------------------------
inline void XRayDetector::setResolutionInUnitsOfLengthPerPixel(const VEC2& aResolution)
//-------------------------------------------------------------------------------------
{
    // The resolution has changed
    m_resolution_in_unit_of_length_per_pixel = aResolution;

    // Update the size of the detector
    updateSizeInUnitOfLength();

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//----------------------------------------------------------------------
inline void XRayDetector::setNumberOfPixels(const Vec2ui& aSizeInPixels)
//----------------------------------------------------------------------
{
	// The number of pixels has changed
	m_size_in_number_of_pixels = aSizeInPixels;

	// Update the size of the detector
	updateSizeInUnitOfLength();

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//--------------------------------------------------------------------------
inline void XRayDetector::setDetectorPosition(const VEC3& aDetectorPosition)
//--------------------------------------------------------------------------
{
	// The position of the source has changed
	m_detector_position = aDetectorPosition;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//---------------------------------------------------------------------------
inline void XRayDetector::setXrayPointSource(const VEC3& aXraySourcePosition)
//---------------------------------------------------------------------------
{
	// Store the centre position
	m_xray_source_centre = aXraySourcePosition;

	// Clear the sample set
	m_p_xray_source_position_set.clear();

	// Add the centre
	m_p_xray_source_position_set.push_back(m_xray_source_centre);

	// Save the source type
	m_source_shape = POINT;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//--------------------------------------------
inline void XRayDetector::setXrayPointSource()
//--------------------------------------------
{
	// Save the source type
	m_source_shape = POINT;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//----------------------------------------
inline void XRayDetector::setCubicSource()
//----------------------------------------
{
	// Save the source type
	m_source_shape = CUBE;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-----------------------------------------
inline void XRayDetector::setSquareSource()
//-----------------------------------------
{
	// Save the source type
	m_source_shape = SQUARE;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//---------------------------------------
inline void XRayDetector::setLineSource()
//---------------------------------------
{
	// Save the source type
	m_source_shape = LINE;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-----------------------------------------------------------
inline void XRayDetector::setUpVector(const VEC3& anUpVector)
//-----------------------------------------------------------
{
	m_up_vector = anUpVector;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-----------------------------------------------------------------
inline void XRayDetector::setRotationMatrix(const MATRIX4& aMatrix)
//-----------------------------------------------------------------
{
	m_rotation_matrix = aMatrix;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-----------------------------------------
inline void XRayDetector::setParallelBeam()
//-----------------------------------------
{
	// Save the source type
	m_source_shape = PARALLEL;

	// Clear the sample set
	m_p_xray_source_position_set.clear();

	// Add the centre
	m_p_xray_source_position_set.push_back(m_xray_source_centre);

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-------------------------------------
inline void XRayDetector::setConeBeam()
//-------------------------------------
{
	// Save the source type
	m_source_shape = POINT;

	// Clear the sample set
	m_p_xray_source_position_set.clear();

	// Add the centre
	m_p_xray_source_position_set.push_back(m_xray_source_centre);

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//----------------------------------------
inline void XRayDetector::setPointSource()
//----------------------------------------
{
	setConeBeam();
}


//----------------------------------------------------------
inline const Vec2ui& XRayDetector::getNumberOfPixels() const
//----------------------------------------------------------
{
	return (m_size_in_number_of_pixels);
}


//------------------------------------------------------------
inline const VEC2& XRayDetector::getSizeInUnitOfLength() const
//------------------------------------------------------------
{
	return (m_size_in_unit_of_length);
}


//----------------------------------------------------------
inline const VEC3& XRayDetector::getDetectorPosition() const
//----------------------------------------------------------
{
	return (m_detector_position);
}


//----------------------------------------------------------
inline const VEC3& XRayDetector::getXraySourceCentre() const
//----------------------------------------------------------
{
	return (m_xray_source_centre);
}


//----------------------------------------------------------------
inline unsigned int XRayDetector::getNumberOfSourceSamples() const
//----------------------------------------------------------------
{
	return (m_p_xray_source_position_set.size());
}


//------------------------------------------------------------------------------------
inline const VEC3& XRayDetector::getSourceSamplePosition(unsigned int aSampleID) const
//------------------------------------------------------------------------------------
{
	return (m_p_xray_source_position_set[aSampleID]);
}


//--------------------------------------------------
inline const VEC3& XRayDetector::getUpVector() const
//--------------------------------------------------
{
	return (m_up_vector);
}


//-----------------------------------------------------
inline const VEC3& XRayDetector::getRightVector() const
//-----------------------------------------------------
{
	return (m_right_vector);
}


//-----------------------------------------------------------
inline const MATRIX4& XRayDetector::getRotationMatrix() const
//-----------------------------------------------------------
{
	return (m_rotation_matrix);
}


//-------------------------------------------------------------------
inline XRayDetector::SourceShape XRayDetector::getSourceShape() const
//-------------------------------------------------------------------
{
	return (m_source_shape);
}


//------------------------------------------------------------------------
inline const std::vector<VEC3>& XRayDetector::getSourcePositionSet() const
//------------------------------------------------------------------------
{
	return (m_p_xray_source_position_set);
}


//--------------------------------------------------------------------------------------
inline void XRayDetector::setSourcePositions(const std::vector<VEC3>& aSourcePostionSet)
//--------------------------------------------------------------------------------------
{
	// Set the position of sources
	m_p_xray_source_position_set = aSourcePostionSet;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-----------------------------------------------
inline bool XRayDetector::useParallelBeam() const
//-----------------------------------------------
{
	return (m_source_shape == PARALLEL);
}


//---------------------------------------------------------
inline const RATIONAL_NUMBER& XRayDetector::getFOVY() const
//---------------------------------------------------------
{
    return (m_fovy);
}


//--------------------------------------------------
inline void XRayDetector::updateSizeInUnitOfLength()
//--------------------------------------------------
{
	m_size_in_unit_of_length.setX(m_resolution_in_unit_of_length_per_pixel.getX() * m_size_in_number_of_pixels.getX());
	m_size_in_unit_of_length.setY(m_resolution_in_unit_of_length_per_pixel.getY() * m_size_in_number_of_pixels.getY());
}


//-----------------------------------------------------------------
inline const VEC2& XRayDetector::getPixelSizeInUnitOfLength() const
//-----------------------------------------------------------------
{
	return (m_resolution_in_unit_of_length_per_pixel);
}


//----------------------------------------------
inline void XRayDetector::updateInternalValues()
//----------------------------------------------
{
	// The distance between the source and the detector has changed
	m_distance_source_detector = (m_detector_position - m_xray_source_centre).length();

	// Compute the normal vector
	m_look_at_vector = m_detector_position - m_xray_source_centre;
	m_look_at_vector.normalize();
	m_right_vector = m_look_at_vector ^ m_up_vector;
	m_normal = m_up_vector ^ m_right_vector;

	// Update the view along the Y-axis
	updateFOVY();
}


//------------------------------------------------------------------
inline void XRayDetector::getPlaneEquation(RATIONAL_NUMBER& A,
												 RATIONAL_NUMBER& B,
												 RATIONAL_NUMBER& C,
												 RATIONAL_NUMBER& D)
//------------------------------------------------------------------
{
	// Transform the normal
	VEC3 normal(m_rotation_matrix * m_normal);

	// Compute the plane equation
	A = normal.getX();
	B = normal.getY();
	C = normal.getZ();
	D = -(A * m_detector_position.getX() + B * m_detector_position.getY() + C * m_detector_position.getZ());
}


//------------------------------------
inline void XRayDetector::updateFOVY()
//------------------------------------
{
    m_fovy = 2.0 * (180.0  / PI) * atan2(RATIONAL_NUMBER(m_size_in_unit_of_length.getY() / 2.0), m_distance_source_detector);
}


} // namespace gVirtualXRay
