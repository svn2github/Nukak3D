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
	FIND_PROGRAM(DICOM_PROGRAMS 
		NAMES storescp echoscu
		PATHS ${DCMTK_DIR}
		${DCMTK_DIR}/bin
		${DCMTK_DIR}/../install/bin
		${DCMTK_DIR}/../dcmtk-3.5.4-win32-i386/
		${DCMTK_DIR}/../dcmtk-3.5.4-win32-i386/bin 
		/usr/bin
		/usr/local/bin
		/opt/local/bin
	)
	IF( DICOM_PROGRAMS)
		INSTALL_FILES(/bin FILES ${DICOM_PROGRAMS})
	ENDIF( DICOM_PROGRAMS)
ENDIF(DCMTK_FOUND)