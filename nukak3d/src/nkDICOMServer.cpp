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
 * @file nkDICOMServer.cpp
 * @brief DICOM Server.
 * @details Clases for configure DICOM Servers.
 * @author Alexander Pinzón Fernandez
 * @version 0.1
 * @date 30/01/2009 03:46 p.m.
*/
#include "nkDICOMServer.h"
#include "wx/listimpl.cpp"

WX_DEFINE_LIST(nkListDICOMServer);

nkServersDialog::nkServersDialog(wxWindow *parent, 
						wxWindowID id, 
						wxString title, 
						wxPoint pos, 
						wxSize size, 
						long style, 
						wxString name):
	wxDialog(parent, id, title, pos, size, style, name){

	grid_servers = new wxGrid(this,wxID_ANY, wxPoint(0,0), wxSize(400,220));
	grid_servers->CreateGrid( 1, 5 );
	grid_servers->SetColSize( 0, 100 );
	grid_servers->SetColSize( 1, 100 );
	grid_servers->SetColSize( 2, 100 );
	grid_servers->SetColSize( 3, 100 );
	grid_servers->SetColSize( 4, 60 );
	grid_servers->SetColLabelValue(0, _("Server Name"));
	grid_servers->SetColLabelValue(1, _("Host"));
	grid_servers->SetColLabelValue(2, _("AE Title"));
	grid_servers->SetColLabelValue(3, _("AE Called"));
	grid_servers->SetColLabelValue(4, _("Port"));
	grid_servers->SetEditable(false);
	grid_servers->ForceRefresh();

	cmd_add_server = new wxButton(this,nkServersDialog::ID_ADD_SERVER,_("Add Server"));
	cmd_change_server = new wxButton(this,nkServersDialog::ID_CHANGE_SERVER,_("Change Server"));
	cmd_delete_server = new wxButton(this,nkServersDialog::ID_DELETE_SERVER,_("Delete Server"));

	wxSizer *sizerCmd = new wxBoxSizer(wxHORIZONTAL);
	sizerCmd->Add(cmd_add_server, 0, wxALL, 5);
	sizerCmd->Add(cmd_change_server, 0, wxALL, 5);
	sizerCmd->Add(cmd_delete_server, 0, wxALL, 5);

	wxSizer *sizerDialog = new wxBoxSizer(wxVERTICAL);
	sizerDialog->Add(grid_servers, 1, wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	sizerDialog->Add(sizerCmd, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	sizerDialog->Add(new wxButton(this, wxID_OK, _("Ok")) , 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	sizerDialog->SetMinSize(640,400);

	this->SetSizer(sizerDialog);
	sizerDialog->Fit(this);
	this->SetSize(640,400);

	loadServers();
}

BEGIN_EVENT_TABLE(nkServersDialog, wxDialog)	
	EVT_BUTTON(nkServersDialog::ID_ADD_SERVER, nkServersDialog::eventAddServer)
	EVT_BUTTON(nkServersDialog::ID_CHANGE_SERVER, nkServersDialog::eventChangeServer)
	EVT_BUTTON(nkServersDialog::ID_DELETE_SERVER, nkServersDialog::eventDeleteServer)
	EVT_GRID_SELECT_CELL(nkServersDialog::prEventSelectCell)
END_EVENT_TABLE()

void nkServersDialog::eventAddServer(wxCommandEvent& WXUNUSED(event)){
	wxString etiquetas[100];
	const int num_datos=5;

	etiquetas[0] = _("Server Name"); 
	etiquetas[1] = _("Host");
	etiquetas[2] = _("AE Title");
	etiquetas[3] = _("AE Called");
	etiquetas[4] = _("Port");
	
	nkIODialog * miDlg = new nkIODialog(	this, 
												etiquetas,
												num_datos,
												-1,
												_("Nukak3D: Config Server"),
												wxDefaultPosition,
												wxSize(330,(num_datos+4)*20+40));
	
	miDlg->cambiarValor(wxT("localhost"),0);
	miDlg->cambiarValor(wxT("127.0.0.1"),1);
	miDlg->cambiarValor(wxT("STORESCP"),2);
	miDlg->cambiarValor(wxT("ACME_STORE"),3);
	miDlg->cambiarValor(wxT("5678"),4);
					
	miDlg->ShowModal(); 
	if(miDlg->GetReturnCode() == wxID_OK){
		wxString a_server;
		a_server = (miDlg->obtenerValor(0))<<wxString("|")<<
			(miDlg->obtenerValor(1))<<wxString("|")<<
			(miDlg->obtenerValor(2))<<wxString("|")<<
			(miDlg->obtenerValor(3))<<wxString("|")<<
			(miDlg->obtenerValor(4))<<wxString("|#");

		str_servers = nkDICOMServer::readConfigSystem();
		//this->ReadConfigSystem();
		if(! str_servers.Contains(a_server)){
			str_servers = str_servers.append(a_server);
		}
		//this->WriteConfigSystem();
		nkDICOMServer::writeConfigSystem(str_servers );
		loadServers();
	}
	miDlg->Destroy();
}

void nkServersDialog::eventChangeServer(wxCommandEvent& WXUNUSED(event)){
	int my_rowGrid=0;
	int my_colGrid=0;
	int cantRow = grid_servers->GetNumberRows();
	if(selected_cell != -1 &&  selected_cell < cantRow  ){

		wxString etiquetas[100];
		const int num_datos=5;

		etiquetas[0] = _("Server Name"); 
		etiquetas[1] = _("Host");
		etiquetas[2] = _("AE Title");
		etiquetas[3] = _("AE Called");
		etiquetas[4] = _("Port");
		
		nkIODialog * miDlg = new nkIODialog(	this, 
													etiquetas,
													num_datos,
													-1,
													_("Nukak3D: Config Server"),
													wxDefaultPosition,
													wxSize(330,(num_datos+4)*20+40));
		
		miDlg->cambiarValor(grid_servers->GetCellValue(selected_cell,0) ,0);
		miDlg->cambiarValor(grid_servers->GetCellValue(selected_cell,1) ,1);
		miDlg->cambiarValor(grid_servers->GetCellValue(selected_cell,2) ,2);
		miDlg->cambiarValor(grid_servers->GetCellValue(selected_cell,3) ,3);
		miDlg->cambiarValor(grid_servers->GetCellValue(selected_cell,4) ,4);
						
		miDlg->ShowModal(); 
		if(miDlg->GetReturnCode() == wxID_OK){

			grid_servers->SetCellValue(selected_cell,0, miDlg->obtenerValor(0));
			grid_servers->SetCellValue(selected_cell,1, miDlg->obtenerValor(1));
			grid_servers->SetCellValue(selected_cell,2, miDlg->obtenerValor(2));
			grid_servers->SetCellValue(selected_cell,3, miDlg->obtenerValor(3));
			grid_servers->SetCellValue(selected_cell,4, miDlg->obtenerValor(4));

			str_servers = "";
			for(my_rowGrid=0; my_rowGrid<cantRow; my_rowGrid++){
				for(my_colGrid=0; my_colGrid<5; my_colGrid++){
					str_servers = str_servers.append(grid_servers->GetCellValue(my_rowGrid, my_colGrid));
					str_servers = str_servers.append("|");
				}
				str_servers = str_servers.append("#");
			}			
			//this->WriteConfigSystem();
			nkDICOMServer::writeConfigSystem(str_servers);
			loadServers();
		}
		miDlg->Destroy();
	}
}
void nkServersDialog::eventDeleteServer(wxCommandEvent& WXUNUSED(event)){
	int my_rowGrid=0;
	int my_colGrid=0;
	int cantRow = grid_servers->GetNumberRows();
	if(selected_cell != -1 &&  selected_cell < cantRow  ){
		wxString a_server = _("Delete this server: ");
		a_server.Append(grid_servers->GetCellValue(selected_cell, 0));
			
		wxMessageDialog * miDlg = new wxMessageDialog(this, a_server, _("Nukak3D: Confirmation"));
				
		if(miDlg->ShowModal() == wxID_OK){

			grid_servers->DeleteRows(selected_cell);
			str_servers = "";
			cantRow = cantRow-1;
			for(my_rowGrid=0; my_rowGrid<cantRow; my_rowGrid++){
				for(my_colGrid=0; my_colGrid<5; my_colGrid++){
					str_servers = str_servers.append(grid_servers->GetCellValue(my_rowGrid, my_colGrid));
					str_servers = str_servers.append("|");
				}
				str_servers = str_servers.append("#");
			}			
			//this->WriteConfigSystem();
			nkDICOMServer::writeConfigSystem(str_servers);
			loadServers();
		}
	}
}


void nkServersDialog::loadServers(){
	str_servers = nkDICOMServer::readConfigSystem();
	int n = nkDICOMServer::countServers(str_servers);
	wxString a_server, a_param;
	grid_servers->ClearGrid();
	grid_servers->DeleteRows(0, grid_servers->GetNumberRows());
	grid_servers->InsertRows(0,n);
	int mi_filaGrilla;
	for (mi_filaGrilla = 0;  mi_filaGrilla<n;  mi_filaGrilla++){
		a_server = nkDICOMServer::getStringServer(str_servers, mi_filaGrilla);
		grid_servers->SetCellValue(mi_filaGrilla, 0, nkDICOMServer::getParameterServer(a_server, 0));
		grid_servers->SetCellValue(mi_filaGrilla, 1, nkDICOMServer::getParameterServer(a_server, 1));
		grid_servers->SetCellValue(mi_filaGrilla, 2, nkDICOMServer::getParameterServer(a_server, 2));
		grid_servers->SetCellValue(mi_filaGrilla, 3, nkDICOMServer::getParameterServer(a_server, 3));
		grid_servers->SetCellValue(mi_filaGrilla, 4, nkDICOMServer::getParameterServer(a_server, 4));
	}
	selected_cell = -1;
}

void nkServersDialog::prEventSelectCell(wxGridEvent &event){
	selected_cell = event.GetRow();
	event.Skip();
}

nkDICOMServer::nkDICOMServer(wxString a_name,
			wxString a_host,
			wxString a_AETitle,
			wxString a_AECalled,
			int a_port){
	name = a_name;
	host = a_host;
	AETitle = a_AETitle;
	AECalled = a_AECalled;
	port = a_port;
}

int nkDICOMServer::countServers(wxString my_servers, wxChar a_char){
	return nkUtilities::countCharFromString(my_servers,a_char);
}

wxString nkDICOMServer::getParameter(wxString a_cad, wxChar a_char, int index){
	return nkUtilities::getStringFromCharToken(a_cad, a_char, index);
}

wxString nkDICOMServer::getStringServer(wxString my_servers, int index){
	//return nkDICOMServer::getParameter(my_servers, '#', index);
	return nkUtilities::getStringFromCharToken(my_servers, '#', index);
}

wxString nkDICOMServer::getParameterServer(wxString a_server, int index){
	//return nkDICOMServer::getParameter(a_server, '|', index);
	return nkUtilities::getStringFromCharToken(a_server, '|', index);
}

nkDICOMServer * nkDICOMServer::getDicomServer(wxString my_servers, int index){
	nkDICOMServer * myDS = NULL;
	wxString a_server = nkDICOMServer::getStringServer(my_servers, index);
	if(a_server.Length()>0){
		wxString a_name = nkDICOMServer::getParameterServer(a_server, 0);
		wxString a_host = nkDICOMServer::getParameterServer(a_server, 1);
		wxString a_AETitle = nkDICOMServer::getParameterServer(a_server, 2);
		wxString a_AECalled = nkDICOMServer::getParameterServer(a_server, 3);
		long a_port;
		(nkDICOMServer::getParameterServer(a_server, 4)).ToLong(&a_port);
		myDS = new nkDICOMServer(a_name, a_host, a_AETitle, a_AECalled , a_port);
	}
	return myDS;
}

wxString nkDICOMServer::readConfigSystem(){
	wxString my_servers = "";
	nkUtilities::readConfigBase("DICOM/servers", &my_servers, "");
	return my_servers;
}

bool nkDICOMServer::writeConfigSystem(wxString my_servers){
	return nkUtilities::writeConfigBase("DICOM/servers", my_servers);
}