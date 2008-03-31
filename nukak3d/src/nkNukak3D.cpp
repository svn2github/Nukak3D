/** 
 * @file nkNukak3D.cpp
 * @brief Main windiw of Nukak3D.
 * @details Implementation of gui.
 * @author Alexander Pinzon Fernandez, Byron Perez
 * @version 0.2
 * @date 27/12/2007 03:37 p.m.
*/
#include "nkNukak3D.h"

#include "icon_menu_mas.xpm"

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
	// Menu Bar -> Botton
	wxMenuBar * mi_wxMBMenu = new wxMenuBar();
	SetMenuBar(mi_wxMBMenu);
	
	// File Menu Bar -> Botton
	wxMenu * mi_wxMenuArchivo = new wxMenu(_T(""), wxMENU_TEAROFF);
	mi_wxMenuArchivo->Append(nkNukak3D::ID_SALIR, _("E&xit\tAlt-X"), _("Close application."));
	mi_wxMBMenu->Append(mi_wxMenuArchivo, _("&File"));

	// Help Menu Bar -> Botton
	wxMenu * mi_wxMenuAyuda = new wxMenu;  
	mi_wxMenuAyuda->Append(nkNukak3D::ID_ACERCA_DE, _("&About Nukak3D...\tCtrl-A"), _("Show information of authors."));
	mi_wxMBMenu->Append(mi_wxMenuAyuda, _("Help"));

	//////////////////////////////////
	// ToolBar Tools, // Menu Open
	mi_nkHerramientas = new nkToolBar(this, nkNukak3D::ID_ARBOL, wxDefaultPosition, wxDefaultSize);

	nkMenuTool * mi_nkMenuVolume = mi_nkHerramientas->insertarMenu(-1, _("Open volume images"));
	mi_nkMenuVolume->insertarTool(new nkTool(nkNukak3D::ID_ABRIR_ARCHIVO,_("Volume"), wxNullBitmap, _("Open a single file.")));
	mi_nkMenuVolume->insertarTool(new nkTool(nkNukak3D::ID_ABRIR_ARCHIVO_DICOM,_("Dicom Directory"), wxNullBitmap, _("Open directory with Dicom images.")));

	nkMenuTool * mi_nkMenuObj3D = mi_nkHerramientas->insertarMenu(-1, _("Open Mesh 3D"));
	mi_nkMenuObj3D->insertarTool(new nkTool(nkNukak3D::ID_ABRIR_ARCHIVO_MALLA3D,_("Polygon Mesh"), wxNullBitmap, _("Open file vtk.")));

	// Menu Save
	nkMenuTool * mi_nkMenuGuardar = mi_nkHerramientas->insertarMenu(-1, _("Save"));
	mi_nkMenuGuardar->insertarTool(new nkTool(nkNukak3D::ID_GUARDARVOL,_("Save Volume"), wxNullBitmap, _("Save volume of images.")));
	mi_nkMenuGuardar->insertarTool(new nkTool(nkNukak3D::ID_GUARDARMALLA3D,_("Save Mesh 3D."), wxNullBitmap, _("Save mesh 3D in vtk file format.")));

	// Menu capture screen
	nkMenuTool * mi_nkMenuCaptura = mi_nkHerramientas->insertarMenu(-1, _("Capture Screen"));
	mi_nkMenuCaptura->insertarTool(new nkTool(nkNukak3D::ID_SNAPSHOT3D,_("Save 3D View"), wxNullBitmap, _("Save snapshot of 3d view.")));
	mi_nkMenuCaptura->insertarTool(new nkTool(nkNukak3D::ID_SNAPSHOTAXIAL,_("Save Axial View"), wxNullBitmap, _("Save snapshot of axial view.")));
	mi_nkMenuCaptura->insertarTool(new nkTool(nkNukak3D::ID_SNAPSHOTSAGITAL,_("Save Sagital View"), wxNullBitmap, _("Save snapshot of sagital view.")));
	mi_nkMenuCaptura->insertarTool(new nkTool(nkNukak3D::ID_SNAPSHOTCORONAL,_("Save Coronal View"), wxNullBitmap, _("Save snapshot of coronal view.")));

	// Menu information
	nkMenuTool * mi_nkMenuInfo = mi_nkHerramientas->insertarMenu(-1, _("Information"));
	mi_nkMenuInfo->insertarTool(new nkTool(nkNukak3D::ID_PARIMAGE,_("Volume 3D"), wxNullBitmap, _("Information of volume 3D.")));
	mi_nkMenuInfo->insertarTool(new nkTool(nkNukak3D::ID_PARPOLYGON,_("Polygon mesh"), wxNullBitmap, _("Information of polygon mesh.")));
	mi_nkMenuInfo->insertarTool(new nkTool(nkNukak3D::ID_PARVIDEO,_("Video card"), wxNullBitmap, _("Information of videocard in system.")));
	//////////////////////////////////

	// Toolbar Image
	mi_nkImageViewer = new nkToolBar(this, nkNukak3D::ID_ARBOL, wxDefaultPosition, wxDefaultSize);

	// Menu Lookup table.
	nkMenuTool * mi_nkMenuImageViewer = mi_nkImageViewer->insertarMenu(-1, _("Lookup Table"));
	mi_nkMenuImageViewer->insertarTool(
		new nkTool(nkNukak3D::ID_REINICIAR_PALETA,
			_("Reset Lookup Table"), 
			wxNullBitmap, 
			_("Load window and level by default for this lookup table.")));
	std::vector<std::string> lutNames = vtkLookupTableManager::GetAvailableLookupTables();
	for( unsigned int i=0; i<lutNames.size(); i++){
		mi_nkMenuImageViewer->insertarTool(
			new nkTool(nkNukak3D::ID_ULTIMO + i,
			wxT(lutNames[i].c_str()), 
			wxNullBitmap, 
			_("Change Lookup table.")));
	}
	
	// Menu rendering mode.
	nkMenuTool * mi_nkMenuVol3DMode = mi_nkImageViewer->insertarMenu(-1, _("View volume 3D"));
	
	mi_nkMenuVol3DMode->insertarTool(
		new nkTool(nkNukak3D::ID_VOLVIEWER_RENDERING_ESCALAR,
		_("Escalar: Ortogonal planes"), 
			wxNullBitmap, 
			_("View 3 ortogonal planes over volume 3D.")));
	
	mi_nkMenuVol3DMode->insertarTool(
		new nkTool(nkNukak3D::ID_VOLVIEWER_RENDERING_MRC,
			_("3D: Ray Cast"), 
			wxNullBitmap, 
			_("Rendering volume with Ray Cast method.")));
	
	mi_nkMenuVol3DMode->insertarTool(
		new nkTool(nkNukak3D::ID_VOLVIEWER_RENDERING_TEXTURA,
			_("3D: Texture Mapping"), 
			wxNullBitmap, 
			_("Rendering volume with Texture Mapping method.")));

	// Menu rendering mode.
	nkMenuTool * mi_nkMenuIsoSuperficie = mi_nkImageViewer->insertarMenu(-1, _("IsoSurface"));
	mi_nkMenuIsoSuperficie->insertarTool(new nkTool(nkNukak3D::ID_MARCHING_CUBES,_("3D: Marching Cubes"), wxNullBitmap, _("Reconstruction surface with Marching Cubes method.")));


	/////////////////////////////////////////////////

	// Menu image filters.
	nkMenuTool * mi_nkMenuVolUtilidades = mi_nkImageViewer->insertarMenu(-1, _("Filter and Segmentation"));
	
	mi_nkMenuVolUtilidades->insertarTool(
		new nkTool(nkNukak3D::ID_LSLEVELSETSCOMPLETO,
			_("Segmentation with Levelsets"), 
			wxNullBitmap, 
			_("Segmentation with Levelsets using cursor position as seed.")));
	
	mi_nkMenuVolUtilidades->insertarTool(
		new nkTool(nkNukak3D::ID_AREA,
			_("Area of Axial Image"), 
			wxNullBitmap, 
			_("Area greater that zero in the axial image in square millimeters mm2.")));	

	mi_nkMenuVolUtilidades->insertarTool(
		new nkTool(nkNukak3D::ID_FILVOLGAUSSIAN,
			_("Gaussian Filter"), 
			wxNullBitmap, 
			_("Gaussian filter to volume.")));	

	mi_nkMenuVolUtilidades->insertarTool(
		new nkTool(nkNukak3D::ID_FILVOLMEDIAN,
			_("Median Filter"), 
			wxNullBitmap, 
			_("Median filter to volume.")));	

	/////////////////////////////////////////////////

	// Menu Mesh 3D.
	nkMenuTool * mi_nkMenuFiltrosPoly = mi_nkImageViewer->insertarMenu(-1, _("Modifiers For Mesh 3D"));
	
	mi_nkMenuFiltrosPoly->insertarTool(
		new nkTool(nkNukak3D::ID_FILPOLYTRIANGLE,
			_("Triangulated"), 
			wxNullBitmap, 
			_("Triangulated of mesh 3D.")));
	
	mi_nkMenuFiltrosPoly->insertarTool(
		new nkTool(nkNukak3D::ID_FILPOLYDECIMATE,
			_("Decimate Mesh 3D"), 
			wxNullBitmap, 
			_("Decimate reduction o 3d mesh")));	

	mi_nkMenuFiltrosPoly->insertarTool(
		new nkTool(nkNukak3D::ID_FILPOLYSMOOTH,
			_("Smooth"), 
			wxNullBitmap, 
			_("Smooth mesh 3D")));	

	mi_nkMenuFiltrosPoly->insertarTool(
		new nkTool(nkNukak3D::ID_FILPOLYNORMALS,
			_("Normals"), 
			wxNullBitmap, 
			_("Recalculate Normals.")));	

	mi_nkMenuFiltrosPoly->insertarTool(
		new nkTool(nkNukak3D::ID_FILPOLYDEFORM,
			_("Deform"), 
			wxNullBitmap, 
			_("Deform mesh 3d")));	


	// Menu Navitation.
	nkMenuTool * mi_nkMenuNavegacion = mi_nkImageViewer->insertarMenu(-1, _("Navigation"));
	
	mi_nkMenuNavegacion->insertarTool(
		new nkTool(nkNukak3D::ID_NAVRESET,
			_("Reset Camera"), 
			wxNullBitmap, 
			_("Reset position and orientation of camera.")));

	mi_nkMenuNavegacion->insertarTool(
		new nkTool(nkNukak3D::ID_NAVTRACKBALL,
			_("Trackball"), 
			wxNullBitmap, 
			_("Trackball camera.")));

	mi_nkMenuNavegacion->insertarTool(
		new nkTool(nkNukak3D::ID_NAVJOYSTICK,
			_("Joystick"), 
			wxNullBitmap, 
			_("Joystick camera.")));

	mi_nkMenuNavegacion->insertarTool(
		new nkTool(nkNukak3D::ID_NAVFLIGHT,
			_("Flight"), 
			wxNullBitmap, 
			_("Flight camera.")));

	mi_nkMenuNavegacion->insertarTool(
		new nkTool(nkNukak3D::ID_NAVUNICAM,
			_("Unicam"), 
			wxNullBitmap, 
			_("Unicam camera.")));

	
	// Menu stereoscopy vision
	nkMenuTool * mi_nkMenuEstereo = mi_nkImageViewer->insertarMenu(-1, _("Stereoscopy"));
	
	mi_nkMenuEstereo->insertarTool(
		new nkTool(nkNukak3D::ID_STACTIVO,
			_("Enable Stereoscopy Vision"), 
			wxNullBitmap, 
			_("Enable/Disable Stereoscopy vision.")));

	mi_nkMenuEstereo->insertarTool(
		new nkTool(nkNukak3D::ID_STPASIVO,
			_("Stereo passive"), 
			wxNullBitmap, 
			_("Enable/Disable Stereo passive.")));

	mi_nkMenuEstereo->insertarTool(
		new nkTool(nkNukak3D::ID_STAUMENTAR,
			_("More separation +"), 
			wxNullBitmap, 
			_("More separation between images in Stereoscopy vision")));

	mi_nkMenuEstereo->insertarTool(
		new nkTool(nkNukak3D::ID_STDISMINUIR,
			_("Less separation -"), 
			wxNullBitmap, 
			_("Less separation between images in Stereoscopy vision")));
	
	// Insert nkToolBar Tools
	this->insertarToolBar(mi_nkHerramientas, _("Tools"), _("Tools"));
	// Insert nkToolBar Configuration
	this->insertarToolBar(mi_nkImageViewer, _("Configuration"), _("Configuration"));


	prv_libro = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
		wxAUI_NB_DEFAULT_STYLE | 
		wxAUI_NB_TAB_EXTERNAL_MOVE | 
		wxAUI_NB_WINDOWLIST_BUTTON |
		wxAUI_NB_CLOSE_ON_ALL_TABS |
		wxNO_BORDER);

	prv_auiManager.AddPane(prv_libro, wxAuiPaneInfo().
                  Name(_("Nukak3d Workspace")).Caption(_("Nukak 3D Files")).
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
//		TABLE OF EVENTS
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
//		INSERT TOOLBAR
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
        										  _("Open file..."),
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
//		MENU -> OPEN -> VOLUME DICOM
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
//		MENU -> OPEN -> MESH 3D
//*****************************************************************************************
void nkNukak3D::eventoAbrirMalla3D(wxCommandEvent& WXUNUSED(event)){
	const wxString miWxFilter =
	wxT ("VTK (*.vtk)|*.vtk|")
	wxT ("All (*)|*");

	wxFileDialog * miFileDialog = new wxFileDialog(this, 
        										  _("Open file..."),
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
//		MENU -> OPEN -> VOLUME
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
        										  _("Save as"),
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
//		MENU -> SAVE -> MESH 3D
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
        										  _("Save as"),
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
//		EVENT TREE
//*****************************************************************************************
void nkNukak3D::eventoArbol(wxTreeEvent& event){
	wxCommandEvent mievento;
#ifdef __WIN32__
	wxString mi_item = wxT("NULL");
#else //MAC O UNIX
	wxString mi_item = mi_nkHerramientas->GetItemText(event.GetItem());
#endif //__WIN32__
	if (mi_item == _("Volume")) eventoAbrirVolumen(mievento);
	else if (mi_item == _("Dicom Directory")) eventoAbrirVolumenDicom(mievento);
	else if (mi_item == _("Polygon Mesh")) eventoAbrirMalla3D(mievento);
	else if (mi_item == _("3D: Marching Cubes")) eventoMarchingCubes(mievento);
	else if (mi_item == _("Escalar: Ortogonal planes")) eventoVolViewerRenderingEscalar(mievento);
	else if (mi_item == _("3D: Ray Cast")) eventoVolViewerRenderingMRC(mievento);
	else if (mi_item == _("3D: Texture Mapping")) eventoVolViewerRenderingTextura(mievento);
	else if (mi_item == _("Reset Lookup Table")) eventoReiniciarPaleta(mievento);
	else if (mi_item == _("Segmentation with Levelsets")) eventolsLevelsetsCompleto(mievento);
	else if (mi_item == _("Area of Axial Image")) eventoArea(mievento);
	else if (mi_item == _("Gaussian Filter")) eventoFilVolGaussian(mievento);
	else if (mi_item == _("Median Filter")) eventoFilVolMedian(mievento);
	else if (mi_item == _("Triangulated")) eventoFilPolyTriangle(mievento);
	else if (mi_item == _("Decimate Mesh 3D")) eventoFilPolyDecimate(mievento);
	else if (mi_item == _("Smooth")) eventoFilPolySmooth(mievento);
	else if (mi_item == _("Normals")) eventoFilPolyNormals(mievento);
	else if (mi_item == _("Deform")) eventoFilPolyDeform(mievento);
	else if (mi_item == _("Reset Camera")) eventoNavResetCamara(mievento);
	else if (mi_item == _("Trackball")) eventoNavTrackball(mievento);
	else if (mi_item == _("Joystick")) eventoNavJoystick(mievento);
	else if (mi_item == _("Flight")) eventoNavFlight(mievento);
	else if (mi_item == _("Unicam")) eventoNavUnicam(mievento);
	else if (mi_item == _("Enable Stereoscopy Vision")) eventoStActivo(mievento);
	else if (mi_item == _("Stereo passive")) eventoStPasivo(mievento);
	else if (mi_item == _("More separation +")) eventoStAumentar(mievento);
	else if (mi_item == _("Less separation -")) eventoStDisminuir(mievento);

	std::vector<std::string> lutNames = vtkLookupTableManager::GetAvailableLookupTables();
	int val = -1;
	for( unsigned int i=0; i<lutNames.size(); i++){
		if (mi_item == _(lutNames[i].c_str())) val = i;
	}
	if (val>-1){
		wxCommandEvent event(0, val +nkNukak3D::ID_ULTIMO);
		eventoPaletaColor(event);
	}
}
//*****************************************************************************************
//		MENU -> LOOKUP TABLE -> RESET
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
//		MENU -> LOOKUP TABLE -> SELECT
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
//		MENU -> VIEW VOLUME 3D -> ESCALAR (ORTOGONAL PLANES)
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
//		MENU -> VIEW VOLUME 3D -> RAY CAST
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
//		MENU -> VIEW VOLUME 3D -> TEXTURE MAPPING
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
			etiquetas[0] = _("Numbers of contours");
			etiquetas[1] = _("Value of threshold lower");
			etiquetas[2] = _("Value of threshold upper");
			
			nkIODialog * miDlg = new nkIODialog(	this, 
															etiquetas,
															num_datos,
															-1,
															_("Nukak3D: Marching cubes"),
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
//		MENU -> SEGMENTAtION -> LEVELSETS
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
//		FILTER -> GAUSSIAN
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
//		FILTER -> MEDIAN
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
//		FILTER -> GRADIENT
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
//		FILTER -> THRESHOLD
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
//		FILTER -> TRIANGULATE
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
//		FILTER -> DECIMATE
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
//		FILTER -> SMOOTH
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
//		FILTER -> NORMALS
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
//		FILTER -> DEFORM
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
//		CAPTURE SNAPSHOT 3D
//*****************************************************************************************
void nkNukak3D::eventoSnapshot3D(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_libro->GetPageCount() > 0){
		int mi_pagina = prv_libro->GetSelection();
		wxWindow * pagina = prv_libro->GetPage(size_t( mi_pagina));

		const wxString miWxFilter =	
			wxT ("Jpeg (*.jpg)|*.jpg|")
			wxT ("Bmp (*.bmp)|*.bmp|")			
			wxT ("Tiff (*.tif)|*.tif|");
		wxFileDialog * myFileDialog = new wxFileDialog(this, 
    										  _("Save as"),
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
//		CAPTURE SNAPSHOT AXIAL VIEW
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
    										  _("Save as"),
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
//		CAPTURE SNAPSHOT CORONAL VIEW
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
    										  _("Save as"),
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
//		CAPTURE SNAPSHOT SAGITAL VIEW
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
    										  _("Save as"),
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
//		IMAGE INFORMATION
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
				wxString::Format(_("Size in x = \t%d\n"), d[0])<<
				wxString::Format(_("Size in y = \t%d\n"), d[1])<<
				wxString::Format(_("Size in z = \t%d\n"), d[2])<<
				wxString::Format(_("Spacing in x = \t%1.5f\n"), s[0])<<
				wxString::Format(_("Spacing in y = \t%1.5f\n"), s[1])<<
				wxString::Format(_("Spacing in z = \t%1.5f\n"), s[2]);

			int l_type= current->getVtkImagen()->GetScalarType();

			if( l_type == 2  ) l_text<<_("Scalar type = CHAR");
			if( l_type == 3	 ) l_text<<_("Scalar type = UNSIGNED_CHAR");
			if( l_type == 4  ) l_text<<_("Scalar type = SHORT");
			if( l_type == 5	 ) l_text<<_("Scalar type = UNSIGNED_SHORT");
			if( l_type == 6  ) l_text<<_("Scalar type = INT");
			if( l_type == 7  ) l_text<<_("Scalar type = UNSIGNED_INT");
			if( l_type == 8  ) l_text<<_("Scalar type = LONG");
			if( l_type == 9  ) l_text<<_("Scalar type = UNSIGNED_LONG");
			if( l_type == 10 ) l_text<<_("Scalar type = FLOAT");
			if( l_type == 11 ) l_text<<_("Scalar type = DOUBLE");

			wxMessageDialog* l_dialog = new wxMessageDialog(this, l_text,
                                                    _("Information of volume"),
                                                    wxOK|wxICON_INFORMATION );
			l_dialog->ShowModal();
			delete l_dialog;
		}
	}
}
//*****************************************************************************************
//		MESH INFORMATION
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
				wxString::Format(_("Number of strips = %d\n"), l_strips)<<
				wxString::Format(_("Number of polygons = %d\n"), l_polys)<<
				wxString::Format(_("Number of lines = %d\n"), l_lines)<<
				wxString::Format(_("Number of vertex = %d\n"), l_vertex)<<
				wxString::Format(_("Number of cells = %d\n"), l_cells);
	

			wxMessageDialog* l_dialog = new wxMessageDialog(this, l_text,
                                                    _("Mesh information"),
                                                    wxOK|wxICON_INFORMATION );
			l_dialog->ShowModal();
			delete l_dialog;
		}
	}
}
//*****************************************************************************************
//		VIDEO CARD INFORMATION
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
													_("Nukak3D: Video card."),
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
													_("Nukak3D: Video card"),
													wxDefaultPosition,
													wxSize(640,350));
			miDlg->ShowModal();
		
			delete miDlg;
		}



		
	}

}