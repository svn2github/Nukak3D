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
# Author(s): none yet.
#
###### END GPL LICENSE BLOCK#####

IF(wxWidgets_BUILD_SHARED_LIBS OR wxWidgets_SHARED)

      SET(__install__libs ${__install__libs}
		"${wxWidgets_LIBRARY_DIRS}/libwx_base-2.8.so.0"
        "${wxWidgets_LIBRARY_DIRS}/libwx_base_xml-2.8.so.0"
        "${wxWidgets_LIBRARY_DIRS}/libwx_gtk2_adv-2.8.so.0"
        "${wxWidgets_LIBRARY_DIRS}/libwx_gtk2_aui-2.8.so.0"
        "${wxWidgets_LIBRARY_DIRS}/libwx_gtk2_core-2.8.so.0"
        "${wxWidgets_LIBRARY_DIRS}/libwx_gtk2_html-2.8.so.0"
        "${wxWidgets_LIBRARY_DIRS}/libwx_gtk2_xrc-2.8.so.0"		
		"${wxWidgets_LIBRARY_DIRS}/libwx_base-2.8.so.0.4.0"
        "${wxWidgets_LIBRARY_DIRS}/libwx_base_xml-2.8.so.0.4.0"
        "${wxWidgets_LIBRARY_DIRS}/libwx_gtk2_adv-2.8.so.0.4.0"
        "${wxWidgets_LIBRARY_DIRS}/libwx_gtk2_aui-2.8.so.0.4.0"
        "${wxWidgets_LIBRARY_DIRS}/libwx_gtk2_core-2.8.so.0.4.0"
        "${wxWidgets_LIBRARY_DIRS}/libwx_gtk2_html-2.8.so.0.4.0"
        "${wxWidgets_LIBRARY_DIRS}/libwx_gtk2_xrc-2.8.so.0.4.0"
		"${vtkINRIA3D_LIBRARY_DIRS}/libImageViewer.so"
        "${vtkINRIA3D_LIBRARY_DIRS}/libwxAddOn.so"
		"${vtkINRIA3D_LIBRARY_DIRS}/libwxVtk.so"

        )
ENDIF(wxWidgets_BUILD_SHARED_LIBS OR wxWidgets_SHARED)
  
FOREACH(lib
      ${__install__libs}
      )
    IF(EXISTS ${lib})
      SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS
        ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} ${lib})
    ENDIF(EXISTS ${lib})
ENDFOREACH(lib)
