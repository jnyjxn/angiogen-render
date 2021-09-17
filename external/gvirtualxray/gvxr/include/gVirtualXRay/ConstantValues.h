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


#ifndef __ConstantValues_h
#define __ConstantValues_h


/**
********************************************************************************
*
*   @file       ConstantValues.h
*
*   @brief      Constant values, such as the Z number of different atoms, etc.
*
*   @version    1.0
*
*   @date       13/10/2014
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


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  defines
//******************************************************************************
/// Biggest value that can be stored with a real number
#define RATIONAL_NUMBER_MAX   FLT_MAX


/// Smallest value that can be stored with a real number
#define EPSILON               0.0000001


//******************************************************************************
//  Constant declarations
//******************************************************************************
const double PI = 3.14159265358979323846; ///< Pi
const double PI_2 = PI / 2.0; ///< Pi divided by 2.0

const double N_Avogadro = 6.02552 * 1.0e23; ///< Avogadro number

const unsigned short Z_H  =  1; ///< Hydrogen atomic number
const unsigned short Z_C  =  6; ///< Carbon atomic number
const unsigned short Z_N  =  7; ///< Nitrogen atomic number
const unsigned short Z_O  =  8; ///< Oxygen atomic number
const unsigned short Z_Na = 11; ///< Sodium atomic number
const unsigned short Z_Mg = 12; ///< Magnesium atomic number
const unsigned short Z_P  = 15; ///< Phosphorus atomic number
const unsigned short Z_S  = 16; ///< Sulfur atomic number
const unsigned short Z_Cl = 17; ///< Chlorine atomic number
const unsigned short Z_Ar = 18; ///< Argon atomic number
const unsigned short Z_K  = 19; ///< Potassium atomic number
const unsigned short Z_Ca = 20; ///< Calcium atomic number
const unsigned short Z_Ti = 22; ///< Titanium atomic number
const unsigned short Z_Fe = 26; ///< Iron atomic number
const unsigned short Z_Cu = 29; ///< Copper atomic number
const unsigned short Z_Zn = 30; ///< Zinc atomic number
const unsigned short Z_Ag = 47; ///< Silver atomic number
const unsigned short Z_Sn = 50; ///< Tin atomic number
const unsigned short Z_I  = 53; ///< Iodine atomic number

} // namespace gVirtualXRay


#endif // __ConstantValues_h
