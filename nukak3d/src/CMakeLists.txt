include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/resources)

IF (WIN32)
 SET(nkToolBar_SRCS
 nkToolBar.cpp
 ${CMAKE_SOURCE_DIR}/include/nkToolBar.h
 )
ELSE(WIN32)
 IF (APPLE)
	SET(GUI_EXECUTABLE MACOSX_BUNDLE)
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
	itkAddOn
	vtkRenderingAddOn
	ImageViewer)
ENDIF(APPLE)

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
INSTALL_TARGETS(/bin/${SISTOPER} nukak3d)