/**
 * nkMacro.h defines macro for automatic generation of teamplate instantation
 */
 
#ifndef __NKMACRO_H__
#define __NKMACRO_H__

#include "itkImage.h"
#include "itkImageToImageFilter.h"

/** XXX Doxumentation.
 * XXX
 * XXX doc. */
#define MAKRO_DECLARE_createitkFilter()\
	MAKRO_FOR_EACH_TYPE(DECLARE_FUNCTION_createitkFilter,_)

#define DECLARE_FUNCTION_createitkFilter(__PIXEL_TYPE__,_)\
	virtual itk::nkImageToImageFilter< TYPEDEF_itkImage2(__PIXEL_TYPE__,3,__PIXEL_TYPE__,3) >::Pointer createitkFilter(__PIXEL_TYPE__) = 0;

#define MAKRO_createitkFilter(__FILTER_NAME__)\
	MAKRO_FOR_EACH_TYPE(FUNCTION_createitkFilter,__FILTER_NAME__)

#define MAKRO_createitkFilter2(__FILTER_NAME__)\
	MAKRO_FOR_EACH_TYPE(FUNCTION_createitkFilter2,__FILTER_NAME__)

#define MAKRO_FOR_EACH_TYPE(__PREFIX__STRING__,...)\
	__PREFIX__STRING__( double, __VA_ARGS__ )\
	__PREFIX__STRING__( float, __VA_ARGS__ )\
	__PREFIX__STRING__( int, __VA_ARGS__ )\
	__PREFIX__STRING__( unsigned int, __VA_ARGS__ )\
	__PREFIX__STRING__( short, __VA_ARGS__ )\
	__PREFIX__STRING__( unsigned short, __VA_ARGS__ )\
	__PREFIX__STRING__( long, __VA_ARGS__ )\
	__PREFIX__STRING__( unsigned long, __VA_ARGS__ )\
	__PREFIX__STRING__( char, __VA_ARGS__ )\
	__PREFIX__STRING__( unsigned char, __VA_ARGS__ )


#define FUNCTION_createitkFilter(__PIXEL_TYPE__,__FILTER_NAME__)\
	itk::nkImageToImageFilter< TYPEDEF_itkImage2(__PIXEL_TYPE__, 3,__PIXEL_TYPE__, 3) >::Pointer createitkFilter(__PIXEL_TYPE__)\
	{\
		return static_cast<itk::nkImageToImageFilter< TYPEDEF_itkImage2(__PIXEL_TYPE__, 3,__PIXEL_TYPE__, 3) >::Pointer> (itk::__FILTER_NAME__ < TYPEDEF_itkImage(__PIXEL_TYPE__, 3) >::New());\
	}

#define FUNCTION_createitkFilter2(__PIXEL_TYPE__,__FILTER_NAME__)\
	itk::nkImageToImageFilter< TYPEDEF_itkImage2(__PIXEL_TYPE__, 3,__PIXEL_TYPE__, 3) >::Pointer createitkFilter(__PIXEL_TYPE__)\
	{\
		return static_cast<itk::nkImageToImageFilter< TYPEDEF_itkImage2(__PIXEL_TYPE__, 3,__PIXEL_TYPE__, 3) >::Pointer> (itk::__FILTER_NAME__ < TYPEDEF_itkImage2(__PIXEL_TYPE__,3,__PIXEL_TYPE__,3) >::New());\
	}

#define TYPEDEF_itkImage3(__PIXEL_TYPE_1__, __IMAGE_DIMENSION_1__,__PIXEL_TYPE_2__, __IMAGE_DIMENSION_2__, __PIXEL_TYPE_3__, __IMAGE_DIMENSION_3__)\
	TYPEDEF_itkImage2(__PIXEL_TYPE_1__, __IMAGE_DIMENSION_1__,__PIXEL_TYPE_2__,__IMAGE_DIMENSION_2__), TYPEDEF_itkImage(__PIXEL_TYPE_3__, __IMAGE_DIMENSION_3__)

#define TYPEDEF_itkImage2(__PIXEL_TYPE_1__, __IMAGE_DIMENSION_1__,__PIXEL_TYPE_2__, __IMAGE_DIMENSION_2__)\
	TYPEDEF_itkImage(__PIXEL_TYPE_1__, __IMAGE_DIMENSION_1__), TYPEDEF_itkImage(__PIXEL_TYPE_2__,__IMAGE_DIMENSION_2__)

#define TYPEDEF_itkImage(__PIXEL_TYPE__, __IMAGE_DIMENSION__)\
	itk::Image< __PIXEL_TYPE__, __IMAGE_DIMENSION__>


#endif