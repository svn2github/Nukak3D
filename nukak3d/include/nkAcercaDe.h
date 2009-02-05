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
#include "wx/hyperlink.h"
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
	nkAcercaDe(wxWindow* parent): wxDialog(parent, -1, "Nukak3D: About", wxDefaultPosition, wxSize(450,600))
	{
		new wxStaticBitmap(this, 
			wxID_ANY,
			wxBitmap(logo_bioingenium_xpm), 
			wxPoint(1,1), wxSize(443, 116),wxSIMPLE_BORDER);
		new wxStaticText(this, 
			wxID_ANY,
			wxString(_("\nNukak3D 2007 "))<<
			wxString(_("\nVersion 5.0"))<<
			wxString(_("\n(c) 2008 Bioingenium Research Group."))<<
			wxString(_("\n\nThis project has been developed within the research"))<<
			wxString(_("\nof computer graphics group Bioingenium."))<<
			wxString(_("\n"))<<
			wxString(_("\n"))<<
			wxString(_("\n"))<<
			wxString(_("\nVersion 5.0 Developed By:"))<<
			wxString(_("\n"))<<
			wxString(_("\nAlexander Pinzon"))<<
			wxString(_("\nByron Perez"))<<
			wxString(_("\nProject Manager: "))<<	
			wxString(_("\nEduardo Romero"))<<
			wxString(_("\n"))<<
			wxString(_("\nUniversidad Nacional de Colombia"))
			, 
			wxPoint(20,130), 
			wxDefaultSize);

		new wxStaticText(this, 
			wxID_ANY,
			  wxString(_("The name nukak was chosen for this software in honour of the Nukak Maku, yet one"))<<
			wxString(_("\nof the last survival ethnias from South America and presently towards extintion"))<<
			wxString(_("\nbecause the drug traffic war. In their cosmology, health is considered as a common"))<<
			wxString(_("\nproblem so that if one of them falls sick, they consider the whole community is ill"))<<
			wxString(_("\nand any particular cure must be shared between them all. This simple principle,"))<<
			wxString(_("\ncurrently forgotten for most western societies."))<<
			wxString(_("\nWas our main motivation to open up this software project.")),
			wxPoint(20,370), 
			wxDefaultSize);


		new wxHyperlinkCtrl(this, wxID_ANY, "Link to Nukak-Maku information page.", "http://www.survival.es/indigenas/nukak", wxPoint(20,510));
		new wxHyperlinkCtrl(this, wxID_ANY, "Link to Bioingenium Research Group.", "http://www.bioingenium.unal.edu.co", wxPoint(20,530));
		new wxButton(this, wxID_OK, _("Ok"),wxPoint(200,550));	
	}
	/**
	 * @brief Class destructor.
	*/
	~nkAcercaDe(){}
};

#endif //_NKACERCADE_H_