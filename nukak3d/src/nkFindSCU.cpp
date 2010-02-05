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
	prv_auiManager.SetManagedWindow(this);
	prv_auiManager.SetFlags(prv_auiManager.GetFlags()|
		wxAUI_MGR_ALLOW_ACTIVE_PANE|
		wxAUI_MGR_HINT_FADE|
		wxAUI_MGR_TRANSPARENT_HINT);

	my_parent =  parent;
	this->my_searchPanel = new wxPanel(this, wxID_ANY);
	
	my_cmdServerLocaltion = new wxButton(my_searchPanel,nkFindSCU::ID_SERVER_LOCATION, _("Server Location") );

	my_listServerLocaltion = new wxListBox(my_searchPanel, wxID_ANY, wxDefaultPosition, wxSize(130,60));
	loadServers();

	my_textPatienID = new wxTextCtrl(my_searchPanel, wxID_ANY, "", wxDefaultPosition, wxSize(130, 20));
	
	my_textFirstName = new wxTextCtrl(my_searchPanel, wxID_ANY, "", wxDefaultPosition, wxSize(130, 20));
	my_textAccessionNumber= new wxTextCtrl(my_searchPanel, wxID_ANY, "", wxDefaultPosition, wxSize(130, 20));


	my_chkStudyDateFrom = new wxCheckBox(my_searchPanel, nkFindSCU::ID_CHECK_STUDY_DATE_FROM, _("Use Study Date(From)")) ;
	my_cmdStudyDateFrom = new wxButton(my_searchPanel, nkFindSCU::ID_CALENDAR_STUDY_DATE_FROM, _("Change")) ;
	my_cmdStudyDateFrom->Disable();
	my_textStudyDateFrom = new wxTextCtrl(my_searchPanel, wxID_ANY, "", wxDefaultPosition, wxSize(130, 20));
	my_textStudyDateFrom->SetEditable(false);
	my_textStudyDateFrom->Disable();

	my_chkStudyDateTo = new wxCheckBox(my_searchPanel, nkFindSCU::ID_CHECK_STUDY_DATE_TO, _("Use Study Date(To)")) ;
	my_cmdStudyDateTo = new wxButton(my_searchPanel, nkFindSCU::ID_CALENDAR_STUDY_DATE_TO, _("Change")) ;
	my_cmdStudyDateTo->Disable();
	my_textStudyDateTo = new wxTextCtrl(my_searchPanel, wxID_ANY, "", wxDefaultPosition, wxSize(130, 20));
	my_textStudyDateTo->SetEditable(false);
	my_textStudyDateTo->Disable();

	my_textstudyDescription = new wxTextCtrl(my_searchPanel, wxID_ANY, "", wxDefaultPosition, wxSize(130, 20));
	my_textInstitutionName = new wxTextCtrl(my_searchPanel, wxID_ANY, "", wxDefaultPosition, wxSize(130, 20));

	wxSizer * sizerServers = new wxGridSizer(1, 2, 1, 1);
	sizerServers->Add(my_cmdServerLocaltion, 0,  wxALL, 2);
	sizerServers->Add(my_listServerLocaltion, 1,  wxGROW | wxALL, 2);

	wxSizer *sizerServer = new wxGridSizer(9, 2, 1, 1);

	sizerServer->Add(new wxStaticText(my_searchPanel, wxID_ANY, _("Patien ID:")), 0,  wxALL, 2);
	sizerServer->Add(my_textPatienID, 1,  wxGROW | wxALL, 2);

	sizerServer->Add(new wxStaticText(my_searchPanel, wxID_ANY, _("Name:")), 0,  wxALL, 2);
	sizerServer->Add(my_textFirstName, 1,  wxGROW | wxALL, 2);


	sizerServer->Add(new wxStaticText(my_searchPanel, wxID_ANY, _("Accession Number:")), 0,  wxALL, 2);
	sizerServer->Add(my_textAccessionNumber, 1,  wxGROW | wxALL, 2);

	sizerServer->Add(my_chkStudyDateFrom, 0,  wxALL, 2);
	sizerServer->Add(my_cmdStudyDateFrom, 0,  wxALL, 2);
	sizerServer->Add(new wxStaticText(my_searchPanel, wxID_ANY, _("Study Date(From)")), 0,  wxALL, 2);
	sizerServer->Add(my_textStudyDateFrom, 1,  wxGROW | wxALL, 2);

	sizerServer->Add(my_chkStudyDateTo, 0,  wxALL, 2);
	sizerServer->Add(my_cmdStudyDateTo, 0,  wxALL, 2);
	sizerServer->Add(new wxStaticText(my_searchPanel, wxID_ANY, _("Study Date(To)")), 0,  wxALL, 2);
	sizerServer->Add(my_textStudyDateTo, 1,  wxGROW | wxALL, 2);

	sizerServer->Add(new wxStaticText(my_searchPanel, wxID_ANY, _("Study Description:")), 0,  wxALL, 2);
	sizerServer->Add(my_textstudyDescription, 1,  wxGROW | wxALL, 2);
	sizerServer->Add(new wxStaticText(my_searchPanel, wxID_ANY, _("Institution Name:")), 0,  wxALL, 2);
	sizerServer->Add(my_textInstitutionName, 1,  wxGROW | wxALL, 2);

	my_cmdSearch = new wxButton(my_searchPanel,nkFindSCU::ID_SEARCH, _("Search") );
	my_cmdClear = new wxButton(my_searchPanel,nkFindSCU::ID_CLEAR, _("Clear") );
	my_getStudy = new wxButton(my_searchPanel,nkFindSCU::ID_GET_STUDY, _("Get selected patient") );
	
	wxSizer * sizerSearch = new wxBoxSizer(wxVERTICAL);
	sizerSearch->Add(sizerServers, 0,  wxALL, 1);
	sizerSearch->Add(sizerServer, 0,  wxALL, 1);
	sizerSearch->Add(my_cmdSearch, 0,  wxALL, 1);
	sizerSearch->Add(my_cmdClear, 0,  wxALL, 1);
	sizerSearch->Add(my_getStudy, 0,  wxALL, 1);

	my_searchPanel->SetSizer(sizerSearch);
	sizerSearch->Fit(my_searchPanel);


	prv_auiManager.AddPane(my_searchPanel, wxAuiPaneInfo().
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

	prv_wxGridPatients = new wxGrid(this,wxID_ANY, wxPoint(0,0), wxSize(400,220));
	prv_wxGridPatients->SetColMinimalAcceptableWidth(0);
	prv_wxGridPatients->CreateGrid(0 , 12 );
	prv_wxGridPatients->SetColSize( 0, 120 );
	prv_wxGridPatients->SetColSize( 1, 160 );
	prv_wxGridPatients->SetColSize( 2, 30 );
	prv_wxGridPatients->SetColSize( 3, 30 );
	prv_wxGridPatients->SetColSize( 4, 120 );
	prv_wxGridPatients->SetColSize( 5, 120 );
	prv_wxGridPatients->SetColSize( 6, 70 );
	prv_wxGridPatients->SetColSize( 7, 120 );
	prv_wxGridPatients->SetColSize( 8, 120 );
	//0
	prv_wxGridPatients->SetColSize( 9, 40 );
	//0
	prv_wxGridPatients->SetColSize( 10, 40 );
	prv_wxGridPatients->SetColSize( 11, 120 );
	prv_wxGridPatients->SetColLabelValue(0, _("Patien ID"));
	prv_wxGridPatients->SetColLabelValue(1, _("Name"));
	prv_wxGridPatients->SetColLabelValue(2, _("Sex"));
	prv_wxGridPatients->SetColLabelValue(3, _("Age"));
	prv_wxGridPatients->SetColLabelValue(4, _("Study Date"));
	prv_wxGridPatients->SetColLabelValue(5, _("Description"));
	prv_wxGridPatients->SetColLabelValue(6, _("Acceccion #"));
	prv_wxGridPatients->SetColLabelValue(7, _("Modality"));
	prv_wxGridPatients->SetColLabelValue(8, _("Institution Name"));
	prv_wxGridPatients->SetColLabelValue(9, _("Study Instance UID"));
	prv_wxGridPatients->SetColLabelValue(10, _("Series Instance UID"));
	prv_wxGridPatients->SetColLabelValue(11, _("Study ID"));
	prv_wxGridPatients->SetEditable(false);
	prv_wxGridPatients->ForceRefresh();

	prv_auiManager.AddPane(prv_wxGridPatients, wxAuiPaneInfo().
              Name("SearchGrid").
			  Center().
			  CloseButton(false).
			  PinButton(false).
			  Floatable(false).
			  Caption(_("Search results: Study's List")));

	prv_auiManager.Update();

	my_selectedCell = -1;

}

BEGIN_EVENT_TABLE(nkFindSCU, wxPanel)
	EVT_CHECKBOX(nkFindSCU::ID_CHECK_STUDY_DATE_FROM, nkFindSCU::prEventUseDateFrom)
	EVT_CHECKBOX(nkFindSCU::ID_CHECK_STUDY_DATE_TO, nkFindSCU::prEventUseDateTo)
	EVT_BUTTON(nkFindSCU::ID_CALENDAR_STUDY_DATE_FROM, nkFindSCU::prEventChangeDateFrom)
	EVT_BUTTON(nkFindSCU::ID_CALENDAR_STUDY_DATE_TO, nkFindSCU::prEventChangeDateTo)
	EVT_BUTTON(nkFindSCU::ID_SERVER_LOCATION, nkFindSCU::prEventServerLocation)
	EVT_BUTTON(nkFindSCU::ID_CLEAR, nkFindSCU::prEventClear)
	EVT_BUTTON(nkFindSCU::ID_SEARCH, nkFindSCU::prEventSearch)
	EVT_BUTTON(nkFindSCU::ID_GET_STUDY, nkFindSCU::prEventGetStudy)
	EVT_GRID_SELECT_CELL(nkFindSCU::prEventSelectCell)
END_EVENT_TABLE()

nkFindSCU::~nkFindSCU(){
	if(!prv_listServers.IsEmpty())	prv_listServers.DeleteContents(true);
	//this->DestroyChildren();
	prv_auiManager.UnInit();
}

void nkFindSCU::prEventUseDateFrom(wxCommandEvent& WXUNUSED(event)){
	if(my_chkStudyDateFrom->GetValue()){
		my_cmdStudyDateFrom->Enable();
		my_textStudyDateFrom->Enable();
	}else{
		my_cmdStudyDateFrom->Disable();
		my_textStudyDateFrom->Disable();
	}
}

void nkFindSCU::prEventUseDateTo(wxCommandEvent& WXUNUSED(event)){
	if(my_chkStudyDateTo->GetValue()){
		my_cmdStudyDateTo->Enable();
		my_textStudyDateTo->Enable();
	}else{
		my_cmdStudyDateTo->Disable();
		my_textStudyDateTo->Disable();
	}
}

void nkFindSCU::prEventChangeDateFrom(wxCommandEvent& WXUNUSED(event)){
	nkCalendarDialog * my_calDialog = new nkCalendarDialog(this);
	my_calDialog->ShowModal();
	my_textStudyDateFrom->SetLabel(my_calDialog->getDate().Format("%Y%m%d"));
	my_calDialog->Destroy();
}

void nkFindSCU::prEventChangeDateTo(wxCommandEvent& WXUNUSED(event)){
	nkCalendarDialog * my_calDialog = new nkCalendarDialog(this);
	my_calDialog->ShowModal();
	my_textStudyDateTo->SetLabel(my_calDialog->getDate().Format("%Y%m%d"));
	my_calDialog->Destroy();
}


void nkFindSCU::prEventServerLocation(wxCommandEvent& WXUNUSED(event)){
	nkServersDialog * my_servers = new nkServersDialog(this);
	my_servers->ShowModal();
	my_servers->Destroy();
	loadServers();
}

void nkFindSCU::loadServers(){
	wxString my_servers = nkDICOMServer::ReadConfigSystem();
	int cant = nkDICOMServer::countServers(my_servers);
	int i;
	my_listServerLocaltion->Clear();
	for (i=0; i<cant;i++){
		nkDICOMServer * myDS = nkDICOMServer::getDicomServer(my_servers,i);
		prv_listServers.Append(myDS);
		my_listServerLocaltion->Append(myDS->name);
	}
}

void nkFindSCU::prEventClear(wxCommandEvent& WXUNUSED(event)){
	my_textPatienID->SetValue(wxT(""));
	my_textFirstName->SetValue(wxT(""));
	my_textAccessionNumber->SetValue(wxT(""));

	my_chkStudyDateFrom->SetValue(false);
	my_cmdStudyDateFrom->Disable();
	my_textStudyDateFrom->SetValue("");

	my_chkStudyDateTo->SetValue(false);
	my_cmdStudyDateTo->Disable();
	my_textStudyDateTo->SetValue("");

	my_textstudyDescription->SetValue("");
	my_textInstitutionName->SetValue("");
	
	prv_wxGridPatients->ClearGrid();
	prv_wxGridPatients->DeleteRows(0, prv_wxGridPatients->GetNumberRows());
	my_selectedCell = -1;

}

void nkFindSCU::prEventSearch(wxCommandEvent& WXUNUSED(event)){
	int myindexserver = getServer();
	if(myindexserver ==-1){
		wxMessageBox(_("Select a DICOM server"),_("Nukak3D: Alert"));
		return;
	}
	wxString my_DicomText = "";
	wxString a_date = "";
	if(my_textStudyDateFrom->IsEnabled() || my_textStudyDateTo->IsEnabled()){
		if((my_textStudyDateFrom->GetValue().Length()>0) || (my_textStudyDateTo->GetValue().Length()>0)){	
			a_date.Append( my_textStudyDateFrom->GetValue());
			a_date.Append( "-");
			a_date.Append( my_textStudyDateTo->GetValue());
		}
	}

	wxString a_patientName = "";
	a_patientName.Append(my_textFirstName->GetValue());
	a_patientName.Replace(" ", "*");
	a_patientName.Append("*");
	

	my_DicomText.Append("(0008,0005) CS [] # SpecificCharacterSet\n");
	my_DicomText.Append("(0008,0020) DA ["); 
		my_DicomText.Append(a_date); 
		my_DicomText.Append("] # StudyDate\n");
	my_DicomText.Append("(0008,0021) DA [] # SeriesDate\n");
	my_DicomText.Append("(0008,0050) SH [");
		my_DicomText.Append(my_textAccessionNumber->GetValue());
		my_DicomText.Append("] # AccessionNumber\n");
	//my_DicomText.Append("(0008,0052) CS [STUDY] # QueryRetrieveLevel\n");
		my_DicomText.Append("(0008,0052) CS [SERIES] # QueryRetrieveLevel\n");
	my_DicomText.Append("(0008,0061) CS [] # ModalitiesInStudy\n");
	my_DicomText.Append("(0008,0080) LO [");
		my_DicomText.Append(my_textInstitutionName->GetValue());
		my_DicomText.Append("] # InstitutionName\n");
	my_DicomText.Append("(0008,0090) PN [] # ReferringPhysiciansName\n");
	my_DicomText.Append("(0008,1030) LO [");
		my_DicomText.Append(my_textstudyDescription->GetValue());
		my_DicomText.Append("] # StudyDescription\n");
	my_DicomText.Append("(0010,0010) PN [");
		my_DicomText.Append(a_patientName);
		my_DicomText.Append("] # PatientsName\n");
	my_DicomText.Append("(0010,0020) LO [");
	my_DicomText.Append(my_textPatienID->GetValue());
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
	wxString name_server = my_listServerLocaltion->GetStringSelection();
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
	prv_wxGridPatients->ClearGrid();
	prv_wxGridPatients->DeleteRows(0, prv_wxGridPatients->GetNumberRows());
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
					if(key.Cmp("0010,0020")==0) prv_wxGridPatients->SetCellValue(mi_filagrilla, 0,value);
					if(key.Cmp("0010,0010")==0) prv_wxGridPatients->SetCellValue(mi_filagrilla, 1,value);
					if(key.Cmp("0010,0040")==0) prv_wxGridPatients->SetCellValue(mi_filagrilla, 2,value);
					if(key.Cmp("0010,1010")==0) prv_wxGridPatients->SetCellValue(mi_filagrilla, 3,value);
					
					if(key.Cmp("0008,0020")==0){
						value = value.SubString(0,3) + "/" + value.SubString(4,5) + "/" + value.SubString(6,7);
						prv_wxGridPatients->SetCellValue(mi_filagrilla, 4,value);
					}

					if(key.Cmp("0008,1030")==0) prv_wxGridPatients->SetCellValue(mi_filagrilla, 5,value);
					if(key.Cmp("0008,0050")==0) prv_wxGridPatients->SetCellValue(mi_filagrilla, 6,value);
					if(key.Cmp("0008,0061")==0) prv_wxGridPatients->SetCellValue(mi_filagrilla, 7,value);
					if(key.Cmp("0008,0080")==0) prv_wxGridPatients->SetCellValue(mi_filagrilla, 8,value);
					if(key.Cmp("0020,000D")==0) prv_wxGridPatients->SetCellValue(mi_filagrilla, 9,value);
					if(key.Cmp("0020,000d")==0) prv_wxGridPatients->SetCellValue(mi_filagrilla, 9,value);
					if(key.Cmp("0020,000E")==0) prv_wxGridPatients->SetCellValue(mi_filagrilla, 10,value);
					if(key.Cmp("0020,000e")==0) prv_wxGridPatients->SetCellValue(mi_filagrilla, 10,value);
					if(key.Cmp("0020,0010")==0) prv_wxGridPatients->SetCellValue(mi_filagrilla, 11,value);
					
				}
			break;
			case 'R':
				mi_filagrilla = mi_filagrilla + 1;
				prv_wxGridPatients->InsertRows(mi_filagrilla, 1);
			break;
			case '-':
			break;
			}
		}
	}
	my_selectedCell = -1;
}

void nkFindSCU::prEventSelectCell(wxGridEvent &event){
	my_selectedCell = event.GetRow();
	event.Skip();
}

void nkFindSCU::prEventGetStudy(wxCommandEvent& WXUNUSED(event)){
	if(my_selectedCell != -1){
		int myindexserver = getServer();
		if(myindexserver ==-1){
			wxMessageBox(_("Select a DICOM server"),_("Nukak3D: Alert"));
			return;
		}
		wxString my_DicomText = "";
		wxString a_patientName = prv_wxGridPatients->GetCellValue(my_selectedCell, 1);
		a_patientName.Replace(" ", "_");
		

		my_DicomText.Append("(0008,0005) CS [] # SpecificCharacterSet\n");
		//my_DicomText.Append("(0008,0052) CS [STUDY] # QueryRetrieveLevel\n");
		my_DicomText.Append("(0008,0052) CS [SERIES] # QueryRetrieveLevel\n");
		my_DicomText.Append("(0010,0010) PN [");
			my_DicomText.Append( prv_wxGridPatients->GetCellValue(my_selectedCell, 1) );
			my_DicomText.Append("] # PatientsName\n");
		my_DicomText.Append("(0010,0020) LO [");
			my_DicomText.Append( prv_wxGridPatients->GetCellValue(my_selectedCell, 0) );
			my_DicomText.Append("] # PatientID\n");

		my_DicomText.Append("(0020,000D) UI [");
		wxLogError("] # StudyInstanceUID\n" + prv_wxGridPatients->GetCellValue(my_selectedCell, 9));
			my_DicomText.Append( prv_wxGridPatients->GetCellValue(my_selectedCell, 9) );
			my_DicomText.Append("] # StudyInstanceUID\n");
		my_DicomText.Append("(0020,000E) UI [");
			wxLogError("] # SeriesInstanceUID\n" + prv_wxGridPatients->GetCellValue(my_selectedCell, 10));
			my_DicomText.Append( prv_wxGridPatients->GetCellValue(my_selectedCell, 10) );
			my_DicomText.Append("] # SeriesInstanceUID\n");
		my_DicomText.Append("(0020,0010) SH [");
			my_DicomText.Append( prv_wxGridPatients->GetCellValue(my_selectedCell, 11) );
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
				my_parent->prOpenVolumenDicom(outPathPatient);

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