/** 
 * @file nkToolbarMac.h
 * @brief Toolbar of nukak3d.
 * @author Alexander Pinzon Fernandez.
 * @version 1.0
 * @date 7/12/2007 04:56 p.m.
*/

#ifndef _NKTOOLBARMAC_H_
#define _NKTOOLBARMAC_H_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/treectrl.h>
#include <wx/imaglist.h>

/**
 * @brief Item of menu.
 * @details Class for generate item for toolbar.
*/
class nkTool{
public:
	/**
	 * @brief Class constructor.
	*/
	nkTool(int toolId, 
		const wxString& label, 
		const wxBitmap& bitmap1, 
		const wxString& shortHelpString = "", 
		wxItemKind kind = wxITEM_NORMAL);
	/**
	 * @brief Class destructor.
	*/
	~nkTool();
	/**
	 * @brief Generation and insertion of tool in wxToolBar.
	 * @param un_tool wxToolBar to insert.
	*/
	wxToolBarToolBase* getTool(wxToolBar * un_tool);
	/**
	 * @brief Size: Widh and Height of item of menu.
	*/
	static wxSize LARGO_X_ANCHO_ICONO;
	/** ID of his control.*/
	int prv_toolId;
	/** Label for this control.*/
	wxString prv_label;
	/** Icon.*/
	wxBitmap prv_bitmap1;
	/** Help text for this control.*/
	wxString prv_shortHelpString;
	/** Configuration of this item (Normal, Toogle, Radio) .*/
	wxItemKind prv_kind;
};

/**
 * @brief Menu of items.
 * @details Class for creation menu in Toolbar.
*/
class nkMenuTool{
public:
	/**
	 * @brief Class constructor.
	*/
	nkMenuTool(wxWindow* parent, 
		wxWindowID id, 
		const wxString& nombreMenu = "Menu",
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxNO_BORDER);
	/**
	 * @brief Class destructor.
	 * @details Delete list.
	*/
	~nkMenuTool();

	/** Widht of menu*/
	static int ANCHO_TOOLBAR;

	/**
	 * @brief Insert item in menu.
	*/
	void insertarTool(nkTool* un_tool);
	/**
	 * @brief Is display menu.
	*/
	bool estaDeplegado(void);
	/**
	 * @brief Function for dropdown menu.
	*/
	void desplegarMenu(void);
	/**
	 * @brief Function for hide tools in menu.
	*/
	void replegarMenu(void);
public:
	wxTreeCtrl * prv_Arbol;
	wxTreeItemId prv_TreeItemId;
	/** Label for this menu.*/
	wxString prv_nombreMenu;
	/** State of menu.*/
	bool prv_desplegado;
};

/**
 * @brief Toolbar for Menus.
*/
class nkToolBar: public wxTreeCtrl{
public:
	/** Margins size for this control.*/
	static int ALINEACION;
	/**
	* @brief Class constructor.
	*/
	nkToolBar(wxWindow* parent, 
		wxWindowID id = -1, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTR_HAS_BUTTONS|wxTR_HIDE_ROOT|wxTR_FULL_ROW_HIGHLIGHT, 
		const wxString& name = "scrolledWindow");
	/**
     * @brief Class destructor.
    */
	~nkToolBar(void);

	/**
	 * @brief Procedure for create menu.
	 * @param id for this control.
	 * @param nombreMenu label for this control.
	*/
	nkMenuTool* insertarMenu(long id, const wxString& nombreMenu = "Menu");
	/**
	 * @brief Function for dropdown menu.
	 * @param un_menu for dropdown.
	*/
	void desplegarMenu(nkMenuTool* un_menu);
	/**
	 * @brief Function for hide items of menu.
	 * @param un_menu to hide.
	*/
	void replegarMenu(nkMenuTool* un_menu);
	/**
	 * @brief Func for hide all items of all menus.
	*/
	void replegarMenus(void);
};

#endif //_NKTOOLBARMAC_H_