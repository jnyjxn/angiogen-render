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


#ifndef __ImplicitSurface_h
#define __ImplicitSurface_h


/**
********************************************************************************
*
*   @file       ImplicitSurface.h
*
*   @brief      Class to build 3D implicit surfaces. To know what implicit
*               surfaces are, please visit Paul Bourke's tutorial available at
*               http://paulbourke.net/geometry/implicitsurf/
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
*   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <vector>

#ifndef __Types_h
#include "gVirtualXRay/Types.h"
#endif

#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  ImplicitSurface
*   @brief  ImplicitSurface is a class to build 3D implicit surfaces.
*/
//==============================================================================
class ImplicitSurface
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Type of density function.
    //--------------------------------------------------------------------------
    typedef enum DensityFunctionTypeTag
    {
        SPHERE,             ///< Sphere
        
        BLOBBY_MOLECULE,    ///< Blobby molecule, first created by Jim Blinn and
                            ///< modelled after electron density fields
        
        META_BALL,          ///< Meta balls
        
        SOFT_OBJECT         ///< Soft Objects, first created by
                            ///< the Wyvill brothers
    } DensityFunctionType;


    //--------------------------------------------------------------------------
    /// Type of density function parameter.
    //--------------------------------------------------------------------------
    class DensityFunctionParameterType
    {
    //**************************************************************************
    public:
        //----------------------------------------------------------------------
        /// Default constructor.
        /**
         *  @param aDensityFunctionType:    the type of density function
         *                                  (default value: BLOBBY_MOLECULE)
         *  @param a:                       the first parameter
         *                                  (default value: 1)
         *  @param b:                       the second parameter
         *                                  (default value: 1)
         *  @return the density
         */
        //----------------------------------------------------------------------
        DensityFunctionParameterType(
            const DensityFunctionType aDensityFunctionType = BLOBBY_MOLECULE,
            RATIONAL_NUMBER a = 1,
            RATIONAL_NUMBER b = 1):
        //----------------------------------------------------------------------
            m_density_function(aDensityFunctionType),
            m_a(a),
            m_b(b)
        //----------------------------------------------------------------------
        {}


        /// Type of density function
        DensityFunctionType m_density_function;
    
    
        /// First parameter of the density function
        RATIONAL_NUMBER m_a;
    
    
        /// Secnd parameter of the density function
        /// (not used if m_density_function is SPHERE)
        RATIONAL_NUMBER m_b;
    };


    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    ImplicitSurface();


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    ~ImplicitSurface();


    //--------------------------------------------------------------------------
    /// Evaluate the density function for the distance r.
    /**
     *  @param r:           the distance
     *  @param aParameter:  the parameter of the density function
     *  @return the density
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER evaluate(RATIONAL_NUMBER r,
            const DensityFunctionParameterType& aParameter) const;


    //--------------------------------------------------------------------------
    /// Remove all the control points.
    //--------------------------------------------------------------------------
    void resetControlPoints();


    //--------------------------------------------------------------------------
    /// Remove all the control lines.
    //--------------------------------------------------------------------------
    void resetControlLines();


    //--------------------------------------------------------------------------
    /// Add a control point.
    /**
     *  @param aParameter:      the parameter of the density function
     *  @param aControlPoint:   the control point to add
     */
    //--------------------------------------------------------------------------
    void addControlPoint(const DensityFunctionParameterType& aParameter,
            const VEC3& aControlPoint);


    //--------------------------------------------------------------------------
    /// Add a control line.
    /**
     *  @param aParameter:      the parameter of the density function
     *  @param aControlPoint1:  the first point that defines the line segment
     *  @param aControlPoint2:  the second point that defines the line segment
     */
    //--------------------------------------------------------------------------
    void addControlLine(const DensityFunctionParameterType& aParameter,
            const VEC3& aControlPoint1,
            const VEC3& aControlPoint2);


    //--------------------------------------------------------------------------
    /// Accessor on a given control point.
    /**
     *  @param anIndex: the index of the control point
     *  @return the corresponding control point
     */
    //--------------------------------------------------------------------------
    const VEC3& getControlPoint(unsigned int anIndex) const;


    //--------------------------------------------------------------------------
    /// Accessor on a given control point.
    /**
     *  @param anIndex: the index of the control point
     *  @return the corresponding control point
     */
    //--------------------------------------------------------------------------
    VEC3& getControlPoint(unsigned int anIndex);


    //--------------------------------------------------------------------------
    /// Accessor on a given control line.
    /**
     *  @param anIndex: the index of the control point
     *  @return the corresponding control line segment
     */
    //--------------------------------------------------------------------------
    const std::pair<VEC3, VEC3>& getControlLine(unsigned int anIndex) const;


    //--------------------------------------------------------------------------
    /// Accessor on a given control line.
    /**
     *  @param anIndex: the index of the control point
     *  @return the corresponding control line segment
     */
    //--------------------------------------------------------------------------
    std::pair<VEC3, VEC3>& getControlLine(unsigned int anIndex);


    //--------------------------------------------------------------------------
    /// Accessor on a given control line.
    /**
     *  @param aNumberOfVoxels: the number of voxels
     *  @param aCenter:         the 3D position of the centre of the dataset
     *  @param aVoxelSize:      the size of voxels
     */
    //--------------------------------------------------------------------------
    void voxelise(const Vec3ui& aNumberOfVoxels,
            const VEC3& aCenter,
            const VEC3& aVoxelSize);


    //--------------------------------------------------------------------------
    /// Accessor on a given control line.
    /**
     *  @param aNumberOfVoxels: the number of voxels
     *  @param aCenter:         the 3D position of the centre of the dataset
     *  @param aVoxelSize:      the size of voxels
     */
    //--------------------------------------------------------------------------
    void voxelize(const Vec3ui& aNumberOfVoxels,
            const VEC3& aCenter,
            const VEC3& aVoxelSize);


    //--------------------------------------------------------------------------
    /// Write the current voxel data into a file.
    /**
     *  @param aFileName:   the name of the file where to save the voxel data
     */
    //--------------------------------------------------------------------------
    void writeVoxelData(const char* aFileName) const;


    //--------------------------------------------------------------------------
    /// Write the current voxel data into a file.
    /**
     *  @param aFileName:   the name of the file where to save the voxel data
     */
    //--------------------------------------------------------------------------
    void writeVoxelData(const std::string& aFileName) const;


    //--------------------------------------------------------------------------
    /// Write the polygon data.
    /**
     *  @param aFileName:   the name of the file where to save the polygon data
     */
    //--------------------------------------------------------------------------
    void writePolygonMesh(const char* aFileName);


    //--------------------------------------------------------------------------
    /// Write the polygon data.
    /**
     *  @param aFileName:   the name of the file where to save the polygon data
     */
    //--------------------------------------------------------------------------
    void writePolygonMesh(const std::string& aFileName);


    //--------------------------------------------------------------------------
    /// Extract the surface from the voxel data corresponding to
    /// a given iso-value.
    /**
     *  @param aThreshold:  the threshold value
     */
    //--------------------------------------------------------------------------
    void extractIsoSurface(const RATIONAL_NUMBER& aThreshold = 0);


    //--------------------------------------------------------------------------
    /// Accessor on the polygon data.
    /**
     *  @return the polygon data
     */
    //--------------------------------------------------------------------------
    PolygonMesh& getPolygonMesh();
    
    
    //--------------------------------------------------------------------------
    /// Accessor on the polygon data.
    /**
     *  @return the polygon data
     */
    //--------------------------------------------------------------------------
    const PolygonMesh& getPolygonMesh() const;


//******************************************************************************
protected:
    RATIONAL_NUMBER evaluateSphere(RATIONAL_NUMBER r) const;
    
    
    RATIONAL_NUMBER evaluateBlobbyMolecule(RATIONAL_NUMBER r,
                                           RATIONAL_NUMBER a,
                                           RATIONAL_NUMBER b) const;
    
    
    RATIONAL_NUMBER evaluateMetaBall(RATIONAL_NUMBER r,
                                     RATIONAL_NUMBER a,
                                     RATIONAL_NUMBER b) const;
    
    
    RATIONAL_NUMBER evaluateSoftObject(RATIONAL_NUMBER r,
                                       RATIONAL_NUMBER a,
                                       RATIONAL_NUMBER b) const;

    
    Image<RATIONAL_NUMBER> m_volume;
    
    
    PolygonMesh m_mesh_data;
    
    
    VEC3 m_centre;

    
    std::vector<std::pair<DensityFunctionParameterType, VEC3> >
            m_control_point_set;
    
    
    std::vector<std::pair<DensityFunctionParameterType,
            std::pair<VEC3, VEC3> > > m_control_line_set;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "ImplicitSurface.inl"


#endif // __ImplicitSurface_h
