/** 
 * @file nkPipedProcess.h
 * @brief Piped Process.
 * @details Clases for manage extern process.
 * @author Alexander Pinzón Fernandez
 * @version 0.1
 * @date 26/01/2009 04:04 p.m.
*/

#ifndef _NKPIPEDPROCESS_H_
#define _NKPIPEDPROCESS_H_

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
	nkPipedProcess(wxWindow *parent,const wxString& cmd, wxProcess *process);
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

	wxProcess * m_process;	//! Pointer to process manager of execution program.
private:
	/**
	 * @brief Capture Stream of executed command.
	 * @param text String for return capture stream.
	 * @param in Sream to capture.
	*/
	void DoGetFromStream(wxString &text, wxInputStream& in);
	wxInputStream & m_in;	//! Pointer to output stream of executed program.
	wxInputStream & m_err;	//! Pointer to error stream of executed program.
	wxButton * m_ok;		//! Button of this dialog.
	DECLARE_EVENT_TABLE()	//! Call macro for declaration of events.
};
/** 
 * @brief Class use nkPipedProcess with thread.
 */
class nkPipedProcessThread: public nkPipedProcess{
public:
	/** 
	 * Event identifier.
	*/
	enum{
		ID_DO_GET= wxID_HIGHEST + 1750			/**< Capture Output of server. */
	};
	/**
	 * @brief Class constructor.
	*/
	nkPipedProcessThread(wxWindow *parent,const wxString& cmd, wxProcess *process);
	/**
	 * @brief Capture output and error of executed command.
	 * @details Occurs when the thread makes an event queue.
	*/
	void DoGetEvent(wxCommandEvent & WXUNUSED(event));
private:
	DECLARE_EVENT_TABLE()	//! Call macro for declaration of events.
};
/** 
 * @brief Thread to generate events.
 */
class nkThreadPipedProcess : public wxThread{
public:
	/**
	 * @brief Class Constructor.
	 * @param a_mySCP Parent class.
	*/
	nkThreadPipedProcess(nkPipedProcessThread * a_myPP);
	/**
	 * @brief Function that capture the output of the execution of the extern program.
	*/
    virtual void *Entry();
	/**
	 * @brief On event delete of this thread.
	*/
    virtual void OnExit();
    nkPipedProcessThread * myPP;		//! Parent class.
};

#endif //_NKPIPEDPROCESS_H_