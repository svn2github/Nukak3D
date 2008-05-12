/** 
 * @file nkAcercaDe.h
 * @brief About Nukak3D.
 * @author Alexander Pinzon Fernandez.
 * @version 0.1
 * @date 12/02/2007 03:52 p.m.
*/
#ifndef _NKACERCADE_H_
#define _NKACERCADE_H_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/bitmap.h>
#include <wx/statbmp.h>
#include "wx/intl.h"
#include "logo_bioingenium.xpm"

/**
 * @brief About Dialog.
*/
class nkAcercaDe: public wxDialog
{
public:
	/**
	 * @brief Class constructor.
	*/
	nkAcercaDe(wxWindow* parent): wxDialog(parent, -1, "Nukak3D: About", wxDefaultPosition, wxSize(450,440))
	{
		new wxStaticBitmap(this, 
			wxID_ANY,
			wxBitmap(logo_bioingenium_xpm), 
			wxPoint(1,1), wxSize(443, 116),wxSIMPLE_BORDER);
		new wxStaticText(this, 
			wxID_ANY,
			wxString(_("\nNukak3D 2007 "))<<
			wxString(_("\nVersion 4.0 Revision 3104."))<<
			wxString(_("\n(c) 2007 Bioingenium Research Group."))<<
			wxString(_("\n\nThis project has been developed within the research"))<<
			wxString(_("\nof computer graphics group Bioingenium."))<<
			wxString(_("\n"))<<
			wxString(_("\n"))<<
			wxString(_("\n"))<<
			wxString(_("\nDeveloped By:"))<<
			wxString(_("\n"))<<
			wxString(_("\nAlexander Pinzon"))<<
			wxString(_("\nByron Perez"))<<
			wxString(_("\nWilson Sarmiento"))<<
			wxString(_("\nEduardo Romero"))<<
			wxString(_("\n"))<<
			wxString(_("\nUniversidad Nacional de Colombia"))
			, 
			wxPoint(20,130), 
			wxDefaultSize);

		new wxButton(this, wxID_OK, _("Ok"),wxPoint(200,370));	
	}
	/**
	 * @brief Class destructor.
	*/
	~nkAcercaDe(){}
};

#endif //_NKACERCADE_H_