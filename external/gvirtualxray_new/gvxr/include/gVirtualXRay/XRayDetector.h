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


#ifndef __XRayDetector_h
#define __XRayDetector_h


/**
********************************************************************************
*
*   @file       XRayDetector.h
*
*   @brief      Class to handle virtual X-Ray detectors.
*
*   @version    1.0
*
*   @date       13/01/2017
*
*   @author     Dr Franck P. Vidal
*
*   @todo       Create a class to draw spheres
*               Use this class to display point sources
*               (see line 80 in file XRayDetector.inl)
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
#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <memory>
#include <vector> // To store the energy response
#include <utility> // To create STL pairs

#ifndef TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_MATRIX4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_VBO_H
#include "gVirtualXRay/VBO.h"
#endif

#ifndef GVXR_SPHERE_MESH_H
#include "gVirtualXRay/SphereMesh.h"
#endif

#ifndef GVXR_UBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  XRayDetector
*   @brief  XRayDetector is a class to handle a X-ray detector.
*/
//==============================================================================
class XRayDetector
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    /// SourceShape is a type that defines the shape of X-ray sources.
    typedef enum SourceShapeTag
    {
        POINT,        ///< Infinitively small point source
        SQUARE,       ///< Source whose shape corresponds to a square
        CUBE,         ///< Source whose shape corresponds to a cube
        LINE,         ///< Source whose shape corresponds to a line segment
        PARALLEL,     ///< Parallel source,
                      ///< i.e. a point source infinitely far away
        UNKNOWN_SHAPE ///< Unknown type
    } SourceShape;


    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    XRayDetector();


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    ~XRayDetector();


    //--------------------------------------------------------------------------
    /// Release the data.
    //--------------------------------------------------------------------------
        void release();


    //--------------------------------------------------------------------------
    /// Display the detector using OpenGL.
    //--------------------------------------------------------------------------
    void displayDetector();


    //--------------------------------------------------------------------------
    /// Display the source using OpenGL.
    //--------------------------------------------------------------------------
    void displaySource();


    //--------------------------------------------------------------------------
    /// Display the source using OpenGL.
    //--------------------------------------------------------------------------
    void displaySourceWireframe();


    //--------------------------------------------------------------------------
    /// Display the beam using OpenGL.
    //--------------------------------------------------------------------------
    void displayBeam();


    //--------------------------------------------------------------------------
    /// Set the pixel resolution (in units of length per pixel,
    /// i.e. the inverse of the pixel size)
    /**
    *   @param aResolution: the pixel resolution
    */
    //--------------------------------------------------------------------------
    void setResolutionInUnitsOfLengthPerPixel(
            const RATIONAL_NUMBER& aResolution);


    //--------------------------------------------------------------------------
    /// Set the pixel resolution (in units of length per pixel,
    /// i.e. the inverse of the pixel size)
    /**
    *   @param aResolution: the pixel resolution
    */
    //--------------------------------------------------------------------------
    void setResolutionInUnitsOfLengthPerPixel(const VEC2& aResolution);


    //--------------------------------------------------------------------------
    /// Set the number of pixels.
    /**
     *  @param aSizeInPixels:   the number of pixels
     */
    //--------------------------------------------------------------------------
    void setNumberOfPixels(const Vec2ui& aSizeInPixels);


    //--------------------------------------------------------------------------
    /// Set the position of the X-ray detector.
    /**
     *  @param aDetectorPosition:   the position of the X-ray detector
     */
    //--------------------------------------------------------------------------
    void setDetectorPosition(const VEC3& aDetectorPosition);


    //--------------------------------------------------------------------------
    /// Set the position of the X-ray source.
    /**
     *  @param aXraySourcePosition: the position of the X-ray source
     */
    //--------------------------------------------------------------------------
    void setXrayPointSource(const VEC3& aXraySourcePosition);


    //--------------------------------------------------------------------------
    /// Set the shape of the source as a cube.
    /**
     *  @param aXraySourcePosition:     the position of the centre of the source
     *  @param aNumberOfSamplesAlongOneDirection:   the number of point sources
     *                      along one axis (in total  there will be
     *                      aNumberOfSamplesAlongOneDirection^3 point sources
     *                      in the cube)
     *  @param aLength: the length of the cube
     */
    //--------------------------------------------------------------------------
    void setCubicSource(const VEC3& aXraySourcePosition,
                        unsigned int aNumberOfSamplesAlongOneDirection,
                        RATIONAL_NUMBER aLength);


    //--------------------------------------------------------------------------
    /// Set the shape of the source as a square.
    /**
     *  @param aXraySourcePosition:     the position of the centre of the source
     *  @param aNumberOfSamplesAlongOneDirection:   the number of point sources
     *                          along one axis (in total there will be
     *                          aNumberOfSamplesAlongOneDirection^2 point
     *                          sources in the square)
     *  @param aLength: the length of the square
     */
    //--------------------------------------------------------------------------
    void setSquareSource(const VEC3& aXraySourcePosition,
                         unsigned int aNumberOfSamplesAlongOneDirection,
                         RATIONAL_NUMBER aLength);


    //--------------------------------------------------------------------------
    /// Set the shape of the source as a line segment.
    /**
     *  @param aXraySourcePosition: the position of the centre of the source
     *  @param aDirection:  the direction vector of the line
     *  @param aNumberOfSamplesAlongOneDirection:   the number of point sources
     *  @param aLength: the length of the line
     */
    //--------------------------------------------------------------------------
    void setLineSource(const VEC3& aXraySourcePosition,
                       const VEC3& aDirection,
                       unsigned int aNumberOfSamplesAlongOneDirection,
                       RATIONAL_NUMBER aLength);


    //--------------------------------------------------------------------------
    /// Set the shape of the source as an infinitely small point.
    //--------------------------------------------------------------------------
    void setXrayPointSource();


    //--------------------------------------------------------------------------
    /// Set the shape of the source as a cube.
    //--------------------------------------------------------------------------
    void setCubicSource();


    //--------------------------------------------------------------------------
    /// Set the shape of the source as a square.
    //--------------------------------------------------------------------------
    void setSquareSource();


    //--------------------------------------------------------------------------
    /// Set the shape of the source as a line segment.
    //--------------------------------------------------------------------------
    void setLineSource();


    //--------------------------------------------------------------------------
    /// Set the up vector that defines the detector's orientation.
    /**
     *  @param anUpVector:  the up vector
     */
    //--------------------------------------------------------------------------
    void setUpVector(const VEC3& anUpVector);


    //--------------------------------------------------------------------------
    /// Set the rotation matrix.
    /**
     *  @param aMatrix: the rotation matrix
     */
    //--------------------------------------------------------------------------
    void setRotationMatrix(const MATRIX4& aMatrix);


    //--------------------------------------------------------------------------
    /// Set the projection mode to parallel beam.
    //--------------------------------------------------------------------------
    void setParallelBeam();


    //--------------------------------------------------------------------------
    /// Set the projection mode to cone beam (same as point source).
    //--------------------------------------------------------------------------
    void setConeBeam();


    //--------------------------------------------------------------------------
    /// Set the projection mode to point source (same as cone beam).
    //--------------------------------------------------------------------------
    void setPointSource();


    //--------------------------------------------------------------------------
    /// Accessor on the number of pixels.
    /**
     *  @return the number of pixels
     */
    //--------------------------------------------------------------------------
    const Vec2ui& getNumberOfPixels() const;


    //--------------------------------------------------------------------------
    /// Accessor on the size of the detector (in unit of length).
    /**
     *  @return the size of the detector
     */
    //--------------------------------------------------------------------------
    const VEC2& getSizeInUnitOfLength() const;


    //--------------------------------------------------------------------------
    /// Accessor on the detector position.
    /**
     *  @return the detector position
     */
    //--------------------------------------------------------------------------
    const VEC3& getDetectorPosition() const;


    //--------------------------------------------------------------------------
    /// Accessor on the centre of the source.
    /**
     *  @return the source position
     */
    //--------------------------------------------------------------------------
    const VEC3& getXraySourceCentre() const;


    unsigned int getNumberOfSourceSamples()const;


    const VEC3& getSourceSamplePosition(unsigned int aSampleID) const;


    //--------------------------------------------------------------------------
    /// Accessor on the up vector, which defines the orientation of the detector.
    /**
     *  @return the up vector
     */
    //--------------------------------------------------------------------------
    const VEC3& getUpVector() const;


    //--------------------------------------------------------------------------
    /// Accessor on the rotation matrix.
    /**
     *  @return the rotation matrix
     */
    //--------------------------------------------------------------------------
    const MATRIX4& getRotationMatrix() const;


    //--------------------------------------------------------------------------
    /// Accessor on the vector normal to the detector.
    //--------------------------------------------------------------------------
    const VEC3& getNormal() const;


    //--------------------------------------------------------------------------
    /// Accessor on the cross product between m_look_at_vector and m_up_vector.
    /**
     *  @return the cross product
     */
    //--------------------------------------------------------------------------
    const VEC3& getRightVector() const;


    //--------------------------------------------------------------------------
    /// Accessor on the plane equation (Ax + By + Cz + D = 0).
    /**
     *  @param A: the 1st component of the plane equation
     *  @param B: the 2nd component of the plane equation
     *  @param C: the 3rd component of the plane equation
     *  @param D: the 4th component of the plane equation
     */
    //--------------------------------------------------------------------------
    void getPlaneEquation(RATIONAL_NUMBER& A,
                                RATIONAL_NUMBER& B,
                                RATIONAL_NUMBER& C,
                                RATIONAL_NUMBER& D);


    //--------------------------------------------------------------------------
    /// Accessor on the shape of the source.
    /**
     *  @return true if a parallel beam is used (e.g. orthographic projection),
     *          false if perspective projection is used
     */
    //--------------------------------------------------------------------------
    SourceShape getSourceShape() const;


    const std::vector<VEC3>& getSourcePositionSet() const;


    void setSourcePositions(const std::vector<VEC3>& aSourcePostionSet);


    //--------------------------------------------------------------------------
    /// Accessor on the flag about projection.
    /**
     *  @return true if a parallel beam is used (e.g. orthographic projection),
     *          false if perspective projection is used
     */
    //--------------------------------------------------------------------------
    bool useParallelBeam() const;


    //--------------------------------------------------------------------------
    /// Accessor on the view along the Y-axis.
    /**
     *  @return the view along the Y-axis
     */
    //--------------------------------------------------------------------------
    const RATIONAL_NUMBER& getFOVY() const;


    const VEC2& getPixelSizeInUnitOfLength() const;


    //--------------------------------------------------------------------------
    /// Clear the energy response of the detector.
    //--------------------------------------------------------------------------
    void clearEnergyResponse();


    //--------------------------------------------------------------------------
    /// Load the energy response of the detector from a TSV file.
    /**
     *  @param aFileName: name of the file. Each line of the file is formatted as follows:
     *                    input_energy  output_energy
     *  @param aUnitOfEnergy: the unit of energy corresponding to the data in the file
     */
    //--------------------------------------------------------------------------
    void loadEnergyResponse(const std::string& aFileName,
        const RATIONAL_NUMBER& aUnitOfEnergy);


    //--------------------------------------------------------------------------
    /// Apply the energy response if any.
    /**
    *  @param aUnitOfEnergy: the input energy
    *  @return the output energy
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER applyEnergyResponse(const RATIONAL_NUMBER& anEnergy) const;


//******************************************************************************
protected:
    //--------------------------------------------------------------------------
    /// Compute the size of the detector.
    //--------------------------------------------------------------------------
    void updateSizeInUnitOfLength();


    //--------------------------------------------------------------------------
    /// Compute the distance between the source and the detector, etc.
    //--------------------------------------------------------------------------
    void updateInternalValues();


    //--------------------------------------------------------------------------
    /// Update the VBOs.
    //--------------------------------------------------------------------------
    void updateVBOs();


    //--------------------------------------------------------------------------
    /// Update the VBO of the detector.
    //--------------------------------------------------------------------------
    void updateDetectorVBO();


    //--------------------------------------------------------------------------
    /// Update the VBO of the parallel beam.
    //--------------------------------------------------------------------------
    void updateParallelBeamVBO();


    //--------------------------------------------------------------------------
    /// Update the VBO of the cone beam.
    //--------------------------------------------------------------------------
    void updateConeBeamVBO();


    //--------------------------------------------------------------------------
    /// Update the view along the Y-axis.
    //--------------------------------------------------------------------------
    void updateFOVY();


    //--------------------------------------------------------------------------
    /// Find the index of the closest input energy in the energy response.
    /**
    *  @return the index of the closest input energy
    */
    //--------------------------------------------------------------------------
    int findNearestEnergyIdx(const RATIONAL_NUMBER& anEnergy) const;


     /// The position of the detector centre
    VEC3            m_detector_position;


    /// The position of the source centre
    VEC3            m_xray_source_centre;


    /// The sample positions of the source
    std::vector<VEC3> m_p_xray_source_position_set;


    /// The "up" vector the the detector
    VEC3            m_up_vector;


    /// The rotation matrix
    MATRIX4         m_rotation_matrix;


    /// The size of the detector
    VEC2            m_size_in_unit_of_length;


    /// Resolution (in unit of length per pixel)
    VEC2 m_resolution_in_unit_of_length_per_pixel;


    /// Distance between the source and the detector
    RATIONAL_NUMBER m_distance_source_detector;


    /// Size of the detector (in number of pixels)
    Vec2ui          m_size_in_number_of_pixels;


    /// A flag set to true when the VBOs need to be updated
    bool m_vbo_need_update;


    /// The triangle mesh corresponding to the detector
#if __cplusplus <= 199711L
    std::auto_ptr<VBO> m_detector_geometry;
#else
    std::unique_ptr<VBO> m_detector_geometry;
#endif


    /// The triangle mesh corresponding to the parallel beam
#if __cplusplus <= 199711L
    std::auto_ptr<VBO> m_parallel_beam;
#else
    std::unique_ptr<VBO> m_parallel_beam;
#endif


    /// The triangle mesh corresponding to the cone beam
#if __cplusplus <= 199711L
    std::auto_ptr<VBO> m_cone_beam;
#else
    std::unique_ptr<VBO> m_cone_beam;
#endif


    /// The triangle mesh corresponding to the parallel beam highlight
#if __cplusplus <= 199711L
    std::auto_ptr<VBO> m_parallel_beam_highlight;
#else
    std::unique_ptr<VBO> m_parallel_beam_highlight;
#endif


    /// The triangle mesh corresponding to the cone beam highlight
#if __cplusplus <= 199711L
    std::auto_ptr<VBO> m_cone_beam_highlight;
#else
    std::unique_ptr<VBO> m_cone_beam_highlight;
#endif


    /// Polygon mesh of the source
    SphereMesh m_source_mesh;


    /// Source type
    SourceShape m_source_shape;


    /// The vector normal to the detector
    VEC3 m_normal;


    /// The viewing direction (i.e. the unit vector from the X-ray source
    /// to the centre of the detector)
    VEC3 m_look_at_vector;


    /// The cross product between m_look_at_vector and m_up_vector
    VEC3 m_right_vector;


    /// Field of view along the Y-axis
    RATIONAL_NUMBER m_fovy;


    /// Energy response. The keys of the map are the input energies
    std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> > m_energy_response;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "XRayDetector.inl"


#endif // __XRayDetector_h
