/** 
 * @file nkNukak3D.h
 * @brief Main window.
 * @details Gui implementation 
 * @author Alexander Pinzón Fernandez, Byron Pérez
 * @version 0.2
 * @date 27/12/2007 03:37 p.m.
*/

#ifndef _NKNUKAK3D_H_
#define _NKNUKAK3D_H_

#include "vtkINRIA3DConfigure.h"
#include  "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include  "wx/wx.h"
#endif

/** wx */
#include <wx/aui/aui.h>
#include <wx/aui/auibook.h>
#include <wx/config.h>
#include <wx/imaglist.h>
#include <wx/intl.h>
#include <wx/joystick.h>
#ifndef __WXMSW__
    #include "mondrian.xpm"
#endif
#include "main.xpm"

/** vtk*/
#include <vtkBMPWriter.h>
#include <vtkJPEGWriter.h>
#include "vtkLookupTableManager.h"
#include <vtkOutputWindow.h>
#include <vtkTIFFWriter.h>
#include <vtkWindowToImageFilter.h>

/** nk*/
#include "nkObj3DViewer.h"
#include "nkVolViewer.h"
#include "nkIODialog.h"
#include "nkAcercaDe.h"
#include "nkKernel.h"
#include "nkAdminPluginGui.h"

//#ifdef __WXMAC__
#ifdef __WIN32__
	#include "nkToolBar.h"
#else // mac or unix
	#include "nkToolBarMac.h"
#endif //__WIN32__

#define mensajes 1

/**
 * @brief Main window.
 * @details Gui implementation 
*/
class nkNukak3D: public wxFrame{
public:

	/** 
	 * Event identifier.
	*/
	enum{
		ID_OPEN_FILE = wxID_HIGHEST + 1500, /**< Open file. */
		ID_OPEN_FILE_DICOM,				/**< Open stack of image Dicom. */
		ID_OPEN_FILE_MESH3D,			/**< Open object 3D. */
		ID_OPEN_FILE_VOL,				/**< Open file vol. */
		ID_ABOUT,						/**< Show Dialog About Nukak3d. */
		ID_ABOUT_MAC,					/**< Show Dialog About Nukak3d (Only for Mac ) . */
		ID_ADMIN_PLUGIN_PATH,			/**< Admin plugin's paths. */
		ID_TREE,						/**< Event launch by nkToolBarMac. */
		ID_AREA,						/**< Calc area . */
		ID_prBoundingBox,					/**< Show/hide bounding box. */
		ID_BOXWIDGET,					/**< Show/hide box widget. */
		ID_CAMERAPOS,					/**< Show position of camera. */
		ID_CAMPLANES,					/**< Update planes with position of camera. */
		ID_CLOSE,						/**< Close application. */
		ID_CLOSE_ALL,					/**< Close application. */
		ID_COLDET,						/**< Collision detection. */
		ID_DICOMFIND,					/**< Dicom C-FIND. */
		ID_DICOMSERVER,					/**< Dicom listener SCP. */
		ID_ENDOCAMOBB,					/**< Enable virtual endoscopy. */
		ID_ENDOCAM,						/**< Enable Virtual endoscopy. */
		ID_FILPOLYDECIMATE,				/**< To reduce numbers of polygon. */
		ID_FILPOLYDEFORM,				/**< Deform Mesh. */
		ID_FILPOLYNORMALS,				/**< Recalc normals of faces. */
		ID_FILPOLYSMOOTH,				/**< Smooth mesh. */
		ID_FILPOLYTRIANGLE,				/**< Triangle mesh. */
		ID_FILVOLGAUSSIAN,				/**< Gaussian filter. */
		ID_FILVOLGRADIENT,				/**< Gradient filter. */
		ID_FILVOLMEDIAN,				/**< Median filter. */
		ID_FILVOLTHRESHOLD,				/**< Threshold filter. */
		ID_SAVE_MESH3D,					/**< Save object 3D. */
		ID_SAVE_VOL,					/**< Save vol. */
		ID_JOYSTICK,					/**< Input device to Joystick. */
		ID_LSLEVELSETSCOMPLETO,			/**< Level sets. */
		ID_MARCHING_CUBES,				/**< Surface reconstruction with Marching Cubes. */
		ID_NAVENDOSCOPE,				/**< Mode of navigation camera endoscopy. */
		ID_NAVFLIGHT,					/**< Flight Camera. */
		ID_NAVJOYSTICK,					/**< Joystick Camera. */
		ID_NAVRESET,					/**< Reset camera position and orientation. */
		ID_NAVTRACKBALL,				/**< Trackball Camera. */
		ID_NAVUNICAM,					/**< Unicam Camera. */
		ID_LOOKUP_TABLE,				/**< lookup table. */
		ID_INFORMATION_IMAGE,			/**< Image information. */
		ID_INFORMATION_POLYGON,			/**< Object 3D information. */
		ID_INFORMATION_VIDEO_CARD,		/**< VideoCard information. */
		ID_RESET_LOOKUP_TABLE,			/**< Reset window and level. */
		ID_EXIT,						/**< Close application. */
		ID_SETLANGUAGE,                 /**< Change user language. */
		ID_SNAPSHOT3D,					/**< Snapshot canvas 3D. */
		ID_SNAPSHOTAXIAL,				/**< Snapshot axial view. */
		ID_SNAPSHOTCORONAL,				/**< Snapshot coronal view. */
		ID_SNAPSHOTSAGITAL,				/**< Snapshot sagital view. */
		ID_STEREO_ACTIVE,				/**< Enable stereoscopic vision. */
		ID_STEREO_MORE_SEPARATION,		/**< Stereoscopic vision more separation. */
		ID_STEREO_LESS_SEPARATION,		/**< Stereoscopic vision less separation. */
		ID_STEREO_PASSIVE,				/**< Active stereoscopy vision. */
		ID_VOLVIEWER_RENDERING_ESCALAR,	/**< Ortogonal planes view. */
		ID_VOLVIEWER_RENDERING_MRC_MIP,	/**< Ray Tracing MIP. */
		ID_VOLVIEWER_RENDERING_MRC_COMP,/**< Ray Tracing COMPOSITE. */
		ID_VOLVIEWER_RENDERING_MRC_ISO,	/**< Ray Tracing ISOSURFACE. */
		ID_VOLVIEWER_RENDERING_TEXTURE,	/**< Texture mpping rendering. */
		ID_FPS,							/**< Frames per second. */
		ID_LAST_LOOKUP_TABLE = wxID_HIGHEST+3000,	/**< Event's for lookup table. */
		ID_NK_ITK_PLUGIN_FILTER  = wxID_HIGHEST+3200
	};

	/**
	 * @brief Class constructor.
	 * @details Main window.
	*/
	nkNukak3D(wxWindow* parent, int id=-1,
		const wxString& title="Nukak3D",
        const wxPoint& pos=wxDefaultPosition,
        const wxSize& size=wxDefaultSize,
        long style=wxDEFAULT_FRAME_STYLE
		);
	/**
	 * @brief Class destructor.
	*/
	virtual ~nkNukak3D();

	/**
	 * @brief Insert Toolbar Botton.
	 * @param window nkToolbar to insert.
	 * @param a_name nkToolbar title.
	 * @param a_label Label for this control.
	*/
	void prInsertToolBar(wxWindow* window, wxString a_name, wxString a_label);

	/**
	 * @brief Open volume of images store in unique file.
	*/
	void prEventOpenVolumen(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Open Dicom directories.
	*/
	void prEventOpenVolumenDicom(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Open Dicom directories.
	*/
	void prOpenVolumenDicom(wxString pathDicom);

	/**
	 * @brief Open object 3D (vtk structured grid).
	*/
	void prEventOpenMesh3D(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Open about nkNukak3D.
	*/
	void prEventAbout(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Open about nkNukak3D for Mac.
	*/
	void prEventAboutMAC(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief .nkToolbar event's for Mac and unix systems.
	*/
	void prEventTree(wxTreeEvent& event);

	/**
	 * @brief Save volume.
	*/
	void prEventSaveVol(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Save object 3D in vtk structured grid file.
	*/
	void prEventSaveMesh3D(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Show/hide bounding box
	*/
	void prEventprBoundingBox(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Show/hide box widget
	*/
	void prEventBoxWidget(wxCommandEvent& WXUNUSED(event));
	
	/**
	 * @brief Change lookup table for rendering volume.
	*/
	void prEventLookupTable(wxCommandEvent& event);

	/**
	 * @brief Reset window and level.
	*/
	void prEventResetLookupTable(wxCommandEvent& WXUNUSED(event));
	
	/**
	 * @brief Close application mene.
	*/
	void prEventExit(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Close application.
	*/
	void prOnClose(wxCloseEvent & event);

	/**
	 * @brief View ortogonal planes rendering.
	*/
	void prEventVolViewerRenderingEscalar(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Texture maping rendering.
	*/
	void prEventVolViewerRenderingTextura(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Visualiza el volumen 3D usando el algoritmo ray casting
	 * MIP - Maximum Intensity Projection.
	 * La opacidad se cambia variando la ventana (width and level) de la imagen
	*/
	void prEventVolViewerRenderingMRCmip(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Visualiza el volumen 3D usando el algoritmo ray casting.
	 * COMPOSITE
	 * La opacidad se cambia variando la ventana (width and level) de la imagen
	*/
	void prEventVolViewerRenderingMRCcomp(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Visualiza el volumen 3D usando el algoritmo ray casting.
	 * ISOSURFACE
	 * La opacidad se cambia variando la ventana (width and level) de la imagen
	*/
	void prEventVolViewerRenderingMRCiso(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Marching cubes event.
	*/
	void prEventMarchingCubes(wxCommandEvent& WXUNUSED(event));
	
	/**
	 * @brief Segmentation with levelsets
	*/
	void prEventLevelSets(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Event for calc area.
	*/
	void prEventCalcArea(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Reset position and orientatio camera.
	*/
	void prEventprNavResetCamara(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Trackball camera.
	 * @details Navigation by default.
	*/
	void prEventprNavTrackball(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Joystick camera.
	*/
	void prEventprNavJoystick(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Flight camera.
	*/
	void prEventprNavFlight(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Unicam camera.
	*/
	void prEventprNavUnicam(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Enable/Disable stereoscopic vision.
	*/
	void prEventprActiveStereo(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Enable/Disable passive stereo.
	*/
	void prEventprStereoPassive(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief More separation for images in stereoscopic vision.
	*/
	void prEventStMoreSeparation(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Less separation for images in stereoscopic vision.
	*/
	void prEventStLessSeparation(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Gaussian filter.
	*/
	void prEventFilVolGaussian(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Median filter.
	*/
	void prEventFilVolMedian(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Gradient filter.
	*/
	void prEventFilVolGradient(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Threshold filter.
	*/
	void prEventFilVolThreshold(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Triangle mesh.
	*/
	void prEventFilprPolyTriangle(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Decimate mesh.
	*/
	void prEventFilprPolyDecimate(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Smooth mesh.
	*/
	void prEventFilprPolySmooth(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Recalc normals.
	*/
	void prEventFilprPolyNormals(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Deform mesh.
	*/
	void prEventFilprPolyDeform(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Snapshot 3d view.
	*/
	void prEventSnapshot3D(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Snapshot axial view.
	*/
	void prEventSnapshotAxial(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Snapshot axial sagital.
	*/
	void prEventSnapshotSagital(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Snapshot axial coronal.
	*/
	void prEventSnapshotCoronal(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Show information of volume.
	*/
	void prEventInformationImage(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Show information of mesh.
	*/
	void prEventInformationPolygon(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Show information of video card.
	*/
	void prEventInformationVideoCard(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Change the user preferences, language.
	*/
	void prEventChangeLanguage(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Get notebok to make other pages.
	 * @return wxAuiNotebook *
	*/
	wxAuiNotebook * getWxAuiNotebook(void);
	
	/**
	 * @brief Active a joystick/gamepad for navigation
	*/
	void prEventActiveJoystick(wxCommandEvent& WXUNUSED(event));
	
	/**
	 * @brief Show/save camera's position
	*/
	void prEventPositionCamera(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Navigation type endoscopy
	 * @details Mode of navigation type endoscopy
	*/
	void prEventNavEndoscope(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Frames per second
	 * @details Show frames per second of this render
	*/
	void prEventFPS(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Dicom SCP Listener
	 * @details Dicom SCP Listener, bassed on OFFIS DICOM storescp server.
	*/
	void prEventDicomListener(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Dicom C-FIND
	 * @details Dialog for search in Dicom server with C-Find option.
	*/
	void prEventDicomFind(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Admin Plugin's paths
	 * @details Dialog for admin plugin's paths.
	*/
	void prEventAdminPluginsPaths(wxCommandEvent& WXUNUSED(event));


	nukak3d::nkKernel & getNukakKernel(void);
private:
	wxAuiNotebook * prv_wxAuiNotebook;		//! notebook for manage pages.
	wxAuiManager prv_auiManager;	//! Administrator for Aui.
	nkToolBar *  prv_nkToolBarTools;	//! nkToolBar for tools.
	nkToolBar *  prv_nkToolBarImageViewer;	//! nkToolBar for image tools.
	nukak3d::nkKernel prv_nkKernel;

	DECLARE_EVENT_TABLE()			//! Call macro for declaration of events.
};


#endif //_NKNUKAK3D_H_