/** 
 * @file nkMain.h
 * @brief Aplicación nukak3d.
 * @details Clase para la creacion de la aplicación.
 * Incluye splash screen.
 * @author Alexander Pinzon Fernandez.
 * @version 0.1
 * @date 18/10/2007 02:50 p.m.
 */

#ifndef _NKMAIN_H_
#define _NKMAIN_H_

#include  "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include  "wx/wx.h"
#endif

#include <wx/splash.h>

/** 
 * @brief Aplicación nukak3d.
 * @details Clase para la creacion de la aplicación.
 * Incluye splash screen.
 */
class nkMain : public wxApp{
public:
	/**
     * @brief Método donde se llama a la ventana principal
	 * @return bool
	 */
	virtual bool OnInit ( void );  
};

#endif //_NKMAIN_H_