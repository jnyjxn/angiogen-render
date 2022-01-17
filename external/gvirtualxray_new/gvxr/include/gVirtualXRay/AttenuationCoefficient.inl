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
*	@file		AttenuationCoefficient.inl
*
*	@brief		Class to manage an attenuation coefficient
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
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-----------------------------------------------------------------------------------------------------
inline AttenuationCoefficient::AttenuationCoefficient(const RATIONAL_NUMBER& anEnergy,
													  const RATIONAL_NUMBER& anAttenuationCoefficient):
//-----------------------------------------------------------------------------------------------------
		m_energy(anEnergy),
		m_mu(anAttenuationCoefficient)
//-----------------------------------------------------------------------------------------------------
{}


//--------------------------------------------------------------
inline RATIONAL_NUMBER AttenuationCoefficient::getEnergy() const
//--------------------------------------------------------------
{
	return (m_energy);
}


//------------------------------------------------------------------------------
inline RATIONAL_NUMBER AttenuationCoefficient::getAttenuationCoefficient() const
//------------------------------------------------------------------------------
{
	return (m_mu);
}


//-----------------------------------------------------------
inline std::istream& operator >>(std::istream &is,
								 AttenuationCoefficient &obj)
//-----------------------------------------------------------
{
	is >> obj.m_energy;
	is >> obj.m_mu;
	
	obj.m_energy *= MeV;
	obj.m_mu *= 1.0 / cm;
	
	return (is);
}


//-----------------------------------------------------------------
inline std::ostream& operator <<(std::ostream &os,
								 const AttenuationCoefficient &obj)
//-----------------------------------------------------------------
{
	os << obj.m_energy / MeV << " MeV\t" << obj.m_mu / (cm2/g) << " cm2/g";
	return (os);
}


} // namespace gVirtualXRay