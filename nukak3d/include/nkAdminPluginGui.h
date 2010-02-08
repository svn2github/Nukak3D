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
 * @file nkAdminPluginGui.h
 * @brief Admin Plugin's paths.
 * @details Clases for display and admin paths, for plugins.
 * @author Alexander Pinzón Fernandez
 * @date 24/04/2009 1:13 p.m.
*/

#ifndef _NKADMINPLUGINGUI_H_
#define _NKADMINPLUGINGUI_H_

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
#include "wx/dirdlg.h"

#include "nkIODialog.h"
#include "nkUtilities.h"
#include "nkKernel.h"


class nkAdminPluginGui : public wxDialog
{
public:
	/** 
	 * Event identifier.
	*/
	enum{
		ID_ADD_PLUGIN  = wxID_HIGHEST + 2100,	/**< Add Plugin path. */
		ID_DELETE_PLUGIN						/**< Delete Plugin path. */
	};
	/**
	 * @brief Class constructor.
	*/
	nkAdminPluginGui(wxWindow* parent, 
					nukak3d::nkKernel & prvKernel,
					wxWindowID id = wxID_ANY, 
					wxString title = _("Nukak3D: Plugin's Paths"), 
					wxPoint pos = wxDefaultPosition, 
					wxSize size = wxDefaultSize, 
					long style = wxDEFAULT_DIALOG_STYLE, 
					wxString name = "dialogBox");
	/**
	 * @brief Read plugin's of register.
	*/
	void loadPlugins();
	/**
	 * @brief Show plugin's loaded.
	*/
	void loadPluginsLoaded();
protected:
	/**
	 * @brief Manager for add plugin path.
	*/
	void eventAddPlugin(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Manager for delete plugin path.
	*/
	void eventDeletePlugin(wxCommandEvent& WXUNUSED(event));
	/**
	 * @brief Capture number of cell selected.
	*/
	void eventSelectCell(wxGridEvent &event);
private:
	wxGrid * prv_wxGridPluginsPaths;		//! Grid for dislay plugin's paths.
	wxGrid * prv_wxGridPluginsLoaded;		//! Grid for dislay plugin's loaded.
	wxButton * my_cmdAddPlugin;		//! Button for Add Plugin path.
	wxButton * my_cmdDeletePlugin;	//! Button for Delete Plugin path.
	wxArrayString my_plugins;		//! Variable for read plugin's paths of register.
	wxArrayString my_pluginsLoades;	//! Variable for show plugin's loaded.
	nukak3d::nkKernel & prvKernel;
	int my_selectedCell;

	DECLARE_EVENT_TABLE()
};
#endif //_NKADMINPLUGINGUI_H_
