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

	prv_wxGridServers = new wxGrid(this,wxID_ANY, wxPoint(0,0), wxSize(400,220));
	prv_wxGridServers->CreateGrid( 1, 5 );
	prv_wxGridServers->SetColSize( 0, 100 );
	prv_wxGridServers->SetColSize( 1, 100 );
	prv_wxGridServers->SetColSize( 2, 100 );
	prv_wxGridServers->SetColSize( 3, 100 );
	prv_wxGridServers->SetColSize( 4, 60 );
	prv_wxGridServers->SetColLabelValue(0, _("Server Name"));
	prv_wxGridServers->SetColLabelValue(1, _("Host"));
	prv_wxGridServers->SetColLabelValue(2, _("AE Title"));
	prv_wxGridServers->SetColLabelValue(3, _("AE Called"));
	prv_wxGridServers->SetColLabelValue(4, _("Port"));
	prv_wxGridServers->SetEditable(false);
	prv_wxGridServers->ForceRefresh();

	my_cmdAddServer = new wxButton(this,nkServersDialog::ID_ADD_SERVER,_("Add Server"));
	my_cmdChangeServer = new wxButton(this,nkServersDialog::ID_CHANGE_SERVER,_("Change Server"));
	my_cmdDeleteServer = new wxButton(this,nkServersDialog::ID_DELETE_SERVER,_("Delete Server"));

	wxSizer *sizerCmd = new wxBoxSizer(wxHORIZONTAL);
	sizerCmd->Add(my_cmdAddServer, 0, wxALL, 5);
	sizerCmd->Add(my_cmdChangeServer, 0, wxALL, 5);
	sizerCmd->Add(my_cmdDeleteServer, 0, wxALL, 5);

	wxSizer *sizerDialog = new wxBoxSizer(wxVERTICAL);
	sizerDialog->Add(prv_wxGridServers, 1, wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
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
	EVT_BUTTON(nkServersDialog::ID_CHANGE_SERVER, nkServersDialog::prEventChangeServer)
	EVT_BUTTON(nkServersDialog::ID_DELETE_SERVER, nkServersDialog::prEventDeleteServer)
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

		my_servers = nkDICOMServer::ReadConfigSystem();
		//this->ReadConfigSystem();
		if(! my_servers.Contains(a_server)){
			my_servers = my_servers.append(a_server);
		}
		//this->WriteConfigSystem();
		nkDICOMServer::WriteConfigSystem(my_servers );
		loadServers();
	}
	miDlg->Destroy();
}

void nkServersDialog::prEventChangeServer(wxCommandEvent& WXUNUSED(event)){
	int my_rowGrid=0;
	int my_colGrid=0;
	int cantRow = prv_wxGridServers->GetNumberRows();
	if(my_selectedCell != -1 &&  my_selectedCell < cantRow  ){

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
		
		miDlg->cambiarValor(prv_wxGridServers->GetCellValue(my_selectedCell,0) ,0);
		miDlg->cambiarValor(prv_wxGridServers->GetCellValue(my_selectedCell,1) ,1);
		miDlg->cambiarValor(prv_wxGridServers->GetCellValue(my_selectedCell,2) ,2);
		miDlg->cambiarValor(prv_wxGridServers->GetCellValue(my_selectedCell,3) ,3);
		miDlg->cambiarValor(prv_wxGridServers->GetCellValue(my_selectedCell,4) ,4);
						
		miDlg->ShowModal(); 
		if(miDlg->GetReturnCode() == wxID_OK){

			prv_wxGridServers->SetCellValue(my_selectedCell,0, miDlg->obtenerValor(0));
			prv_wxGridServers->SetCellValue(my_selectedCell,1, miDlg->obtenerValor(1));
			prv_wxGridServers->SetCellValue(my_selectedCell,2, miDlg->obtenerValor(2));
			prv_wxGridServers->SetCellValue(my_selectedCell,3, miDlg->obtenerValor(3));
			prv_wxGridServers->SetCellValue(my_selectedCell,4, miDlg->obtenerValor(4));

			my_servers = "";
			for(my_rowGrid=0; my_rowGrid<cantRow; my_rowGrid++){
				for(my_colGrid=0; my_colGrid<5; my_colGrid++){
					my_servers = my_servers.append(prv_wxGridServers->GetCellValue(my_rowGrid, my_colGrid));
					my_servers = my_servers.append("|");
				}
				my_servers = my_servers.append("#");
			}			
			//this->WriteConfigSystem();
			nkDICOMServer::WriteConfigSystem(my_servers);
			loadServers();
		}
		miDlg->Destroy();
	}
}
void nkServersDialog::prEventDeleteServer(wxCommandEvent& WXUNUSED(event)){
	int my_rowGrid=0;
	int my_colGrid=0;
	int cantRow = prv_wxGridServers->GetNumberRows();
	if(my_selectedCell != -1 &&  my_selectedCell < cantRow  ){
		wxString a_server = _("Delete this server: ");
		a_server.Append(prv_wxGridServers->GetCellValue(my_selectedCell, 0));
			
		wxMessageDialog * miDlg = new wxMessageDialog(this, a_server, _("Nukak3D: Confirmation"));
				
		if(miDlg->ShowModal() == wxID_OK){

			prv_wxGridServers->DeleteRows(my_selectedCell);
			my_servers = "";
			cantRow = cantRow-1;
			for(my_rowGrid=0; my_rowGrid<cantRow; my_rowGrid++){
				for(my_colGrid=0; my_colGrid<5; my_colGrid++){
					my_servers = my_servers.append(prv_wxGridServers->GetCellValue(my_rowGrid, my_colGrid));
					my_servers = my_servers.append("|");
				}
				my_servers = my_servers.append("#");
			}			
			//this->WriteConfigSystem();
			nkDICOMServer::WriteConfigSystem(my_servers);
			loadServers();
		}
	}
}


void nkServersDialog::loadServers(){
	my_servers = nkDICOMServer::ReadConfigSystem();
	int n = nkDICOMServer::countServers(my_servers);
	wxString a_server, a_param;
	prv_wxGridServers->ClearGrid();
	prv_wxGridServers->DeleteRows(0, prv_wxGridServers->GetNumberRows());
	prv_wxGridServers->InsertRows(0,n);
	int mi_filaGrilla;
	for (mi_filaGrilla = 0;  mi_filaGrilla<n;  mi_filaGrilla++){
		a_server = nkDICOMServer::getStringServer(my_servers, mi_filaGrilla);
		prv_wxGridServers->SetCellValue(mi_filaGrilla, 0, nkDICOMServer::getParameterServer(a_server, 0));
		prv_wxGridServers->SetCellValue(mi_filaGrilla, 1, nkDICOMServer::getParameterServer(a_server, 1));
		prv_wxGridServers->SetCellValue(mi_filaGrilla, 2, nkDICOMServer::getParameterServer(a_server, 2));
		prv_wxGridServers->SetCellValue(mi_filaGrilla, 3, nkDICOMServer::getParameterServer(a_server, 3));
		prv_wxGridServers->SetCellValue(mi_filaGrilla, 4, nkDICOMServer::getParameterServer(a_server, 4));
	}
	my_selectedCell = -1;
}

void nkServersDialog::prEventSelectCell(wxGridEvent &event){
	my_selectedCell = event.GetRow();
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

wxString nkDICOMServer::ReadConfigSystem(){
	wxString my_servers = "";
	nkUtilities::readConfigBase("DICOM/servers", &my_servers, "");
	return my_servers;
}

bool nkDICOMServer::WriteConfigSystem(wxString my_servers){
	return nkUtilities::writeConfigBase("DICOM/servers", my_servers);
}