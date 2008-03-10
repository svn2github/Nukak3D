/** 
 * @file nkToolbarMac.cpp
 * @brief Toolbar de nukak3d.
 * @details Clase para la creacion de un toolbar vertical.
 * Especialmente diseÒado para el nukak3d para Mac.
 * @author Alexander Pinzon Fernandez
 * @version 1.0
 * @date 7/12/2007 04:56 p.m.
*/
#include "nkToolBarMac.h"
#include <wx/listimpl.cpp>
#include "icon_menu_mas.xpm"
#include "icon_menu_menos.xpm"

nkTool::nkTool(int toolId, 
		const wxString& label, 
		const wxBitmap& bitmap1, 
		const wxString& shortHelpString, 
		wxItemKind kind){
	prv_toolId = toolId;
	prv_label = label;
	prv_bitmap1 = bitmap1;
	prv_shortHelpString = shortHelpString;
	prv_kind = kind;
}
nkTool::~nkTool(){}

wxToolBarToolBase* nkTool::getTool(wxToolBar * un_tool){
	return un_tool->AddTool(prv_toolId,
		prv_label, 
		wxBitmap(prv_bitmap1.
			ConvertToImage().
			Scale(LARGO_X_ANCHO_ICONO.GetWidth(), LARGO_X_ANCHO_ICONO.GetHeight())),
		prv_shortHelpString,
		prv_kind);
}

nkMenuTool::nkMenuTool(wxWindow* parent, 
		wxWindowID id, 
		const wxString& nombreMenu,
		const wxPoint& pos, 
		const wxSize& size, 
		long style)
{
	prv_Arbol = ((wxTreeCtrl*)parent);
	prv_TreeItemId = prv_Arbol->AppendItem(prv_Arbol->GetRootItem(),nombreMenu,0);
	prv_desplegado = false;
}

nkMenuTool::~nkMenuTool(){
}

void nkMenuTool::insertarTool(nkTool* un_tool){
	int un_icono =  -1 ;
	prv_Arbol->AppendItem(prv_TreeItemId, un_tool->prv_label, un_icono);
}

bool nkMenuTool::estaDeplegado(void){
	return prv_desplegado;
}

void nkMenuTool::desplegarMenu(void){
	if (!estaDeplegado()){
		prv_desplegado = true;
		prv_Arbol->Expand(this);
	}
}

void nkMenuTool::replegarMenu(void){
	if (estaDeplegado()){
		prv_desplegado = false;
		prv_Arbol->Collapse(this);
	}
}

nkToolBar::nkToolBar(wxWindow* parent, 
                    wxWindowID id, 
                    const wxPoint& pos, 
					const wxSize& size, 
					long style, 
					const wxString& name):
		wxTreeCtrl(parent, id, pos, size, style)
{
	//
	wxImageList * mi_wxImgLista = new wxImageList(
		nkTool::LARGO_X_ANCHO_ICONO.GetWidth(),
		nkTool::LARGO_X_ANCHO_ICONO.GetHeight());
	mi_wxImgLista->Add(
		wxBitmap(wxBitmap(icon_menu_mas_xpm).
			ConvertToImage().
			Scale(nkTool::LARGO_X_ANCHO_ICONO.GetWidth(),
			nkTool::LARGO_X_ANCHO_ICONO.GetHeight())));
	SetImageList(mi_wxImgLista);
	AddRoot(name);
	SetBackgroundColour(wxColour(255,255,255));
}
nkToolBar::~nkToolBar(void){
}

nkMenuTool* nkToolBar::insertarMenu(long id, const wxString& nombreMenu){
	nkMenuTool* un_menu = new nkMenuTool(this, id, nombreMenu);
	return un_menu;
}

void nkToolBar::desplegarMenu(nkMenuTool* un_menu){
}

void nkToolBar::replegarMenu(nkMenuTool* un_menu){
}

void nkToolBar::replegarMenus(void){
	this->ExpandAll();
}
