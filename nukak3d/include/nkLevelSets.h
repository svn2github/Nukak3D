/** 
 * @file nkLevelSets.h
 * @brief Class for Level Sets segmentation wizard.
 * @details Implement all functions for level sets proccess.
 * @author Alexander Pinzón Fernandez, Byron Pérez
 * @version 0.1
 * @date 27/06/2008 03:15 p.m.
*/
#ifndef _NKLEVELSETS_H_
#define _NKLEVELSETS_H_

/** itk*/
#include <itkBinaryThresholdImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkCommand.h>
#include <itkFastMarchingImageFilter.h>
#include <itkGradientMagnitudeRecursiveGaussianImageFilter.h>
#include <itkImageFileWriter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkShapeDetectionLevelSetImageFilter.h>
#include <itkSigmoidImageFilter.h>

/** nk*/
#include "nkIODialog.h"
#include "nkNukak3D.h"
#include "nkVolViewer.h"
#include "wiztest.xpm"
#include "wiztest2.xpm"

/** STL */
#include <vector>

/** vtkINRIA3D*/
#include <itkImageToVTKImageFilter.h>
#include <vtkIsosurfaceManager.h>
#include <vtkViewImage2DWithTracer.h>
#include <wxVTKRenderWindowInteractor.h>

/** wxWidgets*/
#include <wx/aui/auibook.h>
#include <wx/button.h>
#include <wx/grid.h>
#include <wx/laywin.h>
#include <wx/progdlg.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/textctrl.h>
#include <wx/wizard.h>

/** 
 * @brief Class for Level Sets Segmentation.
 */
class nkLevelSets{
public:
	/**
	 * @brief Class Constructor.
	*/
	nkLevelSets(nkVolViewer * un_volViewer,wxAuiNotebook * p_libro);
	/**
	 * @brief Configure preproccesing image for Level Sets.
	*/
	void Configurar();
	/**
	 * @brief Configure params for Level Sets.
	*/
	bool ConfigurarLevelSet();
	/**
	 * @brief Get propagation scaling, curvature scaling, number of iterations of input dialog.
	*/
	bool ObtenerDatosLevelSet();
	/**
	 * @brief Image to segmentation
	*/
	void SetInput(itk::Image<unsigned short,3>::Pointer una_imagen);
	/**
	 * @brief run Update of shape Detection Level Set filter, and show progress dialog.
	*/
	void UpdateLevelSets();
	/**
	 * @brief run Update of Gradient filter.
	 * @param un_sigma Sigma for Gradient filter.
	*/
	void UpdateGradient(itk::NumericTraits<float>::RealType un_sigma);
	/**
	 * @brief run Update of Sigmoid filter.
	 * @param un_alpha Alpha for sigmoid filter
	 * @param un_beta Beta for sigmoid filter
	*/
	void UpdateSigmoid(double un_alpha, double un_beta);
	/**
	 * @brief Get original image.
	 * @return pointer to input image.
	*/
	itk::Image<unsigned short,3>::Pointer GetInput();
	/**
	 * @brief Get segmented result.
	 * @return pointer to segemented image.
	*/
	itk::Image<unsigned short,3>::Pointer GetOutput();
	/**
	 * @brief Get result of Sigmoid filter.
	 * @return pointer to image.
	*/
	itk::Image<float, 3>::Pointer GetOutputSigmoid();
	/**
	 * @brief Get result of Gradient filter.
	 * @return pointer to image.
	*/
	itk::Image<float, 3>::Pointer GetOutputGradient();
	/**
	 * @brief Make, configure, and run Wizard.
	*/
	void CrearAsistente();
	/**
	 * @brief Make new page in Notebook, with the segmentating image.
	*/
	void WriteGradientImage();
	/**
	 * @brief Function to update mesh 3d in view, for each iteration of filter.
	*/
	void ActualizarSegmentacionLevelSet();
	/**
	 * @brief Remove mesh if exist, and add new isosurface, generated from input image.
	 * @param una_vtkImage Image for make isosurface.
	 * @param una_opacidad Alpha channel for Blending image original with this.
	 * @param un_dataSet Id of image.
	*/
	void SetUpdateOverlappingImage(vtkImageData * una_vtkImage, double una_opacidad, int un_dataSet);
	/**
	 * @brief Equal to ActualizarSegmentacionLevelSet, but in the end proccess.
	 * @param una_vtkImage Image for make isosurface.
	 * @param una_opacidad Alpha channel for Blending image original with this.
	 * @param un_dataSet Id of image.
	*/
	void AddOverlappingImage(vtkImageData * una_vtkImage, double una_opacidad, int un_dataSet);

	typedef unsigned short	PixelTypeUShort; //! Pixel data type unsigned short
	typedef float			PixelTypeFloat;  //! Pixel data type float

	typedef itk::Image<PixelTypeUShort,3>	ImageUShort3; //! Image 3D of unsigned short
	typedef itk::Image<PixelTypeFloat ,3>	ImageFloat3;  //! Image 3D of float

	typedef itk::RescaleIntensityImageFilter<ImageFloat3,ImageUShort3> CastFilterOutType; 
	typedef itk::ShapeDetectionLevelSetImageFilter< ImageFloat3,ImageFloat3,PixelTypeFloat> ShapeDetectionFilterType;
	typedef itk::BinaryThresholdImageFilter< ImageFloat3, ImageFloat3 > ThresholdingFilterType;
	typedef itk::CastImageFilter< ImageUShort3,ImageFloat3 > CastFilterInType;
	typedef itk::RescaleIntensityImageFilter< ImageFloat3,ImageUShort3 > CastFilterOutType;
	typedef itk::GradientMagnitudeRecursiveGaussianImageFilter< ImageFloat3, ImageFloat3 >  GradientFilterType;
	typedef itk::SigmoidImageFilter< ImageFloat3, ImageFloat3 > SigmoidFilterType;	
	typedef itk::FastMarchingImageFilter< ImageFloat3, ImageFloat3 > FastMarchingFilterType;
	typedef itk::ImageFileWriter< ImageUShort3 > WriterType;		
	typedef itk::ImageToVTKImageFilter<ImageUShort3> ITKToVTKType;

	typedef itk::SimpleMemberCommand< nkLevelSets > SimpleCommandType;
	SimpleCommandType::Pointer prv_iterationCommand;
	
	CastFilterOutType::Pointer prv_castOutLevetSet;
	ShapeDetectionFilterType::Pointer prv_shapeDetectionLevelSet;
	ThresholdingFilterType::Pointer prv_thresholder;
	CastFilterInType::Pointer prv_castUnShortToFloat;
	CastFilterOutType::Pointer prv_castOutLevetSetVelocity;
	GradientFilterType::Pointer prv_gradientMagnitude;
	SigmoidFilterType::Pointer prv_sigmoid;
	FastMarchingFilterType::Pointer prv_fastMarching;
	ITKToVTKType::Pointer prv_vtkToItk;

	vtkIsosurfaceManager* prv_isoSurfaceManager;
	std::list<std::vector<int> > pub_listSeeds;
private:
	bool finalizo_level_set;
	bool parar ;
	bool pausar;
	wxProgressDialog * wxp;
	std::vector<vtkImageData *> prv_vectorDataSets;
	unsigned int prv_CurrentDataSets;

	nkVolViewer * prv_nkVolViewer;
	nkVolViewer * prv_nkVVSigmoid;
	wxAuiNotebook * prv_libro;
	ImageUShort3::Pointer prv_imgInput;
	ImageUShort3::Pointer prv_imgGradient;
	ImageUShort3::Pointer prv_imgSigmoid;
	ImageUShort3::Pointer prv_imgLevelSets;

	itk::NumericTraits<float>::RealType prv_sigmaGradient;
	double prv_alpha;
	double prv_beta;
	double prv_propagationScaling;
	double prv_curvatureScaling;
	double prv_numberOfIterations;
	wxWizard * prv_asistente;
};

class nkLevelSetsPageGradient: public wxWizardPageSimple {
public:
	enum{
		ID_EVENTO_SLIDER = wxID_HIGHEST + 2000
	};
	nkLevelSetsPageGradient(wxWizard* parent = NULL, 
					wxWizardPage* prev = NULL, 
					wxWizardPage* next = NULL, 
					const wxBitmap& bitmap = wxNullBitmap,
					nkLevelSets * un_nkLevelSets = NULL);
	~nkLevelSetsPageGradient();
	vtkViewImage2DWithTracer*	prv_vistaOriginal;
	vtkViewImage2DWithTracer*	prv_vistaModificada;
	wxVTKRenderWindowInteractor* prv_wxVtkVistaOriginal;
	wxVTKRenderWindowInteractor* prv_wxVtkVistaModificada;
private:
	void eventoCambiarSigma(wxScrollEvent& WXUNUSED(event));
	void eventoCambiarTextoSigma(wxScrollEvent& WXUNUSED(event));
	wxGridSizer * prv_wxGSLayout;
	wxSlider * prv_wxSLSigma;
	wxTextCtrl * prv_wxTCSigma;
	nkLevelSets * prv_nkLevelSets;
	DECLARE_EVENT_TABLE()
};

class nkLevelSetsPageSigmoid: public wxWizardPageSimple {
public:
	enum{
		ID_EVENTO_SLIDER_SIGMOID_ALPHA = wxID_HIGHEST + 2100,
		ID_EVENTO_SLIDER_SIGMOID_BETA
	};
	nkLevelSetsPageSigmoid(wxWizard* parent = NULL, 
					wxWizardPage* prev = NULL, 
					wxWizardPage* next = NULL, 
					const wxBitmap& bitmap = wxNullBitmap,
					nkLevelSets * un_nkLevelSets = NULL);
	~nkLevelSetsPageSigmoid();
	vtkViewImage2DWithTracer*	prv_vistaOriginal;
	vtkViewImage2DWithTracer*	prv_vistaModificada;
	wxVTKRenderWindowInteractor* prv_wxVtkVistaOriginal;
	wxVTKRenderWindowInteractor* prv_wxVtkVistaModificada;
private:
	void eventoCambiarAlphaBeta(wxScrollEvent& WXUNUSED(event));
	void eventoCambiarTextoAlpha(wxScrollEvent& WXUNUSED(event));
	void eventoCambiarTextoBeta(wxScrollEvent& WXUNUSED(event));
	wxGridSizer * prv_wxGSLayout;
	wxSlider * prv_wxSLAlpha;
	wxSlider * prv_wxSLBeta;
	wxTextCtrl * prv_wxTCAlpha;
	wxTextCtrl * prv_wxTCBeta;
	nkLevelSets * prv_nkLevelSets;
	DECLARE_EVENT_TABLE()
};

class nkLevelSetsPageSeeds: public wxWizardPageSimple{
public:
	enum{
		ID_EVENTO_BUTTON_SEEDS_ADD = wxID_HIGHEST + 2200,
		ID_EVENTO_BUTTON_SEEDS_DELETE,
		ID_EVENTO_BUTTON_SEEDS_RADIUS
	};
	nkLevelSetsPageSeeds(wxWizard* parent = NULL, 
					wxWizardPage* prev = NULL, 
					wxWizardPage* next = NULL, 
					const wxBitmap& bitmap = wxNullBitmap,
					nkLevelSets * un_nkLevelSets = NULL);
	~nkLevelSetsPageSeeds();
	void Configurar();
	vtkViewImage2DWithTracer*	prv_vistaAxial;
	vtkViewImage2DWithTracer*	prv_vistaCoronal;
	vtkViewImage2DWithTracer*	prv_vistaSagital;
	wxVTKRenderWindowInteractor* prv_wxVtkVistaAxial;
	wxVTKRenderWindowInteractor* prv_wxVtkVistaCoronal;
	wxVTKRenderWindowInteractor* prv_wxVtkVistaSagital;
private:
	void eventoCambiarRadius(wxScrollEvent& WXUNUSED(event));
	void eventoCambiarTextoRadius(wxScrollEvent& WXUNUSED(event));
	void eventoBotonAddSeed(wxCommandEvent &WXUNUSED(event));
	void eventoBotonDeleteSeed(wxCommandEvent &WXUNUSED(event));
	void eventoSeleccionarCelda(wxGridEvent & event);
	wxGridSizer * prv_wxGSLayout;
	wxGrid * prv_wxGridSeeds;
	wxButton * prv_addSeeds;
	wxButton * prv_deleteSeeds;
	wxSlider * prv_wxSLRadius;
	wxTextCtrl * prv_wxTCRadius;
	itk::Image<float,3>::Pointer prv_imgInput;
	nkLevelSets * prv_nkLevelSets;
	DECLARE_EVENT_TABLE()
};

#endif // _NKLEVELSETS_H_