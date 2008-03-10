#include "nkVolViewer.h"
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
													wxT ("wxVtk vista Axial"));

	prv_wxVtkVistaCoronal = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													wxDefaultSize,
													viewStyle,
													wxT ("wxVtk vista Coronal"));

	prv_wxVtkVistaSagital = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													wxDefaultSize,
													viewStyle,
													wxT ("wxVtk vista Sagital"));

	prv_wxVtkVista3D = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													wxDefaultSize,
													viewStyle,
													wxT ("wxVtk vista 3D"));

	
	prv_wxVtkVista3D->GetRenderWindow()->SetStereoCapableWindow(1); //! Init stereo window

	prv_vistaAxial   = vtkViewImage2DWithTracer::New();
	prv_vistaCoronal = vtkViewImage2DWithTracer::New();
	prv_vistaSagital = vtkViewImage2DWithTracer::New();
	prv_vista3D      = vtkViewImage3D::New();

	vtkRenderer* mi_renderAxial   = vtkRenderer::New();
	vtkRenderer* mi_renderCoronal = vtkRenderer::New();
	vtkRenderer* mi_renderSagital = vtkRenderer::New();
	mi_render3D      = vtkRenderer::New();

	prv_camera = vtkCamera::New();
	mi_render3D->SetActiveCamera(prv_camera);

	prv_wxVtkVistaAxial->GetRenderWindow()->AddRenderer   (mi_renderAxial);
	prv_wxVtkVistaCoronal->GetRenderWindow()->AddRenderer (mi_renderCoronal);
	prv_wxVtkVistaSagital->GetRenderWindow()->AddRenderer (mi_renderSagital);
	prv_wxVtkVista3D->GetRenderWindow()->AddRenderer      (mi_render3D);

	prv_vistaAxial->SetRenderWindow   (prv_wxVtkVistaAxial->GetRenderWindow());
	prv_vistaCoronal->SetRenderWindow (prv_wxVtkVistaCoronal->GetRenderWindow());
	prv_vistaSagital->SetRenderWindow (prv_wxVtkVistaSagital->GetRenderWindow());
	prv_vista3D->SetRenderWindow      (prv_wxVtkVista3D->GetRenderWindow());

	prv_vistaAxial->SetRenderer   (mi_renderAxial);
	prv_vistaCoronal->SetRenderer (mi_renderCoronal);
	prv_vistaSagital->SetRenderer (mi_renderSagital);
	prv_vista3D->SetRenderer      (mi_render3D);
	  
	

	mi_renderAxial->Delete();
	mi_renderCoronal->Delete();
	mi_renderSagital->Delete();
	

	prv_auiManager.AddPane(prv_wxVtkVistaAxial, wxAuiPaneInfo().
		Name(wxT("VISTA_AXIAL")).Caption(wxT("Vista Axial")).
		Bottom().Position(0).Layer(0).PinButton(true).
		MinSize(wxSize(200,200)).PaneBorder(true).
        CloseButton(false).MaximizeButton(true));

	prv_auiManager.AddPane(prv_wxVtkVistaCoronal, wxAuiPaneInfo().
		Name(wxT("VISTA_CORONAL")).Caption(wxT("Vista Coronal")).		
		Bottom().Position(1).Layer(0).PinButton(true).
		MinSize(wxSize(200,200)).PaneBorder(true).
        CloseButton(false).MaximizeButton(true));

	prv_auiManager.AddPane(prv_wxVtkVistaSagital, wxAuiPaneInfo().
		Name(wxT("VISTA_SAGITAL")).Caption(wxT("Vista Sagital")).
		Bottom().Position(2).Layer(0).PinButton(true).
		MinSize(wxSize(200,200)).PaneBorder(true).
		CloseButton(false).MaximizeButton(true));

	prv_auiManager.AddPane(prv_wxVtkVista3D, wxAuiPaneInfo().
		Name(wxT("VISTA_3D")).Caption(wxT("Vista 3D")).
		Center().Layer(0).PinButton(true).
		MinSize(wxSize(200,200)).PaneBorder(true).
		CloseButton(false).MaximizeButton(true));

	prv_auiManager.Update();
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
	if (mi_render3D != 0) mi_render3D->Delete();

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
//*****************************************************************************************
//		Establecer nombre de archivo
//*****************************************************************************************
void nkVolViewer::setNombreArchivo(wxString nombreArchivo){
	this->prv_nombreArchivo = nombreArchivo;
}
//*****************************************************************************************
//		Obtener nombre de archivo
//*****************************************************************************************
wxString nkVolViewer::getNombreArchivo(void){
	return this->prv_nombreArchivo;
}
//*****************************************************************************************
//		Establecer imagen
//*****************************************************************************************
void nkVolViewer::setImagen(itk::Image<unsigned short,3>::Pointer una_imagen){
	this->prv_imagen = una_imagen;
}
//*****************************************************************************************
//		Obtener puntero a la imagen ITK
//*****************************************************************************************
itk::Image<unsigned short,3>::Pointer nkVolViewer::getImagen(void){
	return this->prv_imagen;
}
//*****************************************************************************************
//		Obtener puntero a la imagen VTK
//*****************************************************************************************
vtkImageData* nkVolViewer::getVtkImagen(void){
	return this->prv_vista3D->GetImage();
}
//*****************************************************************************************
//		Configurar vista
//*****************************************************************************************
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
//*****************************************************************************************
//		Configurar imagen itk
//*****************************************************************************************
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
}
//*****************************************************************************************
//		Abrir Archivo
//*****************************************************************************************
void nkVolViewer::abrirArchivo(wxString nombreArchivo){
	itk::ImageFileReader<nkVolViewer::ImageType>::Pointer reader;
	reader = itk::ImageFileReader<nkVolViewer::ImageType>::New();
	itk::AnalyzeImageIOFactory::RegisterOneFactory();
	reader->SetFileName (nombreArchivo.c_str());
	try	{
		reader->Update();
	}catch (itk::ExceptionObject & e){
		std::cerr << e;
		wxMessageDialog* myDialog = new wxMessageDialog(this, wxT(e.GetDescription()),
                                                    wxT ("Error"),
                                                    wxOK|wxICON_ERROR);
		myDialog->ShowModal();
		myDialog->Destroy();
		return;
	}

	this->configurarITKimage(nombreArchivo, reader->GetOutput());
}
//*****************************************************************************************
//		Abrir archivo vol
//*****************************************************************************************
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
		wxMessageDialog* myDialog = new wxMessageDialog(this, wxT(e.GetDescription()),
                                                    wxT ("Error"),
                                                    wxOK|wxICON_ERROR);
		myDialog->ShowModal();
		myDialog->Destroy();
		return;
	}

	this->configurarITKimage(nombreArchivo, reader->GetOutput());
}
//*****************************************************************************************
//		Abrir archivo DICOM
//*****************************************************************************************
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
//*****************************************************************************************
//		CAMBIAR PALETA DE COLOR
//*****************************************************************************************
void nkVolViewer::cambiarPaletaColor(vtkLookupTable* una_paleta){
	if( !una_paleta)
		return;
	prv_vistaAxial->SyncSetLookupTable (una_paleta);
	prv_vistaAxial->Render();
	prv_vistaCoronal->Render();
	prv_vistaSagital->Render();
	prv_vista3D->Render();
}
//*****************************************************************************************
//		CAMBIAR FORMA DE RENDERING
//*****************************************************************************************
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
		}
	}
	prv_vista3D->Render();
}
//*****************************************************************************************
//		GUARDAR ARCHIVO
//*****************************************************************************************
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

//*****************************************************************************************
//		Reiniciar paleta de colores
//*****************************************************************************************
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
//*****************************************************************************************
//		Obtener valor actual de contorno
//*****************************************************************************************
double nkVolViewer::obtenerValorActualDeContorno(void){
	return prv_vistaAxial->GetCurrentPointDoubleValue();
}
//*****************************************************************************************
//		Levelsets
//*****************************************************************************************
void nkVolViewer::lsLevelsetsCompleto( wxAuiNotebook * p_libro ){
	
	// tipo de dato interno
	typedef   float           InternalPixelType;
	const     unsigned int    Dimension = 3;
	
	// imagen interna
	typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;

	// imagen de salida
	typedef itk::Image< nkVolViewer::PixelType, Dimension > OutputImageType;

	// casting de unsigned short a float
	typedef itk::CastImageFilter<nkVolViewer::ImageType,InternalImageType>
				CastFilterInType;
	CastFilterInType::Pointer castIn = CastFilterInType::New();


	// casting de float a unsigned short
	typedef itk::RescaleIntensityImageFilter<InternalImageType,nkVolViewer::ImageType>
				CastFilterOutType;
	CastFilterOutType::Pointer castOut = CastFilterOutType::New();
	CastFilterOutType::Pointer castOut2 = CastFilterOutType::New();

	// filtro gradiente para detección de bordes
	typedef   itk::GradientMagnitudeRecursiveGaussianImageFilter< 
								InternalImageType, 
								InternalImageType >  GradientFilterType;
	GradientFilterType::Pointer  gradientMagnitude = GradientFilterType::New();

	// filtro sigmoide para la función de velocidad
	typedef   itk::SigmoidImageFilter<                               
								InternalImageType, 
								InternalImageType >  SigmoidFilterType;	
	SigmoidFilterType::Pointer sigmoid = SigmoidFilterType::New();

	// funcion fastmarching para generar el levelset inicial
	typedef  itk::FastMarchingImageFilter< InternalImageType, InternalImageType >
								FastMarchingFilterType;
	FastMarchingFilterType::Pointer  fastMarching = FastMarchingFilterType::New();

	// funcion para detección de la forma del contorno de acuerdo con la función
	// de velocidad que sale de sigmoid
	typedef  itk::ShapeDetectionLevelSetImageFilter< InternalImageType, 
								InternalImageType, InternalPixelType >    ShapeDetectionFilterType;
	ShapeDetectionFilterType::Pointer shapeDetection = ShapeDetectionFilterType::New(); 
	
	// filtro binario para la salida
	typedef itk::BinaryThresholdImageFilter< InternalImageType, InternalImageType > 
								ThresholdingFilterType;
	ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();	


	// Guardar la imagen segmentada
	typedef  itk::ImageFileWriter<  nkVolViewer::ImageType  > WriterType;		
	WriterType::Pointer writer = WriterType::New();
	WriterType::Pointer writer2 = WriterType::New();
	writer->SetFileName( "segmented.vtk" );
	writer->SetInput( castOut->GetOutput() );


	// pipeline //
	castIn->SetInput( prv_imagen );
	gradientMagnitude->SetInput( castIn->GetOutput() );
	sigmoid->SetInput( gradientMagnitude->GetOutput() );	
	shapeDetection->SetInput( fastMarching->GetOutput() );
	shapeDetection->SetFeatureImage( sigmoid->GetOutput() );
	thresholder->SetInput( shapeDetection->GetOutput() );
	castOut->SetInput( thresholder->GetOutput() );
	writer->SetInput( castOut->GetOutput() );


	// Captura de parametros
	wxString etiquetas[100];
	const int num_datos=9;

	etiquetas[0] = wxT("Radio semilla"); 
	etiquetas[1] = wxT("Gaussiano - sigma");
	etiquetas[2] = wxT("Sigmoide - alpha");
	etiquetas[3] = wxT("Sigmoide - beta");
	etiquetas[4] = wxT("SetPropagationScaling");
	etiquetas[5] = wxT("SetCurvatureScaling"); 
	etiquetas[6] = wxT("SetMaximumRMSError");
	etiquetas[7] = wxT("SetNumberOfIterations"); 
	etiquetas[8] = wxT("Stopping time"); 
	

	nkIODialog * miDlg = new nkIODialog(	this, 
												etiquetas,
												num_datos,
												-1,
												wxT("Nukak3D: levelsets"),
												wxDefaultPosition,
												wxSize(330,(num_datos+4)*20+40));
	

	miDlg->cambiarValor(wxT("5.0"),0);
	miDlg->cambiarValor(wxT("1.2"),1);
	miDlg->cambiarValor(wxT("-1.0"),2);
	miDlg->cambiarValor(wxT("50.0"),3);
	miDlg->cambiarValor(wxT("1.00"),4);
	miDlg->cambiarValor(wxT("0.5"),5);
	miDlg->cambiarValor(wxT("0.02"),6);
	miDlg->cambiarValor(wxT("200"),7);
	miDlg->cambiarValor(wxT("10.0"),8);
	
					
	miDlg->ShowModal();
	
	double datos[num_datos];

	if(miDlg->GetReturnCode() == wxID_OK){
		
		wxBeginBusyCursor();
		nkObj3DViewer * miobj = new nkObj3DViewer(p_libro);
		
		for(int i=0;i<num_datos;i++)
			(miDlg->obtenerValor(i)).ToDouble(&datos[i]);


		// parámetros del filtro rescale intensity
		castOut->SetOutputMinimum ( 0 );
		castOut->SetOutputMaximum ( 255 );


		// parámetro del filtro gradiente-gaussiano
		gradientMagnitude->SetSigma( datos[1] );

		// parámetros de la función sigmoide
		sigmoid->SetOutputMinimum(  0.0  );
		sigmoid->SetOutputMaximum(  1.0  );
		sigmoid->SetAlpha( datos[2] );
		sigmoid->SetBeta(  datos[3] );	

		// parametros de la imagen de salida
		thresholder->SetLowerThreshold( itk::NumericTraits<InternalPixelType>::NonpositiveMin() );
		thresholder->SetUpperThreshold( itk::NumericTraits<InternalPixelType>::Zero );

		thresholder->SetOutsideValue(  0  );
		thresholder->SetInsideValue(  1 );

		// semilla del algoritmo
		typedef FastMarchingFilterType::NodeContainer           NodeContainer;
		typedef FastMarchingFilterType::NodeType                NodeType;
		NodeContainer::Pointer seeds = NodeContainer::New();

		int p_point[3];

		prv_vista3D->GetCurrentVoxelCoordinates (p_point);

		char temp[100]="";
		sprintf(temp,"\nCoordenadas semilla = (%d,%d,%d)\n",p_point[0],p_point[1],p_point[2]);
		if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(temp);
		sprintf(temp,"Radio semilla = %f\nSigma gaussiano = %f\nAlfa = %f\nBeta = %f\n",datos[0],datos[1],datos[2],datos[3]);
		if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(temp);
		sprintf(temp,"Propagation = %f\nCurvature = %f\nError = %f\nIteraciones = %f\n",datos[4],datos[5],datos[6],datos[7]);
		if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(temp);

		InternalImageType::IndexType  seedPosition;

		seedPosition[0] = p_point[0];
		seedPosition[1] = p_point[1];	
		seedPosition[2] = p_point[2];

		// radio de la semilla 
		NodeType node;
		double radius = datos[0];
		const double seedValue = - radius;

		node.SetValue( seedValue );
		node.SetIndex( seedPosition );
		
		// inserción de la semilla a la estructura de datos
		seeds->Initialize();
		seeds->InsertElement( 0, node );

		// Asignar las semillas al filtro fast marching
		fastMarching->SetTrialPoints(  seeds  );

		// Se define velocidad constante para el filtro fastmarching
		// porque solo se usa como generador de carta de distancias
		fastMarching->SetSpeedConstant( 1.0 );
		//fastMarching->SetStoppingValue( datos[8] );
		

		// establecimiento del tamaño de la imagen de salida
		fastMarching->SetOutputSize( 
			   prv_imagen->GetBufferedRegion().GetSize() );
		fastMarching->SetOutputSpacing(
			   prv_imagen->GetSpacing() );

		// parametros de control del filtro para detección de forma con levelsets
		shapeDetection->SetPropagationScaling(  datos[4] );
		shapeDetection->SetCurvatureScaling( datos[5] ); 
		shapeDetection->SetMaximumRMSError( datos[6] );
		shapeDetection->SetNumberOfIterations( (int)datos[7] ); 


		// Guardar la imagen fast marching
		//castOut2->SetOutputMinimum ( 0 );
		//castOut2->SetOutputMaximum ( 255 );
		//castOut2->SetInput( fastMarching->GetOutput() );
		//writer2->SetFileName( "segmented2.vtk" );
		//writer2->SetInput( castOut2->GetOutput() );

		// Guardar la funcion de velocidad
		castOut2->SetOutputMinimum ( 0 );
		castOut2->SetOutputMaximum ( 255 );
		castOut2->SetInput( sigmoid->GetOutput() );
		writer2->SetFileName( "segmented2.vtk" );
		writer2->SetInput( castOut2->GetOutput() );


		try
		{	
			writer2->Update();
			if(mensajes) vtkOutputWindow::GetInstance()->DisplayText("Fastmarching ok\n");
			writer->Update();
			if(mensajes) vtkOutputWindow::GetInstance()->DisplayText("Shape detection ok\n");
			
			nkVolViewer * mivol = new nkVolViewer(p_libro);
			mivol->Configurar();
			mivol->configurarITKimage(_T("Velocidad"),castOut2->GetOutput());
			p_libro->AddPage(mivol, _T("Velocidad"),true );

			nkVolViewer * mivol2 = new nkVolViewer(p_libro);
			mivol2->Configurar();
			mivol2->configurarITKimage(_T("Segmentación"),castOut->GetOutput());
			p_libro->AddPage(mivol2, _T("Segmentación"),true );

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
//		Area de la imagen axial
//*****************************************************************************************
void nkVolViewer::Area(void){
	//Area calculation
	int p_point[3];

	prv_vista3D->GetCurrentVoxelCoordinates (p_point);

	char temp[100]="";
	sprintf(temp,"Coordenadas cursor = (%d,%d,%d)\n",p_point[0],p_point[1],p_point[2]);
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
	
	sprintf(temp,"\nPixeles > 0 = %f\n",area);
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
	mi_render3D->ResetCamera();
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
//		FILTRADO DE VOLUMEN -> GAUSSIANO
//*****************************************************************************************
void nkVolViewer::FilVolGaussian( wxAuiNotebook * p_libro ) 
{
	// Captura de parametros
	wxString etiquetas[100];
	const int num_datos=2;
	etiquetas[0] = wxT("Varianza"); 
	etiquetas[1] = wxT("Radio del kernel");
	
	nkIODialog * miDlg = new nkIODialog(	this, 
												etiquetas,
												num_datos,
												-1,
												wxT("Nukak3D: filtro gaussiano"),
												wxDefaultPosition,
												wxSize(330,(num_datos+4)*20+40));
	
	miDlg->cambiarValor(wxT("1.0"),0);
	miDlg->cambiarValor(wxT("5"),1);
					
	miDlg->ShowModal(); // Mostrar dialogo
	
	if(miDlg->GetReturnCode() == wxID_OK)
	{	
		wxBeginBusyCursor();
		
		double datos[num_datos]; // Arreglo para almacenar los datos

		for(int i=0;i<num_datos;i++)
			(miDlg->obtenerValor(i)).ToDouble(&datos[i]);
						
		typedef    float    InternalPixelType;
		typedef itk::Image< InternalPixelType,  3 >   InternalImageType;

		typedef itk::CastImageFilter<nkVolViewer::ImageType,InternalImageType>
					CastFilterInType;
		CastFilterInType::Pointer castIn = CastFilterInType::New();

		typedef itk::CastImageFilter<InternalImageType, nkVolViewer::ImageType>
					CastFilterOutType;
		CastFilterOutType::Pointer castOut = CastFilterOutType::New();


		typedef itk::DiscreteGaussianImageFilter< 
					InternalImageType,
					InternalImageType > FilterType;
		FilterType::Pointer filter = FilterType::New();
	
		castIn->SetInput(prv_imagen);
		filter->SetVariance( datos[0] );
		filter->SetFilterDimensionality(3);
		filter->SetMaximumKernelWidth((int)datos[1]);
		filter->SetInput(castIn->GetOutput());
		castOut->SetInput ( filter->GetOutput() );

		try
		{					
			castOut->Update();
			
			nkVolViewer * mivol = new nkVolViewer(p_libro);
			mivol->Configurar();
			mivol->configurarITKimage(_T("Filtro gaussiano"),castOut->GetOutput());
			p_libro->AddPage(mivol, _T("Filtro gaussiano"),true );

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
//		FILTRADO DE VOLUMEN -> MEDIANA
//*****************************************************************************************
void nkVolViewer::FilVolMedian( wxAuiNotebook * p_libro ) 
{
	// Captura de parametros
	wxString etiquetas[100];
	const int num_datos=3;

	etiquetas[0] = wxT("Radio del kernel en X"); 
	etiquetas[1] = wxT("Radio del kernel en Y");
	etiquetas[2] = wxT("Radio del kernel en Z");
	
	nkIODialog * miDlg = new nkIODialog(	this, 
												etiquetas,
												num_datos,
												-1,
												wxT("Nukak3D: filtro mediana"),
												wxDefaultPosition,
												wxSize(330,(num_datos+4)*20+40));
	
	miDlg->cambiarValor(wxT("1"),0);
	miDlg->cambiarValor(wxT("1"),1);
	miDlg->cambiarValor(wxT("1"),2);
					
	miDlg->ShowModal(); // Mostrar dialogo
	
	double datos[num_datos]; // Arreglo para almacenar los datos

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
//		FILTRADO DE VOLUMEN -> GRADIENTE
//*****************************************************************************************
void nkVolViewer::FilVolGradient( ) 
{

}
//*****************************************************************************************
//		FILTRADO DE VOLUMEN -> UMBRAL
//*****************************************************************************************
void nkVolViewer::FilVolThreshold( ) 
{

}
//*****************************************************************************************
//		OBTENER VENTANA RASTERIZADA PARA CAPTURA DE PANTALLA
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
//		OBTENER INFORMACION DE LA TARJETA DE VIDEO
//*****************************************************************************************
wxString nkVolViewer::VideoCard( ) 
{
	const char* l_ren  = l_ren = prv_wxVtkVista3D->GetRenderWindow()->ReportCapabilities(); //! Capturing Render capabilities from RenderWindow
	wxString l_text(l_ren,wxConvUTF8);
	return l_text;	
}