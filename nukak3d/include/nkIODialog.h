/** 
 * @file nkIODialog.h
 * @brief Generic dialog for input data.
 * @author  Byron Pérez, Alexander Pinzon
 * @version 0.3
 * @date 27/12/2007 03:37 p.m.
*/

#ifndef _NKIODIALOG_H_
#define _NKIODIALOG_H_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

/** 
 * @brief Generic dialog for input data.
 */
class nkIODialog: public wxDialog{
public:
	/**
	 * @brief Class constructor for input dialog.
	 * @param parent wxWindow Owner.
	 * @param unasEtiquetas array of wxString labels for controls, 
	 * @param un_tamanio numbers of controls.
	 * @param title Title window.
	 * @param pos Window position.
	 * @param size Window size.
	 * @param style Window style.
	 * @param name Object name.
	*/
	nkIODialog(wxWindow* parent, 
	   wxString unasEtiquetas[100],
	   int un_tamanio,
	   wxWindowID id = wxID_ANY, 
	   const wxString& title = "nukak3D", 
	   const wxPoint& pos = wxDefaultPosition, 
	   const wxSize& size = wxDefaultSize, 
	   long style = wxDEFAULT_DIALOG_STYLE, 
	   const wxString& name = "nkIODialog");

	/**
	 * @brief Class constructor for output dialog.
	 * @param parent wxWindow Owner.
	 * @param p_text Information text.
	 * @param title Title Window.
	 * @param pos Window position.
	 * @param size Window Size.
	 * @param style Window style.
	 * @param name Object name.
	*/
	nkIODialog(wxWindow* parent, 
	   wxString p_text,
	   wxWindowID id = wxID_ANY, 
	   const wxString& title = "Nukak3D", 
	   const wxPoint& pos = wxDefaultPosition, 
	   const wxSize& size = wxDefaultSize, 
	   long style = wxDEFAULT_DIALOG_STYLE, 
	   const wxString& name = "nkIODialog");

	/**
	 * @brief Class destructor.
	*/
	~nkIODialog();
	
	/**
	 * @brief Set initial value of the input control.
	 * @param un_valor set value.
	 * @param un_indice index of control to modified in 0 to numbers of controls -1.
	*/
	void cambiarValor(wxString un_valor, int un_indice);
	/**
	 * @brief Get value of control.
	 * @param un_indice index of control to get in 0 to numbers of controls -1.
	*/
	wxString obtenerValor(int un_indice);
private:
	/** List to store labels */
	wxTextCtrl * prv_wxTCValores[100];
	/** Number of controls */
	int prv_tamanio;
};
#endif _nkIODialog_H_