/** 
 * @file nkStoreSCP.cpp
 * @brief Server storeSCP .
 * @details Clases for config and execute server storeSCP.
 * @author Alexander Pinz�n Fernandez
 * @version 0.1
 * @date 16/01/2009 04:07 p.m.
*/

#include "nkStoreSCP.h"

nkPipedProcess::nkPipedProcess(wxFrame *parent,
                         const wxString& cmd,
                         wxProcess *process)
						 : wxDialog( parent, wxID_ANY, "nkDicom: " + cmd,wxDefaultPosition, wxSize(400,150)),
						m_process(process),
						m_in(*process->GetInputStream()),
						m_err(*process->GetErrorStream()){
	m_process->SetNextHandler(this);
	new wxStaticText(this, wxID_ANY,
		wxString(_("\nNukak3D: "))<<cmd<<
		wxString(_("\nProcess Your Request...")),wxPoint(20,10) );
	m_ok = new wxButton(this, wxID_OK, _("Wait a moment..."),wxPoint(60,70));	
	new wxButton(this, wxID_CANCEL, _("Cancel"),wxPoint(180,70));	
	m_ok->Disable();
	m_textIn = "";
	m_textErr = "";
}

BEGIN_EVENT_TABLE(nkPipedProcess, wxDialog)
    EVT_CLOSE(nkPipedProcess::OnClose)
    EVT_END_PROCESS(wxID_ANY, nkPipedProcess::OnProcessTerm)
END_EVENT_TABLE()

void nkPipedProcess::DoGet(){
	if(m_process){
		DoGetFromStream(m_textIn, m_in);
		DoGetFromStream(m_textErr, m_err);
	}
}

void nkPipedProcess::DoGetFromStream(wxString & text, wxInputStream& in){
    while ( in.CanRead() ){
        wxChar buffer[4096];
        buffer[in.Read(buffer, WXSIZEOF(buffer) - 1).LastRead()] = _T('\0');
        text.Append(buffer);
    }
}

void nkPipedProcess::DoClose(){
    m_process->CloseOutput();
}

void nkPipedProcess::OnClose(wxCloseEvent& event){
    if ( m_process ){
        wxProcess *process = m_process;
        m_process = NULL;
        process->SetNextHandler(NULL);
        process->CloseOutput();
    }
    event.Skip();
}

void nkPipedProcess::OnProcessTerm(wxProcessEvent& WXUNUSED(event)){
    DoGet();
	m_ok->SetLabel(_("Ok"));
	m_ok->Enable(true);
    delete m_process;
    m_process = NULL;
}

nkThreadCaptureOutput::nkThreadCaptureOutput(nkStoreSCP * a_mySCP)
        : wxThread(){
    mySCP = a_mySCP;
}

void nkThreadCaptureOutput::OnExit(){
	mySCP = NULL;
}

void *nkThreadCaptureOutput::Entry()
{
	while(true){
		if ( TestDestroy() )
            break;
		if(mySCP){
			//mySCP->DoGet();
			wxCommandEvent event( wxEVT_COMMAND_BUTTON_CLICKED, nkStoreSCP::ID_DO_GET);
			event.SetInt( nkStoreSCP::ID_DO_GET);

			// send in a thread-safe way
			wxPostEvent(this->mySCP, event );
		}
		wxThread::Sleep(200);
	}
	mySCP = NULL;
    return NULL;
}

nkStoreSCP * nkStoreSCP::_instance = 0;

nkStoreSCP::nkStoreSCP(wxFrame *a_parent,int a_port,wxString a_aet, wxString a_od, 	wxString a_options)
	:wxDialog( a_parent, wxID_ANY, "Nukak3D: Dicom Listener" ,wxDefaultPosition, wxSize(400,400)){
	my_StringErr = "";
	my_StringIn = "";

	if(!(this->ReadConfigSystem())){
		port = a_port;
		od = this->getOutputDirectory();
		aet = a_aet;
		options = a_options;
	}
	my_parent = a_parent;

	wxPanel *panel = new wxPanel(this, wxID_ANY);

	wxPanel *panelConfiguration = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_STATIC  );

	my_textLog = new wxTextCtrl(panel, wxID_ANY, 
		wxString(_("Nukak3D: Dicom Listener"))<<
		wxString(_("\nView processing details here.\n")),
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH);
	my_textLog->SetEditable(false);

	my_textAETitle = new wxTextCtrl(panelConfiguration, wxID_ANY, aet);
	my_textPort = new wxTextCtrl(panelConfiguration, wxID_ANY, wxString::Format("%d", port));


	my_textOutDir = new wxTextCtrl(panelConfiguration, wxID_ANY, od ,wxDefaultPosition, wxSize(300,20));
	my_textOutDir->SetEditable(false);
	my_cmdOutDir = new wxButton(panelConfiguration, nkStoreSCP::ID_CHANGE_OUTPUT_DIRECTORY,_("Output Directory: "));
	my_cmdStart = new wxButton(panelConfiguration, nkStoreSCP::ID_START_SERVER,_("Start Server"));
	my_cmdStop = new wxButton(panelConfiguration, nkStoreSCP::ID_STOP_SERVER,_("Stop Server"));

	wxSizer *sizerAE = new wxFlexGridSizer(4, 2, 5, 5);
	sizerAE->Add(new wxStaticText(panelConfiguration, wxID_ANY, _("Set AE title of this computer:")), 0,  wxGROW | wxALL, 5);
	sizerAE->Add(my_textAETitle, 1,  wxALL, 5);
	sizerAE->Add(new wxStaticText(panelConfiguration, wxID_ANY, _("Tcp/Ip port number to listen on:")), 0,  wxGROW | wxALL, 5);
	sizerAE->Add(my_textPort, 1,  wxALL, 5);
	sizerAE->Add(my_cmdOutDir, 0,  wxGROW | wxALL, 5);
	sizerAE->Add(my_textOutDir, 1,  wxALL, 5);
	sizerAE->Add(my_cmdStart, 0,  wxALL, 5);
	sizerAE->Add(my_cmdStop, 0,  wxALL, 5);
	//sizerAE->SetMinSize(380,50);

	wxSizer *sizerConfiguration = new wxBoxSizer(wxVERTICAL);
	sizerConfiguration->Add(new wxStaticText(panelConfiguration, wxID_ANY, _("\nDicom Listener Configuration:")),0, wxCENTRE | wxALL );
	sizerConfiguration->Add(sizerAE, 0, wxALL, 10);
	panelConfiguration->SetSizer(sizerConfiguration);
	sizerConfiguration->Fit(panel);
	
	my_cmdOk = new wxButton(panel, wxID_OK, _("Ok"));
	
	my_cmdCancel = new wxButton(panel, wxID_CANCEL, _("Cancel"));	

	wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
	sizerTop->Add(panelConfiguration, 0, wxALL, 10);
	sizerTop->Add(my_textLog, 1, wxGROW | wxALL, 5);
	sizerTop->Add(my_cmdOk, 0,  wxALL, 5);
	sizerTop->Add(my_cmdCancel,0, wxALL, 5);

	sizerTop->SetMinSize(540,400);

	panel->SetSizer(sizerTop);
    sizerTop->Fit(this);

	my_process = NULL;
	nkThreadCaptureOutput * mithread = new nkThreadCaptureOutput (this);

    if ( mithread->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread!"));
    }
	mithread->Run();

    
}

BEGIN_EVENT_TABLE(nkStoreSCP, wxDialog)
    EVT_CLOSE(nkStoreSCP::OnClose)
    EVT_END_PROCESS(wxID_ANY, nkStoreSCP::OnProcessTerm)
	EVT_BUTTON(nkStoreSCP::ID_CHANGE_OUTPUT_DIRECTORY, nkStoreSCP::OnChangeOutputDirectory)
	EVT_BUTTON(nkStoreSCP::ID_START_SERVER, nkStoreSCP::OnStartServer)
	EVT_BUTTON(nkStoreSCP::ID_STOP_SERVER, nkStoreSCP::OnStopServer)
	EVT_BUTTON(nkStoreSCP::ID_DO_GET, nkStoreSCP::DoGet)
END_EVENT_TABLE()

wxProcess * nkStoreSCP::start(){
	if(!my_process){
		wxString str_echo;
		str_echo = wxString("echoscu localhost ")<<wxString::Format(wxT("%d"),port);
		wxProcess *process = wxProcess::Open(str_echo);
		wxString process_in = "-1";
		if(process){
			nkPipedProcess * miu = new nkPipedProcess(my_parent, "Checking the availability of the port...", process);
			miu->ShowModal();
			process_in = miu->m_textIn;
			delete miu;
		}
		int res = process_in.Find("Echo [1], Complete [Status: Success]");
		if( res != wxNOT_FOUND){
			this->my_textLog->AppendText(wxString::Format(_T("\nError: port in use, PORT: %d"), port));
			return my_process;
		}
		res = process_in.Find("Association Rejected");
		if( res != wxNOT_FOUND){
			this->my_textLog->AppendText(wxString::Format(_T("\nError: port in use, PORT: %d"), port));
			this->my_textLog->AppendText(wxString::Format(_T("\n%S"), process_in));
			return my_process;
		}

		wxString str_storescp;
		str_storescp = wxString("storescp -aet ")<<aet<<
			wxString(" -od ")<<od<<
			wxString(" -v ")<<options<<
			wxString::Format(wxT(" %d"),port);
		my_process = wxProcess::Open(str_storescp, wxEXEC_ASYNC);
		if (my_process){
			my_process->SetNextHandler(this);
			my_in = my_process->GetInputStream();
			my_err = my_process->GetErrorStream();
			if(this->isOk()) this->my_textLog->AppendText(_("\nStart server Ok."));
		}else{
			this->my_textLog->AppendText(_T("\nFailed to initialize server"));
			return my_process;
		}
		this->WriteConfigSystem();
	}else{
		this->my_textLog->AppendText(_("\nServer's exist."));
	}
	return my_process;
}

bool nkStoreSCP::stop(){
	bool res = true;
	if(my_process){
		wxKillError mi_error = this->shutdownSCP();
		switch(mi_error){
		case wxKILL_BAD_SIGNAL:
			this->my_textLog->AppendText(_T("\nFailed to stop server: BAD SIGNAL"));
			res = false;
		break;
		case wxKILL_ACCESS_DENIED:
			this->my_textLog->AppendText(_T("\nFailed to stop server: ACCESS DENIED"));
			res = false;
		break;
		case wxKILL_NO_PROCESS:
			this->my_textLog->AppendText(_T("\nFailed to stop server: NO EXIST PROCESS"));
			res = false;
		break;
		case wxKILL_ERROR :
			this->my_textLog->AppendText(_T("\nFailed to stop server: ERROR"));
			res = false;
		break;
		}
		this->my_textLog->AppendText(_T("\nStop server Ok"));
	}
	this->my_cmdOk->SetLabel( _("Ok"));
	this->my_cmdOk->Enable();
	return res;
}

wxString nkStoreSCP::getInputStream(){
	wxString my_result = "";
	if(my_process){
		if(my_process->IsInputAvailable()){
			while ( my_in->CanRead() ){
				wxChar buffer[4096];
				buffer[my_in->Read(buffer, WXSIZEOF(buffer) - 1).LastRead()] = _T('\0');
				my_result.Append(buffer);
			}
		}
	}
	return my_result;
}

wxString nkStoreSCP::getErrorStream(){
	wxString my_result = "";
	if(my_process){
		if(my_process->IsErrorAvailable() ){
			while ( my_err->CanRead() ){
				wxChar buffer[4096];
				buffer[my_err->Read(buffer, WXSIZEOF(buffer) - 1).LastRead()] = _T('\0');
				my_result.Append(buffer);
			}
		}
	}
	return my_result;
}

bool nkStoreSCP::isOk(){
	bool serverok = false;
	if(this->my_process){
		wxString str_echo;
		str_echo = wxString("echoscu -v localhost ")<<wxString::Format(wxT("%d"),port);
		wxProcess *process = wxProcess::Open(str_echo);
		wxString process_in = "-1";
		if(process){
			nkPipedProcess * miu = new nkPipedProcess(my_parent, "Checking Server...", process);
			miu->ShowModal();
			process_in = miu->m_textIn;
		}
		if(process_in.Find("Echo [1], Complete [Status: Success]") != wxNOT_FOUND) serverok = true;
	}
	return serverok;
}

wxKillError nkStoreSCP::shutdownSCP(){
	int pid = -10000;
	wxKillError my_error = wxKILL_OK;
	if (my_process){
		pid = my_process->GetPid();
	}

	if(wxProcess::Exists(pid)){
		my_process->CloseOutput();
		my_error = wxProcess::Kill(pid,wxSIGKILL);
	}
	return my_error;
}

void nkStoreSCP::OnClose(wxCloseEvent& event){
	this->Hide();
    event.Skip();
}

void nkStoreSCP::OnProcessTerm(wxProcessEvent& event){
	this->my_StringIn = this->getInputStream();
	this->my_StringErr = this->getErrorStream();
	delete my_process;
    my_process = NULL;
}

void nkStoreSCP::DoClose(){
	if(my_process)	my_process->CloseOutput();
}

nkStoreSCP::~nkStoreSCP(){
	my_process = NULL;
	_instance = NULL;
}

int nkStoreSCP::ShowModalStop(){
	this->my_cmdCancel->Disable();
	this->my_cmdCancel->Hide();
	this->my_cmdOk->SetLabel( _("Wait a moment..."));
	this->my_cmdOk->Disable();
	this->my_textLog->AppendText(_("\nStoping server..."));
	this->my_textLog->AppendText(_("\nWait a moment..."));
	if(this->my_process){
		this->stop();
		return this->ShowModal();
	}
	return 0;
}

void nkStoreSCP::OnChangeOutputDirectory(wxCommandEvent & WXUNUSED(evt)){
	wxDirDialog * myDirDialog = new wxDirDialog(this,_("Open Dir..."),this->my_textOutDir->GetLabel());

	int OK = myDirDialog->ShowModal();
    
	if( OK==wxID_OK ){
		if (wxFileName::IsDirWritable(myDirDialog->GetPath()) ){
			this->my_textOutDir->SetLabel( myDirDialog->GetPath());
		}else{
			wxLogError(_("This dir is not writable"));
		}
	}
	myDirDialog->Destroy();
}

bool nkStoreSCP::ReadConfigSystem(){
	bool canread = false;
	wxConfigBase *mi_Config = wxConfigBase::Get();
	if ( mi_Config != NULL){
		port = (int)(mi_Config->Read(_("DICOM/storescp/port"), 115));
		od = this->getOutputDirectory();
		aet = mi_Config->Read(_("DICOM/storescp/aeTitle"), _("STORESCP"));
		options = mi_Config->Read(_("DICOM/storescp/options"), _(" -ss nk -v "));
		canread = true;
	}
	return canread;
}

bool nkStoreSCP::WriteConfigSystem(){
	bool canwrite = false;
	wxConfigBase *mi_Config = wxConfigBase::Get();
	bool DICOMport;			
	bool DICOMod;		
	bool DICOMaet;		
	bool DICOMoptions;	
	if ( mi_Config != NULL){
		DICOMport = mi_Config->Write(_("DICOM/storescp/port"), (long)(this->port));
		DICOMod = mi_Config->Write(_("DICOM/storescp/outputDir"), this->od);
		DICOMaet = mi_Config->Write(_("DICOM/storescp/aeTitle"), this->aet);
		DICOMoptions = mi_Config->Write(_("DICOM/storescp/options"), this->options);
		canwrite = DICOMport && DICOMod && DICOMaet && DICOMoptions;
	}
	return canwrite;
}

wxString nkStoreSCP::getOutputDirectory(){
	wxString DICOMod;
	wxString path = "./";
	bool buscar = false;
	wxConfigBase *mi_Config = wxConfigBase::Get();
	if ( mi_Config != NULL){
		DICOMod = mi_Config->Read(_("DICOM/storescp/outputDir"), _("-1"));
		if(DICOMod.Cmp("-1") == 0){
			buscar = true;
		}else{
			if(wxFileName::DirExists(DICOMod)){
				if(wxFileName::IsDirWritable(DICOMod)){
					path = DICOMod;
				}else{
					buscar = true;
				}
			}else{
				if(wxFileName::Mkdir(DICOMod)){
					if(!wxFileName::IsDirWritable(DICOMod))
						buscar = true;
				}else{
					buscar = true;
				}
			}
		}
	}else buscar = true;
	if(buscar){
		if(wxFileName::DirExists (wxStandardPaths::Get().GetUserLocalDataDir())){
			if(wxFileName::IsDirWritable (wxStandardPaths::Get().GetUserLocalDataDir())){
				path = wxStandardPaths::Get().GetUserLocalDataDir() + wxFileName::GetPathSeparator() + wxT("data");
				if(! (wxFileName::DirExists(path))) wxFileName::Mkdir(path);
			}else if( wxFileName::IsDirWritable(wxStandardPaths::Get().GetDocumentsDir()) ){
				path = wxStandardPaths::Get().GetDocumentsDir() + wxFileName::GetPathSeparator() + 
					wxT("Nukak3D") +  wxFileName::GetPathSeparator() + 
					wxT("data");
				wxFileName::Mkdir(path);
			}else if( wxFileName::IsDirWritable(wxStandardPaths::Get().GetTempDir()) ){
				path = wxStandardPaths::Get().GetTempDir() + wxFileName::GetPathSeparator() + 
					wxT("Nukak3D") +  wxFileName::GetPathSeparator() + 
					wxT("data");
				wxFileName::Mkdir(path);
			}
		}else{
			if(wxFileName::Mkdir(wxStandardPaths::Get().GetUserLocalDataDir())){
				path = wxStandardPaths::Get().GetUserLocalDataDir() + wxFileName::GetPathSeparator() + wxT("data");
				wxFileName::Mkdir(path);
			}else if( wxFileName::IsDirWritable(wxStandardPaths::Get().GetDocumentsDir()) ){
				path = wxStandardPaths::Get().GetDocumentsDir() + wxFileName::GetPathSeparator() + 
					wxT("Nukak3D") +  wxFileName::GetPathSeparator() + 
					wxT("data");
				wxFileName::Mkdir(path);
			}else if( wxFileName::IsDirWritable(wxStandardPaths::Get().GetTempDir()) ){
				path = wxStandardPaths::Get().GetTempDir() + wxFileName::GetPathSeparator() + 
					wxT("Nukak3D") +  wxFileName::GetPathSeparator() + 
					wxT("data");
				wxFileName::Mkdir(path);
			}
		}
	}
	return path;
}

void nkStoreSCP::OnStartServer(wxCommandEvent & WXUNUSED(evt)){
	this->start();
}

void nkStoreSCP::OnStopServer(wxCommandEvent & WXUNUSED(evt)){
	this->stop();
}

int nkStoreSCP::ShowModalAndLog(){
	this->my_textLog->AppendText("\n");
	this->my_textLog->AppendText(this->getInputStream());
	return this->ShowModal();
}

void nkStoreSCP::DoGet(wxCommandEvent & WXUNUSED(evt)){
	this->my_textLog->AppendText(this->getInputStream());
}