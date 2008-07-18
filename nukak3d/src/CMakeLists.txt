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
	itkAddOn
	vtkCommon
    vtkRendering
    vtkRenderingAddOn
    vtkDataManagement
    vtkVisuManagement
	wxVtk
	wxAddOn
	vtkRenderingAddOn
	ImageViewsWithSlidersLib)
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