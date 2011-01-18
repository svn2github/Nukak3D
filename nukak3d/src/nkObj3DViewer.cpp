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

#include "nkObj3DViewer.h"

#include "itkImageToVTKImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include <itkAnalyzeImageIOFactory.h>
#include <vtkImageMarchingCubes.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkDecimatePro.h>
#include "vtkLookupTableManager.h"
#include <vtkActor.h>
#include <vtkProperty.h>
#include <wx/colordlg.h>
#include <vtkrenderer.h>

//*****************************************************************************************
//		CONSTRUCTOR
//*****************************************************************************************
nkObj3DViewer::nkObj3DViewer(wxWindow* parent, 
	wxWindowID id, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString& name):
	wxPanel(parent, id, pos, size, style, name)
{
	prv_auiManager.SetManagedWindow(this);
	prv_auiManager.SetFlags(prv_auiManager.GetFlags()|
		wxAUI_MGR_ALLOW_ACTIVE_PANE|
		wxAUI_MGR_HINT_FADE|
		wxAUI_MGR_TRANSPARENT_HINT);
	
	long viewStyle = wxWANTS_CHARS | wxNO_FULL_REPAINT_ON_RESIZE;

	prv_wxVtkVista3D = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													wxDefaultSize,
													viewStyle,
													wxT ("wxVtk 3D view"));

	prv_wxVtkVista3D->GetRenderWindow()->SetStereoCapableWindow(1); //! Init stereo window

	prv_vista3D = vtkViewImage3D::New();
	prv_camera  = vtkCamera::New();
	prv_render3D = vtkRenderer::New();	
	prv_render3D->SetActiveCamera(prv_camera);
	prv_wxVtkVista3D->GetRenderWindow()->AddRenderer(prv_render3D);
	prv_vista3D->SetRenderWindow(prv_wxVtkVista3D->GetRenderWindow());

	prv_vista3D->SetRenderer(prv_render3D);
	  

	prv_auiManager.AddPane(prv_wxVtkVista3D, wxAuiPaneInfo().
		Name(wxT("VIEW_3D")).Caption(_("View 3D")).
		Center().Layer(1).PinButton(true).
		MinSize(wxSize(200,200)).PaneBorder(true).
		CloseButton(false).MaximizeButton(true));

	prv_auiManager.Update();
	prv_mapper = 0;
	prv_actor = 0;
	prv_data = 0;
	prv_fpsflag = 0;
}
//*****************************************************************************************
//		DESTRUCTOR
//*****************************************************************************************
nkObj3DViewer::~nkObj3DViewer(){
	prv_vista3D->Detach();
	prv_vista3D->Delete();
	if(prv_wxVtkVista3D) prv_wxVtkVista3D->Delete();
	prv_auiManager.UnInit();
	if (prv_render3D != 0) prv_render3D->Delete();

	if(prv_mapper != 0 )prv_mapper->Delete();
	if(prv_actor != 0 )prv_actor->Delete();
}
//*****************************************************************************************
//		CONFIG VIEW
//*****************************************************************************************
void nkObj3DViewer::Configure(void){
	prv_vista3D->SetBackgroundColor (0.0,0.0,0.0);
	prv_vista3D->SetRenderingModeToPlanar();
	this->prBoundingBox();
}
//*****************************************************************************************
//		OPEN FILE
//*****************************************************************************************
void nkObj3DViewer::prOpenFile(wxString a_fileName){
	vtkPolyDataReader* mi_vtkReader = vtkPolyDataReader::New();
	mi_vtkReader->SetFileName(a_fileName.c_str());
	mi_vtkReader->Update();

	prv_actor = vtkActor::New();
	prv_mapper = vtkPolyDataMapper::New();

	prv_data = mi_vtkReader->GetOutput();
	prv_mapper->SetInput(prv_data);

	prv_actor->SetMapper(prv_mapper);
	prv_mapper->ScalarVisibilityOff();
	
	prv_render3D->AddActor(prv_actor);
	this->prBoundingBox();
	prv_render3D->ResetCamera();

	prv_render3D->Render();
	mi_vtkReader->Delete();

}
//*****************************************************************************************
//		MESH CONFIG
//*****************************************************************************************
void nkObj3DViewer::prConfigureMesh3D(vtkPolyData* input){


	prv_actor = vtkActor::New();
	prv_mapper = vtkPolyDataMapper::New();

	prv_data = input;
	prv_mapper->SetInput(prv_data);
	prv_mapper->Update();

	prv_actor->SetMapper(prv_mapper);
	prv_mapper->ScalarVisibilityOff();
	
	prv_render3D->AddActor(prv_actor);
	this->prBoundingBox();
	prv_render3D->ResetCamera();

	prv_render3D->Render();
	prv_wxVtkVista3D->Render();
	prv_wxVtkVista3D->Refresh();
	
}

//*****************************************************************************************
//		Boundig box de la imagen 3D
//*****************************************************************************************
void nkObj3DViewer::prBoundingBox()
{
	vtkOutlineFilter *prBoundingBox = vtkOutlineFilter::New(); //! Bounding Box creation
	prBoundingBox->SetInput(prv_data);

	vtkPolyDataMapper *prv_bboxMapper = vtkPolyDataMapper::New(); //! Bounding Box mapper
	prv_bboxMapper->SetInput(prBoundingBox->GetOutput());

	prv_bboxActor = vtkActor::New(); //! Bounding Box actor
	prv_bboxActor->SetMapper(prv_bboxMapper);
	prv_bboxActor->GetProperty()->SetColor(1,0.1,0.1); //! Bounding Box color
	
	prv_render3D->AddActor(prv_bboxActor);
	prv_bboxActor->VisibilityOff();

	prv_render3D->Render();
	prv_wxVtkVista3D->Render();
	prv_wxVtkVista3D->Refresh();

}
//*****************************************************************************************
//		Boundig box de la imagen 3D
//*****************************************************************************************
void nkObj3DViewer::prBoundingBoxOnOff()
{
	if(prv_bboxActor->GetVisibility()==false )
		prv_bboxActor->VisibilityOn();
	else 
		prv_bboxActor->VisibilityOff();

	prv_render3D->Render();
	prv_wxVtkVista3D->Render();
	prv_wxVtkVista3D->Refresh();
}

//*****************************************************************************************
//		GET POLYDATA
//*****************************************************************************************
vtkPolyData* nkObj3DViewer::GetPolyData()
{
	return prv_data;
}
//*****************************************************************************************
//		MARCHING CUBES
//*****************************************************************************************
void nkObj3DViewer::prImageToIsoSurface(itk::Image<unsigned short,3>::Pointer an_image,
									  int a_numContours, 
									  int a_thresholdLower,
									  int a_thresholdUpper,
									  double a_red,
									  double a_green,
									  double an_blue,
									  double a_opacity
									  ){
	const int Dimension = 3;
	typedef unsigned short PixelType;
	typedef itk::Image<PixelType, Dimension> ImageType;
	typedef itk::ImageToVTKImageFilter<ImageType> Itk2VtkType;
	Itk2VtkType::Pointer  m_Itk2Vtk = Itk2VtkType::New();
    
	m_Itk2Vtk->SetInput(an_image);
	m_Itk2Vtk->Update();

	vtkImageMarchingCubes *marcher = vtkImageMarchingCubes::New();
    marcher->SetInput(m_Itk2Vtk->GetOutput());
	marcher->SetNumberOfContours(a_numContours);
    marcher->SetValue(a_thresholdLower, a_thresholdUpper);
    marcher->Update();

	this->prConfigureMesh3D(marcher->GetOutput());

	vtkLookupTable* lut = vtkLookupTableManager::GetLookupTable(1);
	prv_mapper->SetLookupTable(lut);

	prv_actor->GetProperty()->SetDiffuseColor(a_red,a_green,an_blue);
	prv_actor->GetProperty()->SetOpacity(a_opacity);

	//! Refresh GUI
	prv_wxVtkVista3D->Render();
	prv_wxVtkVista3D->Refresh();	
}

//*****************************************************************************************
//		SAVE MESH
//*****************************************************************************************
void nkObj3DViewer::prSaveFile(wxString a_fileName){
		vtkPolyDataWriter *l_Writer = vtkPolyDataWriter::New();
		l_Writer->SetFileName( a_fileName );
		l_Writer->SetInput( (vtkPolyData*)prv_actor->GetMapper()->GetInput() );	
		l_Writer->Write(); 
		l_Writer->Delete();

}
//*****************************************************************************************
//		ACTIVE STEREO
//*****************************************************************************************
void nkObj3DViewer::prActiveStereo(void){

	if(!prv_wxVtkVista3D->GetRenderWindow()->GetStereoRender()) {
		prv_wxVtkVista3D->GetRenderWindow()->SetStereoTypeToCrystalEyes();
		prv_wxVtkVista3D->GetRenderWindow()->StereoRenderOn();
	}else
		prv_wxVtkVista3D->GetRenderWindow()->StereoRenderOff();
	
	prv_wxVtkVista3D->GetRenderWindow()->StereoUpdate();

	prv_wxVtkVista3D->Refresh();

}

//*****************************************************************************************
//		PASIVE STEREO
//*****************************************************************************************
void nkObj3DViewer::prStereoPassive(void){

	if(!prv_wxVtkVista3D->GetRenderWindow()->GetStereoRender()) {
		prv_wxVtkVista3D->GetRenderWindow()->SetStereoTypeToAnaglyph();
		prv_wxVtkVista3D->GetRenderWindow()->StereoRenderOn();
	}else
		prv_wxVtkVista3D->GetRenderWindow()->StereoRenderOff();
	
	prv_wxVtkVista3D->GetRenderWindow()->StereoUpdate();

	prv_wxVtkVista3D->Render();
	prv_wxVtkVista3D->Refresh();

}
//*****************************************************************************************
//		STEREO INCREASE - Increase stereo separation
//*****************************************************************************************
void nkObj3DViewer::prStereoMoreSeparation( void )
{
	double	sep,
			inc=0.1;
	sep = prv_camera->GetEyeAngle();	//! Get actual separation
	prv_camera->SetEyeAngle(sep+inc);	//! Increase separation

	prv_wxVtkVista3D->Render();
	prv_wxVtkVista3D->Refresh();
}

//*****************************************************************************************
//		STEREO DECREASE - Decrease stereo separation
//*****************************************************************************************
void nkObj3DViewer::prStereoLessSeparation( void )
{
	double	sep,
			inc=0.1;
	sep = prv_camera->GetEyeAngle();	//! Get actual separation
	prv_camera->SetEyeAngle(sep-inc);	//! Decrease separation

	prv_wxVtkVista3D->Render();
	prv_wxVtkVista3D->Refresh();
}

//*****************************************************************************************
//		MENU -> RESET CAMERA
//*****************************************************************************************
void nkObj3DViewer::prNavResetCamara( void )
{
	prv_render3D->ResetCamera();
	prv_wxVtkVista3D->Render();
	prv_wxVtkVista3D->Refresh();

}
//*****************************************************************************************
//		MENU -> NAVIGATION -> TRACKBALL
//*****************************************************************************************
void nkObj3DViewer::prNavTrackball( )
{
	vtkInteractorStyleTrackballCamera *l_style = vtkInteractorStyleTrackballCamera::New();
	prv_wxVtkVista3D->SetInteractorStyle(l_style);
	l_style->Delete();
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> JOYSTICK
//*****************************************************************************************
void nkObj3DViewer::prNavJoystick( )
{
	vtkInteractorStyleJoystickCamera *l_style = vtkInteractorStyleJoystickCamera::New();
	prv_wxVtkVista3D->SetInteractorStyle(l_style);
	l_style->Delete();
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> FLIGHT
//*****************************************************************************************
void nkObj3DViewer::prNavFlight(  )
{
	vtkInteractorStyleFlight  *l_style = vtkInteractorStyleFlight ::New();
	prv_wxVtkVista3D->SetInteractorStyle(l_style);
	l_style->Delete();
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> UNICAM
//*****************************************************************************************
void nkObj3DViewer::prNavUnicam( ) 
{
	vtkInteractorStyleUnicam *l_style = vtkInteractorStyleUnicam::New();
	l_style->SetWorldUpVector(0.0, 1.0, 0.0);
	prv_wxVtkVista3D->SetInteractorStyle(l_style);
	l_style->Delete();
}
//*****************************************************************************************
//		FILTRADO DE POLIGONOS -> TRIANGULACION
//*****************************************************************************************
void nkObj3DViewer::prPolyTriangle( ) 
{
	// Surface triangulation
	vtkTriangleFilter *l_Triangle = vtkTriangleFilter::New();
	l_Triangle->SetInput(prv_data);
	l_Triangle->UpdateWholeExtent(); //! Update algorithm
	
	//! Copy filter output in a_Input
	vtkPolyData *l_temp = vtkPolyData::New();
	l_temp->DeepCopy( l_Triangle->GetOutput() );
	prv_data->DeepCopy( l_temp );
	
	//! Delete filters 
	l_temp->Delete();
	l_Triangle->Delete();
	
	//! Refresh GUI
	prv_wxVtkVista3D->Render();
	prv_wxVtkVista3D->Refresh();
	
}
//*****************************************************************************************
//		FILTER -> DECIMATE
//*****************************************************************************************
void nkObj3DViewer::prPolyDecimate( ) 
{
	wxString etiquetas[100];
	const int num_datos=1;
	etiquetas[0] = _("Decimate ratio"); 
	
	nkIODialog * miDlg = new nkIODialog(	this, 
												etiquetas,
												num_datos,
												-1,
												_("Nukak3D: Decimate mesh"),
												wxDefaultPosition,
												wxSize(330,(num_datos+4)*20+40));
	
	miDlg->cambiarValor(wxT("0.7"),0);
					
	miDlg->ShowModal(); 
	
	double datos[num_datos]; 

	if(miDlg->GetReturnCode() == wxID_OK)
	{	
		wxBeginBusyCursor();

		for(int i=0;i<num_datos;i++)
			(miDlg->obtenerValor(i)).ToDouble(&datos[i]);

		// Triangulate fisrt
		this->prPolyTriangle();

		// a_Decimate approximation
		vtkDecimatePro *l_Decimate = vtkDecimatePro::New();
		l_Decimate->SetInput( prv_data );

		l_Decimate->PreserveTopologyOn(); //! Turn on/off whether to preserve the topology of the original mesh
		l_Decimate->BoundaryVertexDeletionOff(); //! Turn on/off the deletion of vertices on the boundary of a mesh
		l_Decimate->SplittingOn(); //! Split triangles
		l_Decimate->SetMaximumError(VTK_LARGE_FLOAT); //! Maximun error
		l_Decimate->SetTargetReduction (datos[0]); //! Percent of resulting polygons
		l_Decimate->UpdateWholeExtent(); //! Update algorithm

		//! Copy filter output in a_Input
		vtkPolyData *l_temp = vtkPolyData::New();
		l_temp->DeepCopy( l_Decimate->GetOutput() );
		prv_data->DeepCopy( l_temp );
		
		//! Delete filters 
		l_temp->Delete();
		l_Decimate->Delete();
		
		//! Refresh GUI
		prv_wxVtkVista3D->Render();
		prv_wxVtkVista3D->Refresh();

		wxEndBusyCursor();
	}	
	delete miDlg;
}
//*****************************************************************************************
//		FILTER -> SMOOTH
//*****************************************************************************************
void nkObj3DViewer::prPolySmooth(  ) 
{
	wxString etiquetas[100];
	const int num_datos=1;

	etiquetas[0] = _("Filter iterations"); 
	
	nkIODialog * miDlg = new nkIODialog(	this, 
												etiquetas,
												num_datos,
												-1,
												_("Nukak3D: Smooth mesh"),
												wxDefaultPosition,
												wxSize(330,(num_datos+4)*20+40));
	
	miDlg->cambiarValor(wxT("100"),0);				
	miDlg->ShowModal(); 
	double datos[1]; 

	if(miDlg->GetReturnCode() == wxID_OK)
	{	
		wxBeginBusyCursor();

		for(int i=0;i<num_datos;i++)
			(miDlg->obtenerValor(i)).ToDouble(&datos[i]);

		// Polygon a_Smooth
		vtkSmoothPolyDataFilter *l_Smooth = vtkSmoothPolyDataFilter::New();
		l_Smooth->SetInput( prv_data );
		l_Smooth->SetNumberOfIterations((int)datos[0]);
		l_Smooth->SetConvergence(0);
		l_Smooth->UpdateWholeExtent();


		//! Copy filter output in a_Input
		vtkPolyData *l_temp = vtkPolyData::New();
		l_temp->DeepCopy( l_Smooth->GetOutput() );
		prv_data->DeepCopy( l_temp );
	
		//! Delete filters 
		l_temp->Delete();
		l_Smooth->Delete();

		//! Refresh GUI
		prv_wxVtkVista3D->Render();
		prv_wxVtkVista3D->Refresh();
		
		wxEndBusyCursor();
	}
	delete miDlg;
}
//*****************************************************************************************
//		FILTERS -> NORMALS
//*****************************************************************************************
void nkObj3DViewer::prPolyNormals( ) 
{
	wxString etiquetas[100];
	const int num_datos=1;
	etiquetas[0] = _("Angle value for define normal"); 
	
	nkIODialog * miDlg = new nkIODialog(	this, 
												etiquetas,
												num_datos,
												-1,
												_("Nukak3D: Recalc normals"),
												wxDefaultPosition,
												wxSize(330,(num_datos+4)*20+40));
	
	miDlg->cambiarValor(wxT("60"),0);
					
	miDlg->ShowModal(); 
	
	double datos[1]; 

	if(miDlg->GetReturnCode() == wxID_OK)
	{	
		wxBeginBusyCursor();

		for(int i=0;i<num_datos;i++)
			(miDlg->obtenerValor(i)).ToDouble(&datos[i]);

		// Polygon a_Smooth
		vtkPolyDataNormals *l_Normals = vtkPolyDataNormals::New();
		l_Normals->SetInput( prv_data );
		l_Normals->SetFeatureAngle(datos[0]);
		l_Normals->UpdateWholeExtent();


		//! Copy filter output in a_Input
		vtkPolyData *l_temp = vtkPolyData::New();
		l_temp->DeepCopy( l_Normals->GetOutput() );
		prv_data->DeepCopy( l_temp );
		
		//! Delete filters 
		l_temp->Delete();
		l_Normals->Delete();
		
		//! Refresh GUI
		prv_wxVtkVista3D->Render();
		prv_wxVtkVista3D->Refresh();

		wxEndBusyCursor();
	}
	delete miDlg;
}
//*****************************************************************************************
//		FILTER -> DEFORM
//*****************************************************************************************
void nkObj3DViewer::prPolyDeform(  ) 
{
	wxBeginBusyCursor();

	//Deform a mesh
	vtkPoints *l_points = vtkPoints::New();
	int numPts = prv_data->GetNumberOfPoints();
	double p[3];

	for(int i=0;i<numPts;i++)
	{
		prv_data->GetPoint(i,p);
		p[0]=p[0]+5.0;
		p[1]=p[1]+8.0;
		p[2]=p[2]-2.0;
		l_points->InsertPoint(i,p[0],p[1],p[2]);
	}

	//! Copy filter output in a_Input
	vtkPolyData *l_temp = vtkPolyData::New();
	l_temp->CopyStructure(prv_data);
	l_temp->SetPoints(l_points);
	l_temp->Update();
	prv_data->DeepCopy( l_temp );


	l_temp->Delete();
	l_points->Delete();
	
	//! Refresh GUI
	prv_wxVtkVista3D->Render();
	prv_wxVtkVista3D->Refresh();

	wxEndBusyCursor();
}
//*****************************************************************************************
//		RASTERIZER WINDOW
//*****************************************************************************************
vtkWindowToImageFilter* nkObj3DViewer::Snapshot( ) 
{
	//! Save an window snapshot
	vtkWindowToImageFilter* l_w2i = vtkWindowToImageFilter::New();
	l_w2i->SetInput(prv_wxVtkVista3D->GetRenderWindow());
	l_w2i->Update();

	return l_w2i;
}
//*****************************************************************************************
//		VIDEO CARD INFORMATION
//*****************************************************************************************
wxString nkObj3DViewer::VideoCard( ) 
{
	const char* l_ren  = l_ren = prv_wxVtkVista3D->GetRenderWindow()->ReportCapabilities(); //! Capturing Render capabilities from RenderWindow
	wxString l_text(l_ren,wxConvUTF8);
	return l_text;	
}
//*****************************************************************************************
//		Actualización de los planos de image
//*****************************************************************************************
void nkObj3DViewer::FPS()
{
	if(!prv_fpsflag)
	{
		prv_fpsEvent=FpsChange::New();
		prv_fpsEvent->SetRenderer(prv_render3D);
		prv_render3D->AddObserver(vtkCommand::EndEvent,prv_fpsEvent);
		prv_fpsEvent->Delete();
		prv_fpsflag=1;
	}
	else
	{
		prv_render3D->RemoveObserver(vtkCommand::AnyEvent);
		prv_fpsflag=0;
	}
}