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
 * @file nkAdminPluginGui.cpp
 * @brief Admin Plugin's paths.
 * @author Alexander Pinzón Fernandez
 * @version 0.1
 * @date 24/04/2009 01:22 p.m.
*/
#include "nkAdminPluginGui.h"

nkAdminPluginGui::nkAdminPluginGui(wxWindow *parent, 
						nukak3d::nkKernel & mnKernel,
						wxWindowID id, 
						wxString title, 
						wxPoint pos, 
						wxSize size, 
						long style, 
						wxString name):
wxDialog(parent, id, title, pos, size, style, name), prvKernel(mnKernel){

	prv_wxGridPluginsPaths = new wxGrid(this,wxID_ANY, wxPoint(0,0), wxSize(400,220));
	prv_wxGridPluginsPaths->CreateGrid( 1, 1 );
	prv_wxGridPluginsPaths->SetColSize( 0, 500 );
	prv_wxGridPluginsPaths->SetColLabelValue(0, _("Plugin Path"));
	prv_wxGridPluginsPaths->SetEditable(false);
	prv_wxGridPluginsPaths->ForceRefresh();
	

	prv_wxGridPluginsLoaded= new wxGrid(this,wxID_ANY, wxPoint(0,300), wxSize(400,220));
	prv_wxGridPluginsLoaded->CreateGrid( 1, 3 );
	prv_wxGridPluginsLoaded->SetColSize( 0, 300 );
	prv_wxGridPluginsLoaded->SetColSize( 1, 70 );
	prv_wxGridPluginsLoaded->SetColSize( 2, 400 );
	prv_wxGridPluginsLoaded->SetColLabelValue(0, _("Plugin's Name"));
	prv_wxGridPluginsLoaded->SetColLabelValue(1, _("Plugin Type"));
	prv_wxGridPluginsLoaded->SetColLabelValue(2, _("Plugin Description"));

	prv_wxGridPluginsLoaded->SetEditable(false);
	prv_wxGridPluginsLoaded->ForceRefresh();

	my_cmdAddPlugin= new wxButton(this,nkAdminPluginGui::ID_ADD_PLUGIN,_("Add Plugin Path"));
	my_cmdDeletePlugin = new wxButton(this,nkAdminPluginGui::ID_DELETE_PLUGIN,_("Delete Plugin Path"));

	wxSizer *sizerCmd = new wxBoxSizer(wxHORIZONTAL);
	sizerCmd->Add(my_cmdAddPlugin, 0, wxALL, 5);
	sizerCmd->Add(my_cmdDeletePlugin, 0, wxALL, 5);

	wxSizer *sizerDialog = new wxBoxSizer(wxVERTICAL);
	sizerDialog->Add(prv_wxGridPluginsPaths, 1, wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	sizerDialog->Add(sizerCmd, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	sizerDialog->Add(prv_wxGridPluginsLoaded, 1, wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	sizerDialog->Add(new wxButton(this, wxID_OK, _("Ok")) , 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	sizerDialog->SetMinSize(640,480);

	this->SetSizer(sizerDialog);
	sizerDialog->Fit(this);
	this->SetSize(640,480);

	loadPlugins();
	loadPluginsLoaded();
}

BEGIN_EVENT_TABLE(nkAdminPluginGui, wxDialog)	
	EVT_BUTTON(nkAdminPluginGui::ID_ADD_PLUGIN, nkAdminPluginGui::eventAddPlugin)
	EVT_BUTTON(nkAdminPluginGui::ID_DELETE_PLUGIN, nkAdminPluginGui::eventDeletePlugin)
	EVT_GRID_SELECT_CELL(nkAdminPluginGui::eventSelectCell)
END_EVENT_TABLE()

void nkAdminPluginGui::eventAddPlugin(wxCommandEvent& WXUNUSED(event)){
	wxDirDialog * myDirDialog = new wxDirDialog(this,_("Open Dir..."));

	int OK = myDirDialog->ShowModal();
    
	if( OK==wxID_OK ){
		if (wxFileName::DirExists(myDirDialog->GetPath()) ){

			wxString a_plugin;
			a_plugin = myDirDialog->GetPath();

			nkUtilities::addNukak3DPluginPath(a_plugin);
			nkUtilities::writePluginsPaths();
			loadPlugins();
		}else{
			wxLogError(_("This dir not exist"));
		}
	}
	myDirDialog->Destroy();
}

void nkAdminPluginGui::eventDeletePlugin(wxCommandEvent& WXUNUSED(event))
{
	int my_rowGrid=0;
	int my_colGrid=0;
	int cantRow = prv_wxGridPluginsPaths->GetNumberRows();
	if(my_selectedCell != -1 &&  my_selectedCell < cantRow  ){
		wxString a_plugin = _("Delete this plugin path: ");
		a_plugin.Append(prv_wxGridPluginsPaths->GetCellValue(my_selectedCell, 0));
			
		wxMessageDialog * miDlg = new wxMessageDialog(this, a_plugin, _("Nukak3D: Confirmation"));
				
		if(miDlg->ShowModal() == wxID_OK){

			prv_wxGridPluginsPaths->DeleteRows(my_selectedCell);
			nkUtilities::getNukak3DPluginsPaths().clear();
			cantRow = cantRow-1;
			for(my_rowGrid=0; my_rowGrid<cantRow; my_rowGrid++){
					nkUtilities::addNukak3DPluginPath(prv_wxGridPluginsPaths->GetCellValue(my_rowGrid, 0));
			}			
			nkUtilities::writePluginsPaths();
			loadPlugins();
		}
	}
}

void nkAdminPluginGui::loadPlugins()
{
	nkUtilities::readPluginsPaths();
	size_t n = nkUtilities::getNukak3DPluginsPaths().size();
	wxString a_plugin;
	prv_wxGridPluginsPaths->ClearGrid();
	prv_wxGridPluginsPaths->DeleteRows(0, prv_wxGridPluginsPaths->GetNumberRows());
	prv_wxGridPluginsPaths->InsertRows(0,n);
	size_t mi_filaGrilla;
	for (mi_filaGrilla = 0;  mi_filaGrilla<n;  mi_filaGrilla++){
		a_plugin = nkUtilities::getNukak3DPluginsPaths().Item(mi_filaGrilla) ;
		prv_wxGridPluginsPaths->SetCellValue(mi_filaGrilla, 0, a_plugin);
	}
	my_selectedCell = -1;
}

void nkAdminPluginGui::loadPluginsLoaded(){
	size_t n = prvKernel.getnkITKFilterServer().getnkITKFilterCount();
	wxString a_param;
	prv_wxGridPluginsLoaded->ClearGrid();
	prv_wxGridPluginsLoaded->DeleteRows(0, prv_wxGridPluginsLoaded->GetNumberRows());
	prv_wxGridPluginsLoaded->InsertRows(0,n);
	size_t mi_filaGrilla;
	for (mi_filaGrilla = 0;  mi_filaGrilla<n;  mi_filaGrilla++){
		a_param = prvKernel.getnkITKFilterServer().getkITKFilter(mi_filaGrilla).getName();
		prv_wxGridPluginsLoaded->SetCellValue(mi_filaGrilla, 0, a_param);
		prv_wxGridPluginsLoaded->SetCellValue(mi_filaGrilla, 1, _("ITK Plugin"));
		a_param = prvKernel.getnkITKFilterServer().getkITKFilter(mi_filaGrilla).getDescription();
		prv_wxGridPluginsLoaded->SetCellValue(mi_filaGrilla, 2, a_param);
	}
	my_selectedCell = -1;

}

void nkAdminPluginGui::eventSelectCell(wxGridEvent &event){
	my_selectedCell = event.GetRow();
	event.Skip();
}