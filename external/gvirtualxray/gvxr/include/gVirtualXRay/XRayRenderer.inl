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
*	@file		XRayRenderer.inl
*
*	@brief		Class to compute and renderer X-ray images on GPU.
*
*	@version	1.0
*
*	@date		01/12/2013
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
#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <sstream>

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef __Utilities_h
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef __ElementSet_h
#include "gVirtualXRay/ElementSet.h"
#endif

#ifndef __MaterialSet_h
#include "gVirtualXRay/MaterialSet.h"
#endif


//******************************************************************************
//  define
//******************************************************************************

// 2D FBOs first, then 3D FBOs
#define XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID           0
#define XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID             1
#define XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID          2
#define XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID              3
#define XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID                  4

// 2D textures first, then 3D textures
#define XRAY_DETECTOR_UNCLEANED_LBUFFER_TEXTURE_NAME_ID  0
#define XRAY_DETECTOR_CLEANED_LBUFFER_TEXTURE_NAME_ID    1
#define XRAY_DETECTOR_CUMULATED_LBUFFERS_TEXTURE_NAME_ID 2
#define XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID     3
#define XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID         4
#define XRAY_DETECTOR_ZBUFFER_TEXTURE_NAME_ID            5


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


    //----------------------------------
    inline XRayRenderer::~XRayRenderer()
    //----------------------------------
    {
        release();
    }


    //-----------------------------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getTotalEnergyWithDetectorResponse() const
    //-----------------------------------------------------------------------------
    {
        RATIONAL_NUMBER total_energy = 0.0;

        // Proceed each energy channel
        for (unsigned int current_slice(0);
            current_slice < m_p_xray_beam->getEnergyChannelNumber();
            ++current_slice)
        {
            RATIONAL_NUMBER photon_number(m_p_xray_beam->getEnergyChannel(current_slice).getPhotonNumber());
    				RATIONAL_NUMBER photon_energy(m_p_xray_beam->getEnergyChannel(current_slice).getPhotonEnergy());

            //std::cout << current_slice << "\t" << photon_number << "\t" << photon_energy;

            // Apply the energy response
    				photon_energy = m_p_detector->applyEnergyResponse(photon_energy);
    				RATIONAL_NUMBER input_energy(photon_number * photon_energy / RATIONAL_NUMBER(m_p_detector->getNumberOfSourceSamples()));

            //std::cout << "\t" << photon_energy << std::endl;
            total_energy += input_energy;
        }

        return total_energy;
    }


    //------------------------------------------------------------
    inline void XRayRenderer::useNegativeFilteringFlag(bool aFlag)
    //------------------------------------------------------------
    {
        m_negative_display_flag = aFlag;
    }


    //--------------------------------------------------------
    inline bool XRayRenderer::getNegativeFilteringFlag() const
    //--------------------------------------------------------
    {
        return (m_negative_display_flag);
    }


    //------------------------------------------------------------------------
    inline void XRayRenderer::addInnerSurface(const SceneGraphNode* apSurface)
    //------------------------------------------------------------------------
    {
        m_p_inner_surface_set.push_back(const_cast<SceneGraphNode*>(apSurface));
        m_surface_per_material_set_needs_update = true;
    }


    //------------------------------------------------------------------------
    inline void XRayRenderer::addOuterSurface(const SceneGraphNode* apSurface)
    //------------------------------------------------------------------------
    {
        m_p_outer_surface = const_cast<SceneGraphNode*>(apSurface);
        m_surface_per_material_set_needs_update = true;
    }


    //---------------------------------------------------------------------
    inline void XRayRenderer::addInnerSurface(const PolygonMesh* apSurface)
    //---------------------------------------------------------------------
    {
    	// Create a node label
    	std::stringstream label;
    	label << "node_" << m_p_internal_data_storage.size();

    	// Add a new SceneGraphNode into the internal storage of the instance
    	// (not an actual SceneGraphNode, but it allows backward compatibility)
    	m_p_internal_data_storage.push_back(
    			new SceneGraphNode(label.str(),
    					const_cast<PolygonMesh*>(apSurface),
						VEC3(),
						NULL));

    	// Use the node as an inner surface
        m_p_inner_surface_set.push_back(m_p_internal_data_storage.back());

        // Make sure the grouping per material is up-to-date when needed
        m_surface_per_material_set_needs_update = true;
    }


    //---------------------------------------------------------------------
    inline void XRayRenderer::addOuterSurface(const PolygonMesh* apSurface)
    //---------------------------------------------------------------------
    {
    	// Create a node label
    	std::stringstream label;
    	label << "node_" << m_p_internal_data_storage.size();

    	// Add a new SceneGraphNode into the internal storage of the instance
    	// (not an actual SceneGraphNode, but it allows backward compatibility)
    	m_p_internal_data_storage.push_back(
    			new SceneGraphNode(label.str(),
    					const_cast<PolygonMesh*>(apSurface),
						VEC3(),
						NULL));

    	// Use the node as an outer surface
    	m_p_outer_surface = m_p_internal_data_storage.back();

        // Make sure the grouping per material is up-to-date when needed
        m_surface_per_material_set_needs_update = true;
    }


    //---------------------------------------------
    inline void XRayRenderer::removeInnerSurfaces()
    //---------------------------------------------
    {
        m_p_inner_surface_set.clear();
        m_surface_per_material_set_needs_update = true;

        std::vector<std::vector<SceneGraphNode*>::iterator> waiting_list;

        for (std::vector<SceneGraphNode*>::iterator ite = m_p_internal_data_storage.begin();
        		ite != m_p_internal_data_storage.end();
        		++ite)
        {
        	if (*ite != m_p_outer_surface)
        	{
        		delete *ite;
        		waiting_list.push_back(ite);
        	}
        }

        for (std::vector<std::vector<SceneGraphNode*>::iterator>::reverse_iterator ite = waiting_list.rbegin();
        		ite != waiting_list.rend();
        		++ite)
        {
        	m_p_internal_data_storage.erase(*ite);
        }
    }


    //--------------------------------------------
    inline void XRayRenderer::removeOuterSurface()
    //--------------------------------------------
    {
        std::vector<std::vector<SceneGraphNode*>::iterator> waiting_list;

        for (std::vector<SceneGraphNode*>::iterator ite = m_p_internal_data_storage.begin();
        		ite != m_p_internal_data_storage.end();
        		++ite)
        {
        	if (*ite == m_p_outer_surface)
        	{
        		delete *ite;
        		waiting_list.push_back(ite);
        	}
        }

        for (std::vector<std::vector<SceneGraphNode*>::iterator>::reverse_iterator ite = waiting_list.rbegin();
        		ite != waiting_list.rend();
        		++ite)
        {
        	m_p_internal_data_storage.erase(*ite);
        }

        m_p_outer_surface = 0;
    }


    //--------------------------------------------------------------
    inline void XRayRenderer::printFBO(unsigned int aFBOID,
                                       const std::string& aFileName,
									   bool useCompression)
    //--------------------------------------------------------------
    {
        printFBO(aFBOID, aFileName.data(), useCompression);
    }


    //--------------------------------------------------------------------
    inline void XRayRenderer::print2DTexture(unsigned int aTextureID,
                                             const std::string& aFileName,
											 bool useCompression)
    //--------------------------------------------------------------------
    {
        print2DTexture(aTextureID, aFileName.data(), useCompression);
    }


    //------------------------------------------------------------------
    inline void XRayRenderer::printLBuffer(const std::string& aFileName,
    									   bool useCompression)
    //------------------------------------------------------------------
    {
        printLBuffer(aFileName.data(), useCompression);
    }


    //---------------------------------------------------------------------------
    inline void XRayRenderer::printCumulatedLBuffer(const std::string& aFileName,
    												bool useCompression)
    //---------------------------------------------------------------------------
    {
        printCumulatedLBuffer(aFileName.data(), useCompression);
    }


    //-------------------------------------------------------------------
    inline void XRayRenderer::printSumMuxDx(const std::string& aFileName, bool useCompression)
    //-------------------------------------------------------------------
    {
        printSumMuxDx(aFileName.data(), useCompression);
    }


    //------------------------------------------------------------------------
    inline void XRayRenderer::printEnergyFluence(const std::string& aFileName, bool useCompression)
    //------------------------------------------------------------------------
    {
        printEnergyFluence(aFileName.data(), useCompression);
    }


    //--------------------------------------------------------------------
    inline void XRayRenderer::printXRayImage(const std::string& aFileName, bool useCompression)
    //--------------------------------------------------------------------
    {
        printXRayImage(aFileName.data(), useCompression);
    }


    //-----------------------------------------------------------------------
    inline void XRayRenderer::printMuWaterTable(std::ostream& anOutputStream)
    //-----------------------------------------------------------------------
    {
        for (std::vector<AttenuationCoefficient>::const_iterator ite(m_mu_water_set.begin());
                ite != m_mu_water_set.end();
                ++ite)
        {
            anOutputStream << *ite << std::endl;
        }
    }


    //----------------------------------------
    inline void XRayRenderer::initialisePBOs()
    //----------------------------------------
    {
        for (std::vector<unsigned int>::iterator ite(m_p_texture_name_set.begin());
                ite != m_p_texture_name_set.end();
                ++ite)
        {
            // Delete the PBO
            try
            {
                if (*ite != 0)
                {
                    deletePBO(*ite);
                }
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }

            // Make sure the value is reset
            *ite = 0;

            // Create a new PBO
            createPBO(*ite);
        }
    }


    //------------------------------------------------------------------------------
    inline void XRayRenderer::init2DTextures(int anInternalTextureFormat)
    //------------------------------------------------------------------------------
    {
        // Store the texturing flags
        pushTexture2D();

        // Initialise textures
        for (int i(0); i < XRAY_DETECTOR_2D_TEXTURE_NUMBER; ++i)
        {
            create2DTexture(&m_p_texture_name_set[i], anInternalTextureFormat, GL_RGBA);
        }

        create2DTexture(&m_zbuffer_texture_id,  GL_DEPTH_COMPONENT32F,  GL_DEPTH_COMPONENT);

        // Restore the texturing flags
        popTexture();
    }


    //-----------------------------------------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getMuFromHUAndMuWater(const RATIONAL_NUMBER& HU,
                                                               const RATIONAL_NUMBER& mu_water)
    //-----------------------------------------------------------------------------------------
    {
        // Convert a Hounsfield value into a liner attenuation coefficient given mu(water)
        return RATIONAL_NUMBER(mu_water * (1.0 + HU / 1000.0));
    }


    //-------------------------------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getMuFromHU(const RATIONAL_NUMBER& HU,
                                                     const RATIONAL_NUMBER& anEnergy)
    //-------------------------------------------------------------------------------
    {
        return (MaterialSet::getInstance().getMaterial(HU).getMu(HU, anEnergy));
    }


    //--------------------------------------------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getMassAttenuationFromHU(const RATIONAL_NUMBER& HU,
                                                                  const RATIONAL_NUMBER& anEnergy)
    //--------------------------------------------------------------------------------------------
    {
        return (MaterialSet::getInstance().getMaterial(HU).getMassAttenuationTotal(anEnergy));
    }


    //------------------------------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getDensityFromHU(const RATIONAL_NUMBER& HU)
    //------------------------------------------------------------------------------
    {
        return (TissueMaterial::getDensity(HU));
    }


    //-------------------------------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getElementMu(const char* anElementSymbol,
                                                     const RATIONAL_NUMBER& anEnergy)
    //-------------------------------------------------------------------------------
    {
        return (ElementSet::getInstance().getElement(anElementSymbol).getMU(anEnergy));
    }


    //--------------------------------------------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getElementMassAttenuation(const char* anElementSymbol,
                                                                  const RATIONAL_NUMBER& anEnergy)
    //--------------------------------------------------------------------------------------------
    {
        return (ElementSet::getInstance().getElement(anElementSymbol).getMassAttenuationTotal(anEnergy));
    }


    //------------------------------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getElementDensity(const char* anElementSymbol)
    //------------------------------------------------------------------------------
    {
        return (ElementSet::getInstance().getElement(anElementSymbol).getDensity());
    }


    //----------------------------------------------------------------
    inline unsigned int XRayRenderer::getNumberOfInnerSurfaces() const
    //----------------------------------------------------------------
    {
        return (m_p_inner_surface_set.size());
    }


    //----------------------------------------------------------------
    inline unsigned int XRayRenderer::getNumberOfOuterSurfaces() const
    //----------------------------------------------------------------
    {
        if (m_p_outer_surface)
        {
            return (1);
        }
        else
        {
            return (0);
        }
    }


    //-----------------------------------------------------------------------
    inline void XRayRenderer::displayInnerSurface(unsigned int anIndex) const
    //-----------------------------------------------------------------------
    {
        if (anIndex >= m_p_inner_surface_set.size())
        {
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

        m_p_inner_surface_set[anIndex]->display();
    }


    //---------------------------------------------------
    inline void XRayRenderer::displayOuterSurface() const
    //---------------------------------------------------
    {
        if (!m_p_outer_surface)
        {
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

        m_p_outer_surface->display();
    }


    //-----------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getMinPixelValue() const
    //-----------------------------------------------------------
    {
        return (m_min_value_in_simulated_xray_image);
    }


    //-----------------------------------------------------------
    inline RATIONAL_NUMBER XRayRenderer::getMaxPixelValue() const
    //-----------------------------------------------------------
    {
        return (m_max_value_in_simulated_xray_image);
    }


    //-------------------------------------------------------------------------------
    inline const Sinogram<XRayRenderer::PixelType>& XRayRenderer::getSinogram() const
    //-------------------------------------------------------------------------------
    {
        return (m_sinogram_image);
    }


    //-------------------------------------------------------------------
    inline Sinogram<XRayRenderer::PixelType>& XRayRenderer::getSinogram()
    //-------------------------------------------------------------------
    {
        return (m_sinogram_image);
    }


    //----------------------------------------------------------------------------
    inline const XRayRenderer::PixelType* XRayRenderer::getSinogramRawData() const
    //----------------------------------------------------------------------------
    {
        return (m_sinogram_image.getRawData());
    }


    //--------------------------------------------------------------------------------
    inline const Image<XRayRenderer::PixelType>& XRayRenderer::getProjectionSet() const
    //--------------------------------------------------------------------------------
    {
        return (m_projection_set);
    }


    //--------------------------------------------------------------------
    inline Image<XRayRenderer::PixelType>& XRayRenderer::getProjectionSet()
    //--------------------------------------------------------------------
    {
        return (m_projection_set);
    }


    //---------------------------------------------------------------------------
    inline const XRayRenderer::PixelType* XRayRenderer::getProjectionSetRawData()
    {
        return (m_projection_set.getRawData());
    }


    //---------------------------------------------------------------------------------
    inline const Image<XRayRenderer::PixelType>& XRayRenderer::getEnergyFluence() const
    //---------------------------------------------------------------------------------
    {
        RATIONAL_NUMBER* p_target(const_cast<RATIONAL_NUMBER*>(m_energy_fluence_image.getRawData()));
        RATIONAL_NUMBER* p_source(const_cast<XRayRenderer*>(this)->getFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID));

        memcpy(p_target,
                p_source,
                sizeof(RATIONAL_NUMBER) * m_energy_fluence_image.getWidth() * m_energy_fluence_image.getHeight());

        return (m_energy_fluence_image);
    }


    //--------------------------------------------------------------------
    inline Image<XRayRenderer::PixelType>& XRayRenderer::getEnergyFluence()
    //--------------------------------------------------------------------
    {
        memcpy(m_energy_fluence_image.getRawData(),
                getFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID),
                sizeof(RATIONAL_NUMBER) * m_energy_fluence_image.getWidth() * m_energy_fluence_image.getHeight());

        return (m_energy_fluence_image);
    }


    //---------------------------------------------------------------------------
    inline const XRayRenderer::PixelType* XRayRenderer::getEnergyFluenceRawData()
    //---------------------------------------------------------------------------
    {
        memcpy(m_energy_fluence_image.getRawData(),
                getFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID),
                sizeof(RATIONAL_NUMBER) * m_energy_fluence_image.getWidth() * m_energy_fluence_image.getHeight());

        return (m_energy_fluence_image.getRawData());
    }


    //---------------------------------------------------------------------------
    inline const Image<XRayRenderer::PixelType>& XRayRenderer::getLBuffer() const
    //---------------------------------------------------------------------------
    {
        RATIONAL_NUMBER* p_target(const_cast<RATIONAL_NUMBER*>(m_l_buffer_image.getRawData()));
        RATIONAL_NUMBER* p_source = 0;

        if (m_use_l_buffer_artefact_filtering_on_gpu || m_use_l_buffer_artefact_filtering_on_cpu)
        {
            p_source = const_cast<XRayRenderer*>(this)->getFBO(XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID);
        }
        else
        {
            p_source = const_cast<XRayRenderer*>(this)->getFBO(XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID);
        }

        memcpy(p_target,
               p_source,
               sizeof(RATIONAL_NUMBER) * m_l_buffer_image.getWidth() * m_l_buffer_image.getHeight());

        return (m_l_buffer_image);
    }


    //---------------------------------------------------------------
    inline Image<XRayRenderer::PixelType>& XRayRenderer::getLBuffer()
    //---------------------------------------------------------------
    {
        RATIONAL_NUMBER* p_target(m_l_buffer_image.getRawData());
        RATIONAL_NUMBER* p_source = 0;

        if (m_use_l_buffer_artefact_filtering_on_gpu)
        {
            p_source = const_cast<XRayRenderer*>(this)->getFBO(XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID);
        }
        else
        {
            p_source = const_cast<XRayRenderer*>(this)->getFBO(XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID);
        }

        memcpy(p_target,
               p_source,
               sizeof(RATIONAL_NUMBER) * m_l_buffer_image.getWidth() * m_l_buffer_image.getHeight());

        return (m_l_buffer_image);
    }




    //---------------------------------------------------------------------------
    inline const Image<XRayRenderer::PixelType>& XRayRenderer::getCumulatedLBuffer() const
    //---------------------------------------------------------------------------
    {
        RATIONAL_NUMBER* p_target(const_cast<RATIONAL_NUMBER*>(m_cumulated_lbuffer_image.getRawData()));
        RATIONAL_NUMBER* p_source = 0;

		p_source = const_cast<XRayRenderer*>(this)->getFBO(XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID);

        memcpy(p_target,
               p_source,
               sizeof(RATIONAL_NUMBER) * m_cumulated_lbuffer_image.getWidth() * m_cumulated_lbuffer_image.getHeight());

        return (m_cumulated_lbuffer_image);
    }


    //---------------------------------------------------------------
    inline Image<XRayRenderer::PixelType>& XRayRenderer::getCumulatedLBuffer()
    //---------------------------------------------------------------
    {
        RATIONAL_NUMBER* p_target(m_cumulated_lbuffer_image.getRawData());
        RATIONAL_NUMBER* p_source = 0;

		p_source = const_cast<XRayRenderer*>(this)->getFBO(XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID);

        memcpy(p_target,
               p_source,
               sizeof(RATIONAL_NUMBER) * m_cumulated_lbuffer_image.getWidth() * m_cumulated_lbuffer_image.getHeight());

        return (m_cumulated_lbuffer_image);
    }


    //---------------------------------------------------------------------
    inline const XRayRenderer::PixelType* XRayRenderer::getLBufferRawData()
    //---------------------------------------------------------------------
    {
        RATIONAL_NUMBER* p_target(m_l_buffer_image.getRawData());
        RATIONAL_NUMBER* p_source = 0;

        if (m_use_l_buffer_artefact_filtering_on_gpu)
        {
            p_source = const_cast<XRayRenderer*>(this)->getFBO(XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID);
        }
        else
        {
            p_source = const_cast<XRayRenderer*>(this)->getFBO(XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID);
        }

        memcpy(p_target,
               p_source,
               sizeof(RATIONAL_NUMBER) * m_l_buffer_image.getWidth() * m_l_buffer_image.getHeight());

        return (m_l_buffer_image.getRawData());
    }


    //------------------------------------------------------
    inline void XRayRenderer::enableArtefactFilteringOnGPU()
    //------------------------------------------------------
    {
        m_use_l_buffer_artefact_filtering_on_gpu = true;
        m_use_l_buffer_artefact_filtering_on_cpu = false;
    }


    //------------------------------------------------------
    inline void XRayRenderer::enableArtefactFilteringOnCPU()
    //------------------------------------------------------
    {
        m_use_l_buffer_artefact_filtering_on_gpu = false;
        m_use_l_buffer_artefact_filtering_on_cpu = true;
    }


    //------------------------------------------------------
    inline void XRayRenderer::enableArtifactFilteringOnGPU()
    //------------------------------------------------------
    {
    	enableArtefactFilteringOnGPU();
    }


    //------------------------------------------------------
    inline void XRayRenderer::enableArtifactFilteringOnCPU()
    //------------------------------------------------------
    {
    	enableArtefactFilteringOnCPU();
    }


    //-------------------------------------------------------
    inline void XRayRenderer::disableArtefactFilteringOnGPU()
    //-------------------------------------------------------
    {
        m_use_l_buffer_artefact_filtering_on_gpu = false;
    }


    //-------------------------------------------------------
    inline void XRayRenderer::disableArtefactFilteringOnCPU()
    //-------------------------------------------------------
    {
        m_use_l_buffer_artefact_filtering_on_cpu = false;
    }


    //-------------------------------------------------------
    inline void XRayRenderer::disableArtifactFilteringOnGPU()
    //-------------------------------------------------------
    {
    	disableArtefactFilteringOnGPU();
    }


    //-------------------------------------------------------
    inline void XRayRenderer::disableArtifactFilteringOnCPU()
    //-------------------------------------------------------
    {
    	disableArtefactFilteringOnCPU();
    }


    //-------------------------------------------------------------
    inline void XRayRenderer::useArtefactFilteringOnGPU(bool aFlag)
    //-------------------------------------------------------------
    {
        m_use_l_buffer_artefact_filtering_on_gpu = aFlag;

        if (m_use_l_buffer_artefact_filtering_on_gpu)
        {
        	m_use_l_buffer_artefact_filtering_on_cpu = false;
        }
    }


    //------------------------------------------------------------
    inline void XRayRenderer::useArtefactFilteringOnCPU(bool aFlag)
    //------------------------------------------------------------
    {
        m_use_l_buffer_artefact_filtering_on_cpu = aFlag;

        if (m_use_l_buffer_artefact_filtering_on_cpu)
        {
        	m_use_l_buffer_artefact_filtering_on_gpu = false;
        }
    }


    //-------------------------------------------------------------
    inline void XRayRenderer::useArtifactFilteringOnGPU(bool aFlag)
    //-------------------------------------------------------------
    {
    	useArtefactFilteringOnGPU(aFlag);
    }


    //-------------------------------------------------------------
    inline void XRayRenderer::useArtifactFilteringOnCPU(bool aFlag)
    //-------------------------------------------------------------
    {
    	useArtefactFilteringOnCPU(aFlag);
    }


    //---------------------------------------------------------
    inline bool XRayRenderer::useArtefactFilteringOnGPU() const
    //---------------------------------------------------------
    {
        return (m_use_l_buffer_artefact_filtering_on_gpu);
    }


    //---------------------------------------------------------
	inline bool XRayRenderer::useArtefactFilteringOnCPU() const
	//---------------------------------------------------------
	{
		return (m_use_l_buffer_artefact_filtering_on_cpu);
	}


	//---------------------------------------------------------
    inline bool XRayRenderer::useArtifactFilteringOnGPU() const
    //---------------------------------------------------------
    {
        return (m_use_l_buffer_artefact_filtering_on_gpu);
    }


    //---------------------------------------------------------
    inline bool XRayRenderer::useArtifactFilteringOnCPU() const
    //---------------------------------------------------------
    {
        return (m_use_l_buffer_artefact_filtering_on_cpu);
    }


	//------------------------------------------------------------
    inline void XRayRenderer::printSinogram(const char* aFileName, bool useCompression)
    //------------------------------------------------------------
    {
        m_sinogram_image.save(aFileName, false, "Sinogram", useCompression);
    }


    //-------------------------------------------------------------------
    inline void XRayRenderer::printSinogram(const std::string& aFileName, bool useCompression)
    //-------------------------------------------------------------------
    {
        printSinogram(aFileName.data(), useCompression);
    }


    //-----------------------------------------------------------------
    inline void XRayRenderer::printProjectionSet(const char* aFileName, bool useCompression)
    //-----------------------------------------------------------------
    {
        m_projection_set.save(aFileName, false, "Projection set", useCompression);
    }


    //------------------------------------------------------------------------
    inline void XRayRenderer::printProjectionSet(const std::string& aFileName, bool useCompression)
    //------------------------------------------------------------------------
    {
        printProjectionSet(aFileName.data(), useCompression);
    }


    //--------------------------------------------
    inline void XRayRenderer::resetEnergyFluence()
    //--------------------------------------------
    {
        // The energy fluence is not empty
        if (!m_energy_fluence_empty)
        {
            switch (m_computing_mode)
            {
            case OPENGL:
                reset2DBuffer(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID);
                break;

            case CUDA:
                throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
                break;

            case OPENCL:
                throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
                break;

            default:
                throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
                break;
            }
        }

        // The energy fluence is now empty
        m_energy_fluence_empty = true;
    }


    //---------------------------------------
    inline void XRayRenderer::resetSumMuxDx()
    //---------------------------------------
    {
        switch (m_computing_mode)
        {
        case OPENGL:
            reset2DBuffer(XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID);
            break;

        case CUDA:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
            break;

        case OPENCL:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
            break;
        }
    }


    //-----------------------------------------------------------
    inline void XRayRenderer::printLBuffer(const char* aFileName, bool useCompression)
    //-----------------------------------------------------------
    {
        if (m_use_l_buffer_artefact_filtering_on_gpu)
        {
            //print2DTexture(XRAY_DETECTOR_CLEANED_LBUFFER_TEXTURE_NAME_ID, aFileName);
            printFBO(XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID, aFileName, useCompression);
        }
        else
        {
            //print2DTexture(XRAY_DETECTOR_UNCLEANED_LBUFFER_TEXTURE_NAME_ID, aFileName);
            printFBO(XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID, aFileName, useCompression);
        }
    }


    //--------------------------------------------------------------------
    inline void XRayRenderer::printCumulatedLBuffer(const char* aFileName, bool useCompression)
    //--------------------------------------------------------------------
    {
        //print2DTexture(XRAY_DETECTOR_LBUFFER_TEXTURE_NAME_ID, aFileName);
        printFBO(XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID, aFileName, useCompression);
    }


    //------------------------------------------------------------
    inline void XRayRenderer::printSumMuxDx(const char* aFileName, bool useCompression)
    //------------------------------------------------------------
    {
        //print2DTexture(XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID, aFileName);
        printFBO(XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID, aFileName, useCompression);
    }


    //-----------------------------------------------------------------
    inline void XRayRenderer::printEnergyFluence(const char* aFileName, bool useCompression)
    //-----------------------------------------------------------------
    {
        //print2DTexture(XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID, aFileName);
        printFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID, aFileName, useCompression);
    }


    //-------------------------------------------------------------
    inline void XRayRenderer::printXRayImage(const char* aFileName, bool useCompression)
    //-------------------------------------------------------------
    {
        //print2DTexture(XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID, aFileName);
        printFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID, aFileName, useCompression);
    }

/*
    //------------------------------------------------
    inline RATIONAL_NUMBER* XRayRenderer::getLBuffer()
    //------------------------------------------------
    {
        if (m_use_l_buffer_artefact_filtering)
        {
            //return (get2DTexture(XRAY_DETECTOR_CLEANED_LBUFFER_TEXTURE_NAME_ID));
            return (getFBO(XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID));
        }
        else
        {
            //return (get2DTexture(XRAY_DETECTOR_UNCLEANED_LBUFFER_TEXTURE_NAME_ID));
            return (getFBO(XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID));
        }
    }


    //---------------------------------------------------------
    inline RATIONAL_NUMBER* XRayRenderer::getCumulatedLBuffer()
    //---------------------------------------------------------
    {
        //return (get2DTexture(XRAY_DETECTOR_LBUFFER_TEXTURE_NAME_ID));
        return (getFBO(XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID));
    }


    //-------------------------------------------------
    inline RATIONAL_NUMBER* XRayRenderer::getSumMuxDx()
    //-------------------------------------------------
    {
        //return (get2DTexture(XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID));
        return (getFBO(XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID));
    }


    //------------------------------------------------------
    inline RATIONAL_NUMBER* XRayRenderer::getEnergyFluence()
    //------------------------------------------------------
    {
        //return (get2DTexture(XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID));
        return (getFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID));
    }


    //--------------------------------------------------
    inline RATIONAL_NUMBER* XRayRenderer::getXRayImage()
    //--------------------------------------------------
    {
        //return (get2DTexture(XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID));
        return (getFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID));
    }
*/

    //----------------------------------------
    inline void XRayRenderer::displayLBuffer()
    //----------------------------------------
    {
        if (m_use_l_buffer_artefact_filtering_on_gpu)
        {
            display2DTexture(XRAY_DETECTOR_CLEANED_LBUFFER_TEXTURE_NAME_ID);
        }
        else
        {
            display2DTexture(XRAY_DETECTOR_UNCLEANED_LBUFFER_TEXTURE_NAME_ID);
        }
    }


    //----------------------------------------------
    inline void XRayRenderer::displayEnergyFluence()
    //----------------------------------------------
    {
        display2DTexture(XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID);
    }


    //------------------------------------------
    inline void XRayRenderer::displayXRayImage()
    //------------------------------------------
    {
        display2DTexture(XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID);
    }

    //------------------------------------------
    inline const XRayBeam* XRayRenderer::getXRayBeam( ) const
      //------------------------------------------
    {
      return m_p_xray_beam;
    }

} // namespace gVirtualXRay
