/*
 
 Copyright (c) 2018, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
 *   @file       PhotonCrossSection.inl
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
#include <string>


//******************************************************************************
//  defines
//******************************************************************************


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//--------------------------------------------------
inline PhotonCrossSection::PhotonCrossSection():
//--------------------------------------------------
		m_use_HU(false),
        m_use_element(false),
        m_use_mixture(false),
        m_use_mass_attenuation_coefficient(false),
        m_use_linear_attenuation_coefficient(false),
        m_HU(0),
        m_Z_number(0),
        m_mass_attenuation_coefficient(0.0),
        m_linear_attenuation_coefficient(0.0),
        m_density(-1.0)
//--------------------------------------------------
{}


//-------------------------------------------------------------------------------------
inline PhotonCrossSection::PhotonCrossSection(const std::map<int, double>& aWeightSet):
//-------------------------------------------------------------------------------------
        m_use_HU(false),
        m_use_element(false),
        m_use_mixture(true),
        m_use_mass_attenuation_coefficient(false),
        m_use_linear_attenuation_coefficient(false),
        m_HU(0),
        m_Z_number(0),
        m_mass_attenuation_coefficient(0.0),
        m_linear_attenuation_coefficient(0.0),
        m_mixture(aWeightSet),
        m_density(-1.0),
        m_label(m_mixture.getLabel())
//-------------------------------------------------------------------------------------
{
}


//---------------------------------------------------------------------
inline PhotonCrossSection::PhotonCrossSection(const Mixture& aMixture):
//---------------------------------------------------------------------
		m_use_HU(false),
        m_use_element(false),
        m_use_mixture(true),
        m_use_mass_attenuation_coefficient(false),
        m_use_linear_attenuation_coefficient(false),
        m_HU(0),
        m_Z_number(0),
        m_mass_attenuation_coefficient(0.0),
        m_linear_attenuation_coefficient(0.0),
        m_mixture(aMixture),
        m_density(-1.0),
        m_label(m_mixture.getLabel())
//---------------------------------------------------------------------
{}


//-------------------------------------
inline void PhotonCrossSection::reset()
//-------------------------------------
{
    m_use_HU      = false;
    m_use_element = false;
    m_use_mixture = false;
    m_use_mass_attenuation_coefficient   = false;
    m_use_linear_attenuation_coefficient = false;

    m_density = -1.0;
    m_label.clear();
}


//---------------------------------------------
inline void PhotonCrossSection::useHU(short HU)
//---------------------------------------------
{
    reset();
    m_use_HU      = true;
    m_HU = HU;

    m_label = "HU: ";
    m_label += std::to_string(HU);   
}


//---------------------------------------------------------
inline void PhotonCrossSection::useHounsfieldUnit(short HU)
//---------------------------------------------------------
{
    useHU(HU);
}


//----------------------------------------------------------
inline void PhotonCrossSection::useHounsfieldValue(short HU)
//----------------------------------------------------------
{
    useHU(HU);
}



//-----------------------------------------------------------------
inline void PhotonCrossSection::useMixture(const Mixture& aMixture)
//-----------------------------------------------------------------
{
    reset();
    m_use_mixture = true;
    m_mixture = aMixture;

    m_label = m_mixture.getLabel();
}


//-------------------------------------------------------------------------------
inline void PhotonCrossSection::useMixture(const std::map<int, double>& aMixture)
//-------------------------------------------------------------------------------
{
    reset();
    m_use_mixture = true;
    m_mixture.setMixture(aMixture);

    m_label = m_mixture.getLabel();
}


//--------------------------------------------------------------------------------
inline void PhotonCrossSection::useMassAttenuationCoefficient(double aCoefficient)
//--------------------------------------------------------------------------------
{
    reset();
    m_use_mass_attenuation_coefficient   = true;
    m_mass_attenuation_coefficient = aCoefficient;

    m_label = "mu/rho: ";
    m_label += std::to_string(aCoefficient);   
}


//----------------------------------------------------------------------------------
inline void PhotonCrossSection::useLinearAttenuationCoefficient(double aCoefficient)
//----------------------------------------------------------------------------------
{
    reset();
    m_use_linear_attenuation_coefficient = true;
    m_linear_attenuation_coefficient = aCoefficient;

    m_label = "mu: ";
    m_label += std::to_string(aCoefficient);       
}


//---------------------------------------------------------
inline void PhotonCrossSection::setDensity(double aDensity)
//---------------------------------------------------------
{
    m_density = aDensity;
}


//------------------------------------------------------------
inline double PhotonCrossSection::getMu(double anEnergy) const
//------------------------------------------------------------
{
    return (getLinearAttenuationCoefficient(anEnergy));
}


//------------------------------------------------------------
inline const std::string& PhotonCrossSection::getLabel() const
//------------------------------------------------------------
{
    return (m_label);
}


//-----------------------------------------------------------------------------------
inline bool PhotonCrossSection::operator!=(const PhotonCrossSection& aTestData) const
//-----------------------------------------------------------------------------------
{
	return (!((*this) == aTestData));
}


} // namespace gVirtualXRay
