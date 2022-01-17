macro(build_xcom staging_prefix)


# make a custom XCOM configuration file

SET (XCOM_VERSION_STRING 3.1)
SET (XCOM_VERSION_MAJOR  3.1)
SET (XCOM_VERSION_MINOR  0)

#IF (WIN32)
#    ExternalProject_Add(XCOM
#        URL  "http://physics.nist.gov/PhysRefData/Xcom/XCOM.exe"
#        URL_MD5 "4bf6edf834bf780671160e8e5ad2a53f"
#        UPDATE_COMMAND ""
#        SOURCE_DIR XCOM
#        CONFIGURE_COMMAND ""
#        CMAKE_COMMAND ""
#        BUILD_COMMAND ""
#        INSTALL_COMMAND ""
#    )
#ELSE (WIN32)
#    IF (UNIX)
        ExternalProject_Add(xcom
            URL  "http://physics.nist.gov/PhysRefData/Xcom/XCOM.tar.gz"
            URL_MD5 "747a5be1bf38caad5b347b9d03ab3463"
            UPDATE_COMMAND ""
            SOURCE_DIR XCOM
            CONFIGURE_COMMAND ""
            CMAKE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
        )
#    ELSE (UNIX)
#        SET (USE_SYSTEM_XCOM ON)
#        MESSAGE(WARNING "Unknown system, we cannot install XCOM for you, please install it by hand. You can find it on the NIST's website: http://physics.nist.gov/PhysRefData/Xcom/Text/download.html Set XCOM_PATH to the path where you extracted the archive.")
#    ENDIF (UNIX)
#ENDIF (WIN32)




SET(XCOM_PATH ${staging_prefix}/XCOM)

# copy libraries into install tree
install(DIRECTORY ${staging_prefix}/XCOM     DESTINATION ${INSTALL_DIR}/third_party     COMPONENT Development)

endmacro(build_xcom)

