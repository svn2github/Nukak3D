/** 
 * @file nkStoreSCP.h
 * @brief Server storeSCP .
 * @details Clases for config and execute server storeSCP.
 * @author Alexander Pinzón Fernandez
 * @version 0.1
 * @date 16/01/2009 04:07 p.m.
*/

#ifndef _NKSTORESCP_H_
#define _NKSTORESCP_H_

#include "vtkINRIA3DConfigure.h"
#include  "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include  "wx/wx.h"
#endif

#include "wx/config.h"
#include "wx/confbase.h"

#include "wx/app.h"
#include "wx/dirdlg.h"
#include "wx/dynarray.h"
#include "wx/fileconf.h"
#include "wx/filename.h"
#include "wx/numdlg.h"
#include "wx/process.h"
#include "wx/stdpaths.h"
#include "wx/string.h"
#include "wx/txtstrm.h"
#include "wx/thread.h"

class nkStoreSCP;

/** 
 * @brief Class for execute extern DICOM program (echoscu, findscu, movescu).
 */
class nkPipedProcess : public wxDialog{
public:
	wxString m_textErr;	//! String for caputure error of executed command.
	wxString m_textIn;	//! String for caputure output of executed command.
	/**
	 * @brief Class Constructor.
	 * @param parent Window that call this Dialog.
	 * @param process wxProcess that manage the execution of the extern program.
	*/
	nkPipedProcess(wxFrame *parent,const wxString& cmd, wxProcess *process);
	/**
	 * @brief Capture output and error of executed command.
	*/
	void DoGet();
protected:
	/**
	 * @brief Manage event for close in secuer way.
	*/
	void OnClose(wxCloseEvent& event);
	/**
	 * @brief Manage event for exit of program executed.
	*/
	void OnProcessTerm(wxProcessEvent& event);
	/**
	 * @brief Close in secuer way.
	*/
    void DoClose();
private:
	/**
	 * @brief Capture Stream of executed command.
	 * @param text String for return capture stream.
	 * @param in Sream to capture.
	*/
	void DoGetFromStream(wxString &text, wxInputStream& in);
	wxProcess * m_process;	//! Pointer to process manager of execution program.
	wxInputStream & m_in;	//! Pointer to output stream of executed program.
	wxInputStream & m_err;	//! Pointer to error stream of executed program.
	wxButton * m_ok;		//! Button of this dialog.
	DECLARE_EVENT_TABLE()	//! Call macro for declaration of events.
};

/** 
 * @brief Thread for caputure stream of a server output.
 */
class nkThreadCaptureOutput : public wxThread{
public:
	/**
	 * @brief Class Constructor.
	 * @param a_mySCP Parent class.
	*/
	nkThreadCaptureOutput(nkStoreSCP * a_mySCP);
	/**
	 * @brief Function that capture the output of the execution of the extern program.
	*/
    virtual void *Entry();
	/**
	 * @brief On event delete of this thread.
	*/
    virtual void OnExit();
    nkStoreSCP * mySCP;		//! Parent class.
};


/**
 * @brief Class for DICOM Listener SCP.
 * @details This Class manage singleton pattern to restrict instantiation of a class to one object.
*/
class nkStoreSCP :  public wxDialog {
public:
	/** 
	 * Event identifier.
	*/
	enum{
		ID_CHANGE_OUTPUT_DIRECTORY = wxID_HIGHEST + 1700,	/**< Change Output Directory. */
		ID_DO_GET,			/**< Capture Output of server. */
		ID_START_SERVER,	/**< Start Dicom Listener SCP. */
		ID_STOP_SERVER		/**< Stop Dicom Listener SCP. */
	};
	/**
	 * @brief Get instance.
	 * @param a_parent Parent class, default: 104 .
	 * @param a_port TCP/IP port to listen, default: 104.
	 * @param a_aet AE Title, default: STORESCP.
	 * @param a_od Output Directory for studys receive, default: Local User Data.
	 * @param a_options Aditional options for storescp server, default: -ss nk -v .
	*/
	static nkStoreSCP * getInstance(wxFrame *a_parent, 
		int a_port = 104, 
		wxString a_aet = "STORESCP", 
		wxString a_od = "./", 
		wxString a_options = " -ss nk -v "){
		if( _instance == 0 ){
			_instance = new nkStoreSCP(a_parent, a_port, a_aet, a_od, a_options);
		}
		return _instance;
	}
	/**
	 * @brief Delete the instance.
	*/
	static void Destroy(){
		if( _instance != 0 ){
			delete( _instance );
			_instance = 0;
		}
	}
	/**
	 * @brief Class Constructor.
	 * @param a_parent Parent class, default: 104 .
	 * @param a_port TCP/IP port to listen, default: 104.
	 * @param a_aet AE Title, default: STORESCP.
	 * @param a_od Output Directory for studys receive, default: Local User Data.
	 * @param a_options Aditional options for storescp server, default: -ss nk -v .
	*/
	nkStoreSCP(wxFrame *a_parent, 
		int a_port = 104, 
		wxString a_aet = "STORESCP", 
		wxString a_od = "./", 
		wxString a_options = " -ss nk -v ");
	/**
	 * @brief Delete resources of this class.
	*/
	virtual ~nkStoreSCP();
	/**
	 * @brief Capture output stream.
	 * @return The Stream capture.
	*/
	wxString getInputStream();
	/**
	 * @brief Capture error stream.
	 * @return The Stream capture.
	*/
	wxString getErrorStream();
	/**
	 * @brief Function call of thread.
	*/
	void DoGet(wxCommandEvent & WXUNUSED(evt));
	/**
	 * @brief Kill that execution of the extern program.
	*/
	wxKillError shutdownSCP();
	/**
	 * @brief Start DICOM Listener SCP.
	*/
	wxProcess * start();
	/**
	 * @brief Stop DICOM Listener SCP.
	 * @return True if not error in stop process
	*/
	bool stop();
	/**
	 * @brief Send C-ECHO to this Dicom Listener.
	 * @return True if C-ECHO is succesfuly.
	*/
	bool isOk();
	wxString my_StringErr;	//! String for capture error stream of the Dicom Listener.
	wxString my_StringIn;	//! String for capture input stream of the Dicom Listener.
	/**
	 * @brief Dialog for wait while the stoping server.
	*/
	int ShowModalStop();
	/**
	 * @brief Get input stream in the log and show this dialog.
	*/
	int ShowModalAndLog();

protected:
	/**
	 * @brief Manage event for close.
	*/
	void OnClose(wxCloseEvent& event);
	/**
	 * @brief Manage event for program extern exit.
	*/
	void OnProcessTerm(wxProcessEvent& event);
	/**
	 * @brief Close in secure way.
	*/
    void DoClose();
	/**
	 * @brief Call Directory dialog for change output Directory.
	*/
	void OnChangeOutputDirectory(wxCommandEvent & WXUNUSED(evt));
	/**
	 * @brief Call start() function.
	*/
	void OnStartServer(wxCommandEvent & WXUNUSED(evt));
	/**
	 * @brief Call stop() function.
	*/
	void OnStopServer(wxCommandEvent & WXUNUSED(evt));
	/**
	 * @brief Search for a valid and writable output directory.
	*/
	wxString getOutputDirectory();
	/**
	 * @brief Read configure of this server of register of the OS.
	 * @return true if can read.
	*/
	bool ReadConfigSystem();
	/**
	 * @brief Write configure of this server of register of the OS.
	 * @return true if can write.
	*/
	bool WriteConfigSystem();
private:
	static nkStoreSCP * _instance;	//! Unique instance of this class
	wxProcess * my_process;			//! Pointer to procces manager
	wxInputStream * my_in;			//! Pointer to procces manager input stream
	wxInputStream * my_err;			//! Pointer to procces manager error stream
	wxString my_command;			

	int port;			//! TCP/IP port to listen
	wxString od;		//! Output Directory for studys receive
	wxString aet;		//! Set AE Title default STORESCP
	wxString options;	//! Aditional options for storescp server

	wxStaticText * my_dialogText;		//! Dialog text for information
	wxButton * my_cmdOk;				//! Button Ok
	wxButton * my_cmdCancel;			//! Button Cancel
	wxButton * my_cmdOutDir;			//! Button change output directory
	wxButton * my_cmdStart;				//! Button start DICOM Listener
	wxButton * my_cmdStop;				//! Button stop DICOM Listener
	wxTextCtrl *my_textLog;				//! Text Control for put input stream of DICOM Listener
	wxFrame *my_parent;					//! Parent Class

	wxTextCtrl *my_textAETitle;			//! Text Control for AE Title
	wxTextCtrl *my_textPort;			//! Text Control for Port
	wxTextCtrl *my_textOutDir;			//! Text Control for output directory
	
	DECLARE_EVENT_TABLE()				//! Call macro for declaration of events.
};

#endif //_NKSTORESCP_H_