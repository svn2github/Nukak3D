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

#include <wx/config.h>
#include <wx/intl.h>
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

	/**
     * @brief Exit method of application, for delete wxConfigBase::Set()
	 * @return int
	 */
	virtual int OnExit();
protected:
    wxLocale pro_wxLocLanguage; // locale we'll be using
};

#endif //_NKMAIN_H_