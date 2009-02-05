/** 
 * @file nkNukak3D.cpp
 * @brief Main window of Nukak3D.
 * @details Implementation of gui.
 * @author Alexander Pinzon Fernandez, Byron Perez
 * @version 0.2
 * @date 27/12/2007 03:37 p.m.
*/
#include "nkNukak3D.h"
#include "nkStoreSCP.h"
#include "nkFindSCU.h"

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
		wxApp::s_macAboutMenuItemId = nkNukak3D::ID_ABOUT_MAC;
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
	mi_wxMenuArchivo->Append(nkNukak3D::ID_EXIT, _("E&xit\tAlt-X"), _("Close application."));
	mi_wxMBMenu->Append(mi_wxMenuArchivo, _("&File"));

	// DICOM Menu Bar -> Botton
	wxMenu * mi_wxMenuDicom = new wxMenu;  
	mi_wxMenuDicom->Append(nkNukak3D::ID_DICOMFIND, _("DICOM Search"), _("Search patient in DICOM server."));
	mi_wxMenuDicom->Append(nkNukak3D::ID_DICOMSERVER, _("DICOM Listener"), _("Configure and start Dicom Listener."));
	mi_wxMBMenu->Append(mi_wxMenuDicom, _("DICOM"));

	// Tools Menu Bar -> Botton
	wxMenu * mi_wxMenuHerramientas = new wxMenu;  
	mi_wxMenuHerramientas->Append(nkNukak3D::ID_SETLANGUAGE, _("Choose language"), _("Change language for controls."));
	mi_wxMBMenu->Append(mi_wxMenuHerramientas, _("Tools"));

	// Help Menu Bar -> Botton
	wxMenu * mi_wxMenuAyuda = new wxMenu;  
	mi_wxMenuAyuda->Append(nkNukak3D::ID_ABOUT, _("&About Nukak3D...\tCtrl-A"), _("Show information of authors."));
	mi_wxMBMenu->Append(mi_wxMenuAyuda, _("Help"));

	//////////////////////////////////
	// ToolBar Tools, // Menu Open
	prv_nkToolBarTools = new nkToolBar(this, nkNukak3D::ID_TREE, wxDefaultPosition, wxDefaultSize);

	nkMenuTool * mi_nkMenuVolume = prv_nkToolBarTools->insertarMenu(-1, _("Open volume images"));
	mi_nkMenuVolume->insertarTool(new nkTool(nkNukak3D::ID_OPEN_FILE,_("Volume"), wxNullBitmap, _("Open a single file.")));
	mi_nkMenuVolume->insertarTool(new nkTool(nkNukak3D::ID_OPEN_FILE_DICOM,_("Dicom Directory"), wxNullBitmap, _("Open directory with Dicom images.")));

	nkMenuTool * mi_nkMenuObj3D = prv_nkToolBarTools->insertarMenu(-1, _("Open Mesh 3D"));
	mi_nkMenuObj3D->insertarTool(new nkTool(nkNukak3D::ID_OPEN_FILE_MESH3D,_("Polygon Mesh"), wxNullBitmap, _("Open file vtk.")));

	// Menu Save
	nkMenuTool * mi_nkMenuGuardar = prv_nkToolBarTools->insertarMenu(-1, _("Save"));
	mi_nkMenuGuardar->insertarTool(new nkTool(nkNukak3D::ID_SAVE_VOL,_("Save Volume"), wxNullBitmap, _("Save volume of images.")));
	mi_nkMenuGuardar->insertarTool(new nkTool(nkNukak3D::ID_SAVE_MESH3D,_("Save Mesh 3D."), wxNullBitmap, _("Save mesh 3D in vtk file format.")));

	// Menu capture screen
	nkMenuTool * mi_nkMenuCaptura = prv_nkToolBarTools->insertarMenu(-1, _("Capture Screen"));
	mi_nkMenuCaptura->insertarTool(new nkTool(nkNukak3D::ID_SNAPSHOT3D,_("Save 3D View"), wxNullBitmap, _("Save snapshot of 3d view.")));
	mi_nkMenuCaptura->insertarTool(new nkTool(nkNukak3D::ID_SNAPSHOTAXIAL,_("Save Axial View"), wxNullBitmap, _("Save snapshot of axial view.")));
	mi_nkMenuCaptura->insertarTool(new nkTool(nkNukak3D::ID_SNAPSHOTSAGITAL,_("Save Sagital View"), wxNullBitmap, _("Save snapshot of sagital view.")));
	mi_nkMenuCaptura->insertarTool(new nkTool(nkNukak3D::ID_SNAPSHOTCORONAL,_("Save Coronal View"), wxNullBitmap, _("Save snapshot of coronal view.")));

	// Menu information
	nkMenuTool * mi_nkMenuInfo = prv_nkToolBarTools->insertarMenu(-1, _("Information"));
	mi_nkMenuInfo->insertarTool(new nkTool(nkNukak3D::ID_INFORMATION_IMAGE,_("Volume 3D"), wxNullBitmap, _("Information of volume 3D.")));
	mi_nkMenuInfo->insertarTool(new nkTool(nkNukak3D::ID_INFORMATION_POLYGON,_("Polygon mesh"), wxNullBitmap, _("Information of polygon mesh.")));
	mi_nkMenuInfo->insertarTool(new nkTool(nkNukak3D::ID_INFORMATION_VIDEO_CARD,_("Video card"), wxNullBitmap, _("Information of videocard in system.")));
	mi_nkMenuInfo->insertarTool(new nkTool(nkNukak3D::ID_FPS,_("Frames per second"), wxNullBitmap, _("Frames per second of the renderer.")));
	//////////////////////////////////

	// Toolbar Image
	prv_nkToolBarImageViewer = new nkToolBar(this, nkNukak3D::ID_TREE, wxDefaultPosition, wxDefaultSize);

	// Menu Lookup table.
	nkMenuTool * mi_nkMenuImageViewer = prv_nkToolBarImageViewer->insertarMenu(-1, _("Lookup Table"));
	mi_nkMenuImageViewer->insertarTool(
		new nkTool(nkNukak3D::ID_RESET_LOOKUP_TABLE,
			_("Reset Lookup Table"), 
			wxNullBitmap, 
			_("Load window and level by default for this lookup table.")));
	std::vector<std::string> lutNames = vtkLookupTableManager::GetAvailableLookupTables();
	for( unsigned int i=0; i<lutNames.size(); i++){
		mi_nkMenuImageViewer->insertarTool(
			new nkTool(nkNukak3D::ID_LAST_LOOKUP_TABLE + i,
			wxT(lutNames[i].c_str()), 
			wxNullBitmap, 
			_("Change Lookup table.")));
	}
	
	// Menu rendering mode.
	nkMenuTool * mi_nkMenuVol3DMode = prv_nkToolBarImageViewer->insertarMenu(-1, _("View volume 3D"));
	
	mi_nkMenuVol3DMode->insertarTool(
		new nkTool(nkNukak3D::ID_VOLVIEWER_RENDERING_ESCALAR,
		_("Scalar: Ortogonal planes"), 
			wxNullBitmap, 
			_("View 3 ortogonal planes over volume 3D.")));
			
	mi_nkMenuVol3DMode->insertarTool(
		new nkTool(nkNukak3D::ID_VOLVIEWER_RENDERING_TEXTURE,
			_("3D: Texture Mapping"), 
			wxNullBitmap, 
			_("Rendering volume with Texture Mapping method.")));
	
	mi_nkMenuVol3DMode->insertarTool(
		new nkTool(nkNukak3D::ID_VOLVIEWER_RENDERING_MRC_MIP,
			_("3D: Raycasting MIP"), 
			wxNullBitmap, 
			_("RayCasting - Maximum Intensity Projection")));

	mi_nkMenuVol3DMode->insertarTool(
		new nkTool(nkNukak3D::ID_VOLVIEWER_RENDERING_MRC_COMP,
			_("3D: Raycasting composite"), 
			wxNullBitmap, 
			_("RayCasting - Composite Look Up Table")));

	mi_nkMenuVol3DMode->insertarTool(
		new nkTool(nkNukak3D::ID_VOLVIEWER_RENDERING_MRC_ISO,
			_("3D: Raycasting isosurface"), 
			wxNullBitmap, 
			_("RayCasting - Isosurface")));
			
	mi_nkMenuVol3DMode->insertarTool(
		new nkTool(nkNukak3D::ID_prBoundingBox,
			_("Show/hide bounding box"), 
			wxNullBitmap, 
			_("Show/hide bounding box")));

	mi_nkMenuVol3DMode->insertarTool(
		new nkTool(nkNukak3D::ID_BOXWIDGET,
			_("Show/hide box widget"), 
			wxNullBitmap, 
			_("Show/hide box widget")));

	// Menu rendering mode.
	nkMenuTool * mi_nkMenuIsoSuperficie = prv_nkToolBarImageViewer->insertarMenu(-1, _("IsoSurface"));
	mi_nkMenuIsoSuperficie->insertarTool(new nkTool(nkNukak3D::ID_MARCHING_CUBES,_("3D: Marching Cubes"), wxNullBitmap, _("Reconstruction surface with Marching Cubes method.")));


	/////////////////////////////////////////////////

	// Menu image filters.
	nkMenuTool * mi_nkMenuVolUtilidades = prv_nkToolBarImageViewer->insertarMenu(-1, _("Filter and Segmentation"));
	
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
	nkMenuTool * mi_nkMenuFiltrosPoly = prv_nkToolBarImageViewer->insertarMenu(-1, _("Modifiers For Mesh 3D"));
	
	mi_nkMenuFiltrosPoly->insertarTool(
		new nkTool(nkNukak3D::ID_prBoundingBox,
			_("Show/hide bounding box"), 
			wxNullBitmap, 
			_("Show/hide bounding box")));
	
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
	nkMenuTool * mi_nkMenuNavegacion = prv_nkToolBarImageViewer->insertarMenu(-1, _("Navigation"));
	
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
	nkMenuTool * mi_nkMenuEstereo = prv_nkToolBarImageViewer->insertarMenu(-1, _("Stereoscopy"));
	
	mi_nkMenuEstereo->insertarTool(
		new nkTool(nkNukak3D::ID_STEREO_ACTIVE,
			_("Enable Stereoscopy Vision"), 
			wxNullBitmap, 
			_("Enable/Disable Stereoscopy vision.")));

	mi_nkMenuEstereo->insertarTool(
		new nkTool(nkNukak3D::ID_STEREO_PASSIVE,
			_("Stereo passive"), 
			wxNullBitmap, 
			_("Enable/Disable Stereo passive.")));

	mi_nkMenuEstereo->insertarTool(
		new nkTool(nkNukak3D::ID_STEREO_MORE_SEPARATION,
			_("More separation +"), 
			wxNullBitmap, 
			_("More separation between images in Stereoscopy vision")));

	mi_nkMenuEstereo->insertarTool(
		new nkTool(nkNukak3D::ID_STEREO_LESS_SEPARATION,
			_("Less separation -"), 
			wxNullBitmap, 
			_("Less separation between images in Stereoscopy vision")));
			
	//Menu grupo "configuración" -> Colisiones -> lateral
	nkMenuTool * mi_nkMenuColisiones = prv_nkToolBarImageViewer->insertarMenu(-1, _("Collision detection"));
	
	mi_nkMenuColisiones->insertarTool(
		new nkTool(nkNukak3D::ID_CAMERAPOS,
			_("Activate/deactivate collision detection"), 
			wxNullBitmap, 
			_("Activate/deactivate collision detection")));
	mi_nkMenuColisiones->insertarTool(
		new nkTool(nkNukak3D::ID_NAVENDOSCOPE,
			_("Endoscopic camera"), 
			wxNullBitmap, 
			_("Endoscopic camera")));
	
	// Insert nkToolBar Tools
	this->prInsertToolBar(prv_nkToolBarTools, _("Tools"), _("Tools"));
	// Insert nkToolBar Configuration
	this->prInsertToolBar(prv_nkToolBarImageViewer, _("Configuration"), _("Configuration"));


	prv_wxAuiNotebook = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
		wxAUI_NB_DEFAULT_STYLE | 
		wxAUI_NB_TAB_EXTERNAL_MOVE | 
		wxAUI_NB_WINDOWLIST_BUTTON |
		wxAUI_NB_CLOSE_ON_ALL_TABS |
		wxNO_BORDER);

	prv_auiManager.AddPane(prv_wxAuiNotebook, wxAuiPaneInfo().
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
	/*if(m_stick)
	{
		m_stick->ReleaseCapture();
		delete m_stick;
	}*/
}

//*****************************************************************************************
//		TABLE OF EVENTS
//*****************************************************************************************
BEGIN_EVENT_TABLE(nkNukak3D, wxFrame)
	EVT_CLOSE(nkNukak3D::prOnClose)
	EVT_TREE_ITEM_ACTIVATED(nkNukak3D::ID_TREE, nkNukak3D::prEventTree)
	EVT_MENU(nkNukak3D::ID_EXIT, nkNukak3D::prEventExit)
	EVT_MENU(nkNukak3D::ID_ABOUT, nkNukak3D::prEventAbout)
	EVT_MENU(nkNukak3D::ID_ABOUT_MAC, nkNukak3D::prEventAboutMAC)
	EVT_MENU(nkNukak3D::ID_OPEN_FILE, nkNukak3D::prEventOpenVolumen)
	EVT_MENU(nkNukak3D::ID_OPEN_FILE_DICOM, nkNukak3D::prEventOpenVolumenDicom)
	EVT_MENU(nkNukak3D::ID_OPEN_FILE_MESH3D, nkNukak3D::prEventOpenMesh3D)
	EVT_MENU(nkNukak3D::ID_prBoundingBox, nkNukak3D::prEventprBoundingBox)
	EVT_MENU(nkNukak3D::ID_BOXWIDGET, nkNukak3D::prEventBoxWidget)
	EVT_MENU(nkNukak3D::ID_SAVE_VOL, nkNukak3D::prEventSaveVol)
	EVT_MENU(nkNukak3D::ID_SAVE_MESH3D, nkNukak3D::prEventSaveMesh3D)
	EVT_MENU(nkNukak3D::ID_MARCHING_CUBES, nkNukak3D::prEventMarchingCubes)
	EVT_MENU(nkNukak3D::ID_RESET_LOOKUP_TABLE, nkNukak3D::prEventResetLookupTable)
	EVT_MENU(nkNukak3D::ID_VOLVIEWER_RENDERING_ESCALAR, nkNukak3D::prEventVolViewerRenderingEscalar)
	EVT_MENU(nkNukak3D::ID_VOLVIEWER_RENDERING_MRC_MIP, nkNukak3D::prEventVolViewerRenderingMRCmip)
	EVT_MENU(nkNukak3D::ID_VOLVIEWER_RENDERING_MRC_COMP, nkNukak3D::prEventVolViewerRenderingMRCcomp)
	EVT_MENU(nkNukak3D::ID_VOLVIEWER_RENDERING_MRC_ISO, nkNukak3D::prEventVolViewerRenderingMRCiso)
	EVT_MENU(nkNukak3D::ID_VOLVIEWER_RENDERING_TEXTURE, nkNukak3D::prEventVolViewerRenderingTextura)
	EVT_MENU(nkNukak3D::ID_LSLEVELSETSCOMPLETO, nkNukak3D::prEventLevelSets)
	EVT_MENU(nkNukak3D::ID_AREA, nkNukak3D::prEventCalcArea)
	EVT_MENU(nkNukak3D::ID_NAVRESET, nkNukak3D::prEventprNavResetCamara)
	EVT_MENU(nkNukak3D::ID_NAVTRACKBALL,nkNukak3D::prEventprNavTrackball)
	EVT_MENU(nkNukak3D::ID_NAVJOYSTICK,nkNukak3D::prEventprNavJoystick)
	EVT_MENU(nkNukak3D::ID_NAVFLIGHT,nkNukak3D::prEventprNavFlight)
	EVT_MENU(nkNukak3D::ID_NAVUNICAM,nkNukak3D::prEventprNavUnicam)
	EVT_MENU(nkNukak3D::ID_STEREO_ACTIVE, nkNukak3D::prEventprActiveStereo)
	EVT_MENU(nkNukak3D::ID_STEREO_PASSIVE, nkNukak3D::prEventprStereoPassive)
	EVT_MENU(nkNukak3D::ID_STEREO_MORE_SEPARATION, nkNukak3D::prEventStMoreSeparation)
	EVT_MENU(nkNukak3D::ID_STEREO_LESS_SEPARATION, nkNukak3D::prEventStLessSeparation)
	EVT_MENU(nkNukak3D::ID_FILVOLGAUSSIAN, nkNukak3D::prEventFilVolGaussian)
	EVT_MENU(nkNukak3D::ID_FILVOLMEDIAN, nkNukak3D::prEventFilVolMedian)
	EVT_MENU(nkNukak3D::ID_FILVOLGRADIENT, nkNukak3D::prEventFilVolGradient)
	EVT_MENU(nkNukak3D::ID_FILVOLTHRESHOLD, nkNukak3D::prEventFilVolThreshold)
	EVT_MENU(nkNukak3D::ID_FILPOLYTRIANGLE, nkNukak3D::prEventFilprPolyTriangle)
	EVT_MENU(nkNukak3D::ID_FILPOLYDECIMATE, nkNukak3D::prEventFilprPolyDecimate)
	EVT_MENU(nkNukak3D::ID_FILPOLYSMOOTH, nkNukak3D::prEventFilprPolySmooth)
	EVT_MENU(nkNukak3D::ID_FILPOLYNORMALS, nkNukak3D::prEventFilprPolyNormals)
	EVT_MENU(nkNukak3D::ID_FILPOLYDEFORM, nkNukak3D::prEventFilprPolyDeform)
	EVT_MENU(nkNukak3D::ID_SNAPSHOT3D, nkNukak3D::prEventSnapshot3D)
	EVT_MENU(nkNukak3D::ID_SNAPSHOTAXIAL, nkNukak3D::prEventSnapshotAxial)
	EVT_MENU(nkNukak3D::ID_SNAPSHOTSAGITAL, nkNukak3D::prEventSnapshotSagital)
	EVT_MENU(nkNukak3D::ID_SNAPSHOTCORONAL, nkNukak3D::prEventSnapshotCoronal)
	EVT_MENU(nkNukak3D::ID_SETLANGUAGE, nkNukak3D::prEventChangeLanguage)
	EVT_MENU(nkNukak3D::ID_INFORMATION_IMAGE, nkNukak3D::prEventInformationImage)
	EVT_MENU(nkNukak3D::ID_INFORMATION_POLYGON, nkNukak3D::prEventInformationPolygon)
	EVT_MENU(nkNukak3D::ID_INFORMATION_VIDEO_CARD,  nkNukak3D::prEventInformationVideoCard)
	EVT_MENU(nkNukak3D::ID_CAMERAPOS, nkNukak3D::prEventPositionCamera)
	EVT_MENU(nkNukak3D::ID_NAVENDOSCOPE, nkNukak3D::prEventNavEndoscope)
	EVT_MENU(nkNukak3D::ID_FPS, nkNukak3D::prEventFPS)
	EVT_MENU(nkNukak3D::ID_DICOMSERVER, nkNukak3D::prEventDicomListener)
	EVT_MENU(nkNukak3D::ID_DICOMFIND, nkNukak3D::prEventDicomFind)
	EVT_MENU(wxID_ANY, nkNukak3D::prEventLookupTable)
END_EVENT_TABLE()

//*****************************************************************************************
//		INSERT TOOLBAR
//*****************************************************************************************
void nkNukak3D::prInsertToolBar(wxWindow* window, wxString a_name, wxString a_label){
	int separacion = 15;
#ifdef __WIN32__
	separacion = 15;
#else //mac o unix
	separacion = 50;
#endif //__WIN32__
	prv_auiManager.AddPane(window, wxAuiPaneInfo().
              Name(a_name).
			  Left().
			  Caption(a_label).
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
void nkNukak3D::prEventExit(wxCommandEvent& WXUNUSED(event)){
	Close(TRUE);
}

void nkNukak3D::prOnClose(wxCloseEvent & event){
	nkStoreSCP * miscp =  nkStoreSCP::getInstance(this,115);
	if(miscp){
		miscp->ShowModalStop();
		miscp->Destroy();
	}
	event.Skip();
}

//*****************************************************************************************
//		MENU -> AYUDA -> ACERCA DE
//*****************************************************************************************
void nkNukak3D::prEventAbout(wxCommandEvent& WXUNUSED(event)){
	nkAcercaDe * miAcercaDe = new nkAcercaDe(this);
	miAcercaDe->ShowModal();
	delete miAcercaDe;
	
}
//*****************************************************************************************
//		MENU -> AYUDA -> ACERCA DE    Version MAC
//*****************************************************************************************
void nkNukak3D::prEventAboutMAC(wxCommandEvent& WXUNUSED(event)){
	nkAcercaDe * miAcercaDe = new nkAcercaDe(this);
	miAcercaDe->ShowModal();
	delete miAcercaDe;
}
//*****************************************************************************************
//		MENU -> ABRIR -> VOLUMEN
//*****************************************************************************************
void nkNukak3D::prEventOpenVolumen(wxCommandEvent& WXUNUSED(event)){

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
	wxString extension, a_fileName;

	for( unsigned int i=0; i<Nimages; i++ )	{
		a_fileName = nombresArchivos[i];
		extension = a_fileName.Mid(a_fileName.Length()-3,3);
		if( extension == "vol" ){
			nkVolViewer * mivol = new nkVolViewer(prv_wxAuiNotebook);
			mivol->Configure();
			mivol->prOpenFile_vol(a_fileName);
			prv_wxAuiNotebook->AddPage(mivol, a_fileName,true );
		}else{
			nkVolViewer * mivol = new nkVolViewer(prv_wxAuiNotebook);
			mivol->Configure();
			mivol->prOpenFile(a_fileName);
			prv_wxAuiNotebook->AddPage(mivol, a_fileName,true );
		}
	}

	wxEndBusyCursor();
}
//*****************************************************************************************
//		MENU -> OPEN -> VOLUME DICOM
//*****************************************************************************************
void nkNukak3D::prEventOpenVolumenDicom(wxCommandEvent& WXUNUSED(event)){
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
		nkVolViewer * mi_Vol2 = new nkVolViewer(prv_wxAuiNotebook, wxID_ANY);
		mi_Vol2->Configure();
		wxString basename = wxT (miImporter->GetDescription (i).c_str());
		prv_wxAuiNotebook->AddPage(mi_Vol2, basename,true );
		mi_Vol2->prOpenFile_dicom(basename, miImporter, i);
	}
	miImporter->Destroy();

	wxEndBusyCursor();
}

void nkNukak3D::prOpenVolumenDicom(wxString pathDicom){
	wxVtkDICOMImporter* miImporter = new wxVtkDICOMImporter (this, true);

	miImporter->LoadDirectory(pathDicom.c_str());
	miImporter->RunFromPage(2);
	
	/*if( miImporter->GetReturnCode()!=wxID_OK ){
		miImporter->Destroy();
		return;
	}*/
	
	wxBeginBusyCursor();

	unsigned int Nimages = miImporter->GetOutputs().size();
	double step = 100.0/(double)(Nimages);

	for (unsigned int i=0; i<Nimages ; i++){
		if (!miImporter->GetOutput(i)){
		continue;
		}
		nkVolViewer * mi_Vol2 = new nkVolViewer(prv_wxAuiNotebook, wxID_ANY);
		mi_Vol2->Configure();
		wxString basename = wxT (miImporter->GetDescription (i).c_str());
		prv_wxAuiNotebook->AddPage(mi_Vol2, basename,true );
		mi_Vol2->prOpenFile_dicom(basename, miImporter, i);
	}
	miImporter->Destroy();

	wxEndBusyCursor();
}

//*****************************************************************************************
//		MENU -> OPEN -> MESH 3D
//*****************************************************************************************
void nkNukak3D::prEventOpenMesh3D(wxCommandEvent& WXUNUSED(event)){
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
	wxString a_fileName;
    nombresArchivos.Empty();
    int OK = miFileDialog->ShowModal();
    
    if( OK==wxID_OK )
      miFileDialog->GetPaths(nombresArchivos);
    
	miFileDialog->Destroy();
    
	wxBeginBusyCursor();

	unsigned int Nimages = nombresArchivos.Count();

	for( unsigned int i=0; i<Nimages; i++ )	{
		a_fileName = nombresArchivos[i];
		nkObj3DViewer * miobj = new nkObj3DViewer(prv_wxAuiNotebook);
		miobj->Configure();
		miobj->prOpenFile(a_fileName);
		prv_wxAuiNotebook->AddPage(miobj, a_fileName,true );
	}
	wxEndBusyCursor();
}
//*****************************************************************************************
//		MENU -> OPEN -> VOLUME
//*****************************************************************************************
void nkNukak3D::prEventSaveVol(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
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
			wxString a_fileName;
			a_fileName.Empty();
			int OK = myFileDialog->ShowModal();
		    
			if( OK==wxID_OK )
			a_fileName = myFileDialog->GetPath();
		    
			myFileDialog->Destroy();
			if( a_fileName.IsEmpty() )
			{
				return;
			}
			wxBeginBusyCursor();
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prSaveFile(a_fileName);
			wxEndBusyCursor();
		}
	}
}
//*****************************************************************************************
//		MENU -> SAVE -> MESH 3D
//*****************************************************************************************
void nkNukak3D::prEventSaveMesh3D(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
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
			wxString a_fileName;
			a_fileName.Empty();
			int OK = myFileDialog->ShowModal();
		    
			if( OK==wxID_OK )
			a_fileName = myFileDialog->GetPath();
		    
			myFileDialog->Destroy();
			if( a_fileName.IsEmpty() )
			{
				return;
			}
			wxBeginBusyCursor();
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prSaveFile(a_fileName);
			wxEndBusyCursor();
		}
	}
}
//*****************************************************************************************
//		EVENT TREE
//*****************************************************************************************
void nkNukak3D::prEventTree(wxTreeEvent& event){
	wxCommandEvent mievento;
#ifdef __WIN32__
	wxString mi_item = wxT("NULL");
#else //MAC O UNIX
	wxString mi_item = prv_nkToolBarTools->GetItemText(event.GetItem());
#endif //__WIN32__
	if (mi_item == _("Activate/deactivate collision detection")) prEventPositionCamera(mievento);
	else if (mi_item == _("Area of Axial Image")) prEventCalcArea(mievento);
	else if (mi_item == _("Decimate Mesh 3D")) prEventFilprPolyDecimate(mievento);
	else if (mi_item == _("Deform")) prEventFilprPolyDeform(mievento);
	else if (mi_item == _("Dicom Directory")) prEventOpenVolumenDicom(mievento);
	else if (mi_item == _("Enable Stereoscopy Vision")) prEventprActiveStereo(mievento);
	else if (mi_item == _("Endoscopic camera")) prEventNavEndoscope(mievento);
	else if (mi_item == _("Flight")) prEventprNavFlight(mievento);
	else if (mi_item == _("Gaussian Filter")) prEventFilVolGaussian(mievento);
	else if (mi_item == _("Joystick")) prEventprNavJoystick(mievento);
	else if (mi_item == _("Less separation -")) prEventStLessSeparation(mievento);
	else if (mi_item == _("Median Filter")) prEventFilVolMedian(mievento);
	else if (mi_item == _("More separation +")) prEventStMoreSeparation(mievento);
	else if (mi_item == _("Normals")) prEventFilprPolyNormals(mievento);
	else if (mi_item == _("Polygon Mesh")) prEventOpenMesh3D(mievento);
	else if (mi_item == _("Reset Lookup Table")) prEventResetLookupTable(mievento);
	else if (mi_item == _("Segmentation with Levelsets")) prEventLevelSets(mievento);
	else if (mi_item == _("Reset Camera")) prEventprNavResetCamara(mievento);
	else if (mi_item == _("Scalar: Ortogonal planes")) prEventVolViewerRenderingEscalar(mievento);
	else if (mi_item == _("Smooth")) prEventFilprPolySmooth(mievento);
	else if (mi_item == _("Show/hide bounding box")) prEventprBoundingBox(mievento);
	else if (mi_item == _("Show/hide box widget")) prEventBoxWidget(mievento);
	else if (mi_item == _("Stereo passive")) prEventprStereoPassive(mievento);
	else if (mi_item == _("Trackball")) prEventprNavTrackball(mievento);
	else if (mi_item == _("Triangulated")) prEventFilprPolyTriangle(mievento);
	else if (mi_item == _("Unicam")) prEventprNavUnicam(mievento);
	else if (mi_item == _("Volume")) prEventOpenVolumen(mievento);
	else if (mi_item == _("3D: Marching Cubes")) prEventMarchingCubes(mievento);
	else if (mi_item == _("3D: Raycasting MIP")) prEventVolViewerRenderingMRCmip(mievento);
	else if (mi_item == _("3D: Raycasting composite")) prEventVolViewerRenderingMRCcomp(mievento);
	else if (mi_item == _("3D: Raycasting isosurface")) prEventVolViewerRenderingMRCiso(mievento);
	else if (mi_item == _("3D: Texture Mapping")) prEventVolViewerRenderingTextura(mievento);

	std::vector<std::string> lutNames = vtkLookupTableManager::GetAvailableLookupTables();
	int val = -1;
	for( unsigned int i=0; i<lutNames.size(); i++){
		if (mi_item == _(lutNames[i].c_str())) val = i;
	}
	if (val>-1){
		wxCommandEvent event(0, val +nkNukak3D::ID_LAST_LOOKUP_TABLE);
		prEventLookupTable(event);
	}
}

//*****************************************************************************************
//		SHOW/HIDE BOUNDING BOX
//*****************************************************************************************
void nkNukak3D::prEventprBoundingBox(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prBoundingBoxOnOff();
		}
		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prBoundingBoxOnOff();
		}
	}
}
//*****************************************************************************************
//		SHOW/HIDE BOX WIDGET
//*****************************************************************************************
void nkNukak3D::prEventBoxWidget(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->BoxWidgetOnOff();
		}
	}
}

//*****************************************************************************************
//		MENU -> LOOKUP TABLE -> RESET
//*****************************************************************************************
void nkNukak3D::prEventResetLookupTable(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			wxBeginBusyCursor();
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->reiniciarNiveleseDePaleta();
			wxEndBusyCursor();
		}
	}
}
//*****************************************************************************************
//		MENU -> LOOKUP TABLE -> SELECT
//*****************************************************************************************
void nkNukak3D::prEventLookupTable(wxCommandEvent& event){
	if (event.GetId() >= nkNukak3D::ID_LAST_LOOKUP_TABLE){
		if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
			int mi_pagina = prv_wxAuiNotebook->GetSelection();
			wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			if (pagina->GetName() == wxT("nkVolViewer")){
				nkVolViewer *page = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
				int val = event.GetId() - nkNukak3D::ID_LAST_LOOKUP_TABLE;
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
void nkNukak3D::prEventVolViewerRenderingEscalar(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			wxBeginBusyCursor();
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->cambiarFormaDeProcesamiento(vtkViewImage3D::PLANAR_RENDERING);
			wxEndBusyCursor();
		}
	}
}
//*****************************************************************************************
//		MENU -> VISUALIZACION DE VOLUMENES 3D -> RAY CASTING MIP
//*****************************************************************************************
void nkNukak3D::prEventVolViewerRenderingMRCmip(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			wxBeginBusyCursor();
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->cambiarFormaDeProcesamiento(vtkViewImage3D::VOLUME_RENDERING, 2);
			wxEndBusyCursor();
		}
	}
}
//*****************************************************************************************
//		MENU -> VISUALIZACION DE VOLUMENES 3D -> RAY CASTING COMPOSITE
//*****************************************************************************************
void nkNukak3D::prEventVolViewerRenderingMRCcomp(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			wxBeginBusyCursor();
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->cambiarFormaDeProcesamiento(vtkViewImage3D::VOLUME_RENDERING, 3);
			wxEndBusyCursor();
		}
	}
}
//*****************************************************************************************
//		MENU -> VISUALIZACION DE VOLUMENES 3D -> RAY CASTING ISOSURFACE
//*****************************************************************************************
void nkNukak3D::prEventVolViewerRenderingMRCiso(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			wxBeginBusyCursor();
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->cambiarFormaDeProcesamiento(vtkViewImage3D::VOLUME_RENDERING, 4);
			wxEndBusyCursor();
		}
	}
}
//*****************************************************************************************
//		MENU -> VISUALIZACION DE VOLUMENES 3D -> TEXTURE RENDERING
//*****************************************************************************************
void nkNukak3D::prEventVolViewerRenderingTextura(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			wxBeginBusyCursor();
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->cambiarFormaDeProcesamiento(vtkViewImage3D::VOLUME_RENDERING, 1);
			wxEndBusyCursor();
		}
	}
}
//*****************************************************************************************
//		MENU -> ISOSUPERFICIE -> MARCHING CUBES
//*****************************************************************************************
void nkNukak3D::prEventMarchingCubes(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));

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

				nkObj3DViewer * miobj = new nkObj3DViewer(prv_wxAuiNotebook);

				(miDlg->obtenerValor(0)).ToLong(&num_contornos);
				(miDlg->obtenerValor(1)).ToLong(&umbralInferior);
				(miDlg->obtenerValor(2)).ToLong(&umbralSuperior);

				miobj->Configure();
				miobj->prImageToIsoSurface(current->getImagen(),
					(int)num_contornos, 
					(int)umbralInferior,  
					(int)umbralSuperior,1.0,1.0,1.0,1.0);
				prv_wxAuiNotebook->AddPage(miobj, current->geta_fileName(),true );
				delete miDlg;

				wxEndBusyCursor();
			}
		}
	}
}
//*****************************************************************************************
//		MENU -> SEGMENTAtION -> LEVELSETS
//*****************************************************************************************
void nkNukak3D::prEventLevelSets(wxCommandEvent& WXUNUSED(event)){
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			//current->lsLevelsetsCompleto( nkNukak3D::getWxAuiNotebook() );			
			current->NuevoLevelSets(nkNukak3D::getWxAuiNotebook() );			
		}
	}
}

wxAuiNotebook * nkNukak3D::getWxAuiNotebook(void){
	return prv_wxAuiNotebook;
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> CALCULAR AREA AXIAL
//*****************************************************************************************
void nkNukak3D::prEventCalcArea(wxCommandEvent& WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			wxBeginBusyCursor();
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->Area();	
			wxEndBusyCursor();
		}
	}	

}
//*****************************************************************************************
//		MENU -> NAVIGATION -> RESET CAMERA
//*****************************************************************************************
void nkNukak3D::prEventprNavResetCamara(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prNavResetCamara();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer")) 
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prNavResetCamara();
		}
	}
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> TRACKBALL
//*****************************************************************************************
void nkNukak3D::prEventprNavTrackball(wxCommandEvent& WXUNUSED(event) )
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prNavTrackball();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer")) 
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prNavTrackball();
		}
	}
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> JOYSTICK
//*****************************************************************************************
void nkNukak3D::prEventprNavJoystick(wxCommandEvent& WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prNavJoystick();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer"))
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prNavJoystick();
		}
	}
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> FLIGHT
//*****************************************************************************************
void nkNukak3D::prEventprNavFlight(wxCommandEvent& WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prNavFlight();
		}
		else 
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prNavFlight();
		}
	}
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> UNICAM
//*****************************************************************************************
void nkNukak3D::prEventprNavUnicam(wxCommandEvent& WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prNavUnicam();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer"))
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prNavUnicam();
		}
	}
}
//*****************************************************************************************
//		ACTIVE STEREO
//*****************************************************************************************
void nkNukak3D::prEventprActiveStereo(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prActiveStereo();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer"))
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prActiveStereo();
		}
	}
}
//*****************************************************************************************
//		PASIVE STEREO
//*****************************************************************************************
void nkNukak3D::prEventprStereoPassive(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prStereoPassive();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer"))
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prStereoPassive();
		}
	}
}
//*****************************************************************************************
//		STEREO INCREASE - Increase stereo separation
//*****************************************************************************************
void nkNukak3D::prEventStMoreSeparation(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prStereoMoreSeparation();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer"))
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prStereoMoreSeparation();
		}
	}
}
//*****************************************************************************************
//		STEREO DECREASE - Decrease stereo separation
//*****************************************************************************************
void nkNukak3D::prEventStLessSeparation(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prStereoLessSeparation();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer"))
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prStereoLessSeparation();
		}
	}
}
//*****************************************************************************************
//		FILTER -> GAUSSIAN
//*****************************************************************************************
void nkNukak3D::prEventFilVolGaussian(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->FilVolGaussian( nkNukak3D::getWxAuiNotebook() );
		}
	}
}
//*****************************************************************************************
//		FILTER -> MEDIAN
//*****************************************************************************************
void nkNukak3D::prEventFilVolMedian(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->FilVolMedian( nkNukak3D::getWxAuiNotebook() );
		}
	}
}
//*****************************************************************************************
//		FILTER -> GRADIENT
//*****************************************************************************************
void nkNukak3D::prEventFilVolGradient(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->FilVolGradient();
		}
	}
}
//*****************************************************************************************
//		FILTER -> THRESHOLD
//*****************************************************************************************
void nkNukak3D::prEventFilVolThreshold(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->FilVolThreshold();
		}
	}
}

//*****************************************************************************************
//		FILTER -> TRIANGULATE
//*****************************************************************************************
void nkNukak3D::prEventFilprPolyTriangle(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prPolyTriangle();
		}
	}
}
//*****************************************************************************************
//		FILTER -> DECIMATE
//*****************************************************************************************
void nkNukak3D::prEventFilprPolyDecimate(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prPolyDecimate();
		}
	}
}
//*****************************************************************************************
//		FILTER -> SMOOTH
//*****************************************************************************************
void nkNukak3D::prEventFilprPolySmooth(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prPolySmooth();
		}
	}
}
//*****************************************************************************************
//		FILTER -> NORMALS
//*****************************************************************************************
void nkNukak3D::prEventFilprPolyNormals(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prPolyNormals();
		}
	}
}
//*****************************************************************************************
//		FILTER -> DEFORM
//*****************************************************************************************
void nkNukak3D::prEventFilprPolyDeform(wxCommandEvent &WXUNUSED(event)) 
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->prPolyDeform();
		}
	}
}
//*****************************************************************************************
//		CAPTURE SNAPSHOT 3D
//*****************************************************************************************
void nkNukak3D::prEventSnapshot3D(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));

		const wxString miWxFilter =	
			wxT ("Jpeg (*.jpg)|*.jpg|")
			wxT ("Bmp (*.bmp)|*.bmp|")			
			wxT ("Tiff (*.tif)|*.tif|");
		wxFileDialog * myFileDialog = new wxFileDialog(this, 
    										  _("Save as"),
                                              wxT(""), wxT(""),
                                              miWxFilter,
                                              wxFD_SAVE|wxFD_CHANGE_DIR|wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
		wxString a_fileName;
		a_fileName.Empty();
		int OK = myFileDialog->ShowModal();
	    
		if( OK==wxID_OK )
		a_fileName = myFileDialog->GetPath();
	    
		myFileDialog->Destroy();
		if( a_fileName.IsEmpty() )
		{
			return;
		}

		vtkWindowToImageFilter* l_w2i = vtkWindowToImageFilter::New();

		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			l_w2i = current->Snapshot();
		}
		else if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			l_w2i = current->Snapshot(0);
		}

		if( a_fileName.Mid(a_fileName.Length()-3,3)== "jpg" ){
			vtkJPEGWriter*	l_snapshot = vtkJPEGWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(a_fileName);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( a_fileName.Mid(a_fileName.Length()-3,3)== "bmp" ){
			vtkBMPWriter*	l_snapshot = vtkBMPWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(a_fileName);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( a_fileName.Mid(a_fileName.Length()-3,3)== "tif" ){
			vtkTIFFWriter*	l_snapshot = vtkTIFFWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(a_fileName);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}			
	}
} 
//*****************************************************************************************
//		CAPTURE SNAPSHOT AXIAL VIEW
//*****************************************************************************************
void nkNukak3D::prEventSnapshotAxial(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));

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
		wxString a_fileName;
		a_fileName.Empty();
		int OK = myFileDialog->ShowModal();
	    
		if( OK==wxID_OK )
		a_fileName = myFileDialog->GetPath();
	    
		myFileDialog->Destroy();
		if( a_fileName.IsEmpty() )
		{
			return;
		}

		vtkWindowToImageFilter* l_w2i = vtkWindowToImageFilter::New();					
		nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		l_w2i = current->Snapshot(1);

		 if( a_fileName.Mid(a_fileName.Length()-3,3)== "jpg" ){
			vtkJPEGWriter*	l_snapshot = vtkJPEGWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(a_fileName);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( a_fileName.Mid(a_fileName.Length()-3,3)== "bmp" ){
			vtkBMPWriter*	l_snapshot = vtkBMPWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(a_fileName);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( a_fileName.Mid(a_fileName.Length()-3,3)== "tif" ){
			vtkTIFFWriter*	l_snapshot = vtkTIFFWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(a_fileName);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}			
	}
} 

//*****************************************************************************************
//		CAPTURE SNAPSHOT CORONAL VIEW
//*****************************************************************************************
void nkNukak3D::prEventSnapshotCoronal(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));

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
		wxString a_fileName;
		a_fileName.Empty();
		int OK = myFileDialog->ShowModal();
	    
		if( OK==wxID_OK )
		a_fileName = myFileDialog->GetPath();
	    
		myFileDialog->Destroy();
		if( a_fileName.IsEmpty() )
		{
			return;
		}

		vtkWindowToImageFilter* l_w2i = vtkWindowToImageFilter::New();					
		nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		l_w2i = current->Snapshot(2);

		 if( a_fileName.Mid(a_fileName.Length()-3,3)== "jpg" ){
			vtkJPEGWriter*	l_snapshot = vtkJPEGWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(a_fileName);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( a_fileName.Mid(a_fileName.Length()-3,3)== "bmp" ){
			vtkBMPWriter*	l_snapshot = vtkBMPWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(a_fileName);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( a_fileName.Mid(a_fileName.Length()-3,3)== "tif" ){
			vtkTIFFWriter*	l_snapshot = vtkTIFFWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(a_fileName);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}			
	}
}
//*****************************************************************************************
//		CAPTURE SNAPSHOT SAGITAL VIEW
//*****************************************************************************************
void nkNukak3D::prEventSnapshotSagital(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));

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
		wxString a_fileName;
		a_fileName.Empty();
		int OK = myFileDialog->ShowModal();
	    
		if( OK==wxID_OK )
		a_fileName = myFileDialog->GetPath();
	    
		myFileDialog->Destroy();
		if( a_fileName.IsEmpty() )
		{
			return;
		}

		vtkWindowToImageFilter* l_w2i = vtkWindowToImageFilter::New();					
		nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		l_w2i = current->Snapshot(3);

		 if( a_fileName.Mid(a_fileName.Length()-3,3)== "jpg" ){
			vtkJPEGWriter*	l_snapshot = vtkJPEGWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(a_fileName);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( a_fileName.Mid(a_fileName.Length()-3,3)== "bmp" ){
			vtkBMPWriter*	l_snapshot = vtkBMPWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(a_fileName);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}
		else if( a_fileName.Mid(a_fileName.Length()-3,3)== "tif" ){
			vtkTIFFWriter*	l_snapshot = vtkTIFFWriter::New();
			l_snapshot->SetInputConnection(l_w2i->GetOutputPort());
			l_snapshot->SetFileName(a_fileName);
			l_snapshot->Write();
			l_snapshot->Delete();
			l_w2i->Delete();
		}			
	}
}
//*****************************************************************************************
//		IMAGE INFORMATION
//*****************************************************************************************
void nkNukak3D::prEventInformationImage(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));

		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));

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
void nkNukak3D::prEventInformationPolygon(wxCommandEvent &WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));

		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			
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
void nkNukak3D::prEventInformationVideoCard(wxCommandEvent &WXUNUSED(event))
{	
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		wxString l_text;

		if (pagina->GetName() == wxT("nkObj3DViewer")){
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
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
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
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
//*****************************************************************************************
//		LANGUAGE SELECTION
//*****************************************************************************************
void nkNukak3D::prEventChangeLanguage(wxCommandEvent &WXUNUSED(event)){
	wxDialog * mi_dialogo = new wxDialog(this, wxID_ANY, _("Nukak3D: Choose language"), wxDefaultPosition, wxSize(250,170));
	wxStaticText * mi_text = new wxStaticText(mi_dialogo, -1, _("Choose language"), wxPoint(20,20));
	wxString choices[10];
	choices[0] = _("English");
	choices[1] = _("Spanish");
	wxChoice * mi_choise = new wxChoice(mi_dialogo, -1, wxPoint(20,50), wxDefaultSize, 2, choices);
	wxButton * mi_botonOk = new wxButton(mi_dialogo, wxID_OK, _("OK"),wxPoint(40,90));	
	wxButton * mi_botonCancel = new wxButton(mi_dialogo, wxID_CANCEL, _("Cancel"),wxPoint(130,90));
	CenterOnParent();
	mi_dialogo->ShowModal();
	int mi_language = wxLANGUAGE_ENGLISH ;
	bool writeConfig = false;
	if(mi_dialogo->GetReturnCode() == wxID_OK){
		wxBeginBusyCursor();
		mi_language = mi_choise->GetSelection();
		if (mi_language != wxNOT_FOUND){
			writeConfig = true;
			if(mi_language == 0){ //English
				mi_language = wxLANGUAGE_ENGLISH ;

			}else if(mi_language == 1){ //Spanish
				mi_language = wxLANGUAGE_SPANISH;
			}
		}
		wxEndBusyCursor();
	}
	delete mi_text;
	delete mi_choise;
	delete mi_botonOk;
	delete mi_botonCancel;
	delete mi_dialogo;
	wxConfigBase *mi_Config = wxConfigBase::Get();
	if ( mi_Config != NULL && writeConfig == true){
			// save the language's values to the config
			mi_Config->Write(_T("/Tools/Language"), mi_language);
			wxMessageDialog(this,_("Restart program to apply language configuration")).ShowModal();
	}
}

//*****************************************************************************************
//		Mostrar/guardar posiciones de la camara con colisiones
//*****************************************************************************************
void nkNukak3D::prEventPositionCamera(wxCommandEvent& WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));

		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->CameraPos();				
		}

	}

}

//*****************************************************************************************
//		Interactor para camara endoscopica 
//*****************************************************************************************
void nkNukak3D::prEventNavEndoscope(wxCommandEvent& WXUNUSED(event))
{

	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->NavEndoscope();
		}
		//else  if (pagina->GetName() == wxT("nkObj3DViewer")) 
		//{
		//	nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		//	current->NavEndoscope();
		//}
	}
}

//*****************************************************************************************
//		Frames per second
//*****************************************************************************************
void nkNukak3D::prEventFPS(wxCommandEvent& WXUNUSED(event))
{
	if ((int)prv_wxAuiNotebook->GetPageCount() > 0){
		int mi_pagina = prv_wxAuiNotebook->GetSelection();
		wxWindow * pagina = prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
		if (pagina->GetName() == wxT("nkVolViewer")){
			nkVolViewer *current = (nkVolViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->FPS();
		}
		else  if (pagina->GetName() == wxT("nkObj3DViewer")) 
		{
			nkObj3DViewer *current = (nkObj3DViewer*)prv_wxAuiNotebook->GetPage(size_t( mi_pagina));
			current->FPS();
		}
	}
}

void nkNukak3D::prEventDicomListener(wxCommandEvent& WXUNUSED(event)){
	nkStoreSCP * my_listener = nkStoreSCP::getInstance(this);
	my_listener->ShowModalAndLog();
}

void nkNukak3D::prEventDicomFind(wxCommandEvent& WXUNUSED(event)){
	nkFindSCU * my_find = new nkFindSCU(this, wxID_ANY,"Nukak3D:Find Patient's", wxDefaultPosition, wxSize(800,600));
	//my_find->ShowModal();
	prv_wxAuiNotebook->AddPage(my_find, "apaasd",true );

}