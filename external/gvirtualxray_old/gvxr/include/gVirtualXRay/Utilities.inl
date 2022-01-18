/*

Copyright (c) 2016, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       Utilities.inl
*
*   @brief      Some utility functions that do not fit anywhere else.
*
*   @version    1.0
*
*   @date       25/08/2016
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
#include <cstring>
#include <cmath>


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


    //---------------------------------------
    inline bool isMAT(const char* aFileName)
    //---------------------------------------
    {
        if (checkExtension(aFileName, "mat") ||
                checkExtension(aFileName, "MAT"))
            {
            return (true);
            }
        else
            {
            return (false);
            }
    }


    //---------------------------------------
    inline bool isDAT(const char* aFileName)
    //---------------------------------------
    {
        if (checkExtension(aFileName, "dat") ||
                checkExtension(aFileName, "DAT"))
            {
            return (true);
            }
        else
            {
            return (false);
            }
    }


    //---------------------------------------
    inline bool isTXT(const char* aFileName)
    //---------------------------------------
    {
        if (checkExtension(aFileName, "txt") ||
                checkExtension(aFileName, "TXT"))
            {
            return (true);
            }
        else
            {
            return (false);
            }
    }


    //---------------------------------------
    inline bool isPGM(const char* aFileName)
    //---------------------------------------
    {
        if (checkExtension(aFileName, "pgm") ||
                checkExtension(aFileName, "PGM"))
            {
            return (true);
            }
        else
            {
            return (false);
            }
    }


    //---------------------------------------
    inline bool isRAW(const char* aFileName)
    //---------------------------------------
    {
        if (checkExtension(aFileName, "raw") ||
                checkExtension(aFileName, "RAW"))
            {
            return (true);
            }
        else
            {
            return (false);
            }
    }


    //---------------------------------------
    inline bool isMHA(const char* aFileName)
    //---------------------------------------
    {
        if (checkExtension(aFileName, "mha") ||
                checkExtension(aFileName, "MHA"))
            {
            return (true);
            }
        else
            {
            return (false);
            }
    }


    //---------------------------------------
    inline bool isMHD(const char* aFileName)
    //---------------------------------------
    {
        if (checkExtension(aFileName, "mhd") ||
                checkExtension(aFileName, "MHD"))
            {
            return (true);
            }
        else
            {
            return (false);
            }
    }


    //---------------------------------------
    inline bool isDCM(const char* aFileName)
    //---------------------------------------
    {
        if (checkExtension(aFileName, "dcm") ||
                checkExtension(aFileName, "DCM"))
            {
            return (true);
            }
        else
            {
            return (false);
            }
    }


    //---------------------------------------
    inline bool isJPEG(const char* aFileName)
    //---------------------------------------
    {
        if (checkExtension(aFileName, "jpg") ||
                checkExtension(aFileName, "jpeg") ||
                checkExtension(aFileName, "JPG") ||
                checkExtension(aFileName, "JPEG"))
            {
            return (true);
            }
        else
            {
            return (false);
            }
    }


    //---------------------------------------
    inline bool isTIFF(const char* aFileName)
    //---------------------------------------
    {
        if (checkExtension(aFileName, "tif") ||
                checkExtension(aFileName, "tiff") ||
                checkExtension(aFileName, "TIF") ||
                checkExtension(aFileName, "TIFF"))
            {
            return (true);
            }
        else
            {
            return (false);
            }
    }


    //---------------------------------------------
    inline bool isMAT(const std::string& aFileName)
    //---------------------------------------------
    {
        return (isMAT(aFileName.data()));
    }


    //---------------------------------------------
    inline bool isDAT(const std::string& aFileName)
    //---------------------------------------------
    {
        return (isDAT(aFileName.data()));
    }


    //---------------------------------------------
    inline bool isTXT(const std::string& aFileName)
    //---------------------------------------------
    {
        return (isTXT(aFileName.data()));
    }


    //---------------------------------------------
    inline bool isPGM(const std::string& aFileName)
    //---------------------------------------------
    {
        return (isPGM(aFileName.data()));
    }


    //---------------------------------------------
    inline bool isRAW(const std::string& aFileName)
    //---------------------------------------------
    {
        return (isRAW(aFileName.data()));
    }


//---------------------------------------------
inline bool isMHA(const std::string& aFileName)
//---------------------------------------------
{
    return (isMHA(aFileName.data()));
}


//---------------------------------------------
inline bool isMHD(const std::string& aFileName)
//---------------------------------------------
{
    return (isMHD(aFileName.data()));
}


//---------------------------------------------
inline bool isDCM(const std::string& aFileName)
//---------------------------------------------
{
    return (isDCM(aFileName.data()));
}


//----------------------------------------------
inline bool isJPEG(const std::string& aFileName)
//----------------------------------------------
{
    return (isJPEG(aFileName.data()));
}


//----------------------------------------------
inline bool isTIFF(const std::string& aFileName)
//----------------------------------------------
{
    return (isTIFF(aFileName.data()));
}


//-------------------------------------------------
inline bool checkExtension(const char* aFileName,
                           const char* anExtension)
//-------------------------------------------------
{
    return (checkExtension(std::string(aFileName), std::string(anExtension)));
}


//-----------------------
inline bool isBigEndian()
//-----------------------
{
    unsigned int x(1);
    return (reinterpret_cast<unsigned char*>(&x)[0] == 0);
}


//--------------------------
inline bool isLittleEndian()
//--------------------------
{
    return (!isBigEndian());
}


//-------------------------------
inline bool isLetter(char aValue)
//-------------------------------
{
    return (('a' <= aValue && aValue <= 'z') ||
        ('A' <= aValue && aValue <= 'Z'));
}


//----------------------------------
inline bool isUpperCase(char aValue)
//----------------------------------
{
	return ('A' <= aValue && aValue <= 'Z');
}


//----------------------------------
inline bool isLowerCase(char aValue)
//----------------------------------
{
	return ('a' <= aValue && aValue <= 'z');
}


//-------------------------------
inline bool isNumber(char aValue)
//-------------------------------
{
	return ('0' <= aValue && aValue <= '9');
}


//--------------------------------------------
template<typename T> void swapBytes(T& aValue)
//--------------------------------------------
{
    // There is more than one byte in T
    if (sizeof(T) != 1)
        {
        // Swap bytes
        char* p_casted_format(reinterpret_cast<char*>(&aValue));
        for (unsigned int i(0); i < sizeof(T) / 2; ++i)
            {
            char temp(p_casted_format[i]);
            p_casted_format[i] = p_casted_format[sizeof(T) - 1 - i];
            p_casted_format[sizeof(T) - 1 - i] = temp;
            }
        }
}


//-------------------------------------------------------------
template<typename T> T degreeToRadian(const T& anAngleInDegree)
//-------------------------------------------------------------
{
    return (anAngleInDegree * M_PI / 180.0);
}


//-------------------------------------------------------------
template<typename T> T radianToDegree(const T& anAngleInRadian)
//-------------------------------------------------------------
{
    return (anAngleInRadian * 180.0 / M_PI);
}


//--------------------------------------------
template <typename T> int sgn(const T& aValue)
//--------------------------------------------
{
    return (T(0) < aValue) - (aValue < T(0));
}

} // namespace gVirtualXRay
