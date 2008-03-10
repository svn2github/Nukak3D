/** 
 * @file nkToolbar.cpp
 * @brief Toolbar de nukak3d.
 * @details Clase para la creacion de un toolbar vertical.
 * Especialmente diseñado para el nukak3d.
 * @author Alexander Pinzon Fernandez
 * @version 0.1
 * @date 18/10/2007 02:50 p.m.
*/
#include "nkToolBar.h"
#include <wx/listimpl.cpp>
#include "icon_menu_mas.xpm"
#include "icon_menu_menos.xpm"


nkTool::nkTool(int toolId, 
		const wxString& label, 
		const wxBitmap& bitmap1, 
		const wxString& shortHelpString, 
		wxItemKind kind){
	prv_toolId = toolId;
	prv_label = label;
	prv_bitmap1 = bitmap1;
	prv_shortHelpString = shortHelpString;
	prv_kind = kind;
}
nkTool::~nkTool(){}

wxToolBarToolBase* nkTool::getTool(wxToolBar * un_tool){
	return un_tool->AddTool(prv_toolId,
		prv_label, 
		wxBitmap(prv_bitmap1.
			ConvertToImage().
			Scale(LARGO_X_ANCHO_ICONO.GetWidth(), LARGO_X_ANCHO_ICONO.GetHeight())),
		prv_shortHelpString,
		prv_kind);
}


WX_DEFINE_LIST(nkListaTool);

BEGIN_EVENT_TABLE(nkMenuTool,wxPanel)
	EVT_TOOL(nkMenuTool::ID_CLICKMENU, nkMenuTool::evtClick)
	EVT_PAINT(nkMenuTool::evtRepintar)
	EVT_SIZE(nkMenuTool::evtRedimensionar)
END_EVENT_TABLE()

nkMenuTool::nkMenuTool(wxWindow* parent, 
		wxWindowID id, 
		const wxString& nombreMenu,
		const wxPoint& pos, 
		const wxSize& size, 
		long style):
		wxPanel(parent, id, pos, size, style, nombreMenu)
{
	prv_nombreMenu = nombreMenu;
	prv_toolbar = new wxToolBar(this, 
		wxID_ANY, 
		wxDefaultPosition, 
		wxDefaultSize, 
		wxTB_VERTICAL | wxTB_FLAT | wxTB_TEXT |wxTB_HORZ_LAYOUT | wxTB_NODIVIDER | wxBORDER_NONE 
		);
	this->SetBackgroundColour(wxColour(255,255,255));
	prv_toolbar->SetBackgroundColour(wxColour(255,255,255));
	crearMenu();
	prv_toolbar->Realize();
	prv_desplegado = false;
}

nkMenuTool::~nkMenuTool(){
	prv_listaTool.DeleteContents(true);
}

void nkMenuTool::crearMenu(void){
	prv_toolbar->ClearTools();
	prv_toolbar->SetToolBitmapSize(nkTool::LARGO_X_ANCHO_ICONO);
	prv_toolbar->AddTool(nkMenuTool::ID_CLICKMENU, 
		prv_nombreMenu,
		wxBitmap(wxBitmap(icon_menu_mas_xpm).
			ConvertToImage().
			Scale(nkTool::LARGO_X_ANCHO_ICONO.GetWidth(), nkTool::LARGO_X_ANCHO_ICONO.GetHeight())),
		wxString("Click en ") + prv_nombreMenu + wxString(" para desplegar el menu"),
		wxITEM_NORMAL);
}

void nkMenuTool::insertarTool(nkTool* un_tool){
	prv_listaTool.Append(un_tool);
	repintar();
}

nkTool* nkMenuTool::obtenerTool(size_t un_indice){
	if (un_indice < prv_listaTool.GetCount()){
		return prv_listaTool.Item(un_indice)->GetData();
	}
	return NULL;
}

size_t nkMenuTool::longitudLista(void){
	return prv_listaTool.GetCount();
}

wxSize nkMenuTool::obtenerTamanio(void){
	wxSize un_tamanio;
	if (estaDeplegado()){
		un_tamanio = wxSize(ANCHO_TOOLBAR
		,prv_toolbar->GetToolSize().GetHeight()*(longitudLista()+1)+4);
	}else{
		un_tamanio = wxSize(ANCHO_TOOLBAR
		,prv_toolbar->GetToolSize().GetHeight()+4);

	}
	return un_tamanio;
}
bool nkMenuTool::estaDeplegado(void){
	return prv_desplegado;
}

void nkMenuTool::desplegarMenu(void){
	if (!estaDeplegado()){
		prv_desplegado = true;
		repintar();
	}
}

void nkMenuTool::replegarMenu(void){
	if (estaDeplegado()){
		prv_desplegado = false;
		repintar();
	}
}

void nkMenuTool::repintar(void){
	if (estaDeplegado()){
		crearMenu();
		for ( nkListaTool::Node *node = prv_listaTool.GetFirst(); node; node = node->GetNext() ){
			nkTool *contador = node->GetData();
			contador->getTool(prv_toolbar);
		}
	}else{
		crearMenu();
	}
	SetSize(obtenerTamanio());
	prv_toolbar->Realize();
}

void nkMenuTool::evtClick(wxCommandEvent & WXUNUSED(evt)){
	if (estaDeplegado()){
		this->replegarMenu();
	}else{
		this->desplegarMenu();
	}
}

void nkMenuTool::evtRedimensionar(wxSizeEvent & WXUNUSED(evt)){
	Refresh();
}

void nkMenuTool::evtRepintar(wxPaintEvent& WXUNUSED(evt)){
	wxPaintDC dc(this);
	SetSize(obtenerTamanio());
}

WX_DEFINE_LIST(nkListaMenu);

BEGIN_EVENT_TABLE(nkToolBar, wxScrolledWindow)
	EVT_SIZE(nkToolBar::evtRedimensionar)
	EVT_PAINT(nkToolBar::evtRepintar)
END_EVENT_TABLE()

nkToolBar::nkToolBar(wxWindow* parent, 
                    wxWindowID id, 
                    const wxPoint& pos, 
					const wxSize& size, 
					long style, 
					const wxString& name):
		wxScrolledWindow(parent, id, pos, size, style, name)
{
	SetScrollRate(10,10 );
	SetScrollbars(10,10,100,100);
	SetBackgroundColour(wxColour(255,255,255));
}
nkToolBar::~nkToolBar(void){
	prv_listaMenus.DeleteContents(true);
}
nkMenuTool* nkToolBar::insertarMenu(long id, const wxString& nombreMenu){
	nkMenuTool* un_menu = new nkMenuTool(this, id, nombreMenu,
		wxPoint(ALINEACION,alto()), 
		wxSize(nkMenuTool::ANCHO_TOOLBAR ,
		nkTool::LARGO_X_ANCHO_ICONO.GetHeight() + ALINEACION), 
		wxBORDER_STATIC );
	prv_listaMenus.Append(un_menu);
	return un_menu;
}

nkMenuTool* nkToolBar::obtenerMenu(long id){
	return prv_listaMenus.Find(id)->GetData();
}

void nkToolBar::desplegarMenu(nkMenuTool* un_menu){
	this->Refresh();
}

void nkToolBar::replegarMenu(nkMenuTool* un_menu){
	this->Refresh();
}

void nkToolBar::replegarMenus(void){
	for (nkListaMenu::Node *node = prv_listaMenus.GetFirst(); node; node = node->GetNext() ){
		nkMenuTool* contador = node->GetData();
		contador->replegarMenu();
	}
}

int nkToolBar::alto(void){
	int alto = 0;
	for (nkListaMenu::Node *node = prv_listaMenus.GetFirst(); node; node = node->GetNext() ){
		nkMenuTool *contador = node->GetData();
		alto = alto + contador->obtenerTamanio().GetHeight();
	}
	return alto;
}

void nkToolBar::evtRedimensionar(wxSizeEvent & p_Event){
	Refresh();
}

void nkToolBar::evtRepintar(wxPaintEvent& WXUNUSED(evt)){
	wxPaintDC dc(this);
	int alto = 0, ancho = 0;
	int ancho_real, alto_real;
	int xx = 0, yy = 0;
	CalcScrolledPosition(xx, yy, &xx, &yy);
	for (nkListaMenu::Node *node = prv_listaMenus.GetFirst(); node; node = node->GetNext() ){
		nkMenuTool *contador = node->GetData();
		contador->Move(xx, alto + yy);
		alto = alto + contador->obtenerTamanio().GetHeight();
		if (ancho<contador->obtenerTamanio().GetWidth()){
			ancho = contador->obtenerTamanio().GetWidth();
		}
		contador->Refresh();
	}
	ancho_real = GetClientSize().GetWidth();
	alto_real = GetClientSize().GetHeight();

	SetVirtualSize(ancho, alto);
	SetMaxSize(wxSize(ancho+ALINEACION, alto+ALINEACION));
}
