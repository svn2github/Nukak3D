/** 
 * @file nkVolViewer.h
 * @brief Clase para visualizar volúmenes 3D (pila de imágenes)
 * @details Implementa todas las funciones para visualización de
 * volúmenes 3D y filtros
 * @author Alexander Pinzón Fernandez, Byron Pérez
 * @version 0.2
 * @date 27/12/2007 03:37 p.m.
*/
#ifndef _NKVOLVIEWER_H_
#define _NKVOLVIEWER_H_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

/** vtk/Common*/
#include <vtkLookupTable.h>
#include <vtkByteSwap.h>
#include <vtkOutputWindow.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleJoystickCamera.h>
#include <vtkInteractorStyleFlight.h>	
#include <vtkInteractorStyleUnicam.h>
#include <vtkWindowToImageFilter.h>

/** ITK*/
#include <itkAnalyzeImageIOFactory.h>
#include <itkCastImageFilter.h>
#include <itkExtractImageFilter.h>
#include <itkGDCMImageIOFactory.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageIOBase.h>
#include <itkImageToVTKImageFilter.h>
#include <itkRawImageIO.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkVTKImageToImageFilter.h>
#include <itkMedianImageFilter.h>
#include <itkDiscreteGaussianImageFilter.h>

#include <itkCurvatureAnisotropicDiffusionImageFilter.h>
#include <itkGradientMagnitudeRecursiveGaussianImageFilter.h>
#include <itkSigmoidImageFilter.h>
#include <itkFastMarchingImageFilter.h>
#include <itkShapeDetectionLevelSetImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkImageFileWriter.h>

/** INRIA3D*/
#include "vtkObject.h"
#include "vtkSynchronizedView.h"
#include "vtkViewImage.h"
#include "vtkViewImage2DWithTracer.h"
#include "vtkViewImage3D.h"
#include <wxVtkDICOMImporter.h>
#include "wxVTKRenderWindowInteractor.h"

/** wx*/
#include "wx/aui/aui.h"

/** nk*/
#include "nkNukak3D.h"
#include "nkIODialog.h"

#define mensajes 1

/** 
 * @brief Clase para visualizar volúmenes 3D (pila de imágenes)
 * @details Implementa todas las funciones para visualización de
 * volúmenes 3D y filtros
 */
class nkVolViewer: public wxPanel{
 public:

	/**
	 * @brief Constructor de la clase
	*/
	nkVolViewer(wxWindow* parent, 
		wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTAB_TRAVERSAL|wxBORDER_NONE, 
		const wxString& name = "nkVolViewer");

	/**
	 * @brief Destructor de la clase
	*/	
	~nkVolViewer();

	/**
	 * @brief Establece el nombre del archivo
	 * @param nombreArchivo
	*/
	void setNombreArchivo(wxString nombreArchivo);

	/**
	 * @brief Obtener el nombre del archivo
	 * @return wxString con el nombre de archivo
	*/
	wxString getNombreArchivo(void);

	/**
	 * @brief Establecer volumen 3D (imagen itk)
	 * @param una_imagen itk::Image<unsigned short,3>::Pointer apuntador a la imagen
	*/
	void setImagen(itk::Image<unsigned short,3>::Pointer  una_imagen);

	/**
	 * @brief Obtener volumen 3D (imagen itk)
	 * @return itk::Image<unsigned short,3>::Pointer apuntador a la imagen
	*/
	itk::Image<unsigned short,3>::Pointer getImagen(void);

	/**
	 * @brief Obtener volumen 3D (imagen vtk)
	 * @return vtkImageData* apuntador a la imagen
	*/
	vtkImageData* getVtkImagen(void);

	/**
	 * @brief Configurar vistas del volumen 3D
	*/
	void Configurar(void);

	/**
	 * @brief Configurar volumen 3D (imagen itk)
	 * @param un_nombreArchivo wxString con el nombre y ruta del archivo
	 * @param una_imagen itk::Image<unsigned short,3>::Pointer apuntador a la imagen
	*/
	void configurarITKimage(wxString un_nombreArchivo, itk::Image<unsigned short,3>::Pointer una_imagen);

	/**
	 * @brief Abrir archivo de volumen 3D
	 * @param nombreArchivo wxString con el nombre y ruta del archivo
	*/
	void abrirArchivo(wxString nombreArchivo);

	/**
	 * @brief Abrir archivo de volumen 3D en formato .vol
	 * @param nombreArchivo wxString con el nombre y ruta del archivo
	*/
	void abrirArchivo_vol(wxString nombreArchivo);

	/**
	 * @brief Abrir archivo de volumen 3D en formato DICOM
	 * @param nombreArchivo wxString con el nombre y ruta del archivo
	 * @param myimporter apuntador del asistente de importación DICOM
	 * @param un_index numero de la serie DICOM que se quiere abrir
	*/
	void abrirArchivo_dicom(wxString nombreArchivo, wxVtkDICOMImporter* myimporter, int un_index);

	/**
	 * @brief Guardar archivo de volumen 3D
	 * @param nombreArchivo wxString con el nombre y ruta del archivo
	*/
	void guardarArchivo(wxString nombreArchivo);

	/**
	 * @brief Cambiar la paleta de color (Look Up Table) del volumen 3D
	 * @param una_paleta vtkLookupTable* con la paleta de color seleccionada
	*/
	void cambiarPaletaColor(vtkLookupTable* una_paleta);

	/**
	 * @brief Cambiar el modo de visualización del volumen 3D
	 * @param un_modo Modo de visualización planos ortogonales o render de volumen
	 * @param textura_o_mrc Modo render de volumen: textura o raycasting
	*/
	void cambiarFormaDeProcesamiento (int un_modo, int textura_o_mrc = 0);
	
	/**
	 * @brief Reiniciar la paleta de color (Look Up Table) del volumen 3D con los valores por defecto
	*/
	void reiniciarNiveleseDePaleta(void);

	/**
	 * @brief Segmentación con levelsets
	 * @param p_libro wxAuiNotebook* para crear páginas con la función de velocidad y la imagen segmentada
	*/
	void lsLevelsetsCompleto( wxAuiNotebook * p_libro );

	/**
	 * @brief Calcular el área de la imagen axial con pixeles mayores que cero
	*/
	void Area(void);

	/**
	 * @brief Obtener el valor del voxel seleccionado
	 * @return double con el valor de intesidad
	*/
	double obtenerValorActualDeContorno(void);

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
	 * @brief Filtro gaussiano
	 * @param p_libro wxAuiNotebook* para crear página con la imagen filtrada
	*/
	void FilVolGaussian(wxAuiNotebook * p_libro);

	/**
	 * @brief Filtro mediana
	 * @param p_libro wxAuiNotebook* para crear página con la imagen filtrada
	*/
	void FilVolMedian(wxAuiNotebook * p_libro);

	/**
	 * @brief Filtro gradiente
	 * @warning no implementado aún
	*/
	void FilVolGradient(void);

	/**
	 * @brief Filtro umbral
	 * @warning no implementado aún
	*/
	void FilVolThreshold(void);

	/**
	 * @brief Captura de pantalla
	 * @param p_screen selección de la vista 3D, axial, coronal o sagital
	*/
	vtkWindowToImageFilter* Snapshot(int p_screen);	

	/**
	 * @brief Información tarjeta de video
	 * @return wxString
	*/
	wxString VideoCard(void);	

private:
	wxString					prv_nombreArchivo;	//! Nombre del archivo
	wxAuiManager				prv_auiManager;		//! Administrador de la interfaz de usuario
	vtkViewImage2DWithTracer*	prv_vistaAxial;		//! Vista Axial
	vtkViewImage2DWithTracer*	prv_vistaCoronal;	//! Vista Coronal
	vtkViewImage2DWithTracer*	prv_vistaSagital;	//! Vista Sagital
	vtkViewImage3D*				prv_vista3D;		//! Vista 3D

	wxVTKRenderWindowInteractor* prv_wxVtkVistaAxial;	//! wxVTKInteractor de la Vista Axial
	wxVTKRenderWindowInteractor* prv_wxVtkVistaCoronal;	//! wxVTKInteractor de la Vista Coronal
	wxVTKRenderWindowInteractor* prv_wxVtkVistaSagital;	//! wxVTKInteractor de la Vista Sagital
	wxVTKRenderWindowInteractor* prv_wxVtkVista3D;		//! wxVTKInteractor de la Vista 3D

	vtkRenderer* mi_render3D;		//! Renderer de la vista 3D
	vtkCamera*	prv_camera;			//! Cámara de la vista 3D

	typedef unsigned short PixelType;	//! Tipo de dato de la imagen
	typedef itk::Image<unsigned short,3> ImageType;	//! Tipo de imagen
	ImageType::Pointer  prv_imagen;		//! Apuntador a la imagen
	
};

#endif //_NKVOLVIEWER_H_