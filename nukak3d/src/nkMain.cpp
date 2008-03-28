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
	mi_nukak3D->Maximize(true);
	mi_nukak3D->Show (true);
	return true;
}

IMPLEMENT_APP (nkMain)