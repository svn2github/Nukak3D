/** 
 * @file nkToolbarMac.h
 * @brief Toolbar de nukak3d.
 * @details Clases para la creacion de un toolbar vertical.
 * Especialmente diseñado para el nukak3d Para Mac.
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
 * @brief Item de un menu.
 * @details Clase para la generacion de una item(tool), para un ToolBar.
*/
class nkTool{
public:
	/**
	 * @brief Constructor de Clase.
	 * @details Datos necesarios para la generacion de un nkTool.
	*/
	nkTool(int toolId, 
		const wxString& label, 
		const wxBitmap& bitmap1, 
		const wxString& shortHelpString = "", 
		wxItemKind kind = wxITEM_NORMAL);
	/**
	 * @brief Destructor de clase.
	*/
	~nkTool();
	/**
	 * @brief Funcion para la generacion y adicion de un tool en un wxToolBar.
	 * @param un_tool wxToolBar al cual le adicionaremos este nkTool.
	*/
	wxToolBarToolBase* getTool(wxToolBar * un_tool);
	/**
	 * @brief Tamaño: ancho y alto del icono en un item del menu.
	*/
	static wxSize LARGO_X_ANCHO_ICONO;
	/** ID del evento de este item.*/
	int prv_toolId;
	/** Etiquete que se mostrara en el item.*/
	wxString prv_label;
	/** Icono del item.*/
	wxBitmap prv_bitmap1;
	/** Texto de ayuda que se mostrara cuando el mouse ubique encima de este control.*/
	wxString prv_shortHelpString;
	/** Congiuracion de este item como (Normal, Toogle, Radio) Boton.*/
	wxItemKind prv_kind;
};

/**
 * @brief Menu de items.
 * @details Clase para la creacion de un menu con items dentro del ToolBar.
*/
class nkMenuTool{
public:
	/**
	 * @brief Constructor de Clase.
	 * @details Parametros necesarios para incializar un wxPanel.
	*/
	nkMenuTool(wxWindow* parent, 
		wxWindowID id, 
		const wxString& nombreMenu = "Menu",
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxNO_BORDER);
	/**
	 * @brief Destructor de clase.
	 * @details Se borraran los elementos de la lista de nkTools.
	*/
	~nkMenuTool();

	/** Ancho del menu*/
	static int ANCHO_TOOLBAR;

	/**
	 * @brief Insercion de un item en el menu
	*/
	void insertarTool(nkTool* un_tool);
	/**
	 * @brief Retorna si el menu esta desplegado(true) o no(falso).
	*/
	bool estaDeplegado(void);
	/**
	 * @brief Funcion para desplegar los items del menu.
	*/
	void desplegarMenu(void);
	/**
	 * @brief Funcion para replegar los items del menu.
	*/
	void replegarMenu(void);
public:
	wxTreeCtrl * prv_Arbol;
	wxTreeItemId prv_TreeItemId;
	/** Etiqueta de este menu.*/
	wxString prv_nombreMenu;
	/** Variable que representa el estado del menu [desplegado:true, replegado:false].*/
	bool prv_desplegado;
};

/**
 * @brief Agrupacion de menus.
 * @details Clase para la creacion de un panel con menus
*/
class nkToolBar: public wxTreeCtrl{
public:
	/** Tamaño de las margenes o espacios en este control.*/
	static int ALINEACION;
	/**
     * @brief Constructor de clase.
     * @details Constructor derivado de la clase wxScrolledWindow.
    */
	nkToolBar(wxWindow* parent, 
		wxWindowID id = -1, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTR_HAS_BUTTONS|wxTR_HIDE_ROOT|wxTR_FULL_ROW_HIGHLIGHT, 
		const wxString& name = "scrolledWindow");
	/**
     * @brief Destructor.
    */
	~nkToolBar(void);

	/**
	 * @brief Procedimiento para la creacion de un menu en esta clase
	 * @param id Identificacion del control.
	 * @param nombreMenu Etiqueta del menu que se mostrara, por defecto "Menu".
	*/
	nkMenuTool* insertarMenu(long id, const wxString& nombreMenu = "Menu");
	/**
	 * @brief Funcion que invoca la funcion desplegar() de la clase nkMenuTool.
	 * @param un_menu Menu que sera deplegado.
	*/
	void desplegarMenu(nkMenuTool* un_menu);
	/**
	 * @brief Funcion que invoca la funcion replegar() de la clase nkMenuTool.
	 * @param un_menu Menu que sera replegado.
	*/
	void replegarMenu(nkMenuTool* un_menu);
	/**
	 * @brief Funcion que invoca la funcion replegar() de la clase nkMenuTool.
	 * @details Todos los menus seran replegados.
	*/
	void replegarMenus(void);
};

#endif //_NKTOOLBARMAC_H_