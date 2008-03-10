/** 
 * @file nkToolBar.h
 * @brief Toolbar de nukak3d.
 * @details Clases para la creacion de un toolbar vertical.
 * Especialmente diseñado para el nukak3d.
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
private:
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
 * Declaracion de la lista de nkTool.
*/
WX_DECLARE_LIST(nkTool, nkListaTool);

/**
 * @brief Menu de items.
 * @details Clase para la creacion de un menu con items dentro del ToolBar.
*/
class nkMenuTool: public wxPanel{
public:
	/**
	 * @brief Identificador para el manejo del evento de click sobre el menu.
	*/
	enum{
		ID_CLICKMENU = wxID_HIGHEST + 1000
	};
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
	 * @brief Al desplegar o replegar el menu hay que volverlo a crear.
	*/
	void crearMenu(void);

	/**
	 * @brief Insercion de un item en el menu
	*/
	void insertarTool(nkTool* un_tool);
	/**
	 * @brief Obtener un nkTool de la lista.
	 * @param un_indice Posicion en la lista del nkTool.
	*/
	nkTool* obtenerTool(size_t un_indice);
	/**
	 * @brief Retorna el numero de elementos en la lista.
	*/
	size_t longitudLista(void);
	/**
	 * @brief Tamaño del wxPanel con los items desplegados o replegados en ancho y alto .
	*/
	wxSize obtenerTamanio(void);
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
	/**
	 * @brief Funcion que pinta el contenido del wxPanel
	 * @details Funcion que pinta los items o no lo hace dependiendo de la funcion estaDesplegado().
	 * Ademas pinta el borde y actualiza las variables de tamaño para otros controles que lo necesiten.
	*/
	void repintar(void);
	/**
	 * @brief Funcion que despliega el menu si esta replegado, o lo repliega en caso contrario.
	*/
	void evtClick(wxCommandEvent & WXUNUSED(evt));
	/**
	 * @brief Invoca la funcion Refresh(), del wxPanel.
	*/
	void evtRedimensionar(wxSizeEvent & WXUNUSED(evt));
	/** 
	 * @brief Funcion invocada por el evento de wxWindow, EVT_PAINT.
	 * @details Asigna el tamaño al control.
	*/
	void evtRepintar(wxPaintEvent& WXUNUSED(evt));
private:
	/** Etiqueta de este menu.*/
	wxString prv_nombreMenu;
	/** Toolbar en la que se colocaran los items de este menu.*/
	wxToolBar* prv_toolbar;
	/** Lista con los datos de los items de este menu*/
	nkListaTool prv_listaTool;
	/** Variable que representa el estado del menu [desplegado:true, replegado:false].*/
	bool prv_desplegado;
	/**
	 * @brief Invocacion del macro, para el manejo de eventos en este menu.
	*/
	DECLARE_EVENT_TABLE()
};

/**
 * Declaracion de la lista de menus.
*/
WX_DECLARE_LIST(nkMenuTool, nkListaMenu);

/**
 * @brief Agrupacion de menus.
 * @details Clase para la creacion de un panel con menus
*/
class nkToolBar: public wxScrolledWindow{
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
		long style = wxHSCROLL | wxVSCROLL, 
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
	 * @brief Funcion que retorna un nkMenuTool de la lista de sus menus
	 * @param id identificador que servira de parametro en la lista de menus para la busqueda del elemento.
	*/
	nkMenuTool* obtenerMenu(long id);

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
	/**
	 * @brief Funcion que calcula el alto total basado en las sumas parciales del alto de cada menu.
	*/
	int alto(void);
	/**
	 * @brief Cuando el evento EVT_SIZE suceda se invoca la funcion Refresh() de la clase wxWindow.
	*/
	void evtRedimensionar(wxSizeEvent & p_Event);
	/**
	 * @brief Funcion que pinta los menus.
	 * @details Funcion que pinta y posiciona los menus basado en la altura de cada una, ademas toma en cuenta la posicion de las barras de desplazamiento.
	*/
	void evtRepintar(wxPaintEvent& WXUNUSED(evt));
private:
	/** Lista de todos los menus en este nkToolBar*/
	nkListaMenu prv_listaMenus;
	/**
	 * @brief Invocacion del macro, para el manejo de eventos en esta ventana.
	*/
	DECLARE_EVENT_TABLE()

};

#endif //_NKTOOLBAR_H_