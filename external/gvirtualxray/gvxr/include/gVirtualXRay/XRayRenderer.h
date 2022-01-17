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


#ifndef __XRayRenderer_h
#define __XRayRenderer_h


/**
********************************************************************************
*
*   @file       XRayRenderer.h
*
*   @brief      Class to compute and renderer X-ray images on GPU.
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

#include <vector>
#include <map>
#include <string>
#include <memory>

#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif

#ifndef __Sinogram_h
#include "gVirtualXRay/Sinogram.h"
#endif

#ifndef __SceneGraphNode_h
#include "gVirtualXRay/SceneGraphNode.h"
#endif

#ifndef __PolygonMesh_h
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef __XRayDetector_h
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef __XRayBeam_h
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef __Shader_h
#include "gVirtualXRay/Shader.h"
#endif

#ifndef __AttenuationCoefficient_h
#include "gVirtualXRay/AttenuationCoefficient.h"
#endif

#ifndef __VBO_h
#include "gVirtualXRay/VBO.h"
#endif


//******************************************************************************
//  Definitions
//******************************************************************************
#define XRAY_DETECTOR_PBO_NUMBER        2   ///< The total number of PBOs
#define XRAY_DETECTOR_2D_TEXTURE_NUMBER 4   ///< The total number of 2D textures
#define XRAY_DETECTOR_3D_TEXTURE_NUMBER 1   ///< The total number of 3D textures
#define XRAY_DETECTOR_SHADER_NUMBER     7   ///< The total number of
                                            ///< program shaders


//******************************************************************************
//  Class definition
//******************************************************************************
class FramebufferObject;


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  XRayRenderer
*   @brief  XRayRenderer is a class to compute and renderer X-ray images on GPU.
*/
//==============================================================================
class XRayRenderer
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    typedef RATIONAL_NUMBER PixelType; ///< Pixel type


    //--------------------------------------------------------------------------
    /// Type to store the computation engine (OpenGL, CUDA, OpenCL, etc.).
    //--------------------------------------------------------------------------
    typedef enum ComputingModeTag
    {
        OPENGL, ///< Computation using OpenGL
        CUDA,   ///< Computation using CUDA
        OPENCL  ///< Computation using OpenCL
    } ComputingMode;


    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    XRayRenderer();
    

    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    ~XRayRenderer();


    //--------------------------------------------------------------------------
    /// Initialise the renderer.
    /**
     *  @param aComputingMode:          the computing mode (OPENGL, CUDA, or
     *                                  OPENCL)
     *  @param anInternalTextureFormat: the internal format in which OpenGL
     *                                  textures will be stored (GL_RGB16F_ARB,
     *                                  or GL_RGB32F_ARB)
     *  @param apDetector:              the X-ray detector that will be used
     *  @param apBeamSpectrum:          the X-ray beam spectrum
     */
    //--------------------------------------------------------------------------
    void initialise(ComputingMode aComputingMode,
            int anInternalTextureFormat,
            const XRayDetector* apDetector,
            const XRayBeam* apBeamSpectrum);


    //--------------------------------------------------------------------------
    /// Reset the renderer (release memory on the host and the device,
    /// and restore default values).
    //--------------------------------------------------------------------------
    void release();


    //--------------------------------------------------------------------------
    /// Check if the renderer is ready to simulate X-ray images.
    /**
     *  @return true if the renderer is ready, false if it is not
     */
    //--------------------------------------------------------------------------
    bool isReady() const;


    //--------------------------------------------------------------------------
    /// Add a 3D object to be rendered. It will be used as an internal surface.
    /**
     *  @param apSurface:   the 3D object
     */
    //--------------------------------------------------------------------------
    void addInnerSurface(const SceneGraphNode* apSurface);


    //--------------------------------------------------------------------------
    /// Add a 3D object to be rendered. It will be used as an external surface.
    /**
     *  @param apSurface:   the 3D object
     */
    //--------------------------------------------------------------------------
    void addOuterSurface(const SceneGraphNode* apSurface);


    //--------------------------------------------------------------------------
    /// Add a 3D object to be rendered. It will be used as an internal surface.
    /**
     *  @param apSurface:   the 3D object
     */
    //--------------------------------------------------------------------------
    void addInnerSurface(const PolygonMesh* apSurface);


    //--------------------------------------------------------------------------
    /// Add a 3D object to be rendered. It will be used as an external surface.
    /**
     *  @param apSurface:   the 3D object
     */
    //--------------------------------------------------------------------------
    void addOuterSurface(const PolygonMesh* apSurface);


    //--------------------------------------------------------------------------
    /// Remove the external surface from the renderer if any.
    //--------------------------------------------------------------------------
    void removeInnerSurfaces();


    //--------------------------------------------------------------------------
    /// Remove the internal surfaces from the renderer if any.
    //--------------------------------------------------------------------------
    void removeOuterSurface();


    //--------------------------------------------------------------------------
    /// Compute the L-buffer of a 3D object
    /**
     *  @param anObject:                        the 3D object to process
     *  @param aXRaySourceDetectorTranslation:  the translation vector to use
     *                                          when the source is not aligned
     *                                          with the detector
     *  @param aModellingTransformationMatrix:  the transformation matrix of
     *                                          the 3D object
     */
    //--------------------------------------------------------------------------
    void computeLBuffer(SceneGraphNode& anObject,
                        const VEC3& aXRaySourceDetectorTranslation,
                        const MATRIX4& aModellingTransformationMatrix,
                        bool aGPUStateConservationFlag = true);


    //--------------------------------------------------------------------------
    /// Compute the L-buffer of a 3D object
    /**
     *  @param anObject:                        the 3D object to process
     *  @param aXRaySourceDetectorTranslation:  the translation vector to use
     *                                          when the source is not aligned
     *                                          with the detector
     *  @param aModellingTransformationMatrix:  the transformation matrix of
     *                                          the 3D object
     */
    //--------------------------------------------------------------------------
    void computeLBuffer(PolygonMesh& anObject,
                        const VEC3& aXRaySourceDetectorTranslation,
                        const MATRIX4& aModellingTransformationMatrix,
                        bool aGPUStateConservationFlag = true);


    //--------------------------------------------------------------------------
    /// Compute the X-ray image.
    /**
     *  @param aModellingTransformationMatrix:  a modelling transformation
     *                                          matrix that will be used on all
     *                                          the 3D objects
     */
    //--------------------------------------------------------------------------
    void computeImage(const MATRIX4& aModellingTransformationMatrix);


    //--------------------------------------------------------------------------
    /// Save a FBO into a file.
    /**
     *  @param aFBOID:      the ID of the FBO
     *  @param aFileName:   the file name
     */
    //--------------------------------------------------------------------------
    void printFBO(unsigned int aFBOID, const char* aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save a FBO into a file.
    /**
     *  @param aFBOID:      the ID of the FBO
     *  @param aFileName:   the file name
     */
    //--------------------------------------------------------------------------
    void printFBO(unsigned int aFBOID, const std::string& aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save a texture into a file.
    /**
     *  @param aTextureID:  the ID of the texture
     *  @param aFileName:   the file name
     */
    //--------------------------------------------------------------------------
    void print2DTexture(unsigned int aTextureID, const char* aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save a texture into a file.
    /**
     *  @param aTextureID:  the ID of the texture
     *  @param aFileName:   the file name
     */
    //--------------------------------------------------------------------------
    void print2DTexture(unsigned int aTextureID, const std::string& aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current L-buffer into a file.
    /**
     *  @param aFileName:   the file name
     */
    //--------------------------------------------------------------------------
    void printLBuffer(const char* aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current L-buffer into a file.
    /**
     *  @param aFileName:   the file name
     */
    //--------------------------------------------------------------------------
    void printLBuffer(const std::string& aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current cumulated L-buffer into a file.
    /**
     *  @param aFileName:   the file name
     */
    //--------------------------------------------------------------------------
    void printCumulatedLBuffer(const char* aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current cumulated L-buffer into a file.
    /**
     *  @param aFileName:   the file name
     */
    //--------------------------------------------------------------------------
    void printCumulatedLBuffer(const std::string& aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current Mux Dx into a file.
    /**
     *  @param aFileName:   the file name
     */
    //--------------------------------------------------------------------------
    void printSumMuxDx(const char* aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current Mux Dx into a file.
    /**
     *  @param aFileName:   the file name
     */
    //--------------------------------------------------------------------------
    void printSumMuxDx(const std::string& aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current energy fluence into a file (same as printXRayImage()).
    /**
     *  @param aFileName:   the file name
     */
    //--------------------------------------------------------------------------
    void printEnergyFluence(const char* aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current energy fluence into a file (same as printXRayImage()).
    /**
     *  @param aFileName:   the file name
     */
    //--------------------------------------------------------------------------
    void printEnergyFluence(const std::string& aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current energy fluence into a file
    /// (same as printEnergyFluence()).
    /**
     *  @param aFileName:   the file name
     */
    //--------------------------------------------------------------------------
    void printXRayImage(const char* aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current energy fluence into a file
    /// (same as printEnergyFluence()).
    /**
     *  @param aFileName:   the file name
     */
    //--------------------------------------------------------------------------
    void printXRayImage(const std::string& aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Accessor on the raw data from a FBO.
    /**
     *  @param aFBOID:  the ID of the FBO
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER* getFBO(unsigned int aFBOID);


    //--------------------------------------------------------------------------
    /// Accessor on the raw data from a texture.
    /**
     *  @param aTextureID:  the ID of the texture
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER* get2DTexture(unsigned int aTextureID);


    //--------------------------------------------------------------------------
    /// Display a 2D texture using OpenGL.
    //--------------------------------------------------------------------------
    void display2DTexture(unsigned int aTexutreID);


	//--------------------------------------------------------------------------
	/// Get id 2D texture using OpenGL.
	//--------------------------------------------------------------------------
	unsigned int getID2DTexture();


    //--------------------------------------------------------------------------
    /// Display the L-buffer using OpenGL.
    //--------------------------------------------------------------------------
    void displayLBuffer();


    //--------------------------------------------------------------------------
    /// Display the energy fluence using OpenGL (same as displayXRayImage()).
    //--------------------------------------------------------------------------
    void displayEnergyFluence();


    //--------------------------------------------------------------------------
    /// Display the energy fluence using OpenGL
    /// (same as displayEnergyFluence()).
    //--------------------------------------------------------------------------
    void displayXRayImage();


    //--------------------------------------------------------------------------
    /// Normalise the final result to display in OpenGL.
    //--------------------------------------------------------------------------
    void getEnergyFluenceMinMax();


    //--------------------------------------------------------------------------
    /// Display the X-ray image using OpenGL.
    /**
     *  @param useLogScale: flag set to true to use the log scale,
     *                      to false to use a linear scale (default: false)
     */
    //--------------------------------------------------------------------------
    void display(bool useNormalisation,
        bool useLogScale,
        bool usePowerLaw,
        double shift = 0.0,
        double scale = 1.0,
        double gamma = (1.0 / 2.5));


    //--------------------------------------------------------------------------
    /// Get the attenuation coefficient of water for a given energy.
    /// The value is computed using linear interpolation.
    /**
     *  @param anIncidentEnergy:    the energy
     *  @return the corresponding attenuation coefficient of water
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getMuWaterLinearInterpolation(
            const RATIONAL_NUMBER& anIncidentEnergy);


    //--------------------------------------------------------------------------
    /// Get the attenuation coefficient of water for a given energy.
    /// The value is computed using linear interpolation on a logarithmic scale.
    /**
     *  @param anIncidentEnergy:    the energy
     *  @return the corresponding attenuation coefficient of water
     */
    //--------------------------------------------------------------------------
    static RATIONAL_NUMBER getMuWaterLogLinearInterpolation(
            const RATIONAL_NUMBER& anIncidentEnergy);


    //--------------------------------------------------------------------------
    /// Convert a Hounsfield value into a linear attenuation coefficient.
    /**
     *  @param HU:  the Hounsfield value to convert
     *  @param mu_water:    the attenuation coefficient of water at
    *                       a given energy
     *  @return the corresponding attenuation coefficient
     */
    //--------------------------------------------------------------------------
    static RATIONAL_NUMBER getMuFromHUAndMuWater(const RATIONAL_NUMBER& HU,
            const RATIONAL_NUMBER& mu_water);


    //--------------------------------------------------------------------------
    /// Convert a Hounsfield value into a linear attenuation coefficient.
    /**
     *  @param HU:  the Hounsfield value to convert
     *  @param anEnergy:    a given energy
     *  @return the corresponding attenuation coefficient
     */
    //--------------------------------------------------------------------------
    static RATIONAL_NUMBER getMuFromHU(const RATIONAL_NUMBER& HU,
                                const RATIONAL_NUMBER& anEnergy);


    //--------------------------------------------------------------------------
    /// Convert a Hounsfield value into a mass attenuation coefficient.
    /**
     *  @param HU:  the Hounsfield value to convert
     *  @param anEnergy:    a given energy
     *  @return the corresponding attenuation coefficient
     */
    //--------------------------------------------------------------------------
    static RATIONAL_NUMBER getMassAttenuationFromHU(const RATIONAL_NUMBER& HU,
            const RATIONAL_NUMBER& anEnergy);
    
    
    //--------------------------------------------------------------------------
    /// Get the density (rho) of a tissue given a Hounsfield value.
    /**
     *  @param HU:  the Hounsfield value to consider
     *  @return the corresponding density
     */
    //--------------------------------------------------------------------------
    static RATIONAL_NUMBER getDensityFromHU(const RATIONAL_NUMBER& HU);
    
    
    //--------------------------------------------------------------------------
    /// Convert a Hounsfield value into a linear attenuation coefficient.
    /**
     *  @param HU:  the Hounsfield value to convert
     *  @param anEnergy:    a given energy
     *  @return the corresponding attenuation coefficient
     */
    //--------------------------------------------------------------------------
    static RATIONAL_NUMBER getElementMu(const char* anElementSymbolHU,
                                       const RATIONAL_NUMBER& anEnergy);
    
    
    //--------------------------------------------------------------------------
    /// Convert a Hounsfield value into a mass attenuation coefficient.
    /**
     *  @param HU:  the Hounsfield value to convert
     *  @param anEnergy:    a given energy
     *  @return the corresponding attenuation coefficient
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getElementMassAttenuation(const char* anElementSymbolHU,
                                             const RATIONAL_NUMBER& anEnergy);
    
    
    //--------------------------------------------------------------------------
    /// Get the density (rho) of a tissue given an element symbol.
    /**
     *  @param HU:  the Hounsfield value to consider
     *  @return the corresponding density
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getElementDensity(const char* anElementSymbol);
    
    
    //--------------------------------------------------------------------------
    /// Display the final image in negative or not.
    /**
     *  @param aFlag:   true is the final image is displayed in negative,
     *                  false it is not (default value: true)
     */
    //--------------------------------------------------------------------------
    void useNegativeFilteringFlag(bool aFlag = true);


    //--------------------------------------------------------------------------
    /// Accessor on the flag control the display of the final in negative.
    /**
     *  @return true if the final image is displayed in negative,
     *          false it is not
     */
    //--------------------------------------------------------------------------
    bool getNegativeFilteringFlag() const;


    //--------------------------------------------------------------------------
    /// Output the talbe of attenuation coefficients of water into a data stream
    /**
     *  @param anOutputStream:  the output data stream
     */
    //--------------------------------------------------------------------------
    void printMuWaterTable(std::ostream& anOutputStream);


    //--------------------------------------------------------------------------
    /// Accessor on the number of inner surfaces
    /// (e.g. an internal organ in case of a medical application).
    /**
     *  @return the number of inner surfaces
     */
    //--------------------------------------------------------------------------
    unsigned int getNumberOfInnerSurfaces() const;


    //--------------------------------------------------------------------------
    /// Accessor on the number of outer surfaces
    /// (e.g. the skin surface in case of a medical application).
    /// This value is either equal to 0 or 1.
    /**
     *  @return the number of outer surfaces
     */
    //--------------------------------------------------------------------------
    unsigned int getNumberOfOuterSurfaces() const;


    //--------------------------------------------------------------------------
    /// Display a given inner surface (e.g. an internal organ in case of
    /// a medical application).
    /**
     *  @param anIndex: the index of the inner surface to display
     */
    //--------------------------------------------------------------------------
    void displayInnerSurface(unsigned int anIndex) const;


    //--------------------------------------------------------------------------
    /// Display the outer surface (e.g. the skin surface in case of
    /// a medical application).
    //--------------------------------------------------------------------------
    void displayOuterSurface() const;


    //--------------------------------------------------------------------------
    /// Accessor on the smallest pixel intensity in
    /// the latest simulated X-ray image.
    /**
     *  @return the min value in simulated X-ray image
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getMinPixelValue() const;


    //--------------------------------------------------------------------------
    /// Accessor on the greatest pixel intensity in the latest simulated
    /// X-ray image.
    /**
     *  @return the max value in simulated X-ray image
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getMaxPixelValue() const;


    void computeSinogram(MATRIX4& aModellingTransformationMatrix,
            VEC3 aRotationCenter = VEC3(),
            unsigned int aNumberOfAngle = 180,
            RATIONAL_NUMBER anAngleOffset = 1.0);


    void printSinogram(const char* aFileName, bool useCompression = false);
    void printSinogram(const std::string& aFileName, bool useCompression = false);


    const Sinogram<XRayRenderer::PixelType>& getSinogram() const;
    Sinogram<XRayRenderer::PixelType>& getSinogram();


    const PixelType* getSinogramRawData() const;


    void computeProjectionSet(MATRIX4& aModellingTransformationMatrix,
            VEC3 aRotationCenter = VEC3(),
            unsigned int aNumberOfAngle = 180,
            RATIONAL_NUMBER anAngleOffset = 1.0);


    void printProjectionSet(const char* aFileName, bool useCompression = false);
    void printProjectionSet(const std::string& aFileName, bool useCompression = false);


    const Image<XRayRenderer::PixelType>& getProjectionSet() const;
    Image<XRayRenderer::PixelType>& getProjectionSet();


    const PixelType* getProjectionSetRawData();


    const Image<XRayRenderer::PixelType>& getEnergyFluence() const;
    Image<XRayRenderer::PixelType>& getEnergyFluence();


    const PixelType* getEnergyFluenceRawData();

    
    const Image<XRayRenderer::PixelType>& getLBuffer() const;
    Image<XRayRenderer::PixelType>& getLBuffer();

    
    const Image<XRayRenderer::PixelType>& getCumulatedLBuffer() const;
    Image<XRayRenderer::PixelType>& getCumulatedLBuffer();

    
    const PixelType* getLBufferRawData();

    //--------------------------------------------------------------------------
    /// Returns the actual Beam
    /**
    *  @return a const pointer to the actual Beam
    */
    //--------------------------------------------------------------------------
    const XRayBeam* getXRayBeam( ) const;


    //--------------------------------------------------------------------------
    /// Enable artefact filtering.
    //--------------------------------------------------------------------------
    void enableArtefactFilteringOnGPU();
    void enableArtefactFilteringOnCPU();


    //--------------------------------------------------------------------------
    /// Enable artefact filtering.
    //--------------------------------------------------------------------------
    void enableArtifactFilteringOnGPU();
    void enableArtifactFilteringOnCPU();


    //--------------------------------------------------------------------------
    /// Disable artefact filtering.
    //--------------------------------------------------------------------------
    void disableArtefactFilteringOnGPU();
    void disableArtefactFilteringOnCPU();


    //--------------------------------------------------------------------------
    /// Disable artefact filtering.
    //--------------------------------------------------------------------------
    void disableArtifactFilteringOnGPU();
    void disableArtifactFilteringOnCPU();


    void useArtifactFilteringOnGPU(bool aFlag);
    void useArtifactFilteringOnCPU(bool aFlag);

    void useArtefactFilteringOnGPU(bool aFlag);
    void useArtefactFilteringOnCPU(bool aFlag);

    bool useArtifactFilteringOnGPU() const;
    bool useArtifactFilteringOnCPU() const;

    bool useArtefactFilteringOnGPU() const;
    bool useArtefactFilteringOnCPU() const;


	//--------------------------------------------------------------------------
	/// If a surface changes material, it is needed update surfacepermaterial list 
	//--------------------------------------------------------------------------
	void needUpdateSurfacePerMaterial();


//******************************************************************************
protected:
    //--------------------------------------------------------------------------
    /// Load the table of water attenuation coefficients.
    //--------------------------------------------------------------------------
    static void loadMuWater();


    //--------------------------------------------------------------------------
    /// Check that the device supports all the required OpenGL extensions
    /// that are needed.
    //--------------------------------------------------------------------------
    void checkGPUCapabilities();


    //--------------------------------------------------------------------------
    /// Initialise the shaders.
    //--------------------------------------------------------------------------
    void initShaders();


    //--------------------------------------------------------------------------
    /// Create the FBOs.
    //--------------------------------------------------------------------------
    void createFBOs();


    //--------------------------------------------------------------------------
    /// Delete the FBOs.
    //--------------------------------------------------------------------------
    void deleteFBOs();


    //--------------------------------------------------------------------------
    /// Create a PBO.
    /**
     *  @param aPBO:    ID of the PBO to create
     */
    //--------------------------------------------------------------------------
    void createPBO(unsigned int& aPBO);


    //--------------------------------------------------------------------------
    /// Delete a PBO.
    /**
     *  @param aPBO:    ID of the PBO to delete
     */
    //--------------------------------------------------------------------------
    void deletePBO(unsigned int& aPBO);


    //--------------------------------------------------------------------------
    /// Initialise the PBOs.
    //--------------------------------------------------------------------------
    void initialisePBOs();


    //--------------------------------------------------------------------------
    /// Initialise the 2D textures.
    /**
     *  @param anInternalTextureFormat: the internal format in which OpenGL
     *                                  textures will be stored (GL_RGB16F_ARB,
     *                                  or GL_RGB32F_ARB)
     *  @param aType: the type (e.g. GL_RGBA)
     */
    //--------------------------------------------------------------------------
    void init2DTextures(int anInternalTextureFormat);


    //--------------------------------------------------------------------------
    /// Initialise the 3D textures.
    /**
     *  @param anInternalTextureFormat: the internal format in which OpenGL
     *                                  textures will be stored (GL_RGB16F_ARB,
     *                                  or GL_RGB32F_ARB)
     */
    //--------------------------------------------------------------------------
    void init3DData(int anInternalTextureFormat);


    //--------------------------------------------------------------------------
    /// Initialise a FBO (e.g. attach textures, etc.).
    /**
     *  @param aFBOIndex:       the FBO to initialise
     *  @param aTextureIndex:   the texture to attach
     */
    //--------------------------------------------------------------------------
    void init2DFBO(unsigned int aFBOIndex, unsigned int aColourTextureIndex, unsigned int aZBufferTextureIndex = 0);


    //--------------------------------------------------------------------------
    /// Initialise a FBO with a 3D texture.
    //--------------------------------------------------------------------------
    void init3DFBOs();


    //--------------------------------------------------------------------------
    /// Create a 2D texture.
    /**
     *  @param apTextureID:             the address on the ID of the texture
     *  @param anInternalTextureFormat: the internal format in which the texture
     *                                  will be stored in the device
     */
    //--------------------------------------------------------------------------
    void create2DTexture(unsigned int* apTextureID,
                         int anInternalTextureFormat,
						 int aType);


    //--------------------------------------------------------------------------
    /// Reset the value of a FBO to 0.
    /**
     *  @param aFrameBuffer:    the FBO to reset
     */
    //--------------------------------------------------------------------------
    void reset2DBuffer(unsigned int aFrameBuffer);


    //--------------------------------------------------------------------------
    /// Reset the L-buffer FBO to 0.
    //--------------------------------------------------------------------------
    void resetLBuffer();


    //--------------------------------------------------------------------------
    /// Reset the energy fluence FBO to 0.
    //--------------------------------------------------------------------------
    void resetEnergyFluence();


    //--------------------------------------------------------------------------
    /// Reset the Mu_x * D_x FBO to 0.
    //--------------------------------------------------------------------------
    void resetSumMuxDx();


    //--------------------------------------------------------------------------
    /// Compute the L-buffer of an internal object.
    /**
     *  @param anObject:                        the 3D object to process
     *  @param aXRaySourceSampleOffset:         the translation vector to use
     *                                          when the source is not aligned
     *                                          with the detector
     *  @param aModellingTransformationMatrix:  the transformation matrix of
     *                                          the 3D object
     */
    //--------------------------------------------------------------------------
    void computeInnerSurfaceLBuffer(PolygonMesh& anObject,
            const VEC3& aXRaySourceSampleOffset,
            const MATRIX4& aModellingTransformationMatrix,
            bool aGPUStateConservationFlag);

    //--------------------------------------------------------------------------
    /// Compute the L-buffer of an external object.
    /**
     *  @param anObject:                        the 3D object to process
     *  @param aXRaySourceSampleOffset:         the translation vector to use
     *                                          when the source is not aligned
     *                                          with the detector
     *  @param aModellingTransformationMatrix:  the transformation matrix of
     *                                          the 3D object
     */
    //--------------------------------------------------------------------------
    void computeOuterSurfaceLBuffer(PolygonMesh& anObject,
            const VEC3& aXRaySourceSampleOffset,
            const MATRIX4& aModellingTransformationMatrix);


    //--------------------------------------------------------------------------
    /// Update the L-buffer of inner surfaces.
    /// It integrates the L-buffers of all the inner surfaces.
    /**
     *  @param anObjectID:  the ID of the surface to integrate. If anObjectID is
     *                      equal to zero, the cumulated L-buffer is
     *                      re-initialsed with the L-buffer of this surface.
     *                      If anObjectID is greater than zero, L-buffer of this
     *                      surface is integrated into the cumulated L-buffer.
     */
    //--------------------------------------------------------------------------
    void updateCumulatedLBuffers(unsigned int anObjectID);


    //--------------------------------------------------------------------------
    /// Compute t.
    /**
     *  @param HU:
     *  @param anObjectID:
     */
    //--------------------------------------------------------------------------
    void computeSumMUxDx(int anObjectID,
                         const PhotonCrossSection& aPhotonCrossSection);


    //--------------------------------------------------------------------------
    /// Compute the energy fluence.
    //--------------------------------------------------------------------------
    void computeIntegratedEnergy();


    //--------------------------------------------------------------------------
    /// Clean the L-buffer.
    //--------------------------------------------------------------------------
    void cleanLBuffer(const RATIONAL_NUMBER& aCutoffDistance);


    //--------------------------------------------------------------------------
    /// Update the VBO of the geometry.
    //--------------------------------------------------------------------------
    void updateVBO();


    void preComputeLBuffer(GLint* apViewport, const VEC3& aXRaySourceSampleOffset);
    void postComputeLBuffer(const GLint* apViewport, RATIONAL_NUMBER aDiagonal);

    void updateSurfacePerMaterialSet();

	

    /// The X-ray detector
    XRayDetector* m_p_detector;


    /// The X-ray beam
    XRayBeam* m_p_xray_beam;


    /// Set of 3D geometries
    std::vector<SceneGraphNode*> m_p_inner_surface_set;


    /// The 3D geometry that will wrap the ones in m_p_inner_surface_set
    SceneGraphNode* m_p_outer_surface;


    std::vector<SceneGraphNode*> m_p_internal_data_storage;


    std::vector<std::vector<SceneGraphNode*> > m_p_surface_per_material_set;
    bool m_surface_per_material_set_needs_update;


    /// The computing mode (OPENGL, CUDA, or OPENCL)
    ComputingMode m_computing_mode;


    /// Set of PBOs
    std::vector<unsigned int>        m_p_pbo_set;


    /// Set of textures
    std::vector<unsigned int> m_p_texture_name_set;


    /// Set of shaders
    std::vector<Shader> m_shader_set;


    /// The raw data of the last FBO that was loaded in the main memory
    Image<PixelType> m_temp_raw_data;


    /// The table of water energy fluence coefficients
    static std::vector<AttenuationCoefficient> m_mu_water_set;


    /// The set of FBOs
    std::vector<FramebufferObject*> m_p_fbo_set;


    /// L-buffer
    Image<PixelType> m_l_buffer_image;


    /// Cumulated L-buffer for all the inner surfaces
    Image<PixelType> m_cumulated_lbuffer_image;


    /// Sum(Mu_x * d_x)
    Image<PixelType> m_sum_mux_dx_image;


    /// Energy fluence
    Image<PixelType> m_energy_fluence_image;


    /// Sinogram
    Sinogram<PixelType> m_sinogram_image;


    /// Projection set
    Image<PixelType> m_projection_set;


    /// Min value in simulated X-ray image
    RATIONAL_NUMBER m_min_value_in_simulated_xray_image;


    /// Max value in simulated X-ray image
    RATIONAL_NUMBER m_max_value_in_simulated_xray_image;


    /// Flag to display the result in negative or not
    unsigned int m_negative_display_flag;


    /// Flag set to true to use L-buffer artefact filtering
    bool m_use_l_buffer_artefact_filtering_on_gpu;
    bool m_use_l_buffer_artefact_filtering_on_cpu;


    /// Flag use for lazy evaluation of the L-Buffer
    bool m_l_buffer_empty;


    /// Flag use for lazy evaluation and blending of the energy fluence
    bool m_energy_fluence_empty;


    /// The VBO used to display a rectangle corresponding to the detector
#if __cplusplus <= 199711L
    std::auto_ptr<VBO> m_detector_temporary_geometry;
#else
    std::unique_ptr<VBO> m_detector_temporary_geometry;
#endif

    unsigned int m_zbuffer_texture_id;

    unsigned int m_precision;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "XRayRenderer.inl"


#endif // __XRayRenderer_h
