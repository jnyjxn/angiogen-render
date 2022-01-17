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
*   @file       Mixture.inl
*
*   @brief      Class to manage a mixture (e.g. Ti90Al6V4).
*
*   @version    1.0
*
*   @date       27/09/2018
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
*   http://www.fpvidal.net/, Sept 2018, 2018, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#include <cstring>

#ifndef __ConstantValues_h
#include "gVirtualXRay/ConstantValues.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//------------------------
inline Mixture::Mixture():
//------------------------
        m_molar_mass(0),
        m_density(-1)
//------------------------
{}
    

//---------------------------------------------------------------
inline Mixture::Mixture(const std::map<int, double>& aWeightSet):
//---------------------------------------------------------------
        m_molar_mass(0),
        m_density(-1)
//---------------------------------------------------------------
{
    m_p_weight_set = aWeightSet;

	normaliseWeightSet();
	
	m_molar_mass = computeMolarMass();

    setLabel();
}


//-----------------------------------------------
inline Mixture::Mixture(const Mixture& aMixture):
//-----------------------------------------------
        m_p_weight_set(aMixture.m_p_weight_set),
        m_molar_mass(aMixture.m_molar_mass),
        m_density(aMixture.m_density),
        m_label(aMixture.m_label)
//-----------------------------------------------
{
}


//--------------------------
inline void Mixture::clear()
//--------------------------
{
    m_p_weight_set.clear();
}


//----------------------------------------------------------------------
inline void Mixture::setMixture(const std::map<int, double>& aWeightSet)
//----------------------------------------------------------------------
{
    m_p_weight_set = aWeightSet;
    normaliseWeightSet();
    m_molar_mass = computeMolarMass();

    setLabel();    
}


//-----------------------------------------
inline double Mixture::getMolarMass() const
//-----------------------------------------
{
	return (m_molar_mass);
}


//----------------------------------------------
inline void Mixture::setDensity(double aDensity)
//----------------------------------------------
{
    m_density = aDensity;
}


//---------------------------------------
inline double Mixture::getDensity() const
//---------------------------------------
{
    return m_density;
}


//---------------------------------------------------------------------
inline double Mixture::getLinearAttenuationTotal(double anEnergy) const
//---------------------------------------------------------------------
{
    return (getMassAttenuationTotal(anEnergy) *
            (getDensity()));
}


//-------------------------------------------------
inline double Mixture::getMu(double anEnergy) const
//-------------------------------------------------
{
    return (getLinearAttenuationTotal(anEnergy));
}


//-------------------------------------------------
inline const std::string& Mixture::getLabel() const
//-------------------------------------------------
{
    return (m_label);
}


//---------------------------------------------------------
inline Mixture& Mixture::operator=(const Mixture& aMixture)
//---------------------------------------------------------
{
    m_molar_mass   = aMixture.m_molar_mass;
    m_density      = aMixture.m_density;
    m_p_weight_set = aMixture.m_p_weight_set;
    m_label        = aMixture.m_label;

	return (*this);
}


//------------------------------------------------------------
inline bool Mixture::operator!=(const Mixture& aMixture) const
//------------------------------------------------------------
{
    return (!(operator==(aMixture)));
}


} // namespace gVirtualXRay/
