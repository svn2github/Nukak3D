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
 * @file nkFindSCU.cpp
 * @brief DICOM C-Find.
 * @details Clases for config and execute C-Find request.
 * @author Alexander Pinzón Fernandez
 * @version 0.1
 * @date 26/01/2009 01:12 p.m.
*/

#include "nkFindSCU.h"
#include "wx/listimpl.cpp"

using namespace std;

nkFindSCU::nkFindSCU(nkNukak3D *parent, wxWindowID id , const wxString &title, const wxPoint &pos, const wxSize &size ):
	//wxPanel(parent, id , title, pos, size, wxDEFAULT_FRAME_STYLE | wxSTAY_ON_TOP ){
	wxPanel(parent, id , pos, size, wxDEFAULT_FRAME_STYLE | wxSTAY_ON_TOP ){
	aui_manager.SetManagedWindow(this);
	aui_manager.SetFlags(aui_manager.GetFlags()|
		wxAUI_MGR_ALLOW_ACTIVE_PANE|
		wxAUI_MGR_HINT_FADE|
		wxAUI_MGR_TRANSPARENT_HINT);

	parent =  parent;
	this->search_panel = new wxPanel(this, wxID_ANY);
	
	cmd_server_localtion = new wxButton(search_panel,nkFindSCU::ID_SERVER_LOCATION, _("Server Location") );

	list_server_localtion = new wxListBox(search_panel, wxID_ANY, wxDefaultPosition, wxSize(130,60));
	loadServers();

	text_patien_ID = new wxTextCtrl(search_panel, wxID_ANY, "", wxDefaultPosition, wxSize(130, 20));
	
	text_first_name = new wxTextCtrl(search_panel, wxID_ANY, "", wxDefaultPosition, wxSize(130, 20));
	text_accession_number= new wxTextCtrl(search_panel, wxID_ANY, "", wxDefaultPosition, wxSize(130, 20));


	chk_study_date_from = new wxCheckBox(search_panel, nkFindSCU::ID_CHECK_STUDY_DATE_FROM, _("Use Study Date(From)")) ;
	cmd_study_date_from = new wxButton(search_panel, nkFindSCU::ID_CALENDAR_STUDY_DATE_FROM, _("Change")) ;
	cmd_study_date_from->Disable();
	text_study_date_from = new wxTextCtrl(search_panel, wxID_ANY, "", wxDefaultPosition, wxSize(130, 20));
	text_study_date_from->SetEditable(false);
	text_study_date_from->Disable();

	chk_study_date_to = new wxCheckBox(search_panel, nkFindSCU::ID_CHECK_STUDY_DATE_TO, _("Use Study Date(To)")) ;
	cmd_study_date_to = new wxButton(search_panel, nkFindSCU::ID_CALENDAR_STUDY_DATE_TO, _("Change")) ;
	cmd_study_date_to->Disable();
	text_study_date_to = new wxTextCtrl(search_panel, wxID_ANY, "", wxDefaultPosition, wxSize(130, 20));
	text_study_date_to->SetEditable(false);
	text_study_date_to->Disable();

	text_study_description = new wxTextCtrl(search_panel, wxID_ANY, "", wxDefaultPosition, wxSize(130, 20));
	text_institution_name = new wxTextCtrl(search_panel, wxID_ANY, "", wxDefaultPosition, wxSize(130, 20));

	wxSizer * sizerServers = new wxGridSizer(1, 2, 1, 1);
	sizerServers->Add(cmd_server_localtion, 0,  wxALL, 2);
	sizerServers->Add(list_server_localtion, 1,  wxGROW | wxALL, 2);

	wxSizer *sizerServer = new wxGridSizer(9, 2, 1, 1);

	sizerServer->Add(new wxStaticText(search_panel, wxID_ANY, _("Patien ID:")), 0,  wxALL, 2);
	sizerServer->Add(text_patien_ID, 1,  wxGROW | wxALL, 2);

	sizerServer->Add(new wxStaticText(search_panel, wxID_ANY, _("Name:")), 0,  wxALL, 2);
	sizerServer->Add(text_first_name, 1,  wxGROW | wxALL, 2);


	sizerServer->Add(new wxStaticText(search_panel, wxID_ANY, _("Accession Number:")), 0,  wxALL, 2);
	sizerServer->Add(text_accession_number, 1,  wxGROW | wxALL, 2);

	sizerServer->Add(chk_study_date_from, 0,  wxALL, 2);
	sizerServer->Add( cmd_study_date_from, 0,  wxALL, 2);
	sizerServer->Add(new wxStaticText(search_panel, wxID_ANY, _("Study Date(From)")), 0,  wxALL, 2);
	sizerServer->Add(text_study_date_from, 1,  wxGROW | wxALL, 2);

	sizerServer->Add(chk_study_date_to, 0,  wxALL, 2);
	sizerServer->Add(cmd_study_date_to, 0,  wxALL, 2);
	sizerServer->Add(new wxStaticText(search_panel, wxID_ANY, _("Study Date(To)")), 0,  wxALL, 2);
	sizerServer->Add(text_study_date_to, 1,  wxGROW | wxALL, 2);

	sizerServer->Add(new wxStaticText(search_panel, wxID_ANY, _("Study Description:")), 0,  wxALL, 2);
	sizerServer->Add(text_study_description, 1,  wxGROW | wxALL, 2);
	sizerServer->Add(new wxStaticText(search_panel, wxID_ANY, _("Institution Name:")), 0,  wxALL, 2);
	sizerServer->Add(text_institution_name, 1,  wxGROW | wxALL, 2);

	cmd_search = new wxButton(search_panel,nkFindSCU::ID_SEARCH, _("Search") );
	cmd_clear = new wxButton(search_panel,nkFindSCU::ID_CLEAR, _("Clear") );
	cmd_get_study = new wxButton(search_panel,nkFindSCU::ID_GET_STUDY, _("Get selected patient") );
	
	wxSizer * sizerSearch = new wxBoxSizer(wxVERTICAL);
	sizerSearch->Add(sizerServers, 0,  wxALL, 1);
	sizerSearch->Add(sizerServer, 0,  wxALL, 1);
	sizerSearch->Add(cmd_search, 0,  wxALL, 1);
	sizerSearch->Add(cmd_clear, 0,  wxALL, 1);
	sizerSearch->Add(cmd_get_study, 0,  wxALL, 1);

	search_panel->SetSizer(sizerSearch);
	sizerSearch->Fit(search_panel);


	aui_manager.AddPane(search_panel, wxAuiPaneInfo().
              Name("Search").
			  Left().
			  Caption(_("Search")).
			  TopDockable(false).
			  BottomDockable(false).
			  CloseButton(false).
			  PaneBorder(true).
			  PinButton(true).
			  CaptionVisible(true).
			  Floatable(true).
			  MinSize(300, 400).
			  FloatingSize(300, 400).
			  MaxSize(300, 800));

	//wxTextCtrl * miy = new wxTextCtrl(this, wxID_ANY, "asdasd");

	grid_patients = new wxGrid(this,wxID_ANY, wxPoint(0,0), wxSize(400,220));
	grid_patients->SetColMinimalAcceptableWidth(0);
	grid_patients->CreateGrid(0 , 12 );
	grid_patients->SetColSize( 0, 120 );
	grid_patients->SetColSize( 1, 160 );
	grid_patients->SetColSize( 2, 30 );
	grid_patients->SetColSize( 3, 30 );
	grid_patients->SetColSize( 4, 120 );
	grid_patients->SetColSize( 5, 120 );
	grid_patients->SetColSize( 6, 70 );
	grid_patients->SetColSize( 7, 120 );
	grid_patients->SetColSize( 8, 120 );
	//0
	grid_patients->SetColSize( 9, 40 );
	//0
	grid_patients->SetColSize( 10, 40 );
	grid_patients->SetColSize( 11, 120 );
	grid_patients->SetColLabelValue(0, _("Patien ID"));
	grid_patients->SetColLabelValue(1, _("Name"));
	grid_patients->SetColLabelValue(2, _("Sex"));
	grid_patients->SetColLabelValue(3, _("Age"));
	grid_patients->SetColLabelValue(4, _("Study Date"));
	grid_patients->SetColLabelValue(5, _("Description"));
	grid_patients->SetColLabelValue(6, _("Acceccion #"));
	grid_patients->SetColLabelValue(7, _("Modality"));
	grid_patients->SetColLabelValue(8, _("Institution Name"));
	grid_patients->SetColLabelValue(9, _("Study Instance UID"));
	grid_patients->SetColLabelValue(10, _("Series Instance UID"));
	grid_patients->SetColLabelValue(11, _("Study ID"));
	grid_patients->SetEditable(false);
	grid_patients->ForceRefresh();

	aui_manager.AddPane(grid_patients, wxAuiPaneInfo().
              Name("SearchGrid").
			  Center().
			  CloseButton(false).
			  PinButton(false).
			  Floatable(false).
			  Caption(_("Search results: Study's List")));

	aui_manager.Update();

	selected_cell = -1;

}

BEGIN_EVENT_TABLE(nkFindSCU, wxPanel)
	EVT_CHECKBOX(nkFindSCU::ID_CHECK_STUDY_DATE_FROM, nkFindSCU::eventUseDateFrom)
	EVT_CHECKBOX(nkFindSCU::ID_CHECK_STUDY_DATE_TO, nkFindSCU::eventUseDateTo)
	EVT_BUTTON(nkFindSCU::ID_CALENDAR_STUDY_DATE_FROM, nkFindSCU::eventChangeDateFrom)
	EVT_BUTTON(nkFindSCU::ID_CALENDAR_STUDY_DATE_TO, nkFindSCU::eventChangeDateTo)
	EVT_BUTTON(nkFindSCU::ID_SERVER_LOCATION, nkFindSCU::eventServerLocation)
	EVT_BUTTON(nkFindSCU::ID_CLEAR, nkFindSCU::eventClear)
	EVT_BUTTON(nkFindSCU::ID_SEARCH, nkFindSCU::eventSearch)
	EVT_BUTTON(nkFindSCU::ID_GET_STUDY, nkFindSCU::eventGetStudy)
	EVT_GRID_SELECT_CELL(nkFindSCU::eventSelectCell)
END_EVENT_TABLE()

nkFindSCU::~nkFindSCU(){
	if(!prv_listServers.IsEmpty())	prv_listServers.DeleteContents(true);
	//this->DestroyChildren();
	aui_manager.UnInit();
}

void nkFindSCU::eventUseDateFrom(wxCommandEvent& WXUNUSED(event)){
	if(chk_study_date_from->GetValue()){
		 cmd_study_date_from->Enable();
		text_study_date_from->Enable();
	}else{
		 cmd_study_date_from->Disable();
		text_study_date_from->Disable();
	}
}

void nkFindSCU::eventUseDateTo(wxCommandEvent& WXUNUSED(event)){
	if(chk_study_date_to->GetValue()){
		cmd_study_date_to->Enable();
		text_study_date_to->Enable();
	}else{
		cmd_study_date_to->Disable();
		text_study_date_to->Disable();
	}
}

void nkFindSCU::eventChangeDateFrom(wxCommandEvent& WXUNUSED(event)){
	nkCalendarDialog * my_calDialog = new nkCalendarDialog(this);
	my_calDialog->ShowModal();
	text_study_date_from->SetLabel(my_calDialog->getDate().Format("%Y%m%d"));
	my_calDialog->Destroy();
}

void nkFindSCU::eventChangeDateTo(wxCommandEvent& WXUNUSED(event)){
	nkCalendarDialog * my_calDialog = new nkCalendarDialog(this);
	my_calDialog->ShowModal();
	text_study_date_to->SetLabel(my_calDialog->getDate().Format("%Y%m%d"));
	my_calDialog->Destroy();
}


void nkFindSCU::eventServerLocation(wxCommandEvent& WXUNUSED(event)){
	nkServersDialog * my_servers = new nkServersDialog(this);
	my_servers->ShowModal();
	my_servers->Destroy();
	loadServers();
}

void nkFindSCU::loadServers(){
	wxString my_servers = nkDICOMServer::readConfigSystem();
	int cant = nkDICOMServer::countServers(my_servers);
	int i;
	list_server_localtion->Clear();
	for (i=0; i<cant;i++){
		nkDICOMServer * myDS = nkDICOMServer::getDicomServer(my_servers,i);
		prv_listServers.Append(myDS);
		list_server_localtion->Append(myDS->name);
	}
}

void nkFindSCU::eventClear(wxCommandEvent& WXUNUSED(event)){
	text_patien_ID->SetValue(wxT(""));
	text_first_name->SetValue(wxT(""));
	text_accession_number->SetValue(wxT(""));

	chk_study_date_from->SetValue(false);
	 cmd_study_date_from->Disable();
	text_study_date_from->SetValue("");

	chk_study_date_to->SetValue(false);
	cmd_study_date_to->Disable();
	text_study_date_to->SetValue("");

	text_study_description->SetValue("");
	text_institution_name->SetValue("");
	
	grid_patients->ClearGrid();
	grid_patients->DeleteRows(0, grid_patients->GetNumberRows());
	selected_cell = -1;

}

void nkFindSCU::eventSearch(wxCommandEvent& WXUNUSED(event)){
	int myindexserver = getServer();
	if(myindexserver ==-1){
		wxMessageBox(_("Select a DICOM server"),_("Nukak3D: Alert"));
		return;
	}
	wxString my_DicomText = "";
	wxString a_date = "";
	if(text_study_date_from->IsEnabled() || text_study_date_to->IsEnabled()){
		if((text_study_date_from->GetValue().Length()>0) || (text_study_date_to->GetValue().Length()>0)){	
			a_date.Append( text_study_date_from->GetValue());
			a_date.Append( "-");
			a_date.Append( text_study_date_to->GetValue());
		}
	}

	wxString a_patientName = "";
	a_patientName.Append(text_first_name->GetValue());
	a_patientName.Replace(" ", "*");
	a_patientName.Append("*");
	

	my_DicomText.Append("(0008,0005) CS [] # SpecificCharacterSet\n");
	my_DicomText.Append("(0008,0020) DA ["); 
		my_DicomText.Append(a_date); 
		my_DicomText.Append("] # StudyDate\n");
	my_DicomText.Append("(0008,0021) DA [] # SeriesDate\n");
	my_DicomText.Append("(0008,0050) SH [");
		my_DicomText.Append(text_accession_number->GetValue());
		my_DicomText.Append("] # AccessionNumber\n");
	//my_DicomText.Append("(0008,0052) CS [STUDY] # QueryRetrieveLevel\n");
		my_DicomText.Append("(0008,0052) CS [SERIES] # QueryRetrieveLevel\n");
	my_DicomText.Append("(0008,0061) CS [] # ModalitiesInStudy\n");
	my_DicomText.Append("(0008,0080) LO [");
		my_DicomText.Append(text_institution_name->GetValue());
		my_DicomText.Append("] # InstitutionName\n");
	my_DicomText.Append("(0008,0090) PN [] # ReferringPhysiciansName\n");
	my_DicomText.Append("(0008,1030) LO [");
		my_DicomText.Append(text_study_description->GetValue());
		my_DicomText.Append("] # StudyDescription\n");
	my_DicomText.Append("(0010,0010) PN [");
		my_DicomText.Append(a_patientName);
		my_DicomText.Append("] # PatientsName\n");
	my_DicomText.Append("(0010,0020) LO [");
	my_DicomText.Append(text_patien_ID->GetValue());
		my_DicomText.Append("] # PatientID\n");
	my_DicomText.Append("(0010,0030) DA [] # PatientsBirthDate\n");
	my_DicomText.Append("(0010,0040) CS [] # PatientsSex\n");
	my_DicomText.Append("(0010,1010) AS [] # PatientsAge\n");
	my_DicomText.Append("(0020,000D) UI [] # StudyInstanceUID\n");
	my_DicomText.Append("(0020,000E) UI [] # SeriesInstanceUID\n");
	my_DicomText.Append("(0020,0010) SH [] # StudyID\n");
	my_DicomText.Append("(0020,0011) IS [] # SeriesNumber\n");
	my_DicomText.Append("(0020,1206) IS [] # NumberOfStudyRelatedSeries\n");
	my_DicomText.Append("(0020,1208) IS [] # NumberOfStudyRelatedInstances\n");

	

	//wxString my_path = wxStandardPaths::Get().GetExecutablePath();
	//my_path = my_path.Left(my_path.Find("nukak3d"));
	wxString my_pathDestination = nkUtilities::getNukak3DDataDir();

	wxString my_path = "";
	my_path.Append(my_pathDestination);
	my_path.Append(wxFileName::GetPathSeparator());
	my_path.Append("query.txt");
	wxFile * myfile = new wxFile(my_path, wxFile::write);
	myfile->Write(my_DicomText);
	myfile->Close();

	//Copy dump3dcm
	wxString myDumpExecutableName = "dump2dcm";
	myDumpExecutableName.Append(nkUtilities::getExecutableExtension());
	wxString myDumpOriginal = "";
	
	myDumpOriginal.Append(nkUtilities::getNukak3DPath());
	myDumpOriginal.Append(wxFileName::GetPathSeparator());
	myDumpOriginal.Append(myDumpExecutableName);

	wxString myDumpCopia = "";
	myDumpCopia.Append(my_pathDestination);
	myDumpCopia.Append(wxFileName::GetPathSeparator());
	myDumpCopia.Append(myDumpExecutableName);
	
	nkUtilities::copyFile(myDumpOriginal, myDumpCopia);

	wxString myQueryDcm = "";
	myQueryDcm.Append(nkUtilities::getNukak3DDataDir());
	myQueryDcm.Append(wxFileName::GetPathSeparator());
	myQueryDcm.Append("query.dcm");

	wxString str_dump = "";
	str_dump.Append(myDumpCopia);
	str_dump.Append( " -v ");
	str_dump.Append(my_path);
	str_dump.Append(" ");
	str_dump.Append(myQueryDcm);

	nkUtilities::setWorkingDirectoryNukak3D();
	wxProcess *process = wxProcess::Open(str_dump);
	wxString process_in = "-1";
	if(process){
		nkPipedProcess * miu = new nkPipedProcess(this, "Validating search...", process);
		miu->ShowModal();
		process_in = miu->m_textIn;
		miu->Destroy();
	}

	bool dumpfile = false;
	if(process_in.Find("dump successfully converted.") != wxNOT_FOUND) dumpfile = true;
	if(dumpfile){
		if(myindexserver != -1){
			nkDICOMServer *  myDicomS = prv_listServers.Item(myindexserver)->GetData();

			wxString myFindOriginal, myFindCopy, myFindExecutableName;
			myFindOriginal = "";
			myFindCopy = "";
			myFindExecutableName = "";
			myFindExecutableName.Append("findscu");
			myFindExecutableName.Append(nkUtilities::getExecutableExtension());

			myFindOriginal.Append(nkUtilities::getNukak3DPath());
			myFindOriginal.Append(wxFileName::GetPathSeparator());
			myFindOriginal.Append(myFindExecutableName);

			myFindCopy.Append(nkUtilities::getNukak3DDataDir());
			myFindCopy.Append(wxFileName::GetPathSeparator());
			myFindCopy.Append(myFindExecutableName);
			nkUtilities::copyFile(myFindOriginal, myFindCopy);

			wxString str_find = "";
			str_find.Append(myFindCopy);
			str_find.Append(" -S ");
			str_find.Append("-aet ");	str_find.Append(myDicomS->AETitle);
			str_find.Append(" -aec ");	str_find.Append(myDicomS->AECalled);
			str_find.Append(" ");	str_find.Append(myDicomS->host);
			str_find.Append(" ");	str_find.Append(wxString::Format("%d", myDicomS->port));
			str_find.Append(" ");
			str_find.Append(myQueryDcm);
			nkUtilities::setWorkingDirectoryNukak3D();
			wxProcess *processFind = wxProcess::Open(str_find);
			wxString process_infind = "-1";
			if(processFind){
				nkPipedProcessThread * myfind = new nkPipedProcessThread(this, "Search patient, please wait...", processFind);
				myfind->ShowModal();
				process_infind = myfind->m_textIn;
				analyzeOutput(process_infind);
			}
			wxRemoveFile(myFindCopy);
		}else{
			wxMessageBox(_("Select a DICOM server"),_("Nukak3D: Alert"));
		}
	}
	wxRemoveFile(myQueryDcm);
	wxRemoveFile(my_path);
	wxRemoveFile(myDumpCopia);
}

int nkFindSCU::getServer(){
	int indexServer = -1;
	nkListDICOMServer::compatibility_iterator node = prv_listServers.GetFirst();
	bool not_found = true;
	int count = -1;
	wxString name_server = list_server_localtion->GetStringSelection();
	if (! name_server.IsEmpty()){
		while (node && not_found){
			count = count + 1;
			nkDICOMServer * current = node->GetData();

			if (name_server.Cmp(current->name) == 0 ){
				indexServer = count;
				not_found = false;
			}
			node = node->GetNext();
		}
	}
	return indexServer;
}

void nkFindSCU::analyzeOutput(wxString a_cad){
	int numberLines = nkDICOMServer::countServers(a_cad, '\n');
	wxString cad_line;
	wxString key;
	wxString value;
	int mi_filagrilla = -1;
	grid_patients->ClearGrid();
	grid_patients->DeleteRows(0, grid_patients->GetNumberRows());
	for (int i_line = 0; i_line<numberLines; i_line++){
		cad_line = nkDICOMServer::getParameter(a_cad, '\n', i_line);
		if(cad_line.Length()>1){
			switch(cad_line[0]){
			case '\n':
			break;
			case '#':
			break;
			case '(':
				key = nkDICOMServer::getParameter(cad_line, ')', 0);
				key = key.SubString(1,key.Length());
				if(!cad_line.Contains("no value available")){
					value = cad_line.SubString(cad_line.Find('[')+1,cad_line.Find(']')-1);
					if(key.Cmp("0010,0020")==0) grid_patients->SetCellValue(mi_filagrilla, 0,value);
					if(key.Cmp("0010,0010")==0) grid_patients->SetCellValue(mi_filagrilla, 1,value);
					if(key.Cmp("0010,0040")==0) grid_patients->SetCellValue(mi_filagrilla, 2,value);
					if(key.Cmp("0010,1010")==0) grid_patients->SetCellValue(mi_filagrilla, 3,value);
					
					if(key.Cmp("0008,0020")==0){
						value = value.SubString(0,3) + "/" + value.SubString(4,5) + "/" + value.SubString(6,7);
						grid_patients->SetCellValue(mi_filagrilla, 4,value);
					}

					if(key.Cmp("0008,1030")==0) grid_patients->SetCellValue(mi_filagrilla, 5,value);
					if(key.Cmp("0008,0050")==0) grid_patients->SetCellValue(mi_filagrilla, 6,value);
					if(key.Cmp("0008,0061")==0) grid_patients->SetCellValue(mi_filagrilla, 7,value);
					if(key.Cmp("0008,0080")==0) grid_patients->SetCellValue(mi_filagrilla, 8,value);
					if(key.Cmp("0020,000D")==0) grid_patients->SetCellValue(mi_filagrilla, 9,value);
					if(key.Cmp("0020,000d")==0) grid_patients->SetCellValue(mi_filagrilla, 9,value);
					if(key.Cmp("0020,000E")==0) grid_patients->SetCellValue(mi_filagrilla, 10,value);
					if(key.Cmp("0020,000e")==0) grid_patients->SetCellValue(mi_filagrilla, 10,value);
					if(key.Cmp("0020,0010")==0) grid_patients->SetCellValue(mi_filagrilla, 11,value);
					
				}
			break;
			case 'R':
				mi_filagrilla = mi_filagrilla + 1;
				grid_patients->InsertRows(mi_filagrilla, 1);
			break;
			case '-':
			break;
			}
		}
	}
	selected_cell = -1;
}

void nkFindSCU::eventSelectCell(wxGridEvent &event){
	selected_cell = event.GetRow();
	event.Skip();
}

void nkFindSCU::eventGetStudy(wxCommandEvent& WXUNUSED(event)){
	if(selected_cell != -1){
		int myindexserver = getServer();
		if(myindexserver ==-1){
			wxMessageBox(_("Select a DICOM server"),_("Nukak3D: Alert"));
			return;
		}
		wxString my_DicomText = "";
		wxString a_patientName = grid_patients->GetCellValue(selected_cell, 1);
		a_patientName.Replace(" ", "_");
		

		my_DicomText.Append("(0008,0005) CS [] # SpecificCharacterSet\n");
		//my_DicomText.Append("(0008,0052) CS [STUDY] # QueryRetrieveLevel\n");
		my_DicomText.Append("(0008,0052) CS [SERIES] # QueryRetrieveLevel\n");
		my_DicomText.Append("(0010,0010) PN [");
			my_DicomText.Append( grid_patients->GetCellValue(selected_cell, 1) );
			my_DicomText.Append("] # PatientsName\n");
		my_DicomText.Append("(0010,0020) LO [");
			my_DicomText.Append( grid_patients->GetCellValue(selected_cell, 0) );
			my_DicomText.Append("] # PatientID\n");

		my_DicomText.Append("(0020,000D) UI [");
		wxLogError("] # StudyInstanceUID\n" + grid_patients->GetCellValue(selected_cell, 9));
			my_DicomText.Append( grid_patients->GetCellValue(selected_cell, 9) );
			my_DicomText.Append("] # StudyInstanceUID\n");
		my_DicomText.Append("(0020,000E) UI [");
			wxLogError("] # SeriesInstanceUID\n" + grid_patients->GetCellValue(selected_cell, 10));
			my_DicomText.Append( grid_patients->GetCellValue(selected_cell, 10) );
			my_DicomText.Append("] # SeriesInstanceUID\n");
		my_DicomText.Append("(0020,0010) SH [");
			my_DicomText.Append( grid_patients->GetCellValue(selected_cell, 11) );
			my_DicomText.Append("] # StudyID\n");

		wxString outPath = "";
		wxString outPathPatient = "";
		wxString outPathProgramMoveSCU = "";
		wxString outPathQuertyDCM = "";
		wxString nukakPath = "";
		wxString nukakPathProgramMoveSCU = "";
		wxString nukakPathQuertyTXT = "";
		wxString nameProgramMoveSCU = "";
#ifdef __WIN32__
		nameProgramMoveSCU.Append("movescu.exe");
#else
		nameProgramMoveSCU.Append("movescu");
#endif //__WIN32__

		nkStoreSCP * my_listener = nkStoreSCP::getInstance(this);
		outPath = my_listener->getOutputDirectory();
		my_listener = NULL;

		outPathPatient.Append(outPath);
		outPathPatient.Append(wxFileName::GetPathSeparator());
		outPathPatient.Append(a_patientName);

		if(!wxFileName::DirExists(outPathPatient))wxFileName::Mkdir(outPathPatient);

		outPathProgramMoveSCU.Append(outPathPatient);
		outPathProgramMoveSCU.Append(wxFileName::GetPathSeparator());
		outPathProgramMoveSCU.Append(nameProgramMoveSCU);

		outPathQuertyDCM.Append(outPathPatient);
		outPathQuertyDCM.Append(wxFileName::GetPathSeparator());
		outPathQuertyDCM.Append("query.dcm");

		nukakPath = nkUtilities::getNukak3DPath();
		nukakPathProgramMoveSCU.Append(nukakPath);
		nukakPathProgramMoveSCU.Append(wxFileName::GetPathSeparator());
		nukakPathProgramMoveSCU.Append(nameProgramMoveSCU);

		nkUtilities::copyFile(nukakPathProgramMoveSCU, outPathProgramMoveSCU);
		

		nukakPathQuertyTXT.Append(nkUtilities::getNukak3DDataDir());
		nukakPathQuertyTXT.Append(wxFileName::GetPathSeparator());
		nukakPathQuertyTXT.Append("query.txt");

		wxFile * myfile = new wxFile(nukakPathQuertyTXT, wxFile::write);
		myfile->Write(my_DicomText);
		myfile->Close();

		wxString str_dump;
		str_dump = wxString("dump2dcm -v ");
		str_dump.Append(nukakPathQuertyTXT);
		str_dump.Append(" ");
		str_dump.Append( outPathQuertyDCM);
		nkUtilities::setWorkingDirectoryNukak3D();
		wxProcess *process = wxProcess::Open(str_dump);
		wxString process_in = "-1";
		if(process){
			nkPipedProcess * miu = new nkPipedProcess(this, "Validating search...", process);
			miu->ShowModal();
			process_in = miu->m_textIn;
		}
		bool dumpfile = false;
		if(process_in.Find("dump successfully converted.") != wxNOT_FOUND) dumpfile = true;
		if(dumpfile){
			if(myindexserver != -1){
				nkDICOMServer *  myDicomS = prv_listServers.Item(myindexserver)->GetData();

				wxString etiquetas[100];
				const int num_datos=2;

				etiquetas[0] = _("Move Destination AE Title"); 
				etiquetas[1] = _("Port");
				
				nkIODialog * miDlg = new nkIODialog(	this, 
															etiquetas,
															num_datos,
															-1,
															_("Nukak3D: Dicom Retrieve"),
															wxDefaultPosition,
															wxSize(330,(num_datos+4)*20+40));
				
				wxString my_strMoveSCU = "";
				nkUtilities::readConfigBase("DICOM/movescu/aeTitle",&my_strMoveSCU,"LOCALSCP");

				long my_lnMovePort = 115;
				nkUtilities::readConfigBase("DICOM/movescu/port",&my_lnMovePort, 115);

				miDlg->cambiarValor(my_strMoveSCU,0);
				miDlg->cambiarValor(wxString::Format("%d",my_lnMovePort),1);
								
				miDlg->ShowModal(); 
				if(miDlg->GetReturnCode() == wxID_OK){
					nkUtilities::writeConfigBase("DICOM/movescu/aeTitle",my_strMoveSCU);
					nkUtilities::writeConfigBase("DICOM/movescu/port",my_lnMovePort);
				}
				miDlg->Destroy();

				wxString str_find = "";
				str_find.Append(outPathProgramMoveSCU);
				str_find.Append(" -v -S +P ");
				str_find.Append(wxString::Format("%d", my_lnMovePort));
				str_find.Append(" -aem ");
				str_find.Append(my_strMoveSCU);
				str_find.Append(" -aet ");	str_find.Append(myDicomS->AETitle);
				str_find.Append(" -aec ");	str_find.Append(myDicomS->AECalled);
				str_find.Append(" ");	str_find.Append(myDicomS->host);
				str_find.Append(" ");	str_find.Append(wxString::Format("%d", myDicomS->port));
				str_find.Append(" ");  str_find.Append("query.dcm");

				wxSetWorkingDirectory(outPathPatient);
				wxProcess *processFind = wxProcess::Open(str_find);
				wxString process_infind = "-1";
				if(processFind){
					nkPipedProcessThread * myfind = new nkPipedProcessThread(this, "Get patient, please wait...", processFind);
					myfind->ShowModal();
					process_infind = myfind->m_textIn;
					//analyzeOutput(process_infind);
				}
				parent->prOpenVolumenDicom(outPathPatient);

			}else{
				wxMessageBox(_("Select a DICOM server"),_("Nukak3D: Alert"));
			}
		}
		wxRemoveFile(outPathProgramMoveSCU);
		wxRemoveFile(outPathQuertyDCM);
		wxRemoveFile(nukakPathQuertyTXT);

	wxSetWorkingDirectory(nukakPath);
	}else{
		wxMessageBox(_("Select a patient"),_("Nukak3D: Alert"));
	}
}