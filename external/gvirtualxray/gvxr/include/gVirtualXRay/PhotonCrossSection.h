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


#ifndef __PhotonCrossSection_h
#define __PhotonCrossSection_h


/**
 ********************************************************************************
 *
 *   @file       PhotonCrossSection.h
 *
 *   @brief      Class to manage photon cross sections of elements, compounds and mixtures.
 *
 *   @version    1.0
 *
 *   @date       27/09/2018
 *
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

#ifndef __Mixture_h
#include "gVirtualXRay/Mixture.h"
#endif

//#include <iostream>
//#include <vector>
//#include <string>


//******************************************************************************
//  defines
//******************************************************************************


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {
    
    
    //==============================================================================
    /**
     *   @class  PhotonCrossSection
     *   @brief  PhotonCrossSection is a class to manage photon cross sections of
     *           elements, compounds and mixtures.
     */
    //==============================================================================
    class PhotonCrossSection
    //------------------------------------------------------------------------------
    {
    //******************************************************************************
    public:
        PhotonCrossSection();
        PhotonCrossSection(const std::map<int, double>& aWeightSet);
        PhotonCrossSection(const Mixture& aMixture);

        void reset();
        
        void useHU(short HU);
        void useHounsfieldUnit(short HU);
        void useHounsfieldValue(short HU);

        void useElement(unsigned short Z);
        void useElement(const std::string& aName);
        
        void useMixture(const Mixture& aMixture);
        void useMixture(const std::map<int, double>& aMixture);
        void useMixture(const std::string& aName);

        void useCompound(const std::string& aName);

        void useMassAttenuationCoefficient(double aCoefficient);
        void useLinearAttenuationCoefficient(double aCoefficient);

        void setDensity(double aDensity);
        
        double getMassAttenuationCoefficient(double anEnergy) const;
        double getLinearAttenuationCoefficient(double anEnergy) const;
        double getMu(double anEnergy) const;
        double getDensity() const;
        double getMolarMass() const;
        const std::string& getLabel() const;
        
        bool operator==(const PhotonCrossSection& aTestData) const;
        bool operator!=(const PhotonCrossSection& aTestData) const;

        static std::map<int, double> getMixtureWeightSet(const std::string& aName);
        static std::map<int, double> getCompoundWeightSet(const std::string& aName);

        
    //*****************************************************************************
    protected:
        static void processElement(std::string& aSymbol,
			    std::string& aWeight,
				std::map<int, double>& aWeightSet);

        
        bool  m_use_HU;
        bool  m_use_element;
        bool  m_use_mixture;
        bool  m_use_mass_attenuation_coefficient;
        bool  m_use_linear_attenuation_coefficient;
        short m_HU;
        unsigned short m_Z_number;
        double m_mass_attenuation_coefficient;
        double m_linear_attenuation_coefficient;
        Mixture m_mixture;
        double m_density;
        std::string m_label;
    };
    
    
} // namespace gVirtualXRay


//******************************************************************************
#include "PhotonCrossSection.inl"


#endif // __PhotonCrossSection_h
