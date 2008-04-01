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
#include <vtkOutputWindow.h>
#include <vtkWindowToImageFilter.h>
#include <vtkBMPWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkTIFFWriter.h>
#include "vtkLookupTableManager.h"

/** nk*/
#include "nkObj3DViewer.h"
#include "nkVolViewer.h"
#include "nkIODialog.h"
#include "nkAcercaDe.h"

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
		ID_ABRIR_ARCHIVO = wxID_HIGHEST + 1500, /**< Open file. */
		ID_ABRIR_ARCHIVO_DICOM,			/**< Open stack of image Dicom. */
		ID_ABRIR_ARCHIVO_MALLA3D,		/**< Open object 3D. */
		ID_ABRIR_ARCHIVO_VOL,			/**< Open file vol. */
		ID_ACERCA_DE,					/**< Show Dialog About Nukak3d. */
		ID_ACERCA_DE_MAC,				/**< Show Dialog About Nukak3d (Only for Mac ) . */
		ID_ARBOL,						/**< Event launch by nkToolBarMac. */
		ID_AREA,						/**< Calc area . */
		ID_CERRAR,						/**< Close application. */
		ID_CERRAR_TODO,					/**< Close application. */
		ID_COLDET,						/**< Collision detection. */
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
		ID_GUARDARMALLA3D,				/**< Save object 3D. */
		ID_GUARDARVOL,					/**< Save vol. */
		ID_JOYSTICK,					/**< Input device to Joystick. */
		ID_LSLEVELSETSCOMPLETO,			/**< Level sets. */
		ID_MARCHING_CUBES,				/**< Surface reconstruction with Marching Cubes. */
		ID_NAVFLIGHT,					/**< Flight Camera. */
		ID_NAVJOYSTICK,					/**< Joystick Camera. */
		ID_NAVRESET,					/**< Reset camera position and orientation. */
		ID_NAVTRACKBALL,				/**< Trackball Camera. */
		ID_NAVUNICAM,					/**< Unicam Camera. */
		ID_PALETA_COLOR,				/**< lookup table. */
		ID_PARIMAGE,					/**< Image information. */
		ID_PARPOLYGON,					/**< Object 3D information. */
		ID_PARVIDEO,					/**< VideoCard information. */
		ID_REINICIAR_PALETA,			/**< Reset window and level. */
		ID_SALIR,						/**< Close application. */
		ID_SETLANGUAGE,                 /**< Change user language. */
		ID_SNAPSHOT3D,					/**< Snapshot canvas 3D. */
		ID_SNAPSHOTAXIAL,				/**< Snapshot axial view. */
		ID_SNAPSHOTCORONAL,				/**< Snapshot coronal view. */
		ID_SNAPSHOTSAGITAL,				/**< Snapshot sagital view. */
		ID_STACTIVO,					/**< Enable stereoscopic vision. */
		ID_STAUMENTAR,					/**< Stereoscopic vision more separation. */
		ID_STDISMINUIR,					/**< Stereoscopic vision less separation. */
		ID_STPASIVO,					/**< Activar desactivar visualizacion estereoscopica. */
		ID_VOLVIEWER_RENDERING_ESCALAR,	/**< Ortogonal planes view. */
		ID_VOLVIEWER_RENDERING_MRC,		/**< Ray cast rendering. */
		ID_VOLVIEWER_RENDERING_TEXTURA,	/**< Texture mpping rendering. */
		ID_ULTIMO = wxID_HIGHEST+3000	/**< Event's for lookup table. */
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
	~nkNukak3D();

	/**
	 * @brief Insert Toolbar Botton.
	 * @param window nkToolbar to insert.
	 * @param un_nombre nkToolbar title.
	 * @param una_etiqueta Label for this control.
	*/
	void insertarToolBar(wxWindow* window, wxString un_nombre, wxString una_etiqueta);

	/**
	 * @brief Open volume of images store in unique file.
	*/
	void eventoAbrirVolumen(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Open Dicom directories.
	*/
	void eventoAbrirVolumenDicom(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Open object 3D (vtk structured grid).
	*/
	void eventoAbrirMalla3D(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Open about nkNukak3D.
	*/
	void eventoAcercaDe(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Open about nkNukak3D for Mac.
	*/
	void eventoAcercaDeMAC(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief .nkToolbar event's for Mac and unix systems.
	*/
	void eventoArbol(wxTreeEvent& event);

	/**
	 * @brief Save volume.
	*/
	void eventoGuardarVol(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Save object 3D in vtk structured grid file.
	*/
	void eventoGuardarMalla3D(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Change lookup table for rendering volume.
	*/
	void eventoPaletaColor(wxCommandEvent& event);

	/**
	 * @brief Reset window and level.
	*/
	void eventoReiniciarPaleta(wxCommandEvent& WXUNUSED(event));
	
	/**
	 * @brief Close application.
	*/
	void eventoSalir(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief View ortogonal planes rendering.
	*/
	void eventoVolViewerRenderingEscalar(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Texture maping rendering.
	*/
	void eventoVolViewerRenderingTextura(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Ray cast rendering.
	*/
	void eventoVolViewerRenderingMRC(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Marching cubes event.
	*/
	void eventoMarchingCubes(wxCommandEvent& WXUNUSED(event));
	
	/**
	 * @brief Segmentation with levelsets
	*/
	void eventolsLevelsetsCompleto(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Event for calc area.
	*/
	void eventoArea(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Reset position and orientatio camera.
	*/
	void eventoNavResetCamara(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Trackball camera.
	 * @details Navigation by default.
	*/
	void eventoNavTrackball(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Joystick camera.
	*/
	void eventoNavJoystick(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Flight camera.
	*/
	void eventoNavFlight(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Unicam camera.
	*/
	void eventoNavUnicam(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Enable/Disable stereoscopic vision.
	*/
	void eventoStActivo(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Enable/Disable passive stereo.
	*/
	void eventoStPasivo(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief More separation for images in stereoscopic vision.
	*/
	void eventoStAumentar(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Less separation for images in stereoscopic vision.
	*/
	void eventoStDisminuir(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Gaussian filter.
	*/
	void eventoFilVolGaussian(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Median filter.
	*/
	void eventoFilVolMedian(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Gradient filter.
	*/
	void eventoFilVolGradient(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Threshold filter.
	*/
	void eventoFilVolThreshold(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Triangle mesh.
	*/
	void eventoFilPolyTriangle(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Decimate mesh.
	*/
	void eventoFilPolyDecimate(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Smooth mesh.
	*/
	void eventoFilPolySmooth(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Recalc normals.
	*/
	void eventoFilPolyNormals(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Deform mesh.
	*/
	void eventoFilPolyDeform(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Snapshot 3d view.
	*/
	void eventoSnapshot3D(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Snapshot axial view.
	*/
	void eventoSnapshotAxial(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Snapshot axial sagital.
	*/
	void eventoSnapshotSagital(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Snapshot axial coronal.
	*/
	void eventoSnapshotCoronal(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Show information of volume.
	*/
	void eventoParImage(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Show information of mesh.
	*/
	void eventoParPolygon(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Show information of video card.
	*/
	void eventoParVideo(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Change the user preferences, language.
	*/
	void eventoLanguage(wxCommandEvent& WXUNUSED(event));

	/**
	 * @brief Get notebok to make other pages.
	 * @return wxAuiNotebook *
	*/
	wxAuiNotebook * getNkNotebook(void);

private:
	wxAuiNotebook * prv_libro;		//! notebook for manage pages.
	wxAuiManager prv_auiManager;	//! Administrator for Aui.
	nkToolBar *  mi_nkHerramientas;	//! nkToolBar for tools.
	nkToolBar *  mi_nkImageViewer;	//! nkToolBar for image tools.
        
	// Joystick values
	wxJoystick* m_stick;			//! Joystick/Gamepad.
    int     m_minX;					//! Min value x of Joystick/Gamepad.
    int     m_minY;					//! Min value y of Joystick/Gamepad.
	int     m_minZ;					//! Min value z of Joystick/Gamepad.
    int     m_maxX;					//! Max value x of Joystick/Gamepad.
    int     m_maxY;					//! Max value y of Joystick/Gamepad.
	int     m_maxZ;					//! Max value z of Joystick/Gamepad.
	int		nButtons;				//! numbers of buttons of Joystick/Gamepad.
	int		nAxis;					//! numbers of axes of Joystick/Gamepad.
	unsigned int xpos;				//! Actual position of Joystick/Gamepad.
	unsigned int ypos;				//! Actual position of Joystick/Gamepad.

	DECLARE_EVENT_TABLE()			//! Call macro for declaration of events.
};


#endif //_NKNUKAK3D_H_