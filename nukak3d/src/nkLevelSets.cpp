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

#include "nkLevelSets.h"

nkLevelSets::nkLevelSets(nkVolViewer * un_volViewer, wxAuiNotebook * p_libro){

	this->prv_nkVolViewer = un_volViewer;
	this->prv_wxAuiNotebook = p_libro;
	prv_CurrentDataSets = -1;

	prv_castUnShortToFloat = CastFilterInType::New();
	prv_castOutLevetSetVelocity = CastFilterOutType::New();
	prv_gradientMagnitude = GradientFilterType::New();
	prv_sigmoid = SigmoidFilterType::New();
	prv_fastMarching = FastMarchingFilterType::New();
	prv_castOutLevetSet = CastFilterOutType::New();
	prv_shapeDetectionLevelSet = ShapeDetectionFilterType::New();
	prv_thresholder = ThresholdingFilterType::New();
	prv_vtkToItk = ITKToVTKType::New();

	prv_iterationCommand = SimpleCommandType::New();
	prv_iterationCommand->SetCallbackFunction( this, & nkLevelSets::ActualizarSegmentacionLevelSet );
	prv_shapeDetectionLevelSet->AddObserver( itk::IterationEvent(), prv_iterationCommand );
}

void nkLevelSets::Configure(){

	prv_castUnShortToFloat->SetInput(prv_imgInput);
	prv_gradientMagnitude->SetInput(prv_castUnShortToFloat->GetOutput() );
	prv_sigmoid->SetInput(prv_gradientMagnitude->GetOutput());
	prv_castOutLevetSetVelocity->SetInput(prv_sigmoid->GetOutput());
	prv_sigmoid->SetOutputMinimum(  0.0  );
	prv_sigmoid->SetOutputMaximum(  1.0  );
}

bool nkLevelSets::ConfigureLevelSet(){
	bool ConfiguracionOK = false;
	if (ObtenerDatosLevelSet()){

		prv_isoSurfaceManager = vtkIsosurfaceManager::New();
		prv_isoSurfaceManager->SetRenderWindowInteractor ((prv_nkVVSigmoid->getVista3D())->GetRenderWindowInteractor());
		prv_isoSurfaceManager->SetDecimationOff();

		prv_shapeDetectionLevelSet->SetInput( prv_fastMarching->GetOutput() );
		prv_shapeDetectionLevelSet->SetFeatureImage( prv_sigmoid->GetOutput() );	

		prv_thresholder->SetLowerThreshold( itk::NumericTraits<PixelTypeFloat>::NonpositiveMin() );
		prv_thresholder->SetUpperThreshold( itk::NumericTraits<PixelTypeFloat>::Zero );

		prv_thresholder->SetOutsideValue(  0  );
		prv_thresholder->SetInsideValue(  1 );

		prv_castOutLevetSet->SetOutputMinimum ( 0 );
		prv_castOutLevetSet->SetOutputMaximum ( 255 );

		typedef FastMarchingFilterType::NodeContainer           NodeContainer;
		typedef FastMarchingFilterType::NodeType                NodeType;
		NodeContainer::Pointer seeds = NodeContainer::New();
		seeds->Initialize();

		std::list<std::vector<int> >::iterator mi_iteradorSemillas;
		std::vector<int> mi_vectorSemilla;

		int j = 0;
		for(mi_iteradorSemillas = pub_listSeeds.begin();
		mi_iteradorSemillas != pub_listSeeds.end();mi_iteradorSemillas++){
			mi_vectorSemilla = *mi_iteradorSemillas;
			ImageFloat3::IndexType  seedPosition;
			seedPosition[0] = mi_vectorSemilla[0];
			seedPosition[1] = mi_vectorSemilla[1];
			seedPosition[2] = mi_vectorSemilla[2];

			NodeType node;
			double radius = mi_vectorSemilla[3];
			const double seedValue = - radius;

			node.SetValue( seedValue );
			node.SetIndex( seedPosition );
			seeds->InsertElement( j, node );
			j++;
		}
		prv_fastMarching->SetTrialPoints(  seeds  );
		prv_fastMarching->SetSpeedConstant( 1.0 );
		prv_fastMarching->SetOutputSize( 
			   GetInput()->GetBufferedRegion().GetSize() );
		prv_fastMarching->SetOutputSpacing(
			   GetInput()->GetSpacing() );

		prv_shapeDetectionLevelSet->SetPropagationScaling( prv_propagationScaling );
		prv_shapeDetectionLevelSet->SetCurvatureScaling( prv_curvatureScaling ); 
		prv_shapeDetectionLevelSet->SetMaximumRMSError( 0.02 );
		prv_shapeDetectionLevelSet->SetNumberOfIterations( (int)prv_numberOfIterations );
		prv_castOutLevetSetVelocity->SetOutputMinimum ( 0 );
		prv_castOutLevetSetVelocity->SetOutputMaximum ( 255 );
		ConfiguracionOK = true;
	}
	return ConfiguracionOK;
}

void nkLevelSets::UpdateLevelSets(){
	prv_vectorDataSets.push_back((vtkImageData *)(NULL));
	prv_CurrentDataSets = (unsigned int)(prv_vectorDataSets.size());
	parar = true;
	pausar = false;
	wxp = new wxProgressDialog(wxT("Nukak3D: Level Sets"), wxT("Progeso: "), 200, 0, wxPD_CAN_ABORT |
						 wxPD_ELAPSED_TIME |
                         wxPD_ESTIMATED_TIME |
                         wxPD_REMAINING_TIME);
	try{	
		finalizo_level_set = false;
		prv_shapeDetectionLevelSet->UpdateLargestPossibleRegion();
		finalizo_level_set = true;
		nkLevelSets::ActualizarSegmentacionLevelSet();
		vtkOutputWindow::GetInstance()->DisplayText("Termino level Sets !\n");

		if(prv_shapeDetectionLevelSet->GetProgress() >0.1){
			prv_sigmoid->Update();
				
			prv_castOutLevetSet->Update();
			nkVolViewer * mivol2 = new nkVolViewer(prv_wxAuiNotebook);
			mivol2->Configure();
			mivol2->ConfigureITKimage(_T("Segmentation"),prv_castOutLevetSet->GetOutput());
			prv_wxAuiNotebook->AddPage(mivol2, _T("Segmentation"),false );
		}

	}catch( itk::ExceptionObject & excep ){
		prv_shapeDetectionLevelSet->SetNumberOfIterations(0);
		std::cerr << "Exception caught ->shape->set(0)!" << std::endl;
		std::cerr << excep << std::endl;
		if(mensajes) vtkOutputWindow::GetInstance()->DisplayText("Exception caught  ->shape->set(0)!\n");
		if(mensajes) vtkOutputWindow::GetInstance()->DisplayText(excep.GetDescription());
		delete wxp;
	}
    if(wxp)	delete wxp;
    AddOverlappingImage (prv_vtkToItk->GetOutput(), 1.0,prv_CurrentDataSets);
}

bool nkLevelSets::ObtenerDatosLevelSet(){
	bool operacionOK = false;
    wxString etiquetas[100];
	const int num_datos=3;

	etiquetas[0] = _("SetPropagationScaling");
	etiquetas[1] = _("SetCurvatureScaling"); 
	etiquetas[2] = _("SetNumberOfIterations"); 

	nkIODialog * miDlg = new nkIODialog(	0, 
												etiquetas,
												num_datos,
												-1,
												_("Nukak3D: levelsets"),
												wxDefaultPosition,
												wxSize(330,(num_datos+4)*20+40));
	miDlg->cambiarValor(wxT("1.00"),0);
	miDlg->cambiarValor(wxT("0.5"),1);
	miDlg->cambiarValor(wxT("200"),2);
					
	miDlg->ShowModal();
	double datos[num_datos];
	if(miDlg->GetReturnCode() == wxID_OK){
		operacionOK = true;
		for(int i=0;i<num_datos;i++)
			(miDlg->obtenerValor(i)).ToDouble(&datos[i]);
		prv_propagationScaling = datos[0];
		prv_curvatureScaling = datos[1];
		prv_numberOfIterations = datos[2];
	}
	delete miDlg;
	return operacionOK;
}


void nkLevelSets::SetInput(itk::Image<unsigned short,3>::Pointer an_image){
	prv_imgInput = an_image;
}

void nkLevelSets::UpdateGradient(itk::NumericTraits<float>::RealType un_sigma){
	prv_sigmaGradient = un_sigma;
	prv_gradientMagnitude->SetSigma(prv_sigmaGradient);
	prv_gradientMagnitude->Modified();
	prv_gradientMagnitude->Update();
}

void nkLevelSets::UpdateSigmoid(double un_alpha, double un_beta){
	prv_alpha = un_alpha;
	prv_beta = un_beta;
	prv_sigmoid->SetAlpha(prv_alpha);
	prv_sigmoid->SetBeta(prv_beta);
	prv_sigmoid->Modified();
	prv_sigmoid->Update();
}

itk::Image<unsigned short,3>::Pointer nkLevelSets::GetInput(){
	return prv_imgInput;
}

itk::Image<float, 3>::Pointer nkLevelSets::GetOutputGradient(){
	return prv_gradientMagnitude->GetOutput();
}

itk::Image<float, 3>::Pointer nkLevelSets::GetOutputSigmoid(){
	return prv_sigmoid->GetOutput();
}

void nkLevelSets::CrearAsistente(){
	prv_asistente = new wxWizard(prv_wxAuiNotebook, 
					-1, 
					_("Nukak3D: Level Sets Segmentation Configuration Wizard."), 
					wxBitmap(logoLS_xpm), wxDefaultPosition, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
	prv_asistente->SetPageSize(wxSize(780,600));
	wxWizardPageSimple * mi_1Page = new wxWizardPageSimple(prv_asistente);
	wxString mi_textoPage1 =	wxString(_("Level Sets Segmentation\n\n"))<<
								wxString(_("Configuration in 3 steps:\n"))<<
								wxString(_("\t1. Gradient Filter :\n"))<<
								wxString(_("\t2. Sigmoid Filter :\n"))<<
								wxString(_("\t3. Add Seeds :\n"));
	wxStaticText * mi_txt1Page = new wxStaticText(mi_1Page, wxID_ANY,
                             mi_textoPage1, wxPoint(5,5), wxSize(620,460));
	nkLevelSetsPageGradient * mi_2page = new nkLevelSetsPageGradient(prv_asistente, 
		mi_1Page, NULL, wxNullBitmap, this);
	nkLevelSetsPageSigmoid * mi_3page = new nkLevelSetsPageSigmoid(prv_asistente, 
		mi_2page, NULL, wxNullBitmap, this);
	nkLevelSetsPageSeeds * mi_4page = new nkLevelSetsPageSeeds(prv_asistente,
		mi_3page, NULL, wxNullBitmap, this);
	mi_4page->Configure();

	mi_1Page->SetNext(mi_2page);
	mi_2page->SetNext(mi_3page);
	mi_3page->SetNext(mi_4page);
	prv_asistente->RunWizard(mi_1Page);
}

void nkLevelSets::WriteGradientImage(){
	prv_castOutLevetSetVelocity->Update();
	prv_nkVVSigmoid = new nkVolViewer(prv_wxAuiNotebook);
	prv_nkVVSigmoid->Configure();
	prv_nkVVSigmoid->ConfigureITKimage(_T("Level Set Veocity"), prv_castOutLevetSetVelocity->GetOutput());
	prv_wxAuiNotebook->AddPage(prv_nkVVSigmoid, _T("Level Set Velocity"),true );
}

void nkLevelSets::ActualizarSegmentacionLevelSet(){
	if(prv_shapeDetectionLevelSet->GetProgress() >0.1){
        ImageFloat3::Pointer tmp = ImageFloat3::New();
		tmp->SetRequestedRegion( prv_shapeDetectionLevelSet->GetOutput()->GetRequestedRegion() );
		tmp->SetBufferedRegion( prv_shapeDetectionLevelSet->GetOutput()->GetBufferedRegion() );
		tmp->SetLargestPossibleRegion( prv_shapeDetectionLevelSet->GetOutput()->GetLargestPossibleRegion() );
		tmp->SetPixelContainer( prv_shapeDetectionLevelSet->GetOutput()->GetPixelContainer() );
		tmp->CopyInformation( prv_shapeDetectionLevelSet->GetOutput() );

		prv_thresholder->Modified();
		prv_thresholder->SetInput( tmp );
		prv_castOutLevetSet->SetInput( prv_thresholder->GetOutput() );

		prv_vtkToItk->Modified();
		prv_vtkToItk->SetInput (prv_castOutLevetSet->GetOutput());
		prv_vtkToItk->Update();

		SetUpdateOverlappingImage (prv_vtkToItk->GetOutput(), 0.5,prv_CurrentDataSets);
		
		if(!finalizo_level_set)	parar = wxp->Update((int)(200.0*prv_shapeDetectionLevelSet->GetProgress()));
	}
	if(!finalizo_level_set) if(!parar){
		prv_shapeDetectionLevelSet->SetNumberOfIterations(0);
		//*****wxp->Destroy();
	}
	if(finalizo_level_set){
		//****wxp->Destroy();
		//******delete wxp;
	}
}

void nkLevelSets::SetUpdateOverlappingImage(vtkImageData * una_vtkImage, double a_opacity, int un_dataSet){
	prv_isoSurfaceManager->SetInput (una_vtkImage);
	prv_vectorDataSets[prv_CurrentDataSets] = una_vtkImage;
	prv_isoSurfaceManager->GenerateData();
	prv_isoSurfaceManager->SetOpacity (a_opacity);
	if ((prv_nkVVSigmoid->getVistaAxial())->GetDataSet(un_dataSet)){
		(prv_nkVVSigmoid->getVistaAxial())->RemoveDataSet((prv_nkVVSigmoid->getVistaAxial())->GetDataSet(un_dataSet));
		(prv_nkVVSigmoid->getVistaCoronal())->RemoveDataSet((prv_nkVVSigmoid->getVistaCoronal())->GetDataSet(un_dataSet));
		(prv_nkVVSigmoid->getVistaSagital())->RemoveDataSet((prv_nkVVSigmoid->getVistaSagital())->GetDataSet(un_dataSet));
		//prv_vista3D->RemoveDataSet(prv_vista3D->GetDataSet(un_dataSet));
	}
	for( unsigned int i=0; i<prv_isoSurfaceManager->GetIsosurfaces().size(); i++){
		vtkProperty* property = vtkProperty::New();
		property->DeepCopy (prv_isoSurfaceManager->GetIsosurfaces()[i]->GetActor()->GetProperty());
		property->SetOpacity (1.0);
		property->SetAmbient (1.0);
		property->SetDiffuse (0.0);
		property->SetSpecular (0.0);
		property->SetLineWidth (3.0);
		(prv_nkVVSigmoid->getVistaAxial())->AddDataSet( prv_isoSurfaceManager->GetIsosurfaces()[0]->GetPolyData(), property );
		(prv_nkVVSigmoid->getVistaCoronal())->AddDataSet( prv_isoSurfaceManager->GetIsosurfaces()[0]->GetPolyData(), property );
		(prv_nkVVSigmoid->getVistaSagital())->AddDataSet( prv_isoSurfaceManager->GetIsosurfaces()[0]->GetPolyData(), property );
		//prv_vista3D->AddDataSet( prv_isoSurfaceManager->GetIsosurfaces()[0]->GetPolyData(), property );
		property->Delete();
	}
	
	(prv_nkVVSigmoid->getVistaAxial())->Render();
	(prv_nkVVSigmoid->getVistaCoronal())->Render();
	(prv_nkVVSigmoid->getVistaSagital())->Render();
	(prv_nkVVSigmoid->getVista3D())->Render();
}

void nkLevelSets::AddOverlappingImage(vtkImageData * una_vtkImage, double a_opacity, int un_dataSet){
	prv_isoSurfaceManager->SetInput (una_vtkImage);
	prv_vectorDataSets[prv_CurrentDataSets] = una_vtkImage;
	prv_isoSurfaceManager->GenerateData();
	prv_isoSurfaceManager->SetOpacity (a_opacity);
	if ((prv_nkVVSigmoid->getVistaAxial())->GetDataSet(un_dataSet)){
		(prv_nkVVSigmoid->getVistaAxial())->RemoveDataSet((prv_nkVVSigmoid->getVistaAxial())->GetDataSet(un_dataSet));
		(prv_nkVVSigmoid->getVistaCoronal())->RemoveDataSet((prv_nkVVSigmoid->getVistaCoronal())->GetDataSet(un_dataSet));
		(prv_nkVVSigmoid->getVistaSagital())->RemoveDataSet((prv_nkVVSigmoid->getVistaSagital())->GetDataSet(un_dataSet));
	}
	for( unsigned int i=0; i<prv_isoSurfaceManager->GetIsosurfaces().size(); i++){
		vtkProperty* property = vtkProperty::New();
		vtkProperty* property2 = vtkProperty::New();
		property->DeepCopy (prv_isoSurfaceManager->GetIsosurfaces()[i]->GetActor()->GetProperty());
		property2->SetColor(property->GetColor());
		property2->SetOpacity (a_opacity);
		property->SetOpacity (a_opacity);
		property->SetAmbient (1.0);
		property->SetDiffuse (0.0);
		property->SetSpecular (0.0);
		property->SetLineWidth (3.0);

		(prv_nkVVSigmoid->getVistaAxial())->AddDataSet( prv_isoSurfaceManager->GetIsosurfaces()[0]->GetPolyData(), property );
		(prv_nkVVSigmoid->getVistaCoronal())->AddDataSet( prv_isoSurfaceManager->GetIsosurfaces()[0]->GetPolyData(), property );
		(prv_nkVVSigmoid->getVistaSagital())->AddDataSet( prv_isoSurfaceManager->GetIsosurfaces()[0]->GetPolyData(), property );
		(prv_nkVVSigmoid->getVista3D())->AddDataSet( prv_isoSurfaceManager->GetIsosurfaces()[0]->GetPolyData(), property2 );
		property->Delete();
		property2->Delete();
	}
	prv_isoSurfaceManager->ResetData();
	
	(prv_nkVVSigmoid->getVistaAxial())->Render();
	(prv_nkVVSigmoid->getVistaCoronal())->Render();
	(prv_nkVVSigmoid->getVistaSagital())->Render();
	(prv_nkVVSigmoid->getVista3D())->Render();
}

nkLevelSetsPageGradient::nkLevelSetsPageGradient(wxWizard* parent, 
					wxWizardPage* prev, 
					wxWizardPage* next, 
					const wxBitmap& bitmap,
					nkLevelSets * un_nkLevelSets): 
	wxWizardPageSimple(parent, prev, next, bitmap){
	prv_nkLevelSets = un_nkLevelSets;
	prv_wxGSLayout = new wxGridSizer(1,3,3,3);

	long viewStyle = wxWANTS_CHARS | wxNO_FULL_REPAINT_ON_RESIZE;
	prv_wxVtkVistaOriginal = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													/*wxPoint(5,5),*/
													wxSize(256,384),
													viewStyle,
													wxT ("wxVtk Axial view"));
	prv_wxVtkVistaModificada = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													/*wxPoint(389,5),*/
													wxSize(256,384),
													viewStyle,
													wxT ("wxVtk Axial view"));
	prv_vistaOriginal   = vtkViewImage2DWithTracer::New();
	prv_vistaModificada = vtkViewImage2DWithTracer::New();
	vtkRenderer* mi_renderOriginal   = vtkRenderer::New();
	vtkRenderer* mi_renderModificada = vtkRenderer::New();
	prv_wxVtkVistaOriginal->GetRenderWindow()->AddRenderer   (mi_renderOriginal);
	prv_wxVtkVistaModificada->GetRenderWindow()->AddRenderer   (mi_renderModificada);
	prv_vistaOriginal->SetRenderWindow   (prv_wxVtkVistaOriginal->GetRenderWindow());
	prv_vistaModificada->SetRenderWindow   (prv_wxVtkVistaModificada->GetRenderWindow());
	prv_vistaOriginal->SetRenderer		(mi_renderOriginal);
	prv_vistaModificada->SetRenderer	(mi_renderModificada);
	mi_renderOriginal->Delete();
	mi_renderModificada->Delete();

	prv_vistaOriginal->SetInteractionStyle (vtkViewImage2D::SELECT_INTERACTION);
	prv_vistaOriginal->SetWheelInteractionStyle(vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaOriginal->SetMiddleButtonInteractionStyle (vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaOriginal->SetRightButtonInteractionStyle (vtkViewImage2D::FULL_PAGE_INTERACTION);

	prv_vistaModificada->SetInteractionStyle (vtkViewImage2D::SELECT_INTERACTION);
	prv_vistaModificada->SetWheelInteractionStyle(vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaModificada->SetMiddleButtonInteractionStyle (vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaModificada->SetRightButtonInteractionStyle (vtkViewImage2D::FULL_PAGE_INTERACTION);

	prv_vistaOriginal->SetOrientation (vtkViewImage2D::AXIAL_ID);
	prv_vistaModificada->SetOrientation (vtkViewImage2D::AXIAL_ID);

	prv_vistaOriginal->SetAboutData("Original Image");
	prv_vistaModificada->SetAboutData("Modified Image");
	
	prv_vistaOriginal->SetBackgroundColor (0.0,0.0,0.0);
	prv_vistaModificada->SetBackgroundColor (0.0,0.0,0.0);
	prv_vistaOriginal->AddChild (prv_vistaModificada);
	prv_vistaModificada->AddChild (prv_vistaOriginal);

	wxPanel * mi_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(256,100));
	prv_wxTCSigma = new wxTextCtrl(mi_panel, wxID_ANY, _T("Scale of Gaussian blurring (sigma): 1.2  "), 
		wxPoint(2,2), wxSize(250,20));
	prv_wxSLSigma = new wxSlider(mi_panel, nkLevelSetsPageGradient::ID_EVENTO_SLIDER, 
					120, 1, 301, 
					wxPoint(2,44),
					wxSize(250,40), wxSL_HORIZONTAL);
	prv_wxTCSigma->SetEditable(false);

	prv_wxGSLayout->Add(prv_wxVtkVistaOriginal, 1, wxEXPAND, 10);
	prv_wxGSLayout->Add(prv_wxVtkVistaModificada, 1, wxEXPAND, 10);
	prv_wxGSLayout->Add(mi_panel, 0, wxEXPAND);

	prv_vistaOriginal->SetITKImage (prv_nkLevelSets->GetInput());
	prv_nkLevelSets->UpdateGradient(1.2);
	prv_vistaModificada->SetITKImage (prv_nkLevelSets->GetOutputGradient());
	
	prv_vistaOriginal->SyncResetCurrentPoint();
	prv_vistaOriginal->SyncResetWindowLevel();

	this->SetSizerAndFit (prv_wxGSLayout);
	this->SetAutoLayout (true);
	this->SetSize(780,480);
	this->Layout();
}

nkLevelSetsPageGradient::~nkLevelSetsPageGradient(){
	prv_vistaOriginal->Detach();
	prv_vistaModificada->Detach();
	prv_vistaOriginal->Delete();
	prv_vistaModificada->Delete();
	prv_wxVtkVistaOriginal->Delete();
	prv_wxVtkVistaModificada->Delete();
}

BEGIN_EVENT_TABLE(nkLevelSetsPageGradient, wxWizardPageSimple)
	EVT_SCROLL_CHANGED(nkLevelSetsPageGradient::eventoCambiarSigma)
	EVT_SCROLL(nkLevelSetsPageGradient::eventoCambiarTextoSigma) 
END_EVENT_TABLE()

void nkLevelSetsPageGradient::eventoCambiarSigma(wxScrollEvent& WXUNUSED(event)){
	itk::NumericTraits<float>::RealType mi_sigma;
	mi_sigma = ((float)(prv_wxSLSigma->GetValue()))/100.0;
	prv_wxTCSigma->SetValue(wxString::Format("Scale of Gaussian blurring (sigma): %1.2f", mi_sigma));
	
	wxBeginBusyCursor();
	prv_nkLevelSets->UpdateGradient(mi_sigma);
	prv_vistaModificada->SetITKImage (prv_nkLevelSets->GetOutputGradient());

	prv_vistaModificada->Render();
	wxEndBusyCursor();
}

void nkLevelSetsPageGradient::eventoCambiarTextoSigma(wxScrollEvent& WXUNUSED(event)){
	itk::NumericTraits<float>::RealType mi_sigma;
	mi_sigma = ((float)(prv_wxSLSigma->GetValue()))/100.0;
	prv_wxTCSigma->SetValue(wxString::Format("Scale of Gaussian blurring (sigma): %1.2f", mi_sigma));
}

nkLevelSetsPageSigmoid::nkLevelSetsPageSigmoid(wxWizard* parent, 
					wxWizardPage* prev, 
					wxWizardPage* next, 
					const wxBitmap& bitmap,
					nkLevelSets * un_nkLevelSets): 
	wxWizardPageSimple(parent, prev, next, bitmap){
	prv_nkLevelSets = un_nkLevelSets;
	prv_wxGSLayout = new wxGridSizer(1,3,3,3);
	long viewStyle = wxWANTS_CHARS | wxNO_FULL_REPAINT_ON_RESIZE;
	prv_wxVtkVistaOriginal = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													wxSize(256,384),
													viewStyle,
													wxT ("wxVtk Axial view"));
	prv_wxVtkVistaModificada = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													wxSize(256,384),
													viewStyle,
													wxT ("wxVtk Axial view"));
	prv_vistaOriginal   = vtkViewImage2DWithTracer::New();
	prv_vistaModificada = vtkViewImage2DWithTracer::New();
	vtkRenderer* mi_renderOriginal   = vtkRenderer::New();
	vtkRenderer* mi_renderModificada = vtkRenderer::New();
	prv_wxVtkVistaOriginal->GetRenderWindow()->AddRenderer   (mi_renderOriginal);
	prv_wxVtkVistaModificada->GetRenderWindow()->AddRenderer   (mi_renderModificada);
	prv_vistaOriginal->SetRenderWindow   (prv_wxVtkVistaOriginal->GetRenderWindow());
	prv_vistaModificada->SetRenderWindow   (prv_wxVtkVistaModificada->GetRenderWindow());
	prv_vistaOriginal->SetRenderer		(mi_renderOriginal);
	prv_vistaModificada->SetRenderer	(mi_renderModificada);
	mi_renderOriginal->Delete();
	mi_renderModificada->Delete();

	prv_vistaOriginal->SetInteractionStyle (vtkViewImage2D::SELECT_INTERACTION);
	prv_vistaOriginal->SetWheelInteractionStyle(vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaOriginal->SetMiddleButtonInteractionStyle (vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaOriginal->SetRightButtonInteractionStyle (vtkViewImage2D::FULL_PAGE_INTERACTION);

	prv_vistaModificada->SetInteractionStyle (vtkViewImage2D::SELECT_INTERACTION);
	prv_vistaModificada->SetWheelInteractionStyle(vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaModificada->SetMiddleButtonInteractionStyle (vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaModificada->SetRightButtonInteractionStyle (vtkViewImage2D::FULL_PAGE_INTERACTION);

	prv_vistaOriginal->SetOrientation (vtkViewImage2D::AXIAL_ID);
	prv_vistaModificada->SetOrientation (vtkViewImage2D::AXIAL_ID);

	prv_vistaOriginal->SetAboutData("Original Image");
	prv_vistaModificada->SetAboutData("Modified Image");
	
	prv_vistaOriginal->SetBackgroundColor (0.0,0.0,0.0);
	prv_vistaModificada->SetBackgroundColor (0.0,0.0,0.0);
	prv_vistaOriginal->AddChild (prv_vistaModificada);
	prv_vistaModificada->AddChild (prv_vistaOriginal);

	wxPanel * mi_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(256,256));
	prv_wxTCAlpha = new wxTextCtrl(mi_panel, wxID_ANY, _T("Alpha: -1.0  "), wxPoint(5,5), wxSize(250,30));
	prv_wxSLAlpha = new wxSlider(mi_panel, nkLevelSetsPageSigmoid::ID_EVENTO_SLIDER_SIGMOID_ALPHA, 
					225, 1, 501, wxPoint(5,35), wxSize(250,30), wxSL_HORIZONTAL);
	prv_wxTCAlpha->SetEditable(false);

	prv_wxTCBeta = new wxTextCtrl(mi_panel, wxID_ANY, _T("Beta: 50.0  "), wxPoint(5,80), wxSize(250,30));
	prv_wxSLBeta = new wxSlider(mi_panel, nkLevelSetsPageSigmoid::ID_EVENTO_SLIDER_SIGMOID_BETA, 
					51, 1, 501, wxPoint(5,115), wxSize(250,30), wxSL_HORIZONTAL);
	prv_wxTCBeta->SetEditable(false);

	prv_wxGSLayout->Add(prv_wxVtkVistaOriginal, 1, wxEXPAND);
	prv_wxGSLayout->Add(prv_wxVtkVistaModificada, 1, wxEXPAND);
	prv_wxGSLayout->Add(mi_panel, 0, wxEXPAND);

	prv_vistaOriginal->SetITKImage (prv_nkLevelSets->GetOutputGradient());
	prv_nkLevelSets->UpdateSigmoid(-1.0, 50.0);
	prv_vistaModificada->SetITKImage (prv_nkLevelSets->GetOutputSigmoid());
	
	prv_vistaOriginal->SyncResetCurrentPoint();
	prv_vistaOriginal->SyncResetWindowLevel();
	
	this->SetSizerAndFit (prv_wxGSLayout);
	this->SetAutoLayout (true);
	this->SetSize(780,480);
	this->Layout();
}

nkLevelSetsPageSigmoid::~nkLevelSetsPageSigmoid(){
	prv_vistaOriginal->Detach();
	prv_vistaModificada->Detach();
	prv_vistaOriginal->Delete();
	prv_vistaModificada->Delete();
	prv_wxVtkVistaOriginal->Delete();
	prv_wxVtkVistaModificada->Delete();
}

BEGIN_EVENT_TABLE(nkLevelSetsPageSigmoid, wxWizardPageSimple)
	EVT_SCROLL_CHANGED(nkLevelSetsPageSigmoid::eventoCambiarAlphaBeta) 
	EVT_COMMAND_SCROLL(nkLevelSetsPageSigmoid::ID_EVENTO_SLIDER_SIGMOID_ALPHA, nkLevelSetsPageSigmoid::eventoCambiarTextoAlpha) 
	EVT_COMMAND_SCROLL(nkLevelSetsPageSigmoid::ID_EVENTO_SLIDER_SIGMOID_BETA, nkLevelSetsPageSigmoid::eventoCambiarTextoBeta) 
END_EVENT_TABLE()

void nkLevelSetsPageSigmoid::eventoCambiarTextoAlpha(wxScrollEvent& WXUNUSED(event)){
	double mi_alpha;
	mi_alpha = -10.0 + (((double)(prv_wxSLAlpha->GetValue()))/500.0)*(20.0);
	prv_wxTCAlpha->SetValue(wxString::Format("Alpha: %1.2f", mi_alpha));
	double mi_beta;
	mi_beta = (((double)(prv_wxSLBeta->GetValue()))/500.0)*(200.0);
	prv_wxTCBeta->SetValue(wxString::Format("Beta: %1.2f", mi_beta));
}

void nkLevelSetsPageSigmoid::eventoCambiarTextoBeta(wxScrollEvent& WXUNUSED(event)){
	double mi_alpha;
	mi_alpha = -10.0 + (((double)(prv_wxSLAlpha->GetValue()))/500.0)*(20.0);
	prv_wxTCAlpha->SetValue(wxString::Format("Alpha: %1.2f", mi_alpha));
	double mi_beta;
	mi_beta = (((double)(prv_wxSLBeta->GetValue()))/500.0)*(200.0);
	prv_wxTCBeta->SetValue(wxString::Format("Beta: %1.2f", mi_beta));
}

void nkLevelSetsPageSigmoid::eventoCambiarAlphaBeta(wxScrollEvent& WXUNUSED(event)){
	double mi_alpha;
	double mi_beta;
	mi_alpha = -10.0 + (((double)(prv_wxSLAlpha->GetValue()))/500.0)*(20.0);
	prv_wxTCAlpha->SetValue(wxString::Format("Alpha: %1.2f", mi_alpha));

	mi_beta = (((double)(prv_wxSLBeta->GetValue()))/500.0)*(200.0);
	prv_wxTCBeta->SetValue(wxString::Format("Beta: %1.2f", mi_beta));
	
	wxBeginBusyCursor();
	prv_nkLevelSets->UpdateSigmoid(mi_alpha, mi_beta);
	prv_vistaModificada->SetITKImage (prv_nkLevelSets->GetOutputSigmoid());

	prv_vistaModificada->Render();
	wxEndBusyCursor();
}

nkLevelSetsPageSeeds::nkLevelSetsPageSeeds(wxWizard* parent, 
					wxWizardPage* prev, 
					wxWizardPage* next, 
					const wxBitmap& bitmap,
					nkLevelSets * un_nkLevelSets): 
	wxWizardPageSimple(parent, prev, next, bitmap){
	prv_nkLevelSets = un_nkLevelSets;
	prv_wxGSLayout = new wxGridSizer(2,2,3,3);
	long viewStyle = wxWANTS_CHARS | wxNO_FULL_REPAINT_ON_RESIZE;
	prv_wxVtkVistaAxial = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													wxSize(380,300),
													viewStyle,
													wxT ("wxVtk Axial view"));
	prv_wxVtkVistaCoronal = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													wxSize(380,300),
													viewStyle,
													wxT ("wxVtk Coronal view"));
	prv_wxVtkVistaSagital = new wxVTKRenderWindowInteractor (this, wxID_ANY,
													wxDefaultPosition,
													wxSize(380,300),
													viewStyle,
													wxT ("wxVtk Sagital view"));
	prv_vistaAxial = vtkViewImage2DWithTracer::New();
	prv_vistaSagital   = vtkViewImage2DWithTracer::New();
	prv_vistaCoronal = vtkViewImage2DWithTracer::New();

	vtkRenderer* mi_renderAxial = vtkRenderer::New();
	vtkRenderer* mi_renderCoronal = vtkRenderer::New();
	vtkRenderer* mi_renderSagital = vtkRenderer::New();

	prv_wxVtkVistaAxial->GetRenderWindow()->AddRenderer   (mi_renderAxial);
	prv_wxVtkVistaSagital->GetRenderWindow()->AddRenderer   (mi_renderSagital);
	prv_wxVtkVistaCoronal->GetRenderWindow()->AddRenderer   (mi_renderCoronal);

	prv_vistaAxial->SetRenderWindow   (prv_wxVtkVistaAxial->GetRenderWindow());
	prv_vistaSagital->SetRenderWindow   (prv_wxVtkVistaSagital->GetRenderWindow());
	prv_vistaCoronal->SetRenderWindow   (prv_wxVtkVistaCoronal->GetRenderWindow());

	prv_vistaAxial->SetRenderer		(mi_renderAxial);
	prv_vistaSagital->SetRenderer		(mi_renderSagital);
	prv_vistaCoronal->SetRenderer		(mi_renderCoronal);

	mi_renderAxial->Delete();
	mi_renderSagital->Delete();
	mi_renderCoronal->Delete();

	wxPanel * mi_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(400,300));

	prv_wxGridSeeds = new wxGrid(mi_panel,wxID_ANY, wxPoint(0,0), wxSize(400,220));
	prv_wxGridSeeds->CreateGrid( 1, 4 );
	prv_wxGridSeeds->SetColSize( 0, 60 );
	prv_wxGridSeeds->SetColSize( 1, 60 );
	prv_wxGridSeeds->SetColSize( 2, 60 );
	prv_wxGridSeeds->SetColSize( 3, 60 );
	prv_wxGridSeeds->SetColLabelValue(0, _T("X"));
	prv_wxGridSeeds->SetColLabelValue(1, _T("Y"));
	prv_wxGridSeeds->SetColLabelValue(2, _T("Z"));
	prv_wxGridSeeds->SetColLabelValue(3, _("Radius"));
	prv_wxGridSeeds->SetEditable(false);
	prv_wxGridSeeds->ForceRefresh();

	prv_addSeeds = new wxButton(mi_panel, nkLevelSetsPageSeeds::ID_EVENTO_BUTTON_SEEDS_ADD,
								_("Add seed"), wxPoint(5,225));
	prv_wxTCRadius = new wxTextCtrl(mi_panel, wxID_ANY, _T("Radius: 5"), wxPoint(155,225), wxSize(70,20));
	prv_wxSLRadius = new wxSlider(mi_panel, nkLevelSetsPageSeeds::ID_EVENTO_BUTTON_SEEDS_RADIUS, 
					5, 1, 51, wxPoint(230,225), wxSize(150,30), wxSL_HORIZONTAL);
	prv_deleteSeeds = new wxButton(mi_panel, nkLevelSetsPageSeeds::ID_EVENTO_BUTTON_SEEDS_DELETE,
								_("Delete selected seed from grid"), wxPoint(5,255));

	prv_wxGSLayout->Add(prv_wxVtkVistaAxial, 1, wxEXPAND);
	prv_wxGSLayout->Add(prv_wxVtkVistaCoronal, 1, wxEXPAND);
	prv_wxGSLayout->Add(prv_wxVtkVistaSagital, 1, wxEXPAND);
	prv_wxGSLayout->Add(mi_panel, 0, wxEXPAND);

	this->SetSizerAndFit (prv_wxGSLayout);
	this->SetAutoLayout (true);
	this->SetSize(780,480);
	this->Layout();
}

void nkLevelSetsPageSeeds::Configure(){
	prv_vistaAxial->SetInteractionStyle (vtkViewImage2D::SELECT_INTERACTION);
	prv_vistaAxial->SetWheelInteractionStyle(vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaAxial->SetMiddleButtonInteractionStyle (vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaAxial->SetRightButtonInteractionStyle (vtkViewImage2D::ZOOM_INTERACTION);

	prv_vistaSagital->SetInteractionStyle (vtkViewImage2D::SELECT_INTERACTION);
	prv_vistaSagital->SetWheelInteractionStyle(vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaSagital->SetMiddleButtonInteractionStyle (vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaSagital->SetRightButtonInteractionStyle (vtkViewImage2D::ZOOM_INTERACTION);

	prv_vistaCoronal->SetInteractionStyle (vtkViewImage2D::SELECT_INTERACTION);
	prv_vistaCoronal->SetWheelInteractionStyle(vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaCoronal->SetMiddleButtonInteractionStyle (vtkViewImage2D::ZOOM_INTERACTION);
	prv_vistaCoronal->SetRightButtonInteractionStyle (vtkViewImage2D::ZOOM_INTERACTION);

	prv_vistaAxial->SetOrientation (vtkViewImage2D::AXIAL_ID);
	prv_vistaCoronal->SetOrientation (vtkViewImage2D::CORONAL_ID);
	prv_vistaSagital->SetOrientation (vtkViewImage2D::SAGITTAL_ID);

	prv_vistaAxial->ScalarBarVisibilityOn();
	prv_vistaCoronal->ScalarBarVisibilityOn();
	prv_vistaSagital->ScalarBarVisibilityOn();
	
	prv_vistaAxial->SetBackgroundColor (0.0,0.0,0.0);
	prv_vistaCoronal->SetBackgroundColor (0.0,0.0,0.0);
	prv_vistaSagital->SetBackgroundColor (0.0,0.0,0.0);

	//prv_vistaAxial->SetManualTracingOn();
	//prv_vistaCoronal->SetManualTracingOn();
	//prv_vistaSagital->SetManualTracingOn();

	prv_vistaAxial->AddChild (prv_vistaCoronal);
	prv_vistaCoronal->AddChild (prv_vistaSagital);
	prv_vistaSagital->AddChild (prv_vistaAxial);

	prv_nkLevelSets->UpdateGradient(1.2);

	prv_imgInput = prv_nkLevelSets->GetOutputSigmoid();
	prv_vistaAxial->SetITKImage (prv_imgInput);
	prv_vistaCoronal->SetITKImage (prv_imgInput);
	prv_vistaSagital->SetITKImage (prv_imgInput);

	
	prv_vistaAxial->SyncResetCurrentPoint();
	prv_vistaAxial->SyncResetWindowLevel();
}

nkLevelSetsPageSeeds::~nkLevelSetsPageSeeds(){
	prv_vistaAxial->Detach();
	prv_vistaCoronal->Detach();
	prv_vistaSagital->Detach();
	prv_vistaAxial->Delete();
	prv_vistaCoronal->Delete();
	prv_vistaSagital->Delete();
	prv_wxVtkVistaAxial->Delete();
	prv_wxVtkVistaCoronal->Delete();
	prv_wxVtkVistaSagital->Delete();
}

BEGIN_EVENT_TABLE(nkLevelSetsPageSeeds, wxWizardPageSimple)
	EVT_SCROLL_CHANGED(nkLevelSetsPageSeeds::eventoCambiarRadius)
	EVT_SCROLL(nkLevelSetsPageSeeds::eventoCambiarTextoRadius) 
	EVT_BUTTON(nkLevelSetsPageSeeds::ID_EVENTO_BUTTON_SEEDS_ADD, nkLevelSetsPageSeeds::eventoBotonAddSeed) 
	EVT_BUTTON(nkLevelSetsPageSeeds::ID_EVENTO_BUTTON_SEEDS_DELETE, nkLevelSetsPageSeeds::eventoBotonDeleteSeed) 
	EVT_GRID_SELECT_CELL(nkLevelSetsPageSeeds::eventoSeleccionarCelda)
END_EVENT_TABLE()

void nkLevelSetsPageSeeds::eventoCambiarRadius(wxScrollEvent& WXUNUSED(event)){
	int mi_radius;
	mi_radius = prv_wxSLRadius->GetValue();
	prv_wxTCRadius->SetValue(wxString::Format("Radius: %d", mi_radius));
}

void nkLevelSetsPageSeeds::eventoCambiarTextoRadius(wxScrollEvent& WXUNUSED(event)){
	int mi_radius;
	mi_radius = prv_wxSLRadius->GetValue();
	prv_wxTCRadius->SetValue(wxString::Format("Radius: %d", mi_radius));
}

void nkLevelSetsPageSeeds::eventoBotonAddSeed(wxCommandEvent &WXUNUSED(event)){
	int p_point[3];
	prv_vistaAxial->GetCurrentVoxelCoordinates (p_point);
	int mi_filaGrilla = prv_wxGridSeeds->GetNumberRows()-1;
	prv_wxGridSeeds->SetCellValue(mi_filaGrilla, 0, wxString::Format("%d", p_point[0]));
	prv_wxGridSeeds->SetCellValue(mi_filaGrilla, 1, wxString::Format("%d", p_point[1]));
	prv_wxGridSeeds->SetCellValue(mi_filaGrilla, 2, wxString::Format("%d", p_point[2]));
	prv_wxGridSeeds->SetCellValue(mi_filaGrilla, 3, wxString::Format("%d", prv_wxSLRadius->GetValue()));
	prv_wxGridSeeds->InsertRows(mi_filaGrilla+1);

	std::vector<int> p;
	p.push_back(p_point[0]);
	p.push_back(p_point[1]);
	p.push_back(p_point[2]);
	p.push_back(prv_wxSLRadius->GetValue());
	this->prv_nkLevelSets->pub_listSeeds.push_back(p);

	std::list<std::vector<int> >::iterator q1;
	std::vector<int> q;
	vtkOutputWindow::GetInstance()->DisplayText("\n");
	for(q1 = prv_nkLevelSets->pub_listSeeds.begin();
	q1 != prv_nkLevelSets->pub_listSeeds.end();q1++){
		char temp[100]="";
		q = *q1;
		sprintf(temp,"Semilla = (%d,%d,%d,%d) \n", q[0],q[1],q[2], q[3]);
		vtkOutputWindow::GetInstance()->DisplayText(temp);
	}
}

void nkLevelSetsPageSeeds::eventoBotonDeleteSeed(wxCommandEvent &WXUNUSED(event)){
	int mi_filaGrilla = prv_wxGridSeeds->GetGridCursorRow();
	if (prv_wxGridSeeds->GetCellValue(mi_filaGrilla, 0) != wxT("")){
		int p_point[4];
		long temp;
		wxString cad;
		cad = prv_wxGridSeeds->GetCellValue(mi_filaGrilla, 0);
		cad.ToLong(&temp); 		p_point[0] = (int)temp;
		cad = prv_wxGridSeeds->GetCellValue(mi_filaGrilla, 1);
		cad.ToLong(&temp); 		p_point[1] = (int)temp;
		cad = prv_wxGridSeeds->GetCellValue(mi_filaGrilla, 2);
		cad.ToLong(&temp); 		p_point[2] = (int)temp;
		cad = prv_wxGridSeeds->GetCellValue(mi_filaGrilla, 3);
		cad.ToLong(&temp); 		p_point[3] = (int)temp;

		std::vector<int> p;
		p.push_back(p_point[0]);
		p.push_back(p_point[1]);
		p.push_back(p_point[2]);
		p.push_back(p_point[3]);

		/*char temp1[100]="";
		sprintf(temp1,"Remover Semilla = (%d,%d,%d,%d) \n", p_point[0],p_point[1],p_point[2], p_point[3]);
		vtkOutputWindow::GetInstance()->DisplayText(temp1);*/

		this->prv_nkLevelSets->pub_listSeeds.remove(p);
		prv_wxGridSeeds->DeleteRows(mi_filaGrilla);
	}
}

void nkLevelSetsPageSeeds::eventoSeleccionarCelda(wxGridEvent &event){
	int mi_filaGrilla = event.GetRow();
	int p_point[3];
	long temp;
	wxString cad;
	if (prv_wxGridSeeds->GetCellValue(mi_filaGrilla, 0) != wxT("")){
		cad = prv_wxGridSeeds->GetCellValue(mi_filaGrilla, 0);
		cad.ToLong(&temp); 		p_point[0] = (int)temp;
		cad = prv_wxGridSeeds->GetCellValue(mi_filaGrilla, 1);
		cad.ToLong(&temp); 		p_point[1] = (int)temp;
		cad = prv_wxGridSeeds->GetCellValue(mi_filaGrilla, 2);
		cad.ToLong(&temp); 		p_point[2] = (int)temp;

		prv_vistaSagital->SyncSetSlice(vtkViewImage2D::SAGITTAL_ID, p_point[0]);
		prv_vistaSagital->SyncSetSlice(vtkViewImage2D::CORONAL_ID, p_point[1]);
		prv_vistaSagital->SyncSetSlice(vtkViewImage2D::AXIAL_ID, p_point[2]);

		/*char temp[100]="";
		sprintf(temp,"\ncelda seleccionada = (%d,%d,%d) Fila: %d\n",p_point[0],p_point[1],p_point[2], mi_filaGrilla);
		vtkOutputWindow::GetInstance()->DisplayText(temp);*/
	}
	event.Skip();
}