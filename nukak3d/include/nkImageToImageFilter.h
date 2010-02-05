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
 * @file nkAdminPluginGui.h
 * @brief Definition of filter.
 * @details Define a generic filter that can be handled by Nukak3D.
 * @author Alexander Pinzón Fernandez
 * @date 13/04/2009 4:11 p.m.
*/

#ifndef __NKIMAGETOIMAGEFILTER__H__
#define __NKIMAGETOIMAGEFILTER__H__

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
