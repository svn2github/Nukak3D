###### BEGIN GPL LICENSE BLOCK#####
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
# The Original Code is Copyright (C) 2007-2010 by Bioingenium Research Group.
# Bogota - Colombia
# All rights reserved.
#
# Author(s): Alexander Pinzón Fernández.
#
###### END GPL LICENSE BLOCK#####

/** 
 * @file nkToolbarMac.cpp
 * @brief Toolbar of nukak3d.
 * @details Class for Toolbar in unix and Mac systems
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
