/** 
 * @file nkAcercaDe.h
 * @brief Acerca De nukak3d.
 * @details Información acerca del diseño y desarrollo del nukak3D.
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
 * @brief Dialogo Acerca De.
 * @details Información acerca del diseño y desarrollo del nukak3D.
*/
class nkAcercaDe: public wxDialog
{
public:
	/**
	 * @brief Constructor de Clase.
	 * @details Datos necesarios para la generacion del dialogo Acerca De.
	*/
	nkAcercaDe(wxWindow* parent): wxDialog(parent, -1, "Nukak3D: Acerca De", wxDefaultPosition, wxSize(450,440))
	{
		new wxStaticBitmap(this, 
			wxID_ANY,
			wxBitmap(logo_bioingenium_xpm), 
			wxPoint(1,1), wxSize(443, 116),wxSIMPLE_BORDER);
		new wxStaticText(this, 
			wxID_ANY,
			wxT("\nNukak3D 2007 ")
			wxT("\nVersión 4.0 Revisión 3104.")
			wxT("\n(c) 2007 Grupo Bioingenium")
			wxT("\n\nEste proyecto ha sido desarrollado dentro de la línea de investigación")
			wxT("\nde computación gráfica del grupo Bioingenium.")
			wxT("\n")
			wxT("\n")
			wxT("\n")
			wxT("\nDesarrollado por:")
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

		new wxButton(this, wxID_OK, wxT("Aceptar"),wxPoint(200,370));	
	}
	/**
	 * @brief Destructor de clase.
	*/
	~nkAcercaDe(){}
};

#endif //_NKACERCADE_H_