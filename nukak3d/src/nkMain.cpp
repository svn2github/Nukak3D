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
 * @file nkMain.cpp
 * @brief Application nukak3d.
 * @details Class for creation of application
 * @author Alexander Pinzon Fernandez.
 * @version 0.1
 * @date 18/10/2007 02:50 p.m.
 */

#include "nkMain.h"
#include "nkNukak3D.h"

bool nkMain::OnInit(){
	SetVendorName(_T("Bioingenium"));
	SetAppName(_T("nukak3d"));
	wxConfigBase *prv_configBase = wxConfigBase::Get();
	prv_configBase->SetRecordDefaults();

	int languageSettings = prv_configBase->Read(_T("/Tools/Language"), wxNOT_FOUND);
	int language_system = pro_wxLocLanguage.GetSystemLanguage();
	if (languageSettings == wxNOT_FOUND) {
		if (language_system >= wxLANGUAGE_SPANISH && language_system <= wxLANGUAGE_SPANISH_VENEZUELA ){
			language_system = wxLANGUAGE_SPANISH;
		}else{
			language_system = wxLANGUAGE_ENGLISH;
		}
	}else{
		if (languageSettings >= wxLANGUAGE_SPANISH && languageSettings <= wxLANGUAGE_SPANISH_VENEZUELA ){
			language_system = wxLANGUAGE_SPANISH;
		}else{
			language_system = wxLANGUAGE_ENGLISH ;
		}
	}
	
	if ( !pro_wxLocLanguage.Init(language_system, wxLOCALE_CONV_ENCODING) ){
		//wxLogError(_T("This language is not supported by the system."));
		return false;
	}

	#ifdef __WXMAC__
		wxLocale::AddCatalogLookupPathPrefix(_("./nukak3d.app/Contents/Resources/locale"));
	#else
		wxLocale::AddCatalogLookupPathPrefix(_(".nukak3d/locale"));
	#endif
	pro_wxLocLanguage.AddCatalog(wxT("nukak3d"));
	#ifndef __WIN32__
		{
        //wxLogNull noLog;
        pro_wxLocLanguage.AddCatalog(_T("fileutils"));
		}
	#endif
	
	//! Object of main window
	nkNukak3D* mi_nukak3D = new nkNukak3D ( (wxWindow*)(NULL) , -1, 
		wxT("nukak3D"),
		wxDefaultPosition, 
		wxSize(1024,768));

	//! Splash screen 
	new wxSplashScreen(wxBitmap(logo_bioingenium_xpm),
          wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
          6000, mi_nukak3D, -1, wxDefaultPosition, wxDefaultSize,
          wxSIMPLE_BORDER|wxSTAY_ON_TOP);
	wxYield();

	//! Show maximize window.
	this->SetTopWindow (mi_nukak3D);
	//mi_nukak3D->Maximize(true);
	mi_nukak3D->Show (true);
	mi_nukak3D->Maximize(true);
	return true;
}

int nkMain::OnExit(){
    delete wxConfigBase::Set((wxConfigBase *) NULL);
    return 0;
}

IMPLEMENT_APP (nkMain)