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

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/resources)

ADD_DEFINITIONS(-DCHECKBOXVIEW=1)
IF (WIN32)
 SET(nkToolBar_SRCS
 nkToolBar.cpp
 ${CMAKE_SOURCE_DIR}/include/nkToolBar.h
 )
ELSE(WIN32)
 IF (APPLE)
	SET( MACOSX_BUNDLE_ICON_FILE iconMac.icns )
	SET( MACOSX_BUNDLE_SHORT_VERSION_STRING 4.6 )
	SET( MACOSX_BUNDLE_VERSION 4.6.0 )
	SET( MACOSX_BUNDLE_LONG_VERSION_STRING 4.6.0.0 )
	SET( CMAKE_OSX_ARCHITECTURES ppc;i386 )
	SET(GUI_EXECUTABLE MACOSX_BUNDLE)
 ELSE(APPLE)
	INCLUDE_DIRECTORIES(
		${GTK_INCLUDE_DIRS}
	)
   ADD_DEFINITIONS(${GTK_CFLAGS})
 ENDIF(APPLE)
 SET(nkToolBar_SRCS
 nkToolBarMac.cpp
 ${CMAKE_SOURCE_DIR}/include/nkToolBarMac.h
 )
ENDIF(WIN32)

SET(nukak3d_SRCS
nkMain.cpp
${CMAKE_SOURCE_DIR}/include/nkMain.h
nkNukak3D.cpp
${CMAKE_SOURCE_DIR}/include/nkNukak3D.h
nkObj3DViewer.cpp
${CMAKE_SOURCE_DIR}/include/nkObj3DViewer.h
${nkToolBar_SRCS}
nkVolViewer.cpp
${CMAKE_SOURCE_DIR}/include/nkVolViewer.h
nkIODialog.cpp
${CMAKE_SOURCE_DIR}/include/nkIODialog.h
${CMAKE_SOURCE_DIR}/include/nkAcercaDe.h
nkLevelSets.cpp
${CMAKE_SOURCE_DIR}/include/nkLevelSets.h
nkInteractorStyleEndoCamera.cpp
${CMAKE_SOURCE_DIR}/include/nkInteractorStyleEndoCamera.h
nkStoreSCP.cpp
${CMAKE_SOURCE_DIR}/include/nkStoreSCP.h
nkFindSCU.cpp
${CMAKE_SOURCE_DIR}/include/nkFindSCU.h
nkDICOMDictionary.cpp
${CMAKE_SOURCE_DIR}/include/nkDICOMDictionary.h
nkPipedProcess.cpp
${CMAKE_SOURCE_DIR}/include/nkPipedProcess.h
nkUtilities.cpp
${CMAKE_SOURCE_DIR}/include/nkUtilities.h
nkDICOMServer.cpp
${CMAKE_SOURCE_DIR}/include/nkDICOMServer.h
nkPlugin.cpp
${CMAKE_SOURCE_DIR}/include/nkPlugin.h
nkEngine.cpp
${CMAKE_SOURCE_DIR}/include/nkEngine.h
${CMAKE_SOURCE_DIR}/include/nkKernel.h
${CMAKE_SOURCE_DIR}/include/nkITKFilterServer.h
${CMAKE_SOURCE_DIR}/include/nkImageToImageFilter.h
${CMAKE_SOURCE_DIR}/include/nkMacro.h
nkAdminPluginGui.cpp
${CMAKE_SOURCE_DIR}/include/nkAdminPluginGui.h
)

IF( WIN32 )
	SET(GUI_EXECUTABLE WIN32)
	SET(nukak3d_SRCS
	${nukak3d_SRCS}
	${CMAKE_SOURCE_DIR}/resources/nukak3d.rc
)
ENDIF( WIN32 )

ADD_EXECUTABLE(nukak3d 
${GUI_EXECUTABLE}
${nukak3d_SRCS}
)

IF(APPLE)
	SET(LIB_VTKINRIA3D 
	vtkVisuManagement
	vtkDataManagement
	wxVtk
	wxAddOn
	vtkRenderingAddOn
	ImageViewer)
ELSE(APPLE)
	SET(LIB_VTKINRIA3D 
   vtkVisuManagement
   vtkDataManagement
   wxVtk
   wxAddOn
   ImageViewer
   DICOMImporter
   getoptcompatlib
   itkIOAddOn
   vtkRenderingAddOn
   vtkWidgetsAddOn
   wxImageViewer)
ENDIF(APPLE)

IF(APPLE)
	TARGET_LINK_LIBRARIES(nukak3d
	${wxWidgets_LIBRARIES}
	${LIB_VTKINRIA3D}
	${ITK_LIBRARIES}
	vtkCommon
	vtkDICOMParser
	vtkexoIIc
	vtkexpat
	vtkFiltering
	vtkfreetype
	vtkftgl
	vtkGenericFiltering
	vtkGraphics
	vtkHybrid
	vtkImaging
	vtkIO
	vtkjpeg
	vtkNetCDF
	vtkParallel
	vtkpng
	vtkRendering
	vtksys
	vtktiff
	vtkVolumeRendering
	vtkWidgets
	vtkzlib
	)
ELSE(APPLE)
	TARGET_LINK_LIBRARIES(nukak3d
	${LIB_VTKINRIA3D}
	${ITK_LIBRARIES}
	ITKIO
	ITKBasicFilters
	vtkCommon
	vtkDICOMParser
	vtkexoIIc
	vtkexpat
	vtkFiltering
	vtkfreetype
	vtkftgl
	vtkGenericFiltering
	vtkGraphics
	vtkHybrid
	vtkImaging
	vtkIO
	vtkjpeg
	vtkNetCDF
	vtkParallel
	vtkpng
	vtkRendering
	vtksys
	vtktiff
	vtkVolumeRendering
	vtkWidgets
	vtkzlib
	${wxWidgets_LIBRARIES}
	)
ENDIF(APPLE)
INSTALL_TARGETS(/bin nukak3d)