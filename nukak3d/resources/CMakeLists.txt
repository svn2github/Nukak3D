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