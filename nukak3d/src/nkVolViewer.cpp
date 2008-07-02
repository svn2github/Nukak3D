#include "nkVolViewer.h"
#include "nkLevelSets.h"
#include <vtkInteractorStyleSwitch.h>
#include <vtkRenderWindowInteractor.h>

//*****************************************************************************************
//		CONSTRUCTOR
//*****************************************************************************************
nkVolViewer::nkVolViewer(wxWindow* parent, 
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

	prv_wxVtkVistaAxial = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													wxDefaultSize,
													viewStyle,
													wxT ("wxVtk Axial view"));

	prv_wxVtkVistaCoronal = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													wxDefaultSize,
													viewStyle,
													wxT ("wxVtk Coronal view"));

	prv_wxVtkVistaSagital = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													wxDefaultSize,
													viewStyle,
													wxT ("wxVtk Sagital view"));

	prv_wxVtkVista3D = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													wxDefaultSize,
													viewStyle,
													wxT ("wxVtk 3D view"));

	
	prv_wxVtkVista3D->GetRenderWindow()->SetStereoCapableWindow(1); //! Init stereo window

	prv_vistaAxial   = vtkViewImage2DWithTracer::New();
	prv_vistaCoronal = vtkViewImage2DWithTracer::New();
	prv_vistaSagital = vtkViewImage2DWithTracer::New();
	prv_vista3D      = vtkViewImage3D::New();

	vtkRenderer* mi_renderAxial   = vtkRenderer::New();
	vtkRenderer* mi_renderCoronal = vtkRenderer::New();
	vtkRenderer* mi_renderSagital = vtkRenderer::New();
	prv_render3D      = vtkRenderer::New();

	prv_camera = vtkCamera::New();
	prv_render3D->SetActiveCamera(prv_camera);

	prv_wxVtkVistaAxial->GetRenderWindow()->AddRenderer   (mi_renderAxial);
	prv_wxVtkVistaCoronal->GetRenderWindow()->AddRenderer (mi_renderCoronal);
	prv_wxVtkVistaSagital->GetRenderWindow()->AddRenderer (mi_renderSagital);
	prv_wxVtkVista3D->GetRenderWindow()->AddRenderer      (prv_render3D);

	prv_vistaAxial->SetRenderWindow   (prv_wxVtkVistaAxial->GetRenderWindow());
	prv_vistaCoronal->SetRenderWindow (prv_wxVtkVistaCoronal->GetRenderWindow());
	prv_vistaSagital->SetRenderWindow (prv_wxVtkVistaSagital->GetRenderWindow());
	prv_vista3D->SetRenderWindow      (prv_wxVtkVista3D->GetRenderWindow());

	prv_vistaAxial->SetRenderer   (mi_renderAxial);
	prv_vistaCoronal->SetRenderer (mi_renderCoronal);
	prv_vistaSagital->SetRenderer (mi_renderSagital);
	prv_vista3D->SetRenderer      (prv_render3D);
	  
	

	mi_renderAxial->Delete();
	mi_renderCoronal->Delete();
	mi_renderSagital->Delete();
	

	prv_auiManager.AddPane(prv_wxVtkVistaAxial, wxAuiPaneInfo().
		Name(wxT("AXIAL_VIEW")).Caption(_("Axial View")).
		Bottom().Position(0).Layer(0).PinButton(true).
		MinSize(wxSize(200,200)).PaneBorder(true).
        CloseButton(false).MaximizeButton(true));

	prv_auiManager.AddPane(prv_wxVtkVistaCoronal, wxAuiPaneInfo().
		Name(wxT("CORONAL_VIEW")).Caption(_("Coronal View")).		
		Bottom().Position(1).Layer(0).PinButton(true).
		MinSize(wxSize(200,200)).PaneBorder(true).
        CloseButton(false).MaximizeButton(true));

	prv_auiManager.AddPane(prv_wxVtkVistaSagital, wxAuiPaneInfo().
		Name(wxT("SAGITAL_VIEW")).Caption(_("Sagital View")).
		Bottom().Position(2).Layer(0).PinButton(true).
		MinSize(wxSize(200,200)).PaneBorder(true).
		CloseButton(false).MaximizeButton(true));

	prv_auiManager.AddPane(prv_wxVtkVista3D, wxAuiPaneInfo().
		Name(wxT("3D_VIEW")).Caption(_("3D View")).
		Center().Layer(0).PinButton(true).
		MinSize(wxSize(200,200)).PaneBorder(true).
		CloseButton(false).MaximizeButton(true));

	prv_auiManager.Update();
	prv_camtracking=1;
	prv_updatePlanes=0;
	prv_fpsflag = 0;
}

//*****************************************************************************************
//		DESTRUCTOR
//*****************************************************************************************
nkVolViewer::~nkVolViewer(){
	prv_vistaAxial->Detach();
	prv_vistaCoronal->Detach();
	prv_vistaSagital->Detach();
	prv_vista3D->Detach();

	if (prv_camera != 0) prv_camera->Delete();
	if (prv_render3D != 0) prv_render3D->Delete();

	prv_vistaAxial->Delete();
	prv_vistaCoronal->Delete();
	prv_vistaSagital->Delete();
	prv_vista3D->Delete();
  
	prv_wxVtkVistaAxial->Delete();
	prv_wxVtkVistaCoronal->Delete();
	prv_wxVtkVistaSagital->Delete();
	prv_wxVtkVista3D->Delete();

	prv_auiManager.UnInit();
}

void nkVolViewer::setNombreArchivo(wxString nombreArchivo){
	this->prv_nombreArchivo = nombreArchivo;
}

wxString nkVolViewer::getNombreArchivo(void){
	return this->prv_nombreArchivo;
}

void nkVolViewer::setImagen(itk::Image<unsigned short,3>::Pointer una_imagen){
	this->prv_imagen = una_imagen;
}

itk::Image<unsigned short,3>::Pointer nkVolViewer::getImagen(void){
	return this->prv_imagen;
}

vtkImageData* nkVolViewer::getVtkImagen(void){
	return this->prv_vista3D->GetImage();
}

void nkVolViewer::Configurar(void){
	prv_vistaAxial->SetInteractionStyle (vtkViewImage2D::SELECT_INTERACTION);
	prv_vistaAxial->SetWheelInteractionStyle(vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaAxial->SetMiddleButtonInteractionStyle (vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaAxial->SetRightButtonInteractionStyle (vtkViewImage2D::WINDOW_LEVEL_INTERACTION);

	prv_vistaCoronal->SetInteractionStyle (vtkViewImage2D::SELECT_INTERACTION);
	prv_vistaCoronal->SetWheelInteractionStyle(vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaCoronal->SetMiddleButtonInteractionStyle (vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaCoronal->SetRightButtonInteractionStyle (vtkViewImage2D::WINDOW_LEVEL_INTERACTION);

	prv_vistaSagital->SetInteractionStyle (vtkViewImage2D::SELECT_INTERACTION);
	prv_vistaSagital->SetWheelInteractionStyle(vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaSagital->SetMiddleButtonInteractionStyle (vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaSagital->SetRightButtonInteractionStyle (vtkViewImage2D::WINDOW_LEVEL_INTERACTION);

	prv_vistaAxial->SetOrientation (vtkViewImage2D::AXIAL_ID);
	prv_vistaCoronal->SetOrientation (vtkViewImage2D::CORONAL_ID);
	prv_vistaSagital->SetOrientation (vtkViewImage2D::SAGITTAL_ID);

	prv_vistaAxial->ScalarBarVisibilityOn();
	prv_vistaCoronal->ScalarBarVisibilityOn();
	prv_vistaSagital->ScalarBarVisibilityOn();
	prv_vista3D->ScalarBarVisibilityOn();

	prv_vistaAxial->SetBackgroundColor (0.0,0.0,0.0);
	prv_vistaCoronal->SetBackgroundColor (0.0,0.0,0.0);
	prv_vistaSagital->SetBackgroundColor (0.0,0.0,0.0);
  
	double color[3] = {0.0,0.0,0.0};
	prv_vista3D->SetTextColor (color);
	prv_vista3D->SetRenderingModeToPlanar();

	prv_vistaAxial->AddChild (prv_vistaCoronal);
	prv_vistaAxial->AddChild (prv_vista3D);
	prv_vistaCoronal->AddChild (prv_vistaSagital);
	prv_vistaSagital->AddChild (prv_vistaAxial); 
}

void nkVolViewer::configurarITKimage(
		wxString un_nombreArchivo, 
		itk::Image<unsigned short,3>::Pointer una_imagen){
	this->prv_nombreArchivo = un_nombreArchivo;
	this->prv_imagen = una_imagen;
	this->prv_vistaAxial->SetITKImage (this->prv_imagen);
	this->prv_vistaCoronal->SetITKImage ( this->prv_imagen);
	this->prv_vistaSagital->SetITKImage ( this->prv_imagen);
	this->prv_vista3D->SetITKImage ( this->prv_imagen);
	this->prv_vistaAxial->SyncResetCurrentPoint();
	this->prv_vistaAxial->SyncResetWindowLevel();
	this->BoundingBox();
}

void nkVolViewer::abrirArchivo(wxString nombreArchivo){
	itk::ImageFileReader<nkVolViewer::ImageType>::Pointer reader;
	reader = itk::ImageFileReader<nkVolViewer::ImageType>::New();
	itk::AnalyzeImageIOFactory::RegisterOneFactory();
	reader->SetFileName (nombreArchivo.c_str());
	try	{
		reader->Update();
	}catch (itk::ExceptionObject & e){
		std::cerr << e;
		wxMessageDialog* myDialog = new wxMessageDialog(this, _(e.GetDescription()),
                                                    wxT ("Error"),
                                                    wxOK|wxICON_ERROR);
		myDialog->ShowModal();
		myDialog->Destroy();
		return;
	}

	this->configurarITKimage(nombreArchivo, reader->GetOutput());
}

void nkVolViewer::abrirArchivo_vol(wxString nombreArchivo){
	FILE *fp;
	int x[3];
	float s[2];
	fp = fopen(nombreArchivo.c_str(),"rb");
	setNombreArchivo(nombreArchivo);
	fread(x,4,3,fp);
	#ifndef VTK_WORDS_BIGENDIAN
		vtkByteSwap::SwapVoidRange(x,3,4);
	#endif
	fread(s,4,2,fp);
	#ifndef VTK_WORDS_BIGENDIAN
		vtkByteSwap::SwapVoidRange(s,2,4);
	#endif
	fclose(fp);

	const int Dimension = 3;
	itk::RawImageIO<nkVolViewer::PixelType, Dimension>::Pointer rawImageIO;
	rawImageIO = itk::RawImageIO<nkVolViewer::PixelType, Dimension>::New();
	rawImageIO->SetHeaderSize(20); 
	rawImageIO->SetFileDimensionality(Dimension); 
	rawImageIO->SetByteOrderToBigEndian();
	rawImageIO->SetNumberOfComponents(1);
	rawImageIO->SetOrigin( 0, 0.0 );
	rawImageIO->SetOrigin( 1, 0.0 ); 
	rawImageIO->SetOrigin( 2, 0.0 );
	rawImageIO->SetDimensions( 0, x[0]); 
	rawImageIO->SetDimensions( 1, x[1]); 
	rawImageIO->SetDimensions( 2, x[2]); 
	rawImageIO->SetSpacing( 0,  s[0] );
	rawImageIO->SetSpacing( 1,  s[0] );
	rawImageIO->SetSpacing( 2,  s[1] );

	itk::ImageFileReader<nkVolViewer::ImageType>::Pointer reader;
	reader = itk::ImageFileReader<ImageType>::New();
	reader->SetImageIO( rawImageIO ); 
	reader->SetFileName( nombreArchivo.c_str()); 

	try{
		reader->Update();
	}catch (itk::ExceptionObject & e){
		std::cerr << e;
		wxMessageDialog* myDialog = new wxMessageDialog(this, _(e.GetDescription()),
                                                    wxT ("Error"),
                                                    wxOK|wxICON_ERROR);
		myDialog->ShowModal();
		myDialog->Destroy();
		return;
	}

	this->configurarITKimage(nombreArchivo, reader->GetOutput());
}

void nkVolViewer::abrirArchivo_dicom(wxString nombreArchivo, wxVtkDICOMImporter* myimporter, int un_index){

	#ifdef __WXMAC__
		typedef itk::GDCMImporter::FloatImageType   DImageType; // vtkInria3D anterior
	#else //win and unix
		typedef itk::GDCMImporter::ImageType   DImageType; // vtkInria3D mas reciente
	#endif
	

	itk::RescaleIntensityImageFilter<DImageType, nkVolViewer::ImageType>::Pointer rescaler=
	itk::RescaleIntensityImageFilter<DImageType, nkVolViewer::ImageType>::New();
	rescaler->SetOutputMinimum ( 0 );
	rescaler->SetOutputMaximum ( 65535 );
	rescaler->SetInput ( myimporter->GetOutput(un_index) );
	try{
		rescaler->Update();
	}catch (itk::ExceptionObject &e){
		std::cerr << e;
		return;
	}
	this->prv_vista3D->SetShift ( rescaler->GetShift() );
	this->prv_vista3D->SetScale ( rescaler->GetScale() );
	this->configurarITKimage(nombreArchivo, rescaler->GetOutput());
}

void nkVolViewer::cambiarPaletaColor(vtkLookupTable* una_paleta){
	if( !una_paleta)
		return;
	prv_vistaAxial->SyncSetLookupTable (una_paleta);
	prv_vistaAxial->Render();
	prv_vistaCoronal->Render();
	prv_vistaSagital->Render();
	prv_vista3D->Render();
}

void nkVolViewer::cambiarFormaDeProcesamiento (int un_modo, int textura_o_mrc){
	prv_vista3D->SetRenderingMode(un_modo);
	if (un_modo == vtkViewImage3D::VOLUME_RENDERING){
		switch(textura_o_mrc ){
			case 1:
				prv_vista3D->SetVolumeMapperToTexture();
				break;
			case 2:
				prv_vista3D->SetVolumeMapperToRayCast();
				prv_vista3D->SetVolumeRayCastFunctionToMIP();
				break;						
			case 3:
				prv_vista3D->SetVolumeMapperToRayCast();
				prv_vista3D->SetVolumeRayCastFunctionToComposite();
				break;
			case 4:
				prv_vista3D->SetVolumeMapperToRayCast();
				prv_vista3D->SetVolumeRayCastFunctionToIsosurface();
				break;
		}
	}
	prv_vista3D->Render();
}

void nkVolViewer::guardarArchivo(wxString nombreArchivo){
	itk::VTKImageToImageFilter<nkVolViewer::ImageType>::Pointer myConverter =
    itk::VTKImageToImageFilter<nkVolViewer::ImageType>::New();
    
	vtkImageData* image = vtkImageData::New();
	image->DeepCopy(prv_vistaAxial->GetImage());

	try	{
		myConverter->SetInput( image );
		myConverter->Update();
	}catch (itk::ExceptionObject & e){
		std::cerr << e;
		return;
	}

	itk::Matrix<double,3,3> cosines;
	cosines[0][0]= 1;
	cosines[0][1]= 0;
	cosines[0][2]= 0;
	cosines[1][0]= 0;
	cosines[1][1]=-1;
	cosines[1][2]= 0;
	cosines[2][0]= 0;
	cosines[2][1]= 0;
	cosines[2][2]= 1;

	ImageType::Pointer itkimage = const_cast<ImageType*>(myConverter->GetOutput());
	itkimage->SetDirection(cosines);

	itk::ImageFileWriter<ImageType>::Pointer writer
	= itk::ImageFileWriter<ImageType>::New();
	writer->SetFileName (nombreArchivo);
	writer->SetInput(itkimage);

	try{
		std::cout << "writing : " << nombreArchivo << std::endl;
		writer->Update();
	}catch (itk::ExceptionObject & e){
		std::cerr << e;
		if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(e.GetDescription());
		return;
	}  
	image->Delete();
}

void nkVolViewer::BoundingBox()
{
	vtkOutlineFilter *boundingBox = vtkOutlineFilter::New(); //! Bounding Box creation
	boundingBox->SetInput(this->getVtkImagen());

	vtkPolyDataMapper *prv_bboxMapper = vtkPolyDataMapper::New(); //! Bounding Box mapper
	prv_bboxMapper->SetInput(boundingBox->GetOutput());

	prv_bboxActor = vtkActor::New(); //! Bounding Box actor
	prv_bboxActor->SetMapper(prv_bboxMapper);
	prv_bboxActor->GetProperty()->SetColor(1,0.1,0.1); //! Bounding Box color
	
	prv_render3D->AddActor(prv_bboxActor);
	prv_bboxActor->VisibilityOff();

}

void nkVolViewer::BoundingBoxOnOff()
{
	if(prv_bboxActor->GetVisibility()==false )
		prv_bboxActor->VisibilityOn();
	else 
		prv_bboxActor->VisibilityOff();

	prv_render3D->Render();
	prv_wxVtkVista3D->Render();
	prv_wxVtkVista3D->Refresh();
}

void nkVolViewer::BoxWidgetOnOff()
{
	if( prv_vista3D->GetBoxWidgetVisibility() )
		prv_vista3D->BoxWidgetOff();
	else
		prv_vista3D->BoxWidgetOn();

	prv_render3D->Render();
	prv_wxVtkVista3D->Render();
	prv_wxVtkVista3D->Refresh();
}

void nkVolViewer::reiniciarNiveleseDePaleta(void){
	prv_vistaAxial->ResetWindowLevel();
	prv_vistaCoronal->ResetWindowLevel();
	prv_vistaSagital->ResetWindowLevel();
	prv_vista3D->ResetWindowLevel();
	prv_vistaAxial->Render();
	prv_vistaCoronal->Render();
	prv_vistaSagital->Render();
	prv_vista3D->Render();
}

double nkVolViewer::obtenerValorActualDeContorno(void){
	return prv_vistaAxial->GetCurrentPointDoubleValue();
}

//*****************************************************************************************
//		Area of axial image
//*****************************************************************************************
void nkVolViewer::Area(void){
	//Area calculation
	int p_point[3];

	prv_vista3D->GetCurrentVoxelCoordinates (p_point);

	char temp[100]="";
	sprintf(temp,"Coordinate cursor = (%d,%d,%d)\n",p_point[0],p_point[1],p_point[2]);
	if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(temp);

	double area=0;

	typedef itk::ImageRegionConstIterator< nkVolViewer::ImageType > ConstIteratorType;			
	ConstIteratorType iterator( prv_imagen, prv_imagen->GetBufferedRegion().GetSize() );

	for(iterator.GoToBegin();!iterator.IsAtEnd();++iterator)
	{
		nkVolViewer::ImageType::IndexType pixelIndex;
		pixelIndex = iterator.GetIndex();

		if( iterator.Get()>0 &&  pixelIndex[2]==p_point[2])
			area++;
	}

	nkVolViewer::ImageType::SpacingType spacing;
	spacing = prv_imagen->GetSpacing();
	
	sprintf(temp,"\nPixels > 0 = %f\n",area);
	if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(temp);

	area*= spacing[0]*spacing[1];
	sprintf(temp,"Z =(%d) Area = (%f)\nSpacing=(%f,%f,%f)",p_point[2],area,spacing[0],spacing[1],spacing[2]);
	if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(temp);
}


//*****************************************************************************************
//		ACTIVE STEREO
//*****************************************************************************************
void nkVolViewer::StActivo(void){

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
void nkVolViewer::StPasivo(void){

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
void nkVolViewer::StAumentar( void )
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
void nkVolViewer::StDisminuir( void )
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
void nkVolViewer::NavResetCamara( void )
{
	prv_render3D->ResetCamera();
	prv_wxVtkVista3D->Render();
	prv_wxVtkVista3D->Refresh();

}
//*****************************************************************************************
//		MENU -> NAVIGATION -> TRACKBALL
//*****************************************************************************************
void nkVolViewer::NavTrackball( )
{
	vtkInteractorStyleTrackballCamera *l_style = vtkInteractorStyleTrackballCamera::New();
	prv_wxVtkVista3D->SetInteractorStyle(l_style);
	l_style->Delete();
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> JOYSTICK
//*****************************************************************************************
void nkVolViewer::NavJoystick( )
{
	vtkInteractorStyleJoystickCamera *l_style = vtkInteractorStyleJoystickCamera::New();
	prv_wxVtkVista3D->SetInteractorStyle(l_style);
	l_style->Delete();
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> FLIGHT
//*****************************************************************************************
void nkVolViewer::NavFlight(  )
{
	vtkInteractorStyleFlight  *l_style = vtkInteractorStyleFlight ::New();
	prv_wxVtkVista3D->SetInteractorStyle(l_style);
	l_style->Delete();
}
//*****************************************************************************************
//		MENU -> NAVIGATION -> UNICAM
//*****************************************************************************************
void nkVolViewer::NavUnicam( ) 
{
	vtkInteractorStyleUnicam *l_style = vtkInteractorStyleUnicam::New();
	l_style->SetWorldUpVector(0.0, 0.0, 1.0);
	prv_wxVtkVista3D->SetInteractorStyle(l_style);
	l_style->Delete();
}
//*****************************************************************************************
//		FILTER -> GAUSSIAN
//*****************************************************************************************
void nkVolViewer::FilVolGaussian( wxAuiNotebook * p_libro ) 
{
	wxString etiquetas[100];
	const int num_datos=2;
	etiquetas[0] = _("Variance"); 
	etiquetas[1] = _("Kernel radius");
	
	nkIODialog * miDlg = new nkIODialog(	this, 
												etiquetas,
												num_datos,
												-1,
												_("Nukak3D: Gauusian filter"),
												wxDefaultPosition,
												wxSize(330,(num_datos+4)*20+40));
	
	miDlg->cambiarValor(wxT("1.0"),0);
	miDlg->cambiarValor(wxT("5"),1);
					
	miDlg->ShowModal(); 
	
	if(miDlg->GetReturnCode() == wxID_OK)
	{	
		wxBeginBusyCursor();
		
		double datos[num_datos]; 

		for(int i=0;i<num_datos;i++)
			(miDlg->obtenerValor(i)).ToDouble(&datos[i]);
						
		typedef    float    InternalPixelType;
		typedef itk::Image< InternalPixelType,  3 >   InternalImageType;

		typedef itk::CastImageFilter<nkVolViewer::ImageType,InternalImageType>
					CastFilterInType;
		CastFilterInType::Pointer mi_castUnShortToFloat = CastFilterInType::New();

		typedef itk::CastImageFilter<InternalImageType, nkVolViewer::ImageType>
					CastFilterOutType;
		CastFilterOutType::Pointer prv_castOutLevetSet = CastFilterOutType::New();


		typedef itk::DiscreteGaussianImageFilter< 
					InternalImageType,
					InternalImageType > FilterType;
		FilterType::Pointer filter = FilterType::New();
	
		mi_castUnShortToFloat->SetInput(prv_imagen);
		filter->SetVariance( datos[0] );
		filter->SetFilterDimensionality(3);
		filter->SetMaximumKernelWidth((int)datos[1]);
		filter->SetInput(mi_castUnShortToFloat->GetOutput());
		prv_castOutLevetSet->SetInput ( filter->GetOutput() );

		try
		{					
			prv_castOutLevetSet->Update();
			
			nkVolViewer * mivol = new nkVolViewer(p_libro);
			mivol->Configurar();
			mivol->configurarITKimage(_T("Gaussian filter"),prv_castOutLevetSet->GetOutput());
			p_libro->AddPage(mivol, _T("Gaussian filter"),true );

		}
		catch( itk::ExceptionObject & excep )
		{
			std::cerr << "Exception caught !" << std::endl;
			std::cerr << excep << std::endl;	
			if(mensajes) vtkOutputWindow::GetInstance()->DisplayText("Exception caught !\n");
			if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(excep.GetDescription());

		}
		wxEndBusyCursor();
	}
	delete miDlg;
}
//*****************************************************************************************
//		FILTER -> MEDIAN
//*****************************************************************************************
void nkVolViewer::FilVolMedian( wxAuiNotebook * p_libro ) 
{
	// Captura de parametros
	wxString etiquetas[100];
	const int num_datos=3;

	etiquetas[0] = _("Kernel radius in X"); 
	etiquetas[1] = _("Kernel radius in Y");
	etiquetas[2] = _("Kernel radius in Z");
	
	nkIODialog * miDlg = new nkIODialog(	this, 
												etiquetas,
												num_datos,
												-1,
												_("Nukak3D: Median Filter"),
												wxDefaultPosition,
												wxSize(330,(num_datos+4)*20+40));
	
	miDlg->cambiarValor(wxT("1"),0);
	miDlg->cambiarValor(wxT("1"),1);
	miDlg->cambiarValor(wxT("1"),2);
					
	miDlg->ShowModal(); 
	
	double datos[num_datos]; 

	if(miDlg->GetReturnCode() == wxID_OK)
	{	
		wxBeginBusyCursor();

		for(int i=0;i<2;i++)
			(miDlg->obtenerValor(i)).ToDouble(&datos[i]);
		
		typedef itk::MedianImageFilter< 
					nkVolViewer::ImageType,
					nkVolViewer::ImageType > FilterType;
		FilterType::Pointer filter = FilterType::New();

		ImageType::SizeType indexRadius;
		indexRadius[0] = datos[0]; // radius along x
		indexRadius[1] = datos[1]; // radius along y
		indexRadius[2] = datos[2]; // radius along z
		
		filter->SetRadius( indexRadius );
		filter->SetInput(prv_imagen);

		try
		{	
			filter->Update();
			
			nkVolViewer * mivol = new nkVolViewer(p_libro);
			mivol->Configurar();
			mivol->configurarITKimage(_T("Filtro mediana"),filter->GetOutput());
			p_libro->AddPage(mivol, _T("Filtro mediana"),true );

		}
		catch( itk::ExceptionObject & excep )
		{
			std::cerr << "Exception caught !" << std::endl;
			std::cerr << excep << std::endl;
			if(mensajes) vtkOutputWindow::GetInstance()->DisplayText("Exception caught !\n");
			if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(excep.GetDescription());
		}
		wxEndBusyCursor();
	}
	delete miDlg;
}
//*****************************************************************************************
//		Gradient Filter
//*****************************************************************************************
void nkVolViewer::FilVolGradient( ) 
{

}
//*****************************************************************************************
//		Threshold filter
//*****************************************************************************************
void nkVolViewer::FilVolThreshold( ) 
{

}
//*****************************************************************************************
//		Rasterizer window
//*****************************************************************************************
vtkWindowToImageFilter* nkVolViewer::Snapshot( int p_screen ) 
{
	//! Save an window snapshot
	vtkWindowToImageFilter* l_w2i = vtkWindowToImageFilter::New();
	
	if(p_screen == 0 )
		l_w2i->SetInput(prv_wxVtkVista3D->GetRenderWindow());
	else if(p_screen == 1 )
		l_w2i->SetInput(prv_wxVtkVistaAxial->GetRenderWindow());
	else if(p_screen == 2 )
		l_w2i->SetInput(prv_wxVtkVistaCoronal->GetRenderWindow());
	else if(p_screen == 3 )
		l_w2i->SetInput(prv_wxVtkVistaSagital->GetRenderWindow());

	l_w2i->Update();

	return l_w2i;
}
//*****************************************************************************************
//		VIDEOCARD INFORMATION
//*****************************************************************************************
wxString nkVolViewer::VideoCard( ) 
{
	const char* l_ren  = l_ren = prv_wxVtkVista3D->GetRenderWindow()->ReportCapabilities(); //! Capturing Render capabilities from RenderWindow
	wxString l_text(l_ren,wxConvUTF8);
	return l_text;	
}
//*****************************************************************************************
//		NUEVO LEVELSETS
//*****************************************************************************************
void nkVolViewer::NuevoLevelSets(wxAuiNotebook * p_libro){
	nkLevelSets * miLS = new nkLevelSets(this, p_libro);
	miLS->SetInput(prv_imagen);
	miLS->Configurar();
	miLS->CrearAsistente();
	miLS->WriteGradientImage();
	if(miLS->ConfigurarLevelSet()){
		miLS->UpdateLevelSets();
	}
}

//*****************************************************************************************
//		PlanStackChange Execute
//*****************************************************************************************
void nkVolViewer::nkCameraCallback::Execute(vtkObject *p_Caller, unsigned long p_EventId, void *p_CallData)
{
	if(!prv_state)
		return;

	char l_temp[100]="";
	double l_camerapos[3];
	double l_cameradir[3];
	int l_voxel[3];
	unsigned short *l_pointer;
	unsigned short l_value;

	// Check camera position 
	prv_camera->GetPosition(l_camerapos);
	prv_camera->GetDirectionOfProjection(l_cameradir);

	//Colisión
	if( l_camerapos[0]>=prv_bounds[0] && l_camerapos[0]<prv_bounds[1] &&
	    l_camerapos[1]>=prv_bounds[2] && l_camerapos[1]<prv_bounds[3] &&
		l_camerapos[2]>=prv_bounds[4] && l_camerapos[2]<prv_bounds[5] )
	{
		if(mensajes) vtkOutputWindow::GetInstance()->DisplayText("\nInside volume\n");
	
		//Camera global coordinates in voxel coordinates
		l_voxel[0]=(int)( prv_imagesize[0]*l_camerapos[0]/(prv_bounds[1]-prv_bounds[0]) );
		l_voxel[1]=(int)( prv_imagesize[1]*l_camerapos[1]/(prv_bounds[3]-prv_bounds[2]) );
		l_voxel[2]=(int)( prv_imagesize[2]*l_camerapos[2]/(prv_bounds[5]-prv_bounds[4]) );

		//Voxel value
		l_pointer = (unsigned short *)prv_imagen->GetScalarPointer(l_voxel[0],l_voxel[1],l_voxel[2]);
		l_value = l_pointer[0];
	
		if( l_value>=prv_umbral ) // umbral value detection
		{			

			prv_colision++;
			// Colision detection
			sprintf(l_temp,"Collision = %d\n",prv_colision);
			if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(l_temp);

			// Display camera coordinates
			sprintf(l_temp,"Coor( %f | %f | %f )  ",l_camerapos[0],l_camerapos[1],l_camerapos[2]);
			if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(l_temp);
			sprintf(l_temp,"Dir( %f | %f | %f )\n",l_cameradir[0],l_cameradir[1],l_cameradir[2]);
			if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(l_temp);

			// Display voxel coordinates and value
			sprintf(l_temp,"Vox( %d | %d | %d ) = %d  \n\n",l_voxel[0],l_voxel[1],l_voxel[2],l_value);
			if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(l_temp);
			
			// IN this point the algorithm must take and action
			// stop forward camera movement and return to previous

			if(prv_colision)
			{
				if(mensajes) vtkOutputWindow::GetInstance()->DisplayText("Camera stopped");
				prv_camera->SetPosition(prv_camerapos);
			}
			
		}
		else
		{
			prv_colision=0;
			//Store last valid camera position
			prv_camerapos[0]=l_camerapos[0];
			prv_camerapos[1]=l_camerapos[1];
			prv_camerapos[2]=l_camerapos[2];
		}
		
		if(prv_updatePlanes)
		{
			//Set orthogonal planes acording camera position
			prv_window->prv_vistaAxial->SetZSlice(l_voxel[2]);
			prv_window->prv_vistaCoronal->SetZSlice(l_voxel[1]);
			prv_window->prv_vistaSagital->SetZSlice(l_voxel[0]);
		}

	}
	
}
//*****************************************************************************************
//		Posición de la camara
//*****************************************************************************************
void nkVolViewer::CameraPos() 
{	
	if(prv_camtracking)
	{		
		//Dialogo para solicitar umbral de colisión
		wxString etiquetas[100];
		const int num_datos=1;
		etiquetas[0] = _("Collision threshold"); 

		nkIODialog * miDlg = new nkIODialog(	this, 
												etiquetas,
												num_datos,
												-1,
												_("Nukak3D: Collision detection"),
												wxDefaultPosition,
												wxSize(330,(num_datos+4)*20+40));

		wxString valor = wxString::Format("%f",this->obtenerValorActualDeContorno());
		miDlg->cambiarValor(valor,0);

			
		miDlg->ShowModal(); // Mostrar dialogo

		if(miDlg->GetReturnCode() == wxID_OK)
		{
			wxBeginBusyCursor();
	
			long datos; // Arreglo para almacenar los datos		
			(miDlg->obtenerValor(0)).ToLong(&datos);

			//Llamada al callback de la camara para detectar colisiones
			prv_cameracallback = nkCameraCallback::New();
			prv_cameracallback->SetUmbral((unsigned short)datos);
			prv_cameracallback->SetState(prv_camtracking);
			prv_cameracallback->SetCameraWindow(prv_camera,prv_camerapos);
			prv_cameracallback->SetVista3D(this, prv_vista3D);
			prv_cameracallback->SetImage(getVtkImagen(),prv_wxVtkVista3D);	
			SetCameraEvent(prv_cameracallback);
			prv_cameracallback->Delete();

			if(mensajes) vtkOutputWindow::GetInstance()->DisplayText("Collision activated\n");
			
			// coordenadas del bounding box de la imagen
			double bounds[6];
			prv_vista3D->GetVolumeActor()->GetBounds(bounds);

			char temp[100]="";
			sprintf(temp,"Bounding Box %f  %f   %f\n",bounds[1],bounds[3],bounds[5]);
			if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(temp);						

			wxEndBusyCursor();
		}
		delete miDlg;
		prv_camtracking=0; // Camera tracking flag

	}
	else{
		if(mensajes) vtkOutputWindow::GetInstance()->DisplayText("Collision deactivated\n");
		prv_cameracallback->SetState(prv_camtracking);
		prv_camtracking=1; // Camera tracking flag
		
	}

}
//*****************************************************************************************
//		Agregar un observador a la cámara para monitorear sus eventos
//*****************************************************************************************
void nkVolViewer::SetCameraEvent(vtkCallbackCommand* p_Event)
{
	prv_camera->AddObserver(vtkCommand::AnyEvent ,p_Event);

}
//*****************************************************************************************
//		Estilo de navegación tipo endoscopio
//*****************************************************************************************
void nkVolViewer::NavEndoscope( )
{
	nkInteractorStyleEndoCamera *l_style = nkInteractorStyleEndoCamera::New();
	prv_wxVtkVista3D->SetInteractorStyle(l_style);
	l_style->Delete();
}
//*****************************************************************************************
//		Actualización de los planos de image
//*****************************************************************************************
void nkVolViewer::NavUpdatePlanes()
{
	if(prv_camtracking){
		if(prv_updatePlanes)
		{
			prv_updatePlanes=0;
			prv_cameracallback->SetUpdatePlanes(prv_updatePlanes);			
		}
		else{
			prv_updatePlanes=1;
			prv_cameracallback->SetUpdatePlanes(prv_updatePlanes);			
		}
	}
}
//*****************************************************************************************
//		Actualización de los planos de image
//*****************************************************************************************
void nkVolViewer::FPS()
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