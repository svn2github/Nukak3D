/** 
 * @file nkToolBar.h
 * @brief Toolbar of nukak3d.
 * @author Alexander Pinzon Fernandez.
 * @version 0.1
 * @date 18/10/2007 02:50 p.m.
*/

#ifndef _NKTOOLBAR_H_
#define _NKTOOLBAR_H_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/intl.h"

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
private:
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
 * List of nkTool declaration.
*/
WX_DECLARE_LIST(nkTool, nkListaTool);

/**
 * @brief Menu of items.
 * @details Class for creation menu in Toolbar.
*/
class nkMenuTool: public wxPanel{
public:
	/**
	 * @brief Id for mouse event in this menu.
	*/
	enum{
		ID_CLICKMENU = wxID_HIGHEST + 1000
	};
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
	 * @brief Make menu.
	*/
	void crearMenu(void);

	/**
	 * @brief Insert item in menu.
	*/
	void insertarTool(nkTool* un_tool);
	/**
	 * @brief get nkTool of list.
	 * @param un_indice Position of nkTool in list.
	*/
	nkTool* obtenerTool(size_t un_indice);
	/**
	 * @brief Return list len.
	*/
	size_t longitudLista(void);
	/**
	 * @brief Return actual size of Menu.
	*/
	wxSize obtenerTamanio(void);
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
	/**
	 * @brief Function for paint content of wxPanel
	 * @details Function for paint items in menu.
	*/
	void repintar(void);
	/**
	 * @brief Function for dropdown menu or hide items in this menu.
	*/
	void evtClick(wxCommandEvent & WXUNUSED(evt));
	/**
	 * @brief Call Refresh() function of wxPanel.
	*/
	void evtRedimensionar(wxSizeEvent & WXUNUSED(evt));
	/** 
	 * @brief Function for event EVT_PAINT.
	*/
	void evtRepintar(wxPaintEvent& WXUNUSED(evt));
private:
	/** Label for this menu.*/
	wxString prv_nombreMenu;
	/** Toolbar for inset this menu.*/
	wxToolBar* prv_toolbar;
	/** List with items of this menu.*/
	nkListaTool prv_listaTool;
	/** State of menu.*/
	bool prv_desplegado;
	/**
	 * @brief call Macro for events fo this menu.
	*/
	DECLARE_EVENT_TABLE()
};

/**
 * Declaration of list of nkMenuTool.
*/
WX_DECLARE_LIST(nkMenuTool, nkListaMenu);

/**
 * @brief Toolbar for Menus.
*/
class nkToolBar: public wxScrolledWindow{
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
		long style = wxHSCROLL | wxVSCROLL, 
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
	 * @brief Function that return nkMenuTool item of list.
	 * @param id key identifier in the list.
	*/
	nkMenuTool* obtenerMenu(long id);

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
	/**
	 * @brief Function to calc height of this toolbar.
	*/
	int alto(void);
	/**
	 * @brief Function manager for event EVT_SIZE.
	*/
	void evtRedimensionar(wxSizeEvent & p_Event);
	/**
	 * @brief Function for paint all menus.
	 * @details Function that paint and put  menus.
	*/
	void evtRepintar(wxPaintEvent& WXUNUSED(evt));
private:
	/** List of nkMenu.*/
	nkListaMenu prv_listaMenus;
	/**
	 * @brief Macro for manage events.
	*/
	DECLARE_EVENT_TABLE()

};

#endif //_NKTOOLBAR_H_