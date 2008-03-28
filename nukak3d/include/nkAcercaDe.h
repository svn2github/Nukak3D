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
			wxT("\nNukak3D 2007 ")
			wxT("\nVersion 4.0 Revision 3104.")
			wxT("\n(c) 2007 Bioingenium Research Group.")
			wxT("\n\nThis project has been developed within the research")
			wxT("\nof computer graphics group Bioingenium.")
			wxT("\n")
			wxT("\n")
			wxT("\n")
			wxT("\nDeveloped By:")
			wxT("\n")
			wxT("\nAlexander Pinzón")
			wxT("\nByron Pérez")
			wxT("\nWilson Sarmiento")
			wxT("\nEduardo Romero")
			wxT("\n")
			wxT("\nUniversidad Nacional de Colombia")
			, 
			wxPoint(20,130), 
			wxDefaultSize);

		new wxButton(this, wxID_OK, wxT("Ok"),wxPoint(200,370));	
	}
	/**
	 * @brief Class destructor.
	*/
	~nkAcercaDe(){}
};

#endif //_NKACERCADE_H_