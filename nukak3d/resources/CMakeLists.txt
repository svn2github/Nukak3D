###### BEGIN GPL LICENSE BLOCK#####
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
# The Original Code is Copyright (C) 2007-2010 by Bioingenium Research Group.
# Bogota - Colombia
# All rights reserved.
#
# Author(s): Alexander Pinzón Fernández.
#
###### END GPL LICENSE BLOCK#####

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
	INSTALL_FILES(/bin FILES ${DCMTK_dump2dcm_PROGRAM})
	INSTALL_FILES(/bin FILES ${DCMTK_echoscu_PROGRAM})
	INSTALL_FILES(/bin FILES ${DCMTK_findscu_PROGRAM})
	INSTALL_FILES(/bin FILES ${DCMTK_movescu_PROGRAM})
	INSTALL_FILES(/bin FILES ${DCMTK_storescp_PROGRAM})
ENDIF(DCMTK_FOUND)