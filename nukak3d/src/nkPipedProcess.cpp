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
 * @file nkPipedProcess.cpp
 * @brief Piped Process.
 * @details Clases for manage extern process.
 * @author Alexander Pinzón Fernandez
 * @version 0.1
 * @date 26/01/2009 04:04 p.m.
*/

#include "nkPipedProcess.h"

nkPipedProcess::nkPipedProcess(wxWindow *parent,
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
	//EVT_BUTTON(nkPipedProcess::ID_DO_GET, nkPipedProcess::DoGetEvent)
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

nkPipedProcessThread::nkPipedProcessThread(wxWindow *parent, const wxString &cmd, wxProcess *process)
	:nkPipedProcess(parent, cmd, process){

	//nkThreadPipedProcess * mithread = new nkThreadPipedProcess(this);
	nkThreadPipedProcess * myThread = new nkThreadPipedProcess(this);

    //if ( mithread->Create() != wxTHREAD_NO_ERROR ){
	if ( myThread->Create() != wxTHREAD_NO_ERROR ){
        wxLogError(wxT("Can't create thread!"));
    }
	//mithread->Run();
	myThread->Run();
}

BEGIN_EVENT_TABLE(nkPipedProcessThread, nkPipedProcess)
	EVT_BUTTON(nkPipedProcessThread::ID_DO_GET, nkPipedProcessThread::DoGetEvent)
END_EVENT_TABLE()


void nkPipedProcessThread::DoGetEvent(wxCommandEvent & WXUNUSED(event)){
	DoGet();
}

nkThreadPipedProcess::nkThreadPipedProcess(nkPipedProcessThread * a_myPP)
        : wxThread(){
    myPP = a_myPP;
}

void nkThreadPipedProcess::OnExit(){
	myPP = NULL;
}

void *nkThreadPipedProcess::Entry()
{
	while(true){
		if ( TestDestroy() )
            break;
		if(myPP ){
			//mySCP->DoGet();
			wxCommandEvent event( wxEVT_COMMAND_BUTTON_CLICKED, nkPipedProcessThread::ID_DO_GET);
			event.SetInt( nkPipedProcessThread::ID_DO_GET);

			// send in a thread-safe way
			wxPostEvent(this->myPP, event );
		}
		wxThread::Sleep(200);
	}
	myPP = NULL;
    return NULL;
}