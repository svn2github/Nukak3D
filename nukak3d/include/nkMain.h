/** 
 * @file nkMain.h
 * @brief nukak3d Application.
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
 * @brief nukak3d Application.
 */
class nkMain : public wxApp{
public:
	/**
     * @brief Init method of application
	 * @return bool
	 */
	virtual bool OnInit ( void );  
};

#endif //_NKMAIN_H_