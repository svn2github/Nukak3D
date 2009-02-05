/** 
 * @file nkDICOMServer.h
 * @brief DICOM Server.
 * @details Clases for configure DICOM Servers.
 * @author Alexander Pinzón Fernandez
 * @version 0.1
 * @date 30/01/2009 03:46 p.m.
*/

#ifndef _NKDICOMSERVER_H_
#define _NKDICOMSERVER_H_

#include "vtkINRIA3DConfigure.h"
#include  "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include  "wx/wx.h"
#endif

#include "wx/dialog.h"
#include "wx/event.h"
#include "wx/gdicmn.h"
#include "wx/grid.h"
#include "wx/list.h"
#include "wx/sizer.h"
#include "wx/string.h"
#include "wx/window.h"

#include "nkIODialog.h"
#include "nkUtilities.h"

/**
 * @brief A Dialog for config extern DICOM servers.
*/
class nkServersDialog: public wxDialog{
public:
	/** 
	 * Event identifier.
	*/
	enum{
		ID_ADD_SERVER  = wxID_HIGHEST + 1900,	/**< Add DICOM server. */
		ID_CHANGE_SERVER,						/**< Change DICOM server. */
		ID_DELETE_SERVER						/**< Delete DICOM server. */
	};
	/**
	 * @brief Class constructor.
	*/
	nkServersDialog(wxWindow* parent, 
					wxWindowID id = wxID_ANY, 
					wxString title = _("Nukak3D: Server's List"), 
					wxPoint pos = wxDefaultPosition, 
					wxSize size = wxDefaultSize, 
					long style = wxDEFAULT_DIALOG_STYLE, 
					wxString name = "dialogBox");
	/**
	 * @brief Read servers of register.
	*/
	void loadServers();
protected:
	/**
	 * @brief Manager for add server.
	*/
	void prEventAddServer(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Manager for change server.
	*/
	void prEventChangeServer(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Manager for delete server.
	*/
	void prEventDeleteServer(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Capture number of cell selected.
	*/
	void prEventSelectCell(wxGridEvent &event);
private:
	wxGrid * prv_wxGridServers;		//! Grid for dislay servers.
	wxButton * my_cmdAddServer;		//! Button for Add Server.
	wxButton * my_cmdChangeServer;	//! Button for Change Server.
	wxButton * my_cmdDeleteServer;	//! Button for Delete Server.
	wxString my_servers;			//! Variable for read servers of register.
	int my_selectedCell;

	DECLARE_EVENT_TABLE()			//! Macro for manage events.
};

/**
 * @brief Class for representation of DICOM server.
*/
class nkDICOMServer{
public:
	/**
	 * @brief Class constructor.
	 * @param a_name Name of host.
	 * @param a_host Network name or ip address.
	 * @param a_AETitle AE Title of this client.
	 * @param a_AECalled AE Title of sever to call.
	 * @param a_port Port of the server to call.
	*/
	nkDICOMServer(wxString a_name,
			wxString a_host,
			wxString a_AETitle,
			wxString a_AECalled,
			int a_port);
	/**
	 * @brief Proccess string that contain DICOM server information.
	 * @param my_servers String that contain servers information.
	 * @param index Server localized in n pos of split this string with a token.
	*/
	static nkDICOMServer * getDicomServer(wxString my_servers, int index);
	/**
	 * @brief Proccess string that contain DICOM server's.
	 * @param my_servers String that contain servers information.
	 * @param index Server localized in n pos of split this string with a token '#'.
	*/
	static wxString getStringServer(wxString my_servers, int index);
	/**
	 * @brief Proccess string that contain DICOM server parameter.
	 * @param a_server String that contain server information.
	 * @param index Parameter localized in n pos of split this string with a token '|'.
	*/
	static wxString getParameterServer(wxString a_server, int index);
	/**
	 * @brief Proccess string that contain parameter separated with tokens.
	 * @param a_cad String that contain parameters.
	 * @param a_char Token Separator.
	 * @param index Parameter localized in n pos of split this string with a token a_char.
	*/
	static wxString getParameter(wxString a_cad, wxChar a_char, int index);
	/**
	 * @brief Count occurrences of character a_char.
	 * @param my_servers String that contain characteres.
	 * @param a_char Character to find.
	*/
	static int countServers(wxString my_servers, wxChar a_char = '#');
	/**
	 * @brief Read list of servers of register.
	*/
	static wxString ReadConfigSystem();
	/**
	 * @brief Write list of servers in register.
	*/
	static bool WriteConfigSystem(wxString my_servers);
	wxString name;			//! Name of host
	wxString host;			//! Network name or ip address.
	wxString AETitle;		//! AE Title of this client.
	wxString AECalled;		//! AE Title of sever to call.
	int port;				//! Port of the server to call.
};

WX_DECLARE_LIST(nkDICOMServer, nkListDICOMServer);	//! A Dicom list of DICOM server's

#endif //_NKDICOMSERVER_H_