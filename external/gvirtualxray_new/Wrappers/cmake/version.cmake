#
#
# Copyright (c) 2020, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
# http://www.fpvidal.net/
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# 3. Neither the name of the Bangor University nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


SET (gvxrWrappers_VERSION_MAJOR 1)
SET (gvxrWrappers_VERSION_MINOR 0)
SET (gvxrWrappers_VERSION_PATCH 6)
SET (gvxrWrappers_VERSION_STRING "${gvxrWrappers_VERSION_MAJOR}.${gvxrWrappers_VERSION_MINOR}.${gvxrWrappers_VERSION_PATCH}")

SET (gvxrWrappers_LICENSE )
SET (PROJECT_NAME         "gvxrWrapper")
SET (PROJECT_DESCRIPTION  "SWIG Wrappers to Simple Virtual X-Ray Imaging Library on GPU")
SET (EMAIL                "franck.p.vidal@fpvidal.net")
SET (MAIN_WEBSITE         "http://gvirtualxray.sourceforge.net/")
SET (MAIN_PACKAGE_NAME    "${PROJECT_NAME}")
SET (MAIN_PROJECT_VERSION "${gvxrWrappers_VERSION_MAJOR}.${gvxrWrappers_VERSION_MINOR}.${gvxrWrappers_VERSION_PATCH}")

SET (INSTALL_DIR "${PROJECT_NAME}-${gvxrWrappers_VERSION_MAJOR}.${gvxrWrappers_VERSION_MINOR}.${gvxrWrappers_VERSION_PATCH}")

SET (CPACK_PACKAGE_VERSION_MAJOR ${gvxrWrappers_VERSION_MAJOR})
SET (CPACK_PACKAGE_VERSION_MINOR ${gvxrWrappers_VERSION_MINOR})
SET (CPACK_PACKAGE_VERSION_PATCH ${gvxrWrappers_VERSION_PATCH})
SET (CPACK_PACKAGE_VERSION "${gvxrWrappers_VERSION_MAJOR}.${gvxrWrappers_VERSION_MINOR}.${gvxrWrappers_VERSION_PATCH}")

SET (CPACK_PACKAGE_VENDOR "gVirtualXRay team")
SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY "gVirtualXRay - Installation")
SET (CPACK_PACKAGE_INSTALL_DIRECTORY "gVirtualXRay")

SET (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt)

SET (COPYRIGHT_STRING "(C) 2019 Dr Franck P. Vidal")
