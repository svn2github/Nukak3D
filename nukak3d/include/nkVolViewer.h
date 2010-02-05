/**
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The Original Code is Copyright (C) 2007-2010 by Bioingenium Research Group.
 * Bogota - Colombia
 * All rights reserved.
 *
 * Author(s): Alexander Pinzón Fernández.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** 
 * @file nkVolViewer.h
 * @brief Class for view volume 3D.
 * @details Implement all fucntions for manage image.
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
#include "wx/intl.h"
#include <wx/progdlg.h>


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
#include <vtkCallbackCommand.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkCallbackCommand.h>

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
#include <itkCommand.h>

/** INRIA3D*/
#include "vtkObject.h"
#include "vtkSynchronizedView.h"
#include "vtkViewImage.h"
#include "vtkViewImage2DWithTracer.h"
#include "vtkViewImage3D.h"
#include <wxVtkDICOMImporter.h>
#include "wxVTKRenderWindowInteractor.h"
#include "vtkIsosurfaceManager.h"
#include "vtkImageToIsosurface.h"

/** wx*/
#include "wx/aui/aui.h"

/** nk*/
#include "nkNukak3D.h"
#include "nkIODialog.h"
#include "nkInteractorStyleEndoCamera.h"
#include "nkKernel.h"
#include "nkITKFilterServer.h"

/** STL */
#include <vector>

#define mensajes 1

/** 
 * @brief Class for view volume 3D.
 */
class nkVolViewer: public wxPanel{

private:
	/** 
	 * @brief Clase para monitorear la posición de la cámara
	 */
	class nkCameraCallback : public vtkCallbackCommand
	{
	  private:
		vtkCamera *prv_camera;
		nkVolViewer *prv_window;
		vtkViewImage3D*	prv_vista3D;
		vtkImageData* prv_imagen;
		wxVTKRenderWindowInteractor* prv_wxVtkVista3D;
		unsigned short prv_umbral;

		double prv_bounds[6];
		int prv_imagesize[3];
		int prv_colision;
		int prv_state;
		int prv_updatePlanes;
		double prv_camerapos[3];

	  public:
		static nkCameraCallback* New()
		{ return new nkCameraCallback; }
		
		void SetState(int &p_state)
		{
			prv_state = p_state;
		}

		void SetUmbral(unsigned short p_umbral)
		{
			prv_umbral = p_umbral;
		}

		void SetCameraWindow(vtkCamera *p_camera, double* p_camerapos)
		{
			prv_colision=0;
			prv_updatePlanes=0;
			this->prv_camera = p_camera;
			prv_camerapos[0]=p_camerapos[0];
			prv_camerapos[1]=p_camerapos[1];
			prv_camerapos[2]=p_camerapos[2];
		}

		void  SetVista3D(nkVolViewer *p_window, vtkViewImage3D* p_vista3D )
		{
			this->prv_window=p_window;
			this->prv_vista3D=p_vista3D;
			this->prv_vista3D->GetVolumeActor()->GetBounds(prv_bounds);			
		}
		void SetImage(vtkImageData* p_imagen, wxVTKRenderWindowInteractor* p_wxVtkVista3D)
		{
			this->prv_imagen = p_imagen;
			this->prv_imagen->GetDimensions(prv_imagesize);
			this->prv_wxVtkVista3D = p_wxVtkVista3D;
		}
		void SetUpdatePlanes(int p_value)
		{
			this->prv_updatePlanes=p_value;
		}

		virtual void Execute(vtkObject *caller, unsigned long p_EventId, void *p_CallData);
	};

	friend class nkVolViewer::nkCameraCallback;

	/** 
	 * @brief Clase para monitorear los cuadros por segundo
	 */
	class FpsChange : public vtkCallbackCommand 
	{
	  private:
		vtkRenderer*	prv_render3D;
		double a_fps;
	  public:
		static FpsChange *New()
		{return new FpsChange;}

		void SetRenderer(vtkRenderer* l_render3D)
		{
			this->prv_render3D=l_render3D;
		}
		virtual void Execute(vtkObject *p_Caller, unsigned long p_EventId, void *p_CallData)
		{
			a_fps = 1/prv_render3D->GetLastRenderTimeInSeconds();
			wxString temp;
			temp.Printf("fps = %3.10f\n", a_fps );
			vtkOutputWindow::GetInstance()->DisplayText(temp);
		}
	};
	friend class nkVolViewer::FpsChange;

 public:

	/**
	 * @brief Class Constructor.
	*/
	nkVolViewer(wxWindow* parent, 
		wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTAB_TRAVERSAL|wxBORDER_NONE, 
		const wxString& name = "nkVolViewer");

	/**
	 * @brief Class destructor.
	*/	
	~nkVolViewer();

	/**
	 * @brief Set name of file.
	 * @param a_fileName
	*/
	void seta_fileName(wxString a_fileName);

	/**
	 * @brief Get name of file.
	 * @return wxString Name of file.
	*/
	wxString geta_fileName(void);

	/**
	 * @brief Set volume.
	 * @param an_image itk::Image<unsigned short,3>::Pointer pointer to image.
	*/
	void setImagen(itk::Image<unsigned short,3>::Pointer  an_image);

	/**
	 * @brief Obtener volumen 3D (imagen itk)
	 * @return itk::Image<unsigned short,3>::Pointer apuntador a la imagen
	*/
	itk::Image<unsigned short,3>::Pointer getImagen(void);

	/**
	 * @brief Get volume.
	 * @return vtkImageData* pointer to image.
	*/
	vtkImageData* getVtkImagen(void);

	/**
	 * @brief Configure views.
	*/
	void Configure(void);

	/**
	 * @brief Configure volume.
	 * @param un_a_fileName wxString Filepath of archive.
	 * @param an_image itk::Image<unsigned short,3>::Pointer pointer to image.
	*/
	void ConfigureITKimage(wxString un_a_fileName, itk::Image<unsigned short,3>::Pointer an_image);

	/**
	 * @brief Open volumen.
	 * @param a_fileName wxString filepath of archive.
	*/
	void prOpenFile(wxString a_fileName);

	/**
	 * @brief Open file of volume 3D in format .vol.
	 * @param a_fileName wxString Filepath of archive.
	*/
	void prOpenFile_vol(wxString a_fileName);

	/**
	 * @brief Open directory Dicom.
	 * @param a_fileName wxString Filepath of archive.
	 * @param myimporter pointer to wizard for Dicom images.
	 * @param un_index number of dicom series.
	*/
	void prOpenFile_dicom(wxString a_fileName, wxVtkDICOMImporter* myimporter, int un_index);

	/**
	 * @brief Save file.
	 * @param a_fileName wxString Filepath of archive.
	*/
	void prSaveFile(wxString a_fileName);

	/**
	 * @brief Bounding box de la imagen 3D
	 */
	void prBoundingBox(void);

	/**
	 * @brief Show/hide Bounding box
	 */
	void prBoundingBoxOnOff(void);

	/**
	 * @brief Show/hide Box Widget
	 */
	void BoxWidgetOnOff(void);
	
	/**
	 * @brief Change Lookup Table.
	 * @param una_paleta vtkLookupTable* Lookup Table to change.
	*/
	void cambiarPaletaColor(vtkLookupTable* una_paleta);

	/**
	 * @brief Change rendering mode.
	 * @param un_modo Mode of rendering
	 * @param textura_o_mrc If mode of rendering is texture choose texture mapping or ray cast.
	*/
	void cambiarFormaDeProcesamiento (int un_modo, int textura_o_mrc = 0);
	
	/**
	 * @brief Reset Look Up Table.
	*/
	void reiniciarNiveleseDePaleta(void);

	/**
	 * @brief Sementation with levelsets
	 * @param p_libro wxAuiNotebook* for make pages.
	*/
	void lsLevelsetsCompleto( wxAuiNotebook * p_libro );

	/**
	 * @brief Calc area.
	*/
	void Area(void);

	/**
	 * @brief Get value of select voxel.
	 * @return double value of voxel.
	*/
	double obtenerValorActualDeContorno(void);

	/**
	 * @brief Enable/Disable Stereoscopy vision.
	*/
	void prActiveStereo(void);

	/**
	 * @brief Enable/Disable passive stereo.
	*/
	void prStereoPassive(void);

	/**
	 * @brief More distance between image in Stereoscopy vision.
	*/
	void prStereoMoreSeparation(void);

	/**
	 * @brief Less distance between image in Stereoscopy vision.
	*/
	void prStereoLessSeparation(void);

	/**
	 * @brief Reset position and orientation of camera
	*/
	void prNavResetCamara(void);

	/**
	 * @brief Tracball camera.
	*/
	void prNavTrackball(void);

	/**
	 * @brief Joystick Camera.
	*/
	void prNavJoystick(void);

	/**
	 * @brief Flight camera.
	*/
	void prNavFlight(void);

	/**
	 * @brief Unicam camera.
	*/
	void prNavUnicam(void);

	/** 
	 * @brief Plugin filter
	 * @param p_libro wxAuiNotebook* for make pages.	*/
	void FilterPluginExecute(wxString pluginName, nukak3d::nkKernel &mKernel, wxAuiNotebook * p_libro);
	/**
	 * @brief Gaussian filter.
	 * @param p_libro wxAuiNotebook* for make pages.
	*/
	void FilVolGaussian(wxAuiNotebook * p_libro);

	/**
	 * @brief Median filter.
	 * @param p_libro wxAuiNotebook* for make pages.
	*/
	void FilVolMedian(wxAuiNotebook * p_libro);

	/**
	 * @brief Gradient filter.
	 * @warning Not define.
	*/
	void FilVolGradient(void);

	/**
	 * @brief Threshold filter.
	 * @warning Not define.
	*/
	void FilVolThreshold(void);

	/**
	 * @brief Capture Screen.
	 * @param p_screen Choose view (3d, axial, coronal, sagital).
	*/
	vtkWindowToImageFilter* Snapshot(int p_screen);	

	/**
	 * @brief Video card information.
	 * @return wxString.
	*/
	wxString VideoCard(void);	

	vtkViewImage3D* getVista3D(){
		return prv_vista3D;
	}

	vtkViewImage2DWithTracer* getVistaAxial(){
		return prv_vistaAxial;
	}
	vtkViewImage2DWithTracer* getVistaCoronal(){
		return prv_vistaCoronal;
	}
	vtkViewImage2DWithTracer* getVistaSagital(){
		return prv_vistaSagital;
	}

	void NuevoLevelSets(wxAuiNotebook * p_libro);
	
	/**
	 * @brief Mostrar/guardar posiciones de la camara
	*/
	void CameraPos();

	/**
	 * @brief Mover solo hacia atras
	*/
	void NavEndoscope(void);

	/**
	 * @brief Actualizar los planos ortogonales con la posicion de la camara
	 */
	void NavUpdatePlanes();

	/**
	 * @brief Frames per second
	 * @return Muestra los cuadros por segundo
	*/
	void FPS(void);	

private:
	wxString					prv_a_fileName;	//! Name of file.
	wxAuiManager				prv_auiManager;		//! Administrator of Aui.
	vtkViewImage2DWithTracer*	prv_vistaAxial;		//! View Axial.
	vtkViewImage2DWithTracer*	prv_vistaCoronal;	//! View Coronal.
	vtkViewImage2DWithTracer*	prv_vistaSagital;	//! View Sagital.
	vtkViewImage3D*				prv_vista3D;		//! View 3D.

	wxVTKRenderWindowInteractor* prv_wxVtkVistaAxial;	//! wxVTKInteractor of view Axial.
	wxVTKRenderWindowInteractor* prv_wxVtkVistaCoronal;	//! wxVTKInteractor of view Coronal.
	wxVTKRenderWindowInteractor* prv_wxVtkVistaSagital;	//! wxVTKInteractor of view Sagital.
	wxVTKRenderWindowInteractor* prv_wxVtkVista3D;		//! wxVTKInteractor of view 3D.

	vtkRenderer* prv_render3D;		//! Render of view  3D.
	vtkCamera*	prv_camera;			//! Camera of view 3D.
	nkCameraCallback*	prv_cameracallback;				//! Camera callback para detección de colisiones
	void SetCameraEvent(vtkCallbackCommand* p_Event);	//! Camera event para monitorear la camara 
	int prv_camtracking;								//! Camera monitoring flag
	int prv_updatePlanes;								//! Update camera planes on collision detection

	typedef unsigned short PixelType;	//! Pixel data type.
	typedef itk::Image<unsigned short,3> ImageType;	//! Image data type.
	ImageType::Pointer  prv_imagen;		//! Pointer to image.
	
	vtkActor*	prv_bboxActor;		//! Actor del bounding box de la imagen
	double		prv_camerapos[3];	//! Posicion de la camara

	FpsChange*	prv_fpsEvent;		//! FPS object
	int			prv_fpsflag;		//! FPS flag for activate/deactivate
};

#endif //_NKVOLVIEWER_H_