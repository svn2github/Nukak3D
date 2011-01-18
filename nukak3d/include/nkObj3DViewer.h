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

#include "wx/intl.h"

/** vtk*/
#include <vtkActor.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkDecimatePro.h>
#include <vtkInteractorStyleFlight.h>	
#include <vtkInteractorStyleJoystickCamera.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleUnicam.h>
#include <vtkOutlineFilter.h>
#include <vtkOutputWindow.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkTextActor.h>
#include <vtkTriangleFilter.h>
#include <vtkWindowToImageFilter.h>
#include <vtkrenderer.h>

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
 
 private:
	/** 
	 * @brief Monitor for FPS
	 */
	class FpsChange : public vtkCallbackCommand {
	private:
		vtkRenderer*	prv_render3D;
		double a_fps;
	public:
		static FpsChange *New(){
			return new FpsChange;
		}

		void SetRenderer(vtkRenderer* l_render3D){
			this->prv_render3D=l_render3D;
		}
		virtual void Execute(vtkObject *p_Caller, unsigned long p_EventId, void *p_CallData){
			a_fps = 1/prv_render3D->GetLastRenderTimeInSeconds();
			wxString temp;
			temp.Printf("fps = %3.10f\n", a_fps );
			vtkOutputWindow::GetInstance()->DisplayText(temp);
		}
	};
	friend class nkObj3DViewer::FpsChange;

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
	void Configure(void);

	/**
	 * @brief Open file.
	 * @param a_fileName Filepath of mesh.
	*/
	void prOpenFile(wxString a_fileName);

	/**
	 * @brief Save mesh.
	 * @param a_fileName Filepath of file.
	*/
	void prSaveFile(wxString a_fileName);

	/**
	 * @brief Config view 3d.
	 * @param input Object to view.
	*/
	void prConfigureMesh3D(vtkPolyData* input);
	
	/**
	 * @brief Bounding box de la imagen 3D
	 */
	void prBoundingBox(void);

	/**
	 * @brief Show/hide Bounding box
	 */
	void prBoundingBoxOnOff(void);

	/**
	 * @brief Marching cubes function.
	 * @param an_image Volume to transform.
	 * @param a_numContours Numbers of contours.
	 * @param a_thresholdLower
	 * @param a_thresholdUpper
	 * @param a_red
	 * @param a_green
	 * @param an_blue
	 * @param a_opacity
	*/
	void prImageToIsoSurface(itk::Image<unsigned short,3>::Pointer an_image,
									  int a_numContours = 20, 
									  int a_thresholdLower = 0,
									  int a_thresholdUpper = 100,
									  double a_red = 1.0,
									  double a_green = 1.0,
									  double an_blue = 1.0,
									  double a_opacity = 1.0
									  );
	
	/**
	 * @brief Enable/Diable stereoscopy vision.
	*/
	void prActiveStereo(void);

	/**
	 * @brief Enable/Diable passive stereo.
	*/
	void prStereoPassive(void);

	/**
	 * @brief More separation of images in stereoscopy vision.
	*/
	void prStereoMoreSeparation(void);

	/**
	 * @brief Less separation of images in stereoscopy vision.
	*/
	void prStereoLessSeparation(void);

	/**
	 * @brief Reset position and orientation of camera.
	*/
	void prNavResetCamara(void);

	/**
	 * @brief Trackball camera.
	*/
	void prNavTrackball(void);

	/**
	 * @brief Joystick camera.
	*/
	void prNavJoystick(void);

	/**
	 * @brief Flight camera.
	*/
	void prNavFlight(void);

	/**
	 * @brief unicam camera.
	*/
	void prNavUnicam(void);

	/**
	 * @brief Triangle mesh.
	*/
	void prPolyTriangle( void );

	/**
	 * @brief Decimate mesh.
	*/
	void prPolyDecimate( void );

	/**
	 * @brief Smooth mesh.
	*/
	void prPolySmooth( void );

	/**
	 * @brief Recalc normals.
	*/
	void prPolyNormals( void );

	/**
	 * @brief Deform mesh.
	*/
	void prPolyDeform( void );

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

	/**
	 * @brief Frames per second
	 * @return Muestra los cuadros por segundo
	*/
	void FPS(void);	

private:
	wxAuiManager					prv_auiManager;		//! Administrator for Aui.

	wxVTKRenderWindowInteractor*	prv_wxVtkVista3D;	//! wxVTKInteractor of view 3D.
	vtkRenderer*					prv_render3D;		//! Render of view 3D.
	vtkCamera*						prv_camera;			//! Camera.
	vtkViewImage3D*					prv_vista3D;		//! View 3D
	
	vtkActor*						prv_bboxActor;		//! Actor del bounding box de la malla poligonal
	vtkActor*						prv_actor;			//! Actor for mesh.
	vtkPolyDataMapper*				prv_mapper;			//! Mapper of mesh.
	vtkPolyData*					prv_data;			//! Data of mesh.
	
	
	vtkCylinderSource*				prv_endoscope;		//! Rigid endoscope
	vtkActor*						prv_endoActor;		//! endoscope vtkActor

	FpsChange*						prv_fpsEvent;		//! FPS object
	int								prv_fpsflag;		//! FPS flag for activate/deactivate
};

#endif //_NKOBJ3DVIEWER_H_