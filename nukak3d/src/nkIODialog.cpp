/** 
 * @file nkIODialog.cpp
 * @brief Generic dialog for input, output information.
 * @author  Byron Pérez, Alexander Pinzon
 * @version 0.3
 * @date 27/12/2007 03:37 p.m.
*/
#include "nkIODialog.h"

nkIODialog::nkIODialog(wxWindow* parent, 
	   wxString unasEtiquetas[100],
	   int un_tamanio,
	   wxWindowID id, 
	   const wxString& title, 
	   const wxPoint& pos, 
	   const wxSize& size, 
	   long style, 
	   const wxString& name):
	wxDialog(parent, id, title, pos, size, style, name)
{
	this->prv_tamanio = un_tamanio;
	for (int i=0; i<prv_tamanio; i++){
		new wxStaticText(this,
			-1,
			unasEtiquetas[i],
			wxPoint(20,20*(i+1)));
		prv_wxTCValores[i] = new wxTextCtrl(this,
			-1,
			wxT("0"),
			wxPoint(200,20*(i+1)));
	}
	new wxButton(this, wxID_OK, _("OK"),wxPoint(40,prv_tamanio*20+40));	
	new wxButton(this, wxID_CANCEL, _("Cancel"),wxPoint(170,prv_tamanio*20+40));

	CenterOnParent();

}

nkIODialog::nkIODialog(wxWindow* parent, 
	   wxString p_text,
	   wxWindowID id, 
	   const wxString& title, 
	   const wxPoint& pos, 
	   const wxSize& size, 
	   long style, 
	   const wxString& name):
	wxDialog(parent, id, title, pos, size, style, name)
{
	new wxTextCtrl(this,-1,p_text,wxPoint(30,30),wxSize(580,240),wxTE_MULTILINE | wxTE_READONLY );
	new wxButton(this, wxID_OK, _("OK"),wxPoint(270,280));	

	CenterOnParent();

}

nkIODialog::~nkIODialog(){
	/**/
}

void nkIODialog::cambiarValor(wxString un_valor, int un_indice){
	if (un_indice<prv_tamanio){
		prv_wxTCValores[un_indice]->SetValue(un_valor);
	}
}

wxString nkIODialog::obtenerValor(int un_indice){
	wxString miValor = wxT("");
	if (un_indice<prv_tamanio){
		miValor = prv_wxTCValores[un_indice]->GetValue();
	}
	return miValor;
}