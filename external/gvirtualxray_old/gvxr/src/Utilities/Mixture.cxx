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
 *   @file       Mixture.cxx
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
 *   http://www.fpvidal.net/, Sept 2018, 2018, version 1.1, BSD 3-Clause License
 *
 ********************************************************************************
 */


//******************************************************************************
//	Include
//******************************************************************************
#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef __Mixture_h
#include "gVirtualXRay/Mixture.h"
#endif

#ifndef __Units_h
#include "gVirtualXRay/Units.h"
#endif

#ifndef __AtomicElement_h
#include "gVirtualXRay/AtomicElement.h"
#endif

#ifndef __ElementSet_h
#include "gVirtualXRay/ElementSet.h"
#endif

#ifndef __ConstantValues_h
#include "gVirtualXRay/ConstantValues.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;


//------------------------------------------------
void Mixture::addElement(const std::string& aName,
                         double aPercentage)
//------------------------------------------------
{
	m_p_weight_set[ElementSet::getInstance().getElement(aName).getAtomicNumber()] = aPercentage;
	m_molar_mass = computeMolarMass();
}


//--------------------------------
void Mixture::normaliseWeightSet()
//--------------------------------
{
    double sum(0.0);

	for (std::map<int, double>::const_iterator ite = m_p_weight_set.begin();
			ite != m_p_weight_set.end();
			++ite)
	{
		sum += ite->second;
	}

    if (sum > EPSILON)
    {
    	for (std::map<int, double>::iterator ite = m_p_weight_set.begin();
    			ite != m_p_weight_set.end();
    			++ite)
    	{
    		ite->second /= sum;
    	}
    }
    m_molar_mass = computeMolarMass();
}


//--------------------------------
double Mixture::computeMolarMass()
//--------------------------------
{
    m_molar_mass = 0.0;

	for (std::map<int, double>::const_iterator ite = m_p_weight_set.begin();
			ite != m_p_weight_set.end();
			++ite)
	{
		m_molar_mass += ite->second * ElementSet::getInstance().getElement(ite->first).getMolarMass();
	}

    return (m_molar_mass);
}


//------------------------------------------------------------
double Mixture::getMassAttenuationTotal(double anEnergy) const
//------------------------------------------------------------
{
	double mass_attenuation_coefficient = 0.0;

	for (std::map<int, double>::const_iterator ite = m_p_weight_set.begin();
			ite != m_p_weight_set.end();
			++ite)
	{
		mass_attenuation_coefficient += ite->second * ElementSet::getInstance().getElement(ite->first).getMassAttenuationTotal(anEnergy);
	}

    return (mass_attenuation_coefficient);
}


//-----------------------------------------------------
bool Mixture::operator==(const Mixture& aMixture) const
//-----------------------------------------------------
{
     for (std::map<int, double>::const_iterator ite = aMixture.m_p_weight_set.begin();
			 ite != aMixture.m_p_weight_set.end();
			 ++ite)
     {
		 if (m_p_weight_set.find(ite->first) == m_p_weight_set.end()) return false;
		 if (m_p_weight_set.find(ite->first)->second != ite->second) return false;
     }
     
     for (std::map<int, double>::const_iterator ite = m_p_weight_set.begin();
			 ite != m_p_weight_set.end();
			 ++ite)
     {
		 if (aMixture.m_p_weight_set.find(ite->first) == aMixture.m_p_weight_set.end()) return false;
		 if (aMixture.m_p_weight_set.find(ite->first)->second != ite->second) return false;
     }
    
     if (m_molar_mass != aMixture.m_molar_mass) return false;
     if (m_density    != aMixture.m_density)    return false;
    
    return true;
}


void Mixture::setLabel()
{
    m_label.clear();
    for (std::map<int, double>::const_iterator ite = m_p_weight_set.begin();
			 ite != m_p_weight_set.end();
			 ++ite)
    {
        m_label += ElementSet::getInstance().getElement(ite->first).getSymbol();
        m_label += std::to_string(ite->second);
    }
}

