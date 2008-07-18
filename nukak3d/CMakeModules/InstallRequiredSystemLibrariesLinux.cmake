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
