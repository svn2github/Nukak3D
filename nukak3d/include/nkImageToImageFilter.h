#ifndef __nkImageToImageFilter_h
#define __nkImageToImageFilter_h

#include "itkImage.h"
#include "itkImageSource.h"
#include "itkConceptChecking.h"
#include "itkImageToImageFilterDetail.h"
#include "itkImageToImageFilter.h"

namespace itk
{

template <class TInputImage, class TOutputImage>
class nkImageToImageFilter : public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef nkImageToImageFilter       Self;
  typedef ImageSource<TOutputImage>  Superclass;
  typedef SmartPointer<Self>         Pointer;
  typedef SmartPointer<const Self>   ConstPointer;
  
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(nkImageToImageFilter, ImageToImageFilter);

  /** Superclass typedefs. */
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;
  typedef typename Superclass::OutputImagePixelType  OutputImagePixelType;

  /** Some convenient typedefs. */
  typedef TInputImage                             InputImageType;
  typedef typename InputImageType::Pointer        InputImagePointer;
  typedef typename InputImageType::ConstPointer   InputImageConstPointer;
  typedef typename InputImageType::RegionType     InputImageRegionType; 
  typedef typename InputImageType::PixelType      InputImagePixelType; 

  virtual void setParameter(void * value, unsigned int index =0) = 0;
  virtual void getParameter(void * value, unsigned int index =0) = 0;
  virtual void getParameterDescription(char * description, unsigned int index =0) = 0;
  virtual unsigned int getNumberOfParameter(void) = 0;
  
protected:
  nkImageToImageFilter(){
	this->SetNumberOfRequiredInputs(1);
  }
  ~nkImageToImageFilter(){}
  
private:
  nkImageToImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace itk

#endif
