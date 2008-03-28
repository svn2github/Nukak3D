/** 
 * @file nkObj3DViewer.h
 * @brief Class for view mesh.
 * @author Alexander Pinzón Fernandez, Byron Pérez
 * @version 0.2
 * @date 27/12/2007 03:37 p.m.
*/
#ifndef _NKOBJ3DVIEWER_H_
#define _NKOBJ3DVIEWER_H_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

/** vtk*/
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataReader.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleJoystickCamera.h>
#include <vtkInteractorStyleFlight.h>	
#include <vtkInteractorStyleUnicam.h>
#include <vtkPolyDataWriter.h>
#include <vtkTriangleFilter.h>
#include <vtkOutputWindow.h>
#include <vtkDecimatePro.h>
#include <vtkPolyDataNormals.h>
#include <vtkWindowToImageFilter.h>
#include <vtkTextActor.h>
#include <vtkCylinderSource.h>
#include <vtkInteractorStyleTrackballActor.h>

/** ITK*/
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageToVTKImageFilter.h>
#include <itkAnalyzeImageIOFactory.h>

/** INRIA3D*/
#include <vtkObject.h>
#include <vtkSynchronizedView.h>
#include <vtkViewImage.h>
#include <vtkViewImage2DWithTracer.h>
#include <vtkViewImage3D.h>
#include <wxVTKRenderWindowInteractor.h>

/** wx*/
#include "wx/aui/aui.h"

/** nk*/
#include "nkIODialog.h"

#define mensajes 1

/** 
 * @brief Class for view mesh.
 */
class nkObj3DViewer: public wxPanel{
 public:

	/**
	 * @brief Class constructor.
	 */
	nkObj3DViewer(wxWindow* parent, 
		wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTAB_TRAVERSAL|wxBORDER_NONE, 
		const wxString& name = "nkObj3DViewer");

	/**
	 * @brief Class destructor.
	*/	
	~nkObj3DViewer();
	
	/**
	 * @brief Config view.
	*/
	void Configurar(void);

	/**
	 * @brief Open file.
	 * @param nombreArchivo Filepath of mesh.
	*/
	void abrirArchivo(wxString nombreArchivo);

	/**
	 * @brief Save mesh.
	 * @param nombreArchivo Filepath of file.
	*/
	void guardarArchivo(wxString nombreArchivo);

	/**
	 * @brief Config view 3d.
	 * @param input Object to view.
	*/
	void configurarMalla3D(vtkPolyData* input);

	/**
	 * @brief Marching cubes function.
	 * @param una_imagen Volume to transform.
	 * @param un_numContornos Numbers of contours.
	 * @param un_umbral_inferior
	 * @param un_umbral_superior
	 * @param un_rojo
	 * @param un_verde
	 * @param un_azul
	 * @param una_opacidad
	*/
	void imagenAIsoSurface(itk::Image<unsigned short,3>::Pointer una_imagen,
									  int un_numContornos = 20, 
									  int un_umbral_inferior = 0,
									  int un_umbral_superior = 100,
									  double un_rojo = 1.0,
									  double un_verde = 1.0,
									  double un_azul = 1.0,
									  double una_opacidad = 1.0
									  );
	
	/**
	 * @brief Enable/Diable stereoscopy vision.
	*/
	void StActivo(void);

	/**
	 * @brief Enable/Diable passive stereo.
	*/
	void StPasivo(void);

	/**
	 * @brief More separation of images in stereoscopy vision.
	*/
	void StAumentar(void);

	/**
	 * @brief Less separation of images in stereoscopy vision.
	*/
	void StDisminuir(void);

	/**
	 * @brief Reset position and orientation of camera.
	*/
	void NavResetCamara(void);

	/**
	 * @brief Trackball camera.
	*/
	void NavTrackball(void);

	/**
	 * @brief Joystick camera.
	*/
	void NavJoystick(void);

	/**
	 * @brief Flight camera.
	*/
	void NavFlight(void);

	/**
	 * @brief unicam camera.
	*/
	void NavUnicam(void);

	/**
	 * @brief Triangle mesh.
	*/
	void PolyTriangle( void );

	/**
	 * @brief Decimate mesh.
	*/
	void PolyDecimate( void );

	/**
	 * @brief Smooth mesh.
	*/
	void PolySmooth( void );

	/**
	 * @brief Recalc normals.
	*/
	void PolyNormals( void );

	/**
	 * @brief Deform mesh.
	*/
	void PolyDeform( void );

	/**
	 * @brief Snapshot view.
	*/
	vtkWindowToImageFilter* Snapshot(void);

	/**
	 * @brief Get polydata
	 * @return vtkPolyData of objet in view 3D.
	*/
	vtkPolyData* GetPolyData(void);

	/**
	 * @brief Video card information.
	 * @return wxString
	*/
	wxString VideoCard(void);	

private:
	wxAuiManager					prv_auiManager;		//! Administrator for Aui.

	wxVTKRenderWindowInteractor*	prv_wxVtkVista3D;	//! wxVTKInteractor of view 3D.
	vtkRenderer*					prv_render3D;		//! Render of view 3D.
	vtkCamera*						prv_camera;			//! Camera.
	vtkViewImage3D*					prv_vista3D;		//! View 3D
	
	vtkActor*						prv_actor;			//! Actor for mesh.
	vtkPolyDataMapper*				prv_mapper;			//! Mapper of mesh.
	vtkPolyData*					prv_data;			//! Data of mesh.
	
	
	vtkCylinderSource*				prv_endoscope;		//! Rigid endoscope
	vtkActor*						prv_endoActor;		//! endoscope vtkActor
};

#endif //_NKOBJ3DVIEWER_H_