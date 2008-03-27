/** 
 * @file nkNukak3D.cpp
 * @brief Ventana principal de Nukak3D.
 * @details Implementa la interfaz grafica 
 * @author Alexander Pinzon Fernandez, Byron Perez
 * @version 0.2
 * @date 27/12/2007 03:37 p.m.
*/
#include "nkNukak3D.h"

#include "icon_menu_mas.xpm"
#include <wx/imaglist.h>
#include "vtkLookupTableManager.h"

wxSize nkTool::LARGO_X_ANCHO_ICONO = wxSize(16,16);
int nkMenuTool::ANCHO_TOOLBAR = 200;
int nkToolBar::ALINEACION = 4;

nkNukak3D::nkNukak3D(wxWindow* parent, int id,
		const wxString& title,
        const wxPoint& pos,
        const wxSize& size,
		long style):
		wxFrame(parent, id, title, pos, size, style)
{
	#ifdef __WXMAC__
		wxApp::s_macAboutMenuItemId = nkNukak3D::ID_ACERCA_DE_MAC;
	#endif

	prv_auiManager.SetManagedWindow(this);
	prv_auiManager.SetFlags(prv_auiManager.GetFlags()|wxAUI_MGR_ALLOW_ACTIVE_PANE|wxAUI_MGR_HINT_FADE
		|wxAUI_MGR_TRANSPARENT_HINT);
	
	//SetIcon(wxICON(mondrian));
	#ifdef __WXMSW__
		SetIcon(wxIcon("NUKAK3D")); //! Window icon
	#endif


	#if wxUSE_STATUSBAR
		CreateStatusBar(2);
		SetStatusText(_T("Nukak 3D!"));
	#endif // wxUSE_STATUSBAR
	
	/////////////////////////
	// Barra de menu -> superior
	wxMenuBar * mi_wxMBMenu = new wxMenuBar();
	SetMenuBar(mi_wxMBMenu);
	
	// Menu archivo -> superior
	wxMenu * mi_wxMenuArchivo = new wxMenu(_T(""), wxMENU_TEAROFF);
	mi_wxMenuArchivo->Append(nkNukak3D::ID_SALIR, _T("S&alir\tAlt-X"), _T("Salir del programa"));
	mi_wxMBMenu->Append(mi_wxMenuArchivo, _T("&Archivo"));

	// Menu ayuda superior
	wxMenu * mi_wxMenuAyuda = new wxMenu;  
	mi_wxMenuAyuda->Append(nkNukak3D::ID_ACERCA_DE, _T("&Acerca De...\tCtrl-A"), _T("Mostrar informacion de los autores."));
	mi_wxMBMenu->Append(mi_wxMenuAyuda, _T("Ayuda"));

	//////////////////////////////////
	// Barra Menu "herramientas"  lateral
	mi_nkHerramientas = new nkToolBar(this, nkNukak3D::ID_ARBOL, wxDefaultPosition, wxDefaultSize);

	// Menu grupo "herramientas" -> Abir volumen de imagenes -> lateral
	nkMenuTool * mi_nkMenuVolume = mi_nkHerramientas->insertarMenu(-1, "Abrir Volumen de Imagenes");
	mi_nkMenuVolume->insertarTool(new nkTool(nkNukak3D::ID_ABRIR_ARCHIVO,wxT("Volumen"), wxT(wxNullBitmap), wxT("Abrir un volumen de  imagenes")));
	mi_nkMenuVolume->insertarTool(new nkTool(nkNukak3D::ID_ABRIR_ARCHIVO_DICOM,wxT("Imagenes Dicom"), wxNullBitmap, wxT("Abrir un volumen de imagenes Dicom")));

	// Menu grupo "herramientas" -> Abir objeto 3D -> lateral
	nkMenuTool * mi_nkMenuObj3D = mi_nkHerramientas->insertarMenu(-1, "Abrir Malla poligonal 3D");
	mi_nkMenuObj3D->insertarTool(new nkTool(nkNukak3D::ID_ABRIR_ARCHIVO_MALLA3D,wxT("Malla Poligonal 3D"), wxNullBitmap, wxT("Abrir una malla poligonal 3D de un archivo vtk")));

	// Menu grupo "herramientas" -> Guardar -> lateral
	nkMenuTool * mi_nkMenuGuardar = mi_nkHerramientas->insertarMenu(-1, "Guardar");
	mi_nkMenuGuardar->insertarTool(new nkTool(nkNukak3D::ID_GUARDARVOL,wxT("Guardar Volumen de Imagenes"), wxNullBitmap, wxT("Guardar un volumen de imagenes")));
	mi_nkMenuGuardar->insertarTool(new nkTool(nkNukak3D::ID_GUARDARMALLA3D,wxT("Guardar Malla Poligonal 3D"), wxNullBitmap, wxT("Guardar una malla poligonal 3D")));

	// Menu grupo "herramientas" -> Captura de pantalla -> lateral
	nkMenuTool * mi_nkMenuCaptura = mi_nkHerramientas->insertarMenu(-1, "Captura de pantalla");
	mi_nkMenuCaptura->insertarTool(new nkTool(nkNukak3D::ID_SNAPSHOT3D,wxT("Guardar vista 3D"), wxNullBitmap, wxT("Guardar captura de pantalla de la vista 3D")));
	mi_nkMenuCaptura->insertarTool(new nkTool(nkNukak3D::ID_SNAPSHOTAXIAL,wxT("Guardar vista axial"), wxNullBitmap, wxT("Guardar captura de pantalla de la vista axial")));
	mi_nkMenuCaptura->insertarTool(new nkTool(nkNukak3D::ID_SNAPSHOTSAGITAL,wxT("Guardar vista sagital"), wxNullBitmap, wxT("Guardar captura de pantalla de la vista sagital")));
	mi_nkMenuCaptura->insertarTool(new nkTool(nkNukak3D::ID_SNAPSHOTCORONAL,wxT("Guardar vista coronal"), wxNullBitmap, wxT("Guardar captura de pantalla de la vista coronal")));

	// Menu grupo "herramientas" -> Informacion -> lateral
	nkMenuTool * mi_nkMenuInfo = mi_nkHerramientas->insertarMenu(-1, "Informacion");
	mi_nkMenuInfo->insertarTool(new nkTool(nkNukak3D::ID_PARIMAGE,wxT("Volumen 3D"), wxNullBitmap, wxT("Informacion del volumen 3D")));
	mi_nkMenuInfo->insertarTool(new nkTool(nkNukak3D::ID_PARPOLYGON,wxT("Malla poligonal"), wxNullBitmap, wxT("Informacion de la malla poligonal")));
	mi_nkMenuInfo->insertarTool(new nkTool(nkNukak3D::ID_PARVIDEO,wxT("Tarjeta de video"), wxNullBitmap, wxT("Informacion de la tarjeta de video en el archivo abierto")));
	//////////////////////////////////

	// Barra Menu "configuracion" lateral 
	mi_nkImageViewer = new nkToolBar(this, nkNukak3D::ID_ARBOL, wxDefaultPosition, wxDefaultSize);

	// Menu grupo "configuracion" -> paleta de colores -> lateral
	nkMenuTool * mi_nkMenuImageViewer = mi_nkImageViewer->insertarMenu(-1, wxT("Paleta de Colores"));
	mi_nkMenuImageViewer->insertarTool(
		new nkTool(nkNukak3D::ID_REINICIAR_PALETA,
			wxT("Reiniciar paleta de Colores"), 
			wxNullBitmap, 
			wxT("Carga los valores predeterminados del ancho y alto de la ventana para la paleta de colores.")));
	std::vector<std::string> lutNames = vtkLookupTableManager::GetAvailableLookupTables();
	for( unsigned int i=0; i<lutNames.size(); i++){
		mi_nkMenuImageViewer->insertarTool(
			new nkTool(nkNukak3D::ID_ULTIMO + i,
			wxT(lutNames[i].c_str()), 
			wxT(wxNullBitmap), 
			wxT("Cambiar la forma de visualizacion de los niveles de la imagen")));
	}
	
	// Menu grupo "configuracion" -> Modo de procesamiento 3D -> lateral
	nkMenuTool * mi_nkMenuVol3DMode = mi_nkImageViewer->insertarMenu(-1, wxT("Visualizacion de volumenes 3D"));
	
	mi_nkMenuVol3DMode->insertarTool(
		new nkTool(nkNukak3D::ID_VOLVIEWER_RENDERING_ESCALAR,
		wxT("Escalar: Planos ortogonales"), 
			wxNullBitmap, 
			wxT("Visualizacion de 3 Planos ortogonales sobre el volumen.")));
	
	mi_nkMenuVol3DMode->insertarTool(
		new nkTool(nkNukak3D::ID_VOLVIEWER_RENDERING_MRC,
			wxT("3D: Trazado de Rayos"), 
			wxNullBitmap, 
			wxT("Visualizacion 3D con un algoritmo de RayCasting.")));
	
	mi_nkMenuVol3DMode->insertarTool(
		new nkTool(nkNukak3D::ID_VOLVIEWER_RENDERING_TEXTURA,
			wxT("3D: Textura"), 
			wxNullBitmap, 
			wxT("Visualizacion 3D normal.")));

	// Menu grupo "configuracion" -> isosuperficies -> lateral
	nkMenuTool * mi_nkMenuIsoSuperficie = mi_nkImageViewer->insertarMenu(-1, "IsoSuperficie");
	mi_nkMenuIsoSuperficie->insertarTool(new nkTool(nkNukak3D::ID_MARCHING_CUBES,wxT("3D: Marching Cubes"), wxNullBitmap, wxT("Generar una IsoSuperficie por medio del algoritmo MarchingCubes")));


	/////////////////////////////////////////////////

	//Menu grupo "configuracion" -> filtros y segmentacion -> lateral
	nkMenuTool * mi_nkMenuVolUtilidades = mi_nkImageViewer->insertarMenu(-1, wxT("Filtros y segmentacion"));
	
	mi_nkMenuVolUtilidades->insertarTool(
		new nkTool(nkNukak3D::ID_LSLEVELSETSCOMPLETO,
			wxT("Segmentacion con Levelsets"), 
			wxNullBitmap, 
			wxT("Segmentacion con Levelsets usando como semilla la posicion actual del cursor")));
	
	mi_nkMenuVolUtilidades->insertarTool(
		new nkTool(nkNukak3D::ID_AREA,
			wxT("Area de la imagen axial"), 
			wxNullBitmap, 
			wxT("Area mayor que cero en la imagen axial actual en mm2")));	

	mi_nkMenuVolUtilidades->insertarTool(
		new nkTool(nkNukak3D::ID_FILVOLGAUSSIAN,
			wxT("Filtro gaussiano"), 
			wxNullBitmap, 
			wxT("Filtro gaussiano del volumen")));	

	mi_nkMenuVolUtilidades->insertarTool(
		new nkTool(nkNukak3D::ID_FILVOLMEDIAN,
			wxT("Filtro mediana"), 
			wxNullBitmap, 
			wxT("Filtro mediana del volumen")));	

	/////////////////////////////////////////////////

	//Menu grupo "configuracion" -> Operaciones sobre mallas 3D -> lateral
	nkMenuTool * mi_nkMenuFiltrosPoly = mi_nkImageViewer->insertarMenu(-1, wxT("Operaciones sobre mallas 3D"));
	
	mi_nkMenuFiltrosPoly->insertarTool(
		new nkTool(nkNukak3D::ID_FILPOLYTRIANGLE,
			wxT("Triangulacion"), 
			wxNullBitmap, 
			wxT("Triangulacion de la malla 3D")));
	
	mi_nkMenuFiltrosPoly->insertarTool(
		new nkTool(nkNukak3D::ID_FILPOLYDECIMATE,
			wxT("Decimado"), 
			wxNullBitmap, 
			wxT("Decimado (simplificacion) de la malla")));	

	mi_nkMenuFiltrosPoly->insertarTool(
		new nkTool(nkNukak3D::ID_FILPOLYSMOOTH,
			wxT("Suavizado"), 
			wxNullBitmap, 
			wxT("Suavizado de la malla")));	

	mi_nkMenuFiltrosPoly->insertarTool(
		new nkTool(nkNukak3D::ID_FILPOLYNORMALS,
			wxT("Normales"), 
			wxNullBitmap, 
			wxT("Angulos de las normales")));	

	mi_nkMenuFiltrosPoly->insertarTool(
		new nkTool(nkNukak3D::ID_FILPOLYDEFORM,
			wxT("Deformar"), 
			wxNullBitmap, 
			wxT("Deformar malla")));	


	//Menu grupo "configuracion" -> Navegacion -> lateral
	nkMenuTool * mi_nkMenuNavegacion = mi_nkImageViewer->insertarMenu(-1, wxT("Navegacion"));
	
	mi_nkMenuNavegacion->insertarTool(
		new nkTool(nkNukak3D::ID_NAVRESET,
			wxT("Reinicializar camara"), 
			wxNullBitmap, 
			wxT("Reinicializa la vista de la camara ajustada a todo el volumen")));

	mi_nkMenuNavegacion->insertarTool(
		new nkTool(nkNukak3D::ID_NAVTRACKBALL,
			wxT("Trackball"), 
			wxNullBitmap, 
			wxT("Modo de navegacion trackball")));

	mi_nkMenuNavegacion->insertarTool(
		new nkTool(nkNukak3D::ID_NAVJOYSTICK,
			wxT("Joystick"), 
			wxNullBitmap, 
			wxT("Modo de navegacion trackball")));

	mi_nkMenuNavegacion->insertarTool(
		new nkTool(nkNukak3D::ID_NAVFLIGHT,
			wxT("Flight"), 
			wxNullBitmap, 
			wxT("Modo de navegacion flight")));

	mi_nkMenuNavegacion->insertarTool(
		new nkTool(nkNukak3D::ID_NAVUNICAM,
			wxT("Unicam"), 
			wxNullBitmap, 
			wxT("Modo de navegacion unicam")));

	
	//Menu grupo "configuracion" -> Vision estereo -> lateral
	nkMenuTool * mi_nkMenuEstereo = mi_nkImageViewer->insertarMenu(-1, wxT("Estereo"));
	
	mi_nkMenuEstereo->insertarTool(
		new nkTool(nkNukak3D::ID_STACTIVO,
			wxT("Estereo activo"), 
			wxNullBitmap, 
			wxT("Inicializa/desactiva el modo de vision estereo activa con gafas de obturacion")));

	mi_nkMenuEstereo->insertarTool(
		new nkTool(nkNukak3D::ID_STPASIVO,
			wxT("Estereo pasivo"), 
			wxNullBitmap, 
			wxT("Inicializa/desactiva el modo de vision estereo pasiva con gafas rojo/cyan")));

	mi_nkMenuEstereo->insertarTool(
		new nkTool(nkNukak3D::ID_STAUMENTAR,
			wxT("Aumentar separacion +"), 
			wxNullBitmap, 
			wxT("Aumenta la separacion entre ojos")));

	mi_nkMenuEstereo->insertarTool(
		new nkTool(nkNukak3D::ID_STDISMINUIR,
			wxT("Disminuir separacion -"), 
			wxNullBitmap, 
			wxT("Disminuye la separacion entre ojos")));
	
	// Insertar Menu grupo "herramientas" a la ventana
	this->insertarToolBar(mi_nkHerramientas, wxT("Herramientas"), wxT("Herramientas"));
	// Insertar Menu grupo "configuracion" a la ventana
	this->insertarToolBar(mi_nkImageViewer, wxT("Configuracion"), wxT("Configuracion"));


	prv_libro = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
		wxAUI_NB_DEFAULT_STYLE | 
		wxAUI_NB_TAB_EXTERNAL_MOVE | 
		wxAUI_NB_WINDOWLIST_BUTTON |
		wxAUI_NB_CLOSE_ON_ALL_TABS |
		wxNO_BORDER);

	prv_auiManager.AddPane(prv_libro, wxAuiPaneInfo().
                  Name(wxT("Nukak 3d Espacio de Trabajo")).Caption(wxT("Nukak 3D Archivos")).
				  CenterPane().
				  PaneBorder(false));

	prv_auiManager.Update();

}
//*****************************************************************************************
//		DESTRUCTOR
//*****************************************************************************************
nkNukak3D::~nkNukak3D()
{
	prv_auiManager.UnInit();
	if(m_stick)
	{
		m_stick->ReleaseCapture();
		delete m_stick;
	}
}

//*****************************************************************************************
//		TABLA DE EVENTOS
//*****************************************************************************************
BEGIN_EVENT_TABLE(nkNukak3D, wxFrame)
	EVT_TREE_ITEM_ACTIVATED(nkNukak3D::ID_ARBOL, nkNukak3D::eventoArbol)
	EVT_MENU(nkNukak3D::ID_SALIR, nkNukak3D::eventoSalir)
	EVT_MENU(nkNukak3D::ID_ACERCA_DE, nkNukak3D::eventoAcercaDe)
	EVT_MENU(nkNukak3D::ID_ACERCA_DE_MAC, nkNukak3D::eventoAcercaDeMAC)
	EVT_MENU(nkNukak3D::ID_ABRIR_ARCHIVO, nkNukak3D::eventoAbrirVolumen)
	EVT_MENU(nkNukak3D::ID_ABRIR_ARCHIVO_DICOM, nkNukak3D::eventoAbrirVolumenDicom)
	EVT_MENU(nkNukak3D::ID_ABRIR_ARCHIVO_MALLA3D, nkNukak3D::eventoAbrirMalla3D)
	EVT_MENU(nkNukak3D::ID_GUARDARVOL, nkNukak3D::eventoGuardarVol)
	EVT_MENU(nkNukak3D::ID_GUARDARMALLA3D, nkNukak3D::eventoGuardarMalla3D)
	EVT_MENU(nkNukak3D::ID_MARCHING_CUBES, nkNukak3D::eventoMarchingCubes)
	EVT_MENU(nkNukak3D::ID_REINICIAR_PALETA, nkNukak3D::eventoReiniciarPaleta)
	EVT_MENU(nkNukak3D::ID_VOLVIEWER_RENDERING_ESCALAR, nkNukak3D::eventoVolViewerRenderingEscalar)
	EVT_MENU(nkNukak3D::ID_VOLVIEWER_RENDERING_MRC, nkNukak3D::eventoVolViewerRenderingMRC)
	EVT_MENU(nkNukak3D::ID_VOLVIEWER_RENDERING_TEXTURA, nkNukak3D::eventoVolViewerRenderingTextura)
	EVT_MENU(nkNukak3D::ID_LSLEVELSETSCOMPLETO, nkNukak3D::eventolsLevelsetsCompleto)
	EVT_MENU(nkNukak3D::ID_AREA, nkNukak3D::eventoArea)
	EVT_MENU(nkNukak3D::ID_NAVRESET, nkNukak3D::eventoNavResetCamara)
	EVT_MENU(nkNukak3D::ID_NAVTRACKBALL,nkNukak3D::eventoNavTrackball)
	EVT_MENU(nkNukak3D::ID_NAVJOYSTICK,nkNukak3D::eventoNavJoystick)
	EVT_MENU(nkNukak3D::ID_NAVFLIGHT,nkNukak3D::eventoNavFlight)
	EVT_MENU(nkNukak3D::ID_NAVUNICAM,nkNukak3D::eventoNavUnicam)
	EVT_MENU(nkNukak3D::ID_STACTIVO, nkNukak3D::eventoStActivo)
	EVT_MENU(nkNukak3D::ID_STPASIVO, nkNukak3D::eventoStPasivo)
	EVT_MENU(nkNukak3D::ID_STAUMENTAR, nkNukak3D::eventoStAumentar)
	EVT_MENU(nkNukak3D::ID_STDISMINUIR, nkNukak3D::eventoStDisminuir)
	EVT_MENU(nkNukak3D::ID_FILVOLGAUSSIAN, nkNukak3D::eventoFilVolGaussian)
	EVT_MENU(nkNukak3D::ID_FILVOLMEDIAN, nkNukak3D::eventoFilVolMedian)
	EVT_MENU(nkNukak3D::ID_FILVOLGRADIENT, nkNukak3D::eventoFilVolGradient)
	EVT_MENU(nkNukak3D::ID_FILVOLTHRESHOLD, nkNukak3D::eventoFilVolThreshold)
	EVT_MENU(nkNukak3D::ID_FILPOLYTRIANGLE, nkNukak3D::eventoFilPolyTriangle)
	EVT_MENU(nkNukak3D::ID_FILPOLYDECIMATE, nkNukak3D::eventoFilPolyDecimate)
	EVT_MENU(nkNukak3D::ID_FILPOLYSMOOTH, nkNukak3D::eventoFilPolySmooth)
	EVT_MENU(nkNukak3D::ID_FILPOLYNORMALS, nkNukak3D::eventoFilPolyNormals)
	EVT_MENU(nkNukak3D::ID_FILPOLYDEFORM, nkNukak3D::eventoFilPolyDeform)
	EVT_MENU(nkNukak3D::ID_SNAPSHOT3D, nkNukak3D::eventoSnapshot3D)
	EVT_MENU(nkNukak3D::ID_SNAPSHOTAXIAL, nkNukak3D::eventoSnapshotAxial)
	EVT_MENU(nkNukak3D::ID_SNAPSHOTSAGITAL, nkNukak3D::eventoSnapshotSagital)
	EVT_MENU(nkNukak3D::ID_SNAPSHOTCORONAL, nkNukak3D::eventoSnapshotCoronal)
	EVT_MENU(nkNukak3D::ID_PARIMAGE, nkNukak3D::eventoParImage)
	EVT_MENU(nkNukak3D::ID_PARPOLYGON, nkNukak3D::eventoParPolygon)
	EVT_MENU(nkNukak3D::ID_PARVIDEO,  nkNukak3D::eventoParVideo)
	EVT_MENU(wxID_ANY, nkNukak3D::eventoPaletaColor)
END_EVENT_TABLE()

//*****************************************************************************************
//		INSERTAR TOOLBAR
//*****************************************************************************************
void nkNukak3D::insertarToolBar(wxWindow* window, wxString un_nombre, wxString una_etiqueta){
	int separacion = 15;
#ifdef __WIN32__
	separacion = 15;
#else //mac o unix
	separacion = 50;
#endif //__WIN32__
	prv_auiManager.AddPane(window, wxAuiPaneInfo().
              Name(un_nombre).
			  Left().
			  Caption(una_etiqueta).
			  TopDockable(false).
			  BottomDockable(false).
			  CloseButton(false).
			  PaneBorder(true).
			  PinButton(true).
			  CaptionVisible(true).
			  MinSize(nkMenuTool::ANCHO_TOOLBAR + nkToolBar::ALINEACION+separacion, 300).
			  FloatingSize(nkMenuTool::ANCHO_TOOLBAR + nkToolBar::ALINEACION+separacion, 300).
			  MaxSize(nkMenuTool::ANCHO_TOOLBAR + nkToolBar::ALINEACION+separacion+1, 1000));

}

//*****************************************************************************************
//		MENU -> ARCHIVO -> SALIR
//*****************************************************************************************
void nkNukak3D::eventoSalir(wxCommandEvent& WXUNUSED(event)){
	Close(TRUE);
}
//*****************************************************************************************
//		MENU -> AYUDA -> ACERCA DE
//*****************************************************************************************
void nkNukak3D::eventoAcercaDe(wxCommandEvent& WXUNUSED(event)){
	nkAcercaDe * miAcercaDe = new nkAcercaDe(this);
	miAcercaDe->ShowModal();
	delete miAcercaDe;
}

void nkNukak3D::eventoAcercaDeMAC(wxCommandEvent& WXUNUSED(event)){
	nkAcercaDe * miAcercaDe = new nkAcercaDe(this);
	miAcercaDe->ShowModal();
	delete miAcercaDe;
}
//*****************************************************************************************
//		MENU -> ABRIR -> VOLUMEN
//*****************************************************************************************
void nkNukak3D::eventoAbrirVolumen(wxCommandEvent& WXUNUSED(event)){

	const wxString miWxFilter =
	wxT ("Image File (*.hdr;*.inr;*.inr.gz;*.gipl;*.mha;*.mhd;*.vtk;*.nrrd;*.nhdr;*.nii;*.nii.gz;*.jpg;*.tif)|*.hdr;*.inr;*.inr.gz;*.gipl;*.mha;*.mhd;*.vtk;*.vol;*.nrrd;*.nhdr;*.nii;*.nii.gz;*.jpg;*.tif|")
	wxT ("Analyze (*.hdr)|*.hdr|")
	wxT ("Inrimage (*.inr;*.inr.gz)|*.inr;*.inr.gz|")
	wxT ("Gipl (*.gipl)|*.gipl|")
	wxT ("Metafile (*.mha;*.mhd)|*.mha;*.mhd|")
	wxT ("VTK (*.vtk)|*.vtk|")
	wxT ("Volume (*.vol)|*.vol|")
	wxT ("Nrrd (*.nrrd;*.nhdr)|*.nrrd;*.nhdr|")
	wxT ("NIfTI (*.nii;*.nii.gz)|*.nii;*.nii.gz|")
	wxT ("Jpeg (*.jpg;*.jpeg)|*.jpg;*.jpeg|")
	wxT ("Tiff (*.tif;*.tiff)|*.tif;*.tiff|")
	wxT ("All (*)|*");

	wxFileDialog * miFileDialog = new wxFileDialog(this, 
        										  wxT("Escoja un archivo..."),
                                                  wxT(""), 
												  wxT(""), 
                                                  miWxFilter,
                                                  wxFD_OPEN|wxFD_CHANGE_DIR|wxFD_MULTIPLE, wxDefaultPosition);
    wxArrayString nombresArchivos;
    nombresArchivos.Empty();
    int OK = miFileDialog->ShowModal();
    
    if( OK==wxID_OK )
      miFileDialog->GetPaths(nombresArchivos);
    
	miFileDialog->Destroy();

	wxBeginBusyCursor();
    
	unsigned int Nimages = nombresArchivos.Count();
	wxString extension, nombreArchivo;

	for( unsigned int i=0; i<Nimages; i++ )	{
		nombreArchivo = nombresArchivos[i];
		extension = nombreArchivo.Mid(nombreArchivo.Length()-3,3);
		if( extension == "vol" ){
			nkVolViewer * mivol = new nkVolViewer(prv_libro);
			mivol->Configurar();
			mivol->abrirArchivo_vol(nombreArchivo);
			prv_libro->AddPage(mivol, nombreArchivo,true );
		}else{
			nkVolViewer * mivol = new nkVolViewer(prv_libro);
			mivol->Configurar();
			mivol->abrirArchivo(nombreArchivo);
			prv_libro->AddPage(mivol, nombreArchivo,true );
		}
	}

	wxEndBusyCursor();
}
//*****************************************************************************************
//		MENU -> ABRIR -> VOLUMEN DICOM
//*****************************************************************************************
void nkNukak3D::eventoAbrirVolumenDicom(wxCommandEvent& WXUNUSED(event)){
	wxVtkDICOMImporter* miImporter = new wxVtkDICOMImporter (this);

	if( miImporter->GetReturnCode()!=wxID_OK ){
		miImporter->Destroy();
		return;
	}
	
	wxBeginBusyCursor();

	unsigned int Nimages = miImporter->GetOutputs().size();
	double step = 100.0/(double)(Nimages);

	for (unsigned int i=0; i<Nimages ; i++){
		if (!miImporter->GetOutput(i)){
		continue;
		}
		nkVolViewer * mi_Vol2 = new nkVolViewer(prv_libro, wxID_ANY);
		mi_Vol2->Configurar();
		wxString basename = wxT (miImporter->GetDescription (i).c_str());
		prv_libro->AddPage(mi_Vol2, basename,true );
		mi_Vol2->abrirArchivo_dicom(basename, miImporter, i);
	}
	miImporter->Destroy();

	wxEndBusyCursor();
}
//*****************************************************************************************
//		MENU -> ABRIR -> MALLA 3D
//*****************************************************************************************
void nkNukak3D::eventoAbrirMalla3D(wxCommandEvent& WXUNUSED(event)){
	const wxString miWxFilter =
	wxT ("VTK (*.vtk)|*.vtk|")
	wxT ("All (*)|*");

	wxFileDialog * miFileDialog = new wxFileDialog(this, 
        										  wxT("Escoja un archivo..."),
                                                  wxT(""), 
												  wxT(""), 
                                                  miWxFilter,
                                                  wxFD_OPEN|wxFD_CHANGE_DIR|wxFD_MULTIPLE, wxDefaultPosition);
    wxArrayString nombresArchivos;
	wxString nombreArchivo;
    nombresArchivos.Empty();
    int OK = miFileDialog->ShowModal();
    
    if( OK==wxID_OK )
      miFileDialog->GetPaths(nombresArchivos);
    
	miFileDialog->Destroy();
    
	wxBeginBusyCursor();

	unsigned int Nimages = nombresArchivos.Count();

	for( unsigned int i=0; i<Nimages; i++ )	{
		nombreArchivo = nombresArchivos[i];
		nkObj3DViewer * miobj = new nkObj3DViewer(prv_libro);
		miobj->Configurar();
		miobj->abrirArchivo(nombreArchivo);
		prv_libro->AddPage(miobj, nombreArchivo,true );
	}
	wxEndBusyCursor();
}
//*****************************************************************************************
//		MENU -> GUARDAR -> VOLUMEN
//*****************************************************************************************
void nkNukak3D::eventoGuardarVol(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			const wxString miWxFilter =
				wxT ("Image File (*.hdr;*.inr;*.inr.gz;*.gipl;*.mha;*.mhd;*.vtk;*.nrrd;*.nhdr;*.nii;*.nii.gz;*.jpg;*.tif)|*.hdr;*.inr;*.inr.gz;*.gipl;*.mha;*.mhd;*.vtk;*.nrrd;*.nhdr;*.nii;*.nii.gz;*.jpg;*.tif|")
				wxT ("Analyze (*.hdr)|*.hdr|")
				wxT ("Inrimage (*.inr;*.inr.gz)|*.inr;*.inr.gz|")
				wxT ("Gipl (*.gipl)|*.gipl|")
				wxT ("Metafile (*.mha;*.mhd)|*.mha;*.mhd|")
				wxT ("VTK (*.vtk)|*.vtk|")
				wxT ("Nrrd (*.nrrd;*.nhdr)|*.nrrd;*.nhdr|")
				wxT ("NIfTI (*.nii;*.nii.gz)|*.nii;*.nii.gz|")
				wxT ("Jpeg (*.jpg;*.jpeg)|*.jpg;*.jpeg|")
				wxT ("Tiff (*.tif;*.tiff)|*.tif;*.tiff|")
				wxT ("All (*)|*");
			wxFileDialog * myFileDialog = new wxFileDialog(this, 
        										  wxT("Guardar Volumen Como"),
                                                  wxT(""), wxT(""),
                                                  miWxFilter,
                                                  wxFD_SAVE|wxFD_CHANGE_DIR|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
			wxString nombreArchivo;
			nombreArchivo.Empty();
			int OK = myFileDialog->ShowModal();
		    
			if( OK==wxID_OK )
			nombreArchivo = myFileDialog->GetPath();
		    
			myFileDialog->Destroy();
			if( nombreArchivo.IsEmpty() )
			{
				return;
			}
			wxBeginBusyCursor();
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->guardarArchivo(nombreArchivo);
			wxEndBusyCursor();
		}
	}
}
//*****************************************************************************************
//		MENU -> GUARDAR -> MALLA 3D
//*****************************************************************************************
void nkNukak3D::eventoGuardarMalla3D(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkObj3DViewer")){
			const wxString miWxFilter =
				wxT ("Poligon mesh File (*.vtk)|")				
				wxT ("VTK (*.vtk)|*.vtk|")
				wxT ("All (*)|*");
			wxFileDialog * myFileDialog = new wxFileDialog(this, 
        										  wxT("Guardar Malla Poligonal Como"),
                                                  wxT(""), wxT(""),
                                                  miWxFilter,
                                                  wxFD_SAVE|wxFD_CHANGE_DIR|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
			wxString nombreArchivo;
			nombreArchivo.Empty();
			int OK = myFileDialog->ShowModal();
		    
			if( OK==wxID_OK )
			nombreArchivo = myFileDialog->GetPath();
		    
			myFileDialog->Destroy();
			if( nombreArchivo.IsEmpty() )
			{
				return;
			}
			wxBeginBusyCursor();
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->guardarArchivo(nombreArchivo);
			wxEndBusyCursor();
		}
	}
}
//*****************************************************************************************
//		EVENTO ARBOL
//*****************************************************************************************
void nkNukak3D::eventoArbol(wxTreeEvent& event){
	wxCommandEvent mievento;
#ifdef __WIN32__
	wxString mi_item = wxT("NULL");
#else //MAC O UNIX
	wxString mi_item = mi_nkHerramientas->GetItemText(event.GetItem());
#endif //__WIN32__
	if (mi_item == wxT("Volumen")) eventoAbrirVolumen(mievento);
	else if (mi_item == wxT("Imagenes Dicom")) eventoAbrirVolumenDicom(mievento);
	else if (mi_item == wxT("Malla Poligonal 3D")) eventoAbrirMalla3D(mievento);
	else if (mi_item == wxT("3D: Marching Cubes")) eventoMarchingCubes(mievento);
	else if (mi_item == wxT("Escalar: Planos ortogonales")) eventoVolViewerRenderingEscalar(mievento);
	else if (mi_item == wxT("3D: Trazado de Rayos")) eventoVolViewerRenderingMRC(mievento);
	else if (mi_item == wxT("3D: Textura")) eventoVolViewerRenderingTextura(mievento);
	else if (mi_item == wxT("Reiniciar paleta de Colores")) eventoReiniciarPaleta(mievento);
	else if (mi_item == wxT("Escalar: Planos ortogonales")) eventoVolViewerRenderingEscalar(mievento);
	else if (mi_item == wxT("Segmentacion con Levelsets")) eventolsLevelsetsCompleto(mievento);
	else if (mi_item == wxT("Area de la imagen axial")) eventoArea(mievento);
	else if (mi_item == wxT("Filtro gaussiano")) eventoFilVolGaussian(mievento);
	else if (mi_item == wxT("Filtro mediana")) eventoFilVolMedian(mievento);
	else if (mi_item == wxT("Triangulacion")) eventoFilPolyTriangle(mievento);
	else if (mi_item == wxT("Decimado")) eventoFilPolyDecimate(mievento);
	else if (mi_item == wxT("Suavizado")) eventoFilPolySmooth(mievento);
	else if (mi_item == wxT("Normales")) eventoFilPolyNormals(mievento);
	else if (mi_item == wxT("Deformar")) eventoFilPolyDeform(mievento);
	else if (mi_item == wxT("Reinicializar camara")) eventoNavResetCamara(mievento);
	else if (mi_item == wxT("Trackball")) eventoNavTrackball(mievento);
	else if (mi_item == wxT("Joystick")) eventoNavJoystick(mievento);
	else if (mi_item == wxT("Flight")) eventoNavFlight(mievento);
	else if (mi_item == wxT("Unicam")) eventoNavUnicam(mievento);
	else if (mi_item == wxT("Estereo activo")) eventoStActivo(mievento);
	else if (mi_item == wxT("Estereo pasivo")) eventoStPasivo(mievento);
	else if (mi_item == wxT("Aumentar separacion +")) eventoStAumentar(mievento);
	else if (mi_item == wxT("Disminuir separacion -")) eventoStDisminuir(mievento);

	std::vector<std::string> lutNames = vtkLookupTableManager::GetAvailableLookupTables();
	int val = -1;
	for( unsigned int i=0; i<lutNames.size(); i++){
		if (mi_item == wxT(lutNames[i].c_str())) val = i;
	}
	if (val>-1){
		wxCommandEvent event(0, val +nkNukak3D::ID_ULTIMO);
		eventoPaletaColor(event);
	}
}
//*****************************************************************************************
//		MENU -> PALETA DE COLORES -> REINICIAR PALETA
//*****************************************************************************************
void nkNukak3D::eventoReiniciarPaleta(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			wxBeginBusyCursor();
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->reiniciarNiveleseDePaleta();
			wxEndBusyCursor();
		}
	}
}
//*****************************************************************************************
//		MENU -> PALETA DE COLORES -> SELECCIoN DE PALETA
//*****************************************************************************************
void nkNukak3D::eventoPaletaColor(wxCommandEvent& event){
	if (event.GetId() >= nkNukak3D::ID_ULTIMO){
		if ((int)prv_libro->GetPageCount() > 0){
			int mi_pagina = prv_libro->GetSelection();
			wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
			if (pagina->GetName() == wxT("nkVolViewer")){
				nkVolViewer *page = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
				int val = event.GetId() - nkNukak3D::ID_ULTIMO;
				vtkLookupTable* lut = vtkLookupTableManager::GetLookupTable(val);
				if( lut && page ){
					wxBeginBusyCursor();
					page->cambiarPaletaColor(lut);
					lut->Delete();
					wxEndBusyCursor();
				}
			}
		}
	}
}
//*****************************************************************************************
//		MENU -> VISUALIZACION DE VOLUMENES 3D -> ESCALAR (PLANOS ORTOGONALES)
//*****************************************************************************************
void nkNukak3D::eventoVolViewerRenderingEscalar(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			wxBeginBusyCursor();
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->cambiarFormaDeProcesamiento(vtkViewImage3D::PLANAR_RENDERING);
			wxEndBusyCursor();
		}
	}
}
//*****************************************************************************************
//		MENU -> VISUALIZACION DE VOLUMENES 3D -> RAY CASTING
//*****************************************************************************************
void nkNukak3D::eventoVolViewerRenderingMRC(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			wxBeginBusyCursor();
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->cambiarFormaDeProcesamiento(vtkViewImage3D::VOLUME_RENDERING, 2);
			wxEndBusyCursor();
		}
	}
}
//*****************************************************************************************
//		MENU -> VISUALIZACION DE VOLUMENES 3D -> TEXTURE RENDERING
//*****************************************************************************************
void nkNukak3D::eventoVolViewerRenderingTextura(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			wxBeginBusyCursor();
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->cambiarFormaDeProcesamiento(vtkViewImage3D::VOLUME_RENDERING, 1);
			wxEndBusyCursor();
		}
	}
}
//*****************************************************************************************
//		MENU -> ISOSUPERFICIE -> MARCHING CUBES
//*****************************************************************************************
void nkNukak3D::eventoMarchingCubes(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));

			wxString etiquetas[100];
			const int num_datos=3;
			etiquetas[0] = wxT("Numero de contornos");
			etiquetas[1] = wxT("Valor de umbral Inferior");
			etiquetas[2] = wxT("Valor de umbral Superior");
			
			nkIODialog * miDlg = new nkIODialog(	this, 
															etiquetas,
															num_datos,
															-1,
															wxT("Nukak3D: Marching cubes"),
															wxDefaultPosition,
															wxSize(330,(num_datos+4)*20+40));



			wxString valor = wxString::Format("%f",current->obtenerValorActualDeContorno());
			miDlg->cambiarValor(valor,2);
			miDlg->ShowModal();
			long num_contornos, umbralInferior, umbralSuperior;
			if(miDlg->GetReturnCode() == wxID_OK){
				wxBeginBusyCursor();

				nkObj3DViewer * miobj = new nkObj3DViewer(prv_libro);

				(miDlg->obtenerValor(0)).ToLong(&num_contornos);
				(miDlg->obtenerValor(1)).ToLong(&umbralInferior);
				(miDlg->obtenerValor(2)).ToLong(&umbralSuperior);

				miobj->Configurar();
				miobj->imagenAIsoSurface(current->getImagen(),
					(int)num_contornos, 
					(int)umbralInferior,  
					(int)umbralSuperior,1.0,1.0,1.0,1.0);
				prv_libro->AddPage(miobj, current->getNombreArchivo(),true );
				delete miDlg;

				wxEndBusyCursor();
			}
		}
	}
}
//*****************************************************************************************
//		MENU -> SEGMENTACION -> LEVELSETS
//*****************************************************************************************
void nkNukak3D::eventolsLevelsetsCompleto(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->lsLevelsetsCompleto( nkNukak3D::getNkNotebook() );			
		}
	}
}

wxAuiNotebook * nkNukak3D::getNkNotebook(void){
	return prv_libro;
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> CALCULAR AREA AXIAL
//*****************************************************************************************
void nkNukak3D::eventoArea(wxCommandEvent& WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			wxBeginBusyCursor();
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->Area();	
			wxEndBusyCursor();
		}
	}	

}
//*****************************************************************************************
//		MENU -> NAVIGATION -> RESET CAMERA
//*****************************************************************************************
void nkNukak3D::eventoNavResetCamara(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->NavResetCamara();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer")) 
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->NavResetCamara();
		}
	}
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> TRACKBALL
//*****************************************************************************************
void nkNukak3D::eventoNavTrackball(wxCommandEvent& WXUNUSED(event) )
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->NavTrackball();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer")) 
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->NavTrackball();
		}
	}
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> JOYSTICK
//*****************************************************************************************
void nkNukak3D::eventoNavJoystick(wxCommandEvent& WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->NavJoystick();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer"))
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->NavJoystick();
		}
	}
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> FLIGHT
//*****************************************************************************************
void nkNukak3D::eventoNavFlight(wxCommandEvent& WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->NavFlight();
		}
		else 
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->NavFlight();
		}
	}
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> UNICAM
//*****************************************************************************************
void nkNukak3D::eventoNavUnicam(wxCommandEvent& WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->NavUnicam();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer"))
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->NavUnicam();
		}
	}
}
//*****************************************************************************************
//		ACTIVE STEREO
//*****************************************************************************************
void nkNukak3D::eventoStActivo(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->StActivo();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer"))
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->StActivo();
		}
	}
}
//*****************************************************************************************
//		PASIVE STEREO
//*****************************************************************************************
void nkNukak3D::eventoStPasivo(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->StPasivo();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer"))
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->StPasivo();
		}
	}
}
//*****************************************************************************************
//		STEREO INCREASE - Increase stereo separation
//*****************************************************************************************
void nkNukak3D::eventoStAumentar(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->StAumentar();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer"))
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->StAumentar();
		}
	}
}
//*****************************************************************************************
//		STEREO DECREASE - Decrease stereo separation
//*****************************************************************************************
void nkNukak3D::eventoStDisminuir(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->StDisminuir();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer"))
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->StDisminuir();
		}
	}
}
//*****************************************************************************************
//		FILTRADO DE VOLUMEN -> GAUSSIANO
//*****************************************************************************************
void nkNukak3D::eventoFilVolGaussian(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->FilVolGaussian( nkNukak3D::getNkNotebook() );
		}
	}
}
//*****************************************************************************************
//		FILTRADO DE VOLUMEN -> MEDIANA
//*****************************************************************************************
void nkNukak3D::eventoFilVolMedian(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->FilVolMedian( nkNukak3D::getNkNotebook() );
		}
	}
}
//*****************************************************************************************
//		FILTRADO DE VOLUMEN -> GRADIENTE
//*****************************************************************************************
void nkNukak3D::eventoFilVolGradient(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->FilVolGradient();
		}
	}
}
//*****************************************************************************************
//		FILTRADO DE VOLUMEN -> THRESHOLD
//*****************************************************************************************
void nkNukak3D::eventoFilVolThreshold(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->FilVolThreshold();
		}
	}
}

//*****************************************************************************************
//		FILTRADO DE POLIGONOS -> TRIANGULACION
//*****************************************************************************************
void nkNukak3D::eventoFilPolyTriangle(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->PolyTriangle();
		}
	}
}
//*****************************************************************************************
//		FILTRADO DE POLIGONOS -> DECIMATE
//*****************************************************************************************
void nkNukak3D::eventoFilPolyDecimate(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->PolyDecimate();
		}
	}
}
//*****************************************************************************************
//		FILTRADO DE POLIGONOS -> SMOOTH
//*****************************************************************************************
void nkNukak3D::eventoFilPolySmooth(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->PolySmooth();
		}
	}
}
//*****************************************************************************************
//		FILTRADO DE POLIGONOS -> NORMALS
//*****************************************************************************************
void nkNukak3D::eventoFilPolyNormals(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->PolyNormals();
		}
	}
}
//*****************************************************************************************
//		FILTRADO DE POLIGONOS -> DEFORMAR MALLA
//*****************************************************************************************
void nkNukak3D::eventoFilPolyDeform(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			current->PolyDeform();
		}
	}
}
//*****************************************************************************************
//		CAPTURA DE LA VISTA 3D
//*****************************************************************************************
void nkNukak3D::eventoSnapshot3D(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));

		// Dialogo para grabar
		const wxString miWxFilter =	
			wxT ("Jpeg (*.jpg)|*.jpg|")
			wxT ("Bmp (*.bmp)|*.bmp|")			
			wxT ("Tiff (*.tif)|*.tif|");
		wxFileDialog * myFileDialog = new wxFileDialog(this, 
    										  wxT("Guardar captura de pantalla como"),
                                              wxT(""), wxT(""),
                                              miWxFilter,
                                              wxFD_SAVE|wxFD_CHANGE_DIR|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
		wxString nombreArchivo;
		nombreArchivo.Empty();
		int OK = myFileDialog->ShowModal();
	    
		if( OK==wxID_OK )
		nombreArchivo = myFileDialog->GetPath();
	    
		myFileDialog->Destroy();
		if( nombreArchivo.IsEmpty() )
		{
			return;
		}

		vtkWindowToImageFilter* l_w2i = vtkWindowToImageFilter::New();

		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			l_w2i = current->Snapshot();
		}
		else if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			l_w2i = current->Snapshot(0);
		}

		if( nombreArchivo.Mid(nombreArchivo.Length()-3,3)== "jpg" ){
			vtkJPEGWriter*	l_snapshot = vtkJPEGWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(nombreArchivo);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( nombreArchivo.Mid(nombreArchivo.Length()-3,3)== "bmp" ){
			vtkBMPWriter*	l_snapshot = vtkBMPWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(nombreArchivo);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( nombreArchivo.Mid(nombreArchivo.Length()-3,3)== "tif" ){
			vtkTIFFWriter*	l_snapshot = vtkTIFFWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(nombreArchivo);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}			
	}
} 
//*****************************************************************************************
//		CAPTURA DE LA VISTA AXIAL
//*****************************************************************************************
void nkNukak3D::eventoSnapshotAxial(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));

		if (pagina->GetName() != wxT("nkVolViewer")){
			return;
		}

		// Dialogo para grabar
		const wxString miWxFilter =
			wxT ("Jpeg (*.jpg)|*.jpg|")
			wxT ("Bmp (*.bmp)|*.bmp|")
			wxT ("Tiff (*.tif)|*.tif|");
		wxFileDialog * myFileDialog = new wxFileDialog(this, 
    										  wxT("Guardar captura de pantalla como"),
                                              wxT(""), wxT(""),
                                              miWxFilter,
                                              wxFD_SAVE|wxFD_CHANGE_DIR|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
		wxString nombreArchivo;
		nombreArchivo.Empty();
		int OK = myFileDialog->ShowModal();
	    
		if( OK==wxID_OK )
		nombreArchivo = myFileDialog->GetPath();
	    
		myFileDialog->Destroy();
		if( nombreArchivo.IsEmpty() )
		{
			return;
		}

		vtkWindowToImageFilter* l_w2i = vtkWindowToImageFilter::New();					
		nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
		l_w2i = current->Snapshot(1);

		 if( nombreArchivo.Mid(nombreArchivo.Length()-3,3)== "jpg" ){
			vtkJPEGWriter*	l_snapshot = vtkJPEGWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(nombreArchivo);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( nombreArchivo.Mid(nombreArchivo.Length()-3,3)== "bmp" ){
			vtkBMPWriter*	l_snapshot = vtkBMPWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(nombreArchivo);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( nombreArchivo.Mid(nombreArchivo.Length()-3,3)== "tif" ){
			vtkTIFFWriter*	l_snapshot = vtkTIFFWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(nombreArchivo);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}			
	}
} 

//*****************************************************************************************
//		CAPTURA DE LA VISTA CORONAL
//*****************************************************************************************
void nkNukak3D::eventoSnapshotCoronal(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));

		if (pagina->GetName() != wxT("nkVolViewer")){
			return;
		}

		// Dialogo para grabar
		const wxString miWxFilter =
			wxT ("Jpeg (*.jpg)|*.jpg|")
			wxT ("Bmp (*.bmp)|*.bmp|")
			wxT ("Tiff (*.tif)|*.tif|");
		wxFileDialog * myFileDialog = new wxFileDialog(this, 
    										  wxT("Guardar captura de pantalla como"),
                                              wxT(""), wxT(""),
                                              miWxFilter,
                                              wxFD_SAVE|wxFD_CHANGE_DIR|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
		wxString nombreArchivo;
		nombreArchivo.Empty();
		int OK = myFileDialog->ShowModal();
	    
		if( OK==wxID_OK )
		nombreArchivo = myFileDialog->GetPath();
	    
		myFileDialog->Destroy();
		if( nombreArchivo.IsEmpty() )
		{
			return;
		}

		vtkWindowToImageFilter* l_w2i = vtkWindowToImageFilter::New();					
		nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
		l_w2i = current->Snapshot(2);

		 if( nombreArchivo.Mid(nombreArchivo.Length()-3,3)== "jpg" ){
			vtkJPEGWriter*	l_snapshot = vtkJPEGWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(nombreArchivo);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( nombreArchivo.Mid(nombreArchivo.Length()-3,3)== "bmp" ){
			vtkBMPWriter*	l_snapshot = vtkBMPWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(nombreArchivo);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( nombreArchivo.Mid(nombreArchivo.Length()-3,3)== "tif" ){
			vtkTIFFWriter*	l_snapshot = vtkTIFFWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(nombreArchivo);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}			
	}
}
//*****************************************************************************************
//		CAPTURA DE LA VISTA SAGITAL
//*****************************************************************************************
void nkNukak3D::eventoSnapshotSagital(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));

		if (pagina->GetName() != wxT("nkVolViewer")){
			return;
		}

		// Dialogo para grabar
		const wxString miWxFilter =
			wxT ("Jpeg (*.jpg)|*.jpg|")
			wxT ("Bmp (*.bmp)|*.bmp|")
			wxT ("Tiff (*.tif)|*.tif|");
		wxFileDialog * myFileDialog = new wxFileDialog(this, 
    										  wxT("Guardar captura de pantalla como"),
                                              wxT(""), wxT(""),
                                              miWxFilter,
                                              wxFD_SAVE|wxFD_CHANGE_DIR|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
		wxString nombreArchivo;
		nombreArchivo.Empty();
		int OK = myFileDialog->ShowModal();
	    
		if( OK==wxID_OK )
		nombreArchivo = myFileDialog->GetPath();
	    
		myFileDialog->Destroy();
		if( nombreArchivo.IsEmpty() )
		{
			return;
		}

		vtkWindowToImageFilter* l_w2i = vtkWindowToImageFilter::New();					
		nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
		l_w2i = current->Snapshot(3);

		 if( nombreArchivo.Mid(nombreArchivo.Length()-3,3)== "jpg" ){
			vtkJPEGWriter*	l_snapshot = vtkJPEGWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(nombreArchivo);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( nombreArchivo.Mid(nombreArchivo.Length()-3,3)== "bmp" ){
			vtkBMPWriter*	l_snapshot = vtkBMPWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(nombreArchivo);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( nombreArchivo.Mid(nombreArchivo.Length()-3,3)== "tif" ){
			vtkTIFFWriter*	l_snapshot = vtkTIFFWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(nombreArchivo);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}			
	}
}
//*****************************************************************************************
//		PARAMETROS DE LA IMAGEN
//*****************************************************************************************
void nkNukak3D::eventoParImage(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));

		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));

			int		*d;			//! Get image size
			double	*s;			//! Get image spacing

			d = current->getVtkImagen()->GetDimensions();	//! Get image dimensions
			s = current->getVtkImagen()->GetSpacing();		//! Get image spacing
			
			wxString l_text =
				wxT( wxString::Format("Size in x = \t%d\n", d[0]) )<<
				wxT( wxString::Format("Size in y = \t%d\n", d[1]) )<<
				wxT( wxString::Format("Size in z = \t%d\n", d[2]) )<<
				wxT( wxString::Format("Spacing in x = \t%1.5f\n", s[0]) )<<
				wxT( wxString::Format("Spacing in y = \t%1.5f\n", s[1]) )<<
				wxT( wxString::Format("Spacing in z = \t%1.5f\n", s[2]) );

			int l_type= current->getVtkImagen()->GetScalarType();

			if( l_type == 2  ) l_text<<wxT("Scalar type = CHAR");
			if( l_type == 3	 ) l_text<<wxT("Scalar type = UNSIGNED_CHAR");
			if( l_type == 4  ) l_text<<wxT("Scalar type = SHORT");
			if( l_type == 5	 ) l_text<<wxT("Scalar type = UNSIGNED_SHORT");
			if( l_type == 6  ) l_text<<wxT("Scalar type = INT");
			if( l_type == 7  ) l_text<<wxT("Scalar type = UNSIGNED_INT");
			if( l_type == 8  ) l_text<<wxT("Scalar type = LONG");
			if( l_type == 9  ) l_text<<wxT("Scalar type = UNSIGNED_LONG");
			if( l_type == 10 ) l_text<<wxT("Scalar type = FLOAT");
			if( l_type == 11 ) l_text<<wxT("Scalar type = DOUBLE");

			wxMessageDialog* l_dialog = new wxMessageDialog(this, l_text,
                                                    wxT ("Parametros del volumen 3D"),
                                                    wxOK|wxICON_INFORMATION );
			l_dialog->ShowModal();
			delete l_dialog;
		}
	}
}
//*****************************************************************************************
//		PARAMETROS DE LA MALLA POLIGONAL
//*****************************************************************************************
void nkNukak3D::eventoParPolygon(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));

		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			
			long  l_strips = current->GetPolyData()->GetNumberOfStrips();
			long  l_polys = current->GetPolyData()->GetNumberOfPolys();
			long  l_lines = current->GetPolyData()->GetNumberOfLines();
			long  l_vertex = current->GetPolyData()->GetNumberOfVerts();
			long  l_cells = current->GetPolyData()->GetNumberOfCells();

			wxString l_text =
				wxT( wxString::Format("Number of strips = %d\n", l_strips) )<<
				wxT( wxString::Format("Number of polygons = %d\n", l_polys) )<<
				wxT( wxString::Format("Number of lines = %d\n", l_lines) )<<
				wxT( wxString::Format("Number of vertex = %d\n", l_vertex) )<<
				wxT( wxString::Format("Number of cells = %d\n", l_cells) );
	

			wxMessageDialog* l_dialog = new wxMessageDialog(this, l_text,
                                                    wxT ("Parametros de la malla poligonal"),
                                                    wxOK|wxICON_INFORMATION );
			l_dialog->ShowModal();
			delete l_dialog;
		}
	}
}
//*****************************************************************************************
//		PARAMETROS DE LA TARJETA DE VIDEO
//*****************************************************************************************
void nkNukak3D::eventoParVideo(wxCommandEvent &WXUNUSED(event))
{	
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));
		wxString l_text;

		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_libro->GetPage(size_t( mi_pagina));
			l_text = current->VideoCard();
			nkIODialog * miDlg = new nkIODialog(	this, 
													l_text,
													-1,
													wxT("Nukak3D: Tarjeta de video"),
													wxDefaultPosition,
													wxSize(640,350));
			miDlg->ShowModal();
		
			delete miDlg;
		}
		else if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_libro->GetPage(size_t( mi_pagina));
			l_text = current->VideoCard();
			nkIODialog * miDlg = new nkIODialog(	this, 
													l_text,
													-1,
													wxT("Nukak3D: Tarjeta de video"),
													wxDefaultPosition,
													wxSize(640,350));
			miDlg->ShowModal();
		
			delete miDlg;
		}



		
	}

}