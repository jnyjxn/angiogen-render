#
#
# Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


FUNCTION(CreateHeaderFiles PREFIX TARGET_NAME RESOURCES_SOURCES)

	SET (BINARY_FILES)
	foreach(F ${RESOURCES_SOURCES})
		# Get the file name
		GET_FILENAME_COMPONENT(FILE_NAME ${F} NAME)

		add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp
			DEPENDS ${F}
			COMMAND ${CMAKE_COMMAND}
			ARGS -E copy ${F} ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp
			COMMENT "Build ${FILE_NAME}.tmp"
		)
		SET (BINARY_FILES ${BINARY_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp)
		set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp)

		add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp.gz
			DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp
			COMMAND ${GZIP_TOOL}
			ARGS  --best --force --quiet ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp -c > ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp.gz
			COMMENT "Build ${FILE_NAME}.tmp.gz"
		)
		SET (BINARY_FILES ${BINARY_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp.gz)
		set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp.gz)

		STRING(REGEX REPLACE "\\." "_" VARIABLE_NAME ${FILE_NAME})

		SET (CACHE_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.cache)
		FILE(WRITE  ${CACHE_FILE_NAME} "")
		FILE(APPEND ${CACHE_FILE_NAME} "set(INPUT_FILE_NAME \"${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp.gz\")\n")
		FILE(APPEND ${CACHE_FILE_NAME} "set(OUTPUT_FILE_NAME \"${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.h\")\n")
		FILE(APPEND ${CACHE_FILE_NAME} "set(VARIABLE_NAME \"g_${VARIABLE_NAME}\")\n")

		SET (CMAKE_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.cmake)
		FILE(WRITE  ${CMAKE_FILE_NAME} "")
		FILE(APPEND ${CMAKE_FILE_NAME} "include(\"${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.cache\")\n")
		FILE(APPEND ${CMAKE_FILE_NAME} "include(\"${CMAKE_CURRENT_BINARY_DIR}/cmake/Bin2C.cmake\")\n")
		FILE(APPEND ${CMAKE_FILE_NAME} "FILE2C(\${INPUT_FILE_NAME} \${VARIABLE_NAME} \${OUTPUT_FILE_NAME})\n")

		add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.h
			DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.tmp.gz
			COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.cmake
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			COMMENT "Build ${FILE_NAME}.h"
		)
		SET (BINARY_FILES ${BINARY_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.h)
		set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.h)
	endforeach()

	ADD_CUSTOM_TARGET(${TARGET_NAME} ALL echo DEPENDS ${BINARY_FILES})
ENDFUNCTION(CreateHeaderFiles)