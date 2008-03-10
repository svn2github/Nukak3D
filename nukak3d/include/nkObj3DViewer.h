/** 
 * @file nkObj3DViewer.h
 * @brief Clase para visualizar mallas poligonales
 * @details Implementa todas las funciones para visualización de
 * mallas poligonales y filtros
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
 * @brief Clase para visualizar mallas poligonales
 * @details Implementa todas las funciones para visualización de
 * mallas poligonales y filtros
 */
class nkObj3DViewer: public wxPanel{
 public:

	/**
	 * @brief Constructor de la clase
	 */
	nkObj3DViewer(wxWindow* parent, 
		wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTAB_TRAVERSAL|wxBORDER_NONE, 
		const wxString& name = "nkObj3DViewer");

	/**
	 * @brief Destructor de la clase
	*/	
	~nkObj3DViewer();
	
	/**
	 * @brief Configurar la vista de la malla 3D
	*/
	void Configurar(void);

	/**
	 * @brief Abrir archivo de malla poligonal
	 * @param nombreArchivo wxString con el nombre y ruta del archivo
	*/
	void abrirArchivo(wxString nombreArchivo);

	/**
	 * @brief Guardar archivo de malla poligonal
	 * @param nombreArchivo wxString con el nombre y ruta del archivo
	*/
	void guardarArchivo(wxString nombreArchivo);

	/**
	 * @brief Configurar la vista de la malla 3D
	 * @param input apuntador con el vtkPolyData* que se quiere visualizar
	*/
	void configurarMalla3D(vtkPolyData* input);

	/**
	 * @brief Algoritmo marching cubes para generar una isosuperficie poligonal
	 * @param una_imagen apuntador al volumen 3D al que se le quiere aplicar el algoritmo
	 * @param un_numContornos cantidad de contornos
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
	 * @brief Activar/Desactivar estéreo activo
	*/
	void StActivo(void);

	/**
	 * @brief Activar/Desactivar estéreo pasivo
	*/
	void StPasivo(void);

	/**
	 * @brief Aumentar distancia entre ojos para el modo estéreo
	*/
	void StAumentar(void);

	/**
	 * @brief Disminuir distancia entre ojos para el modo estéreo
	*/
	void StDisminuir(void);

	/**
	 * @brief Resetear posición de la cámara
	*/
	void NavResetCamara(void);

	/**
	 * @brief Modo de navegación trackball
	*/
	void NavTrackball(void);

	/**
	 * @brief Modo de navegación joystick
	*/
	void NavJoystick(void);

	/**
	 * @brief Modo de navegación flight
	*/
	void NavFlight(void);

	/**
	 * @brief Modo de navegación unicam
	*/
	void NavUnicam(void);

	/**
	 * @brief Triangulación de la malla poligonal
	*/
	void PolyTriangle( void );

	/**
	 * @brief Decimado de la malla poligonal
	*/
	void PolyDecimate( void );

	/**
	 * @brief Suavizado de la malla poligonal
	*/
	void PolySmooth( void );

	/**
	 * @brief Normales de la malla poligonal
	*/
	void PolyNormals( void );

	/**
	 * @brief Deformación de la malla poligonal
	*/
	void PolyDeform( void );

	/**
	 * @brief Captura de pantalla
	*/
	vtkWindowToImageFilter* Snapshot(void);

	/**
	 * @brief Obtener polydata
	 * @return apuntador con el vtkPolyData de la ventana
	*/
	vtkPolyData* GetPolyData(void);

	/**
	 * @brief Información tarjeta de video
	 * @return wxString
	*/
	wxString VideoCard(void);	

private:
	wxAuiManager					prv_auiManager;		//! Administrador de la interfaz de usuario

	wxVTKRenderWindowInteractor*	prv_wxVtkVista3D;	//! wxVTKInteractor de la Vista 3D
	vtkRenderer*					prv_render3D;		//! Renderer de la vista 3D
	vtkCamera*						prv_camera;			//! Cámara de la vista 3D
	vtkViewImage3D*					prv_vista3D;		//! Vista 3D
	
	vtkActor*						prv_actor;			//! Actor de la malla poligonal
	vtkPolyDataMapper*				prv_mapper;			//! Mapper de la malla poligonal
	vtkPolyData*					prv_data;			//! Malla poligonal
	
	
	vtkCylinderSource*				prv_endoscope;		//! Rigid endoscope
	vtkActor*						prv_endoActor;		//! endoscope vtkActor
};

#endif //_NKOBJ3DVIEWER_H_