///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
#ifndef __NKENGINE__H__
#define __NKENGINE__H__

#if defined (WIN32) || defined (_WIN32_)
	#ifdef NKENGINE_EXPORTS
		#define NKENGINE_API __declspec(dllexport)
	#else
		#define NKENGINE_API __declspec(dllimport)
	#endif
#else
	#define NKENGINE_API
#endif

#endif // __NKENGINE__H__