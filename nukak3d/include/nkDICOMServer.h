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
	void eventAddServer(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Manager for change server.
	*/
	void eventChangeServer(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Manager for delete server.
	*/
	void eventDeleteServer(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Capture number of cell selected.
	*/
	void eventSelectCell(wxGridEvent &event);
private:
	wxGrid * grid_servers;		//! Grid for dislay servers.
	wxButton * cmd_add_server;		//! Button for Add Server.
	wxButton * cmd_change_server;	//! Button for Change Server.
	wxButton * cmd_delete_server;	//! Button for Delete Server.
	wxString str_servers;			//! Variable for read servers of register.
	int selected_cell;

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
	static wxString readConfigSystem();
	/**
	 * @brief Write list of servers in register.
	*/
	static bool writeConfigSystem(wxString my_servers);
	wxString name;			//! Name of host
	wxString host;			//! Network name or ip address.
	wxString AETitle;		//! AE Title of this client.
	wxString AECalled;		//! AE Title of sever to call.
	int port;				//! Port of the server to call.
};

WX_DECLARE_LIST(nkDICOMServer, nkListDICOMServer);	//! A Dicom list of DICOM server's

#endif //_NKDICOMSERVER_H_