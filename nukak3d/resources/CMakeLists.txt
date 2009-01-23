INSTALL_FILES(/include FILES nukak3d.rc)
IF (APPLE)
	SET(RESOURCES_LANGUAGE_PATH /nukak3d.app/Contents/Resources/locale)
	INSTALL_FILES(/bin/nukak3d.app/Contents/Resources FILES 
		${CMAKE_SOURCE_DIR}/resources/iconMac.icns)
ELSE (APPLE)
	SET(RESOURCES_LANGUAGE_PATH /.nukak3d/locale)
ENDIF (APPLE)

SET(LANGUAGE en es)
FOREACH(lang ${LANGUAGE})
	INSTALL_FILES(/bin/${RESOURCES_LANGUAGE_PATH}/${lang}/LC_MESSAGES FILES
		${CMAKE_SOURCE_DIR}/resources/.nukak3d/locale/${lang}/LC_MESSAGES/nukak3d.mo
	)
ENDFOREACH(lang)

IF(DCMTK_FOUND)
	SET(DICOM_PROGRAM_PATHS ${DCMTK_DIR}
		${DCMTK_DIR}/bin
		${DCMTK_DIR}/../install/bin
		${DCMTK_DIR}/../dcmtk-3.5.4-win32-i386/
		${DCMTK_DIR}/../dcmtk-3.5.4-win32-i386/bin 
		/usr/bin
		/usr/local/bin
		/opt/local/bin
	)
	FIND_PROGRAM(STORE_SCP 
		NAMES storescp 
		PATHS ${DICOM_PROGRAM_PATHS}
	)
	FIND_PROGRAM(ECHO_SCU 
		NAMES echoscu
		PATHS ${DICOM_PROGRAM_PATHS}
	)
	IF( STORE_SCP)
		INSTALL_FILES(/bin FILES ${STORE_SCP})
	ENDIF( STORE_SCP)
	IF( ECHO_SCU)
		INSTALL_FILES(/bin FILES ${ECHO_SCU})
	ENDIF( ECHO_SCU)
ENDIF(DCMTK_FOUND)