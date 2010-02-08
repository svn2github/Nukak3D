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
 * @file nkFindSCU.h
 * @brief DICOM C-Find.
 * @details Clases for config and execute C-Find request.
 * @author Alexander Pinzón Fernandez
 * @version 0.1
 * @date 26/01/2009 01:12 p.m.
*/

#ifndef _NKFINDSCU_H_
#define _NKFINDSCU_H_

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
#include "wx/aui/aui.h"
#include "wx/aui/auibook.h"
#include "wx/calctrl.h"
#include "wx/checkbox.h"
#include "wx/datetime.h"
#include "wx/dirdlg.h"
#include "wx/dynarray.h"
#include "wx/fileconf.h"
#include "wx/filename.h"
#include "wx/filefn.h"
#include "wx/grid.h"
#include "wx/list.h"
#include "wx/listbox.h"
#include "wx/listctrl.h"
#include "wx/numdlg.h"
#include "wx/panel.h"
#include "wx/process.h"
#include "wx/stdpaths.h"
#include "wx/string.h"
#include "wx/txtstrm.h"
#include "wx/thread.h"

#include <fstream>
#include <string>
#include <stdio.h>
#include <iostream>

#include "nkPipedProcess.h"
#include "nkStoreSCP.h"

#include "nkDICOMDictionary.h"
#include "nkDICOMServer.h"
#include "nkIODialog.h"
#include "nkNukak3D.h"
#include "nkUtilities.h"

/**
 * @brief Panel for search patient's.
 * @details Using DICOM C-FIND dcmtk application for searching patient's. 
*/
class nkFindSCU: public wxPanel{
public:
	/** 
	 * Event identifier.
	*/
	enum{
		ID_CALENDAR_STUDY_DATE_FROM  = wxID_HIGHEST + 1800,	/**< Display Calendar Control. */
		ID_CALENDAR_STUDY_DATE_TO,							/**< Display Calendar Control. */
		ID_CHECK_STUDY_DATE_FROM,							/**< Use Date From. */
		ID_CHECK_STUDY_DATE_TO,								/**< Use Date To. */
		ID_CLEAR,											/**< Clear fields. */
		ID_GET_STUDY,										/**< Get selected study. */
		ID_SEARCH,											/**< Searc in DICOM Q/R. */
		ID_SERVER_LOCATION,									/**< Configure DICOM Server's. */	
		ID_SELECT_CELL										/**< Select study in grid. */
	};
	/**
	 * @brief Class constructor.
	*/
	nkFindSCU(nkNukak3D * parent, 
		wxWindowID id = wxID_ANY, 
		const wxString& title = "Nukak3D: Find Patient's", 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize);
	virtual ~nkFindSCU();
protected:
	/**
	 * @brief Read list of servers register in this OS.
	*/
	void loadServers();
	/**
	 * @brief Event Manager for use date from.
	*/
	void eventUseDateFrom(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Event Manager for use date to.
	*/
	void eventUseDateTo(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Event manager to display calendar dialog.
	*/
	void eventChangeDateFrom(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Event manager to display calendar dialog.
	*/
	void eventChangeDateTo(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Display dialog for set serve's.
	*/
	void eventServerLocation(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Event manager for clear the fields..
	*/
	void eventClear(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Event manager to search a patient.
	*/
	void eventSearch(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Event manager to retrieve study.
	*/
	void eventGetStudy(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Manage event to capture number of row grid selected.
	*/
	void eventSelectCell(wxGridEvent &event);
	/**
	 * @brief Get selected server of the list box.
	*/
	int getServer();
	/**
	 * @brief Analyze the process stream provided in the output.
	*/
	void analyzeOutput(wxString a_cad);
private:
	nkListDICOMServer prv_listServers;

	wxPanel * search_panel;				//! Panel fields to search
	wxButton * cmd_server_localtion;		//! Button to change server's
	wxListBox * list_server_localtion;		//! List for display server's
	wxTextCtrl * text_patien_ID;			//! Search field to patient id.
	wxTextCtrl * text_first_name;			//! Search field to first name.
	wxTextCtrl * text_accession_number;	//! Search field to accession number.
	wxCheckBox * chk_study_date_from;		//! Checkout to use date from.
	wxButton * cmd_study_date_from;			//! Button to call calendar dialog.
	wxTextCtrl * text_study_date_from;		//! Search field to date from.
	wxCheckBox * chk_study_date_to;			//! Checkout to use date to.
	wxButton * cmd_study_date_to;			//! Button to call calendar dialog.
	wxTextCtrl * text_study_date_to;		//! Search field to date from.
	wxTextCtrl * text_study_description;	//! Search field to study description.
	wxTextCtrl * text_institution_name;	//! Search field to Institution name.
	wxButton * cmd_search;				//! Command button to start search.
	wxButton * cmd_clear;					//! Command button to clear fields.
	wxButton * cmd_get_study;					//! Command button to get study.
	wxGrid * grid_patients;			//! Grid to view patient's.
	int selected_cell;					//! Variable to save selected study in grid.
	nkNukak3D * parent;					//! Pointer to Nukak3D to load DICOM images.
	wxAuiManager aui_manager;			//! Administrator for Aui.
	DECLARE_EVENT_TABLE()					//! Macro for declare events.
};


#endif //_NKFINDSCU_H_