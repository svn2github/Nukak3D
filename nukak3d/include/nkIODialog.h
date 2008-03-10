/** 
 * @file nkIODialog.h
 * @brief Dialogo genérico para ver o capturar información del Nukak3D.
 * @author  Byron Pérez, Alexander Pinzon
 * @version 0.3
 * @date 27/12/2007 03:37 p.m.
*/

#ifndef _NKIODIALOG_H_
#define _NKIODIALOG_H_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

/** 
 * @brief Dialogo genérico del Nukak3D.
 */
class nkIODialog: public wxDialog{
public:
	/**
	 * @brief Constructor de Clase.
	 * @details Datos necesarios para la generacion de un Dialogo de entrada de Datos.
	 * @param parent wxWindow desde donde es invocado.
	 * @param unasEtiquetas arreglo de wxString en el cual se guarda cada una de las etiquetas de cada parametro.
	 * @param un_tamanio numero de etiquetas.
	 * @param title Titulo de la ventana.
	 * @param pos Posición de la ventana.
	 * @param size Tamaño de la ventana.
	 * @param style Estilo de diseño de la ventana.
	 * @param name Nombre del objeto.
	*/
	nkIODialog(wxWindow* parent, 
	   wxString unasEtiquetas[100],
	   int un_tamanio,
	   wxWindowID id = wxID_ANY, 
	   const wxString& title = "nukak3D", 
	   const wxPoint& pos = wxDefaultPosition, 
	   const wxSize& size = wxDefaultSize, 
	   long style = wxDEFAULT_DIALOG_STYLE, 
	   const wxString& name = "nkIODialog");

	/**
	 * @brief Constructor de Clase.
	 * @details Datos necesarios para la generacion de un Dialogo de Salida de Datos.
	 * @param parent wxWindow desde donde es invocado.
	 * @param p_text Texto informativo a mostrar.
	 * @param title Titulo de la ventana.
	 * @param pos Posición de la ventana.
	 * @param size Tamaño de la ventana.
	 * @param style Estilo de diseño de la ventana.
	 * @param name Nombre del objeto.
	*/
	nkIODialog(wxWindow* parent, 
	   wxString p_text,
	   wxWindowID id = wxID_ANY, 
	   const wxString& title = "nukak3D", 
	   const wxPoint& pos = wxDefaultPosition, 
	   const wxSize& size = wxDefaultSize, 
	   long style = wxDEFAULT_DIALOG_STYLE, 
	   const wxString& name = "nkIODialog");

	/**
	 * @brief Destructor de clase.
	*/
	~nkIODialog();
	
	/**
	 * @brief Editar el valor opcional que se mostrara para un dato requerido.
	 * @param un_valor Valor que se mostrara.
	 * @param un_indice numero entre 0 y tamaño-1 que indica la posición del valor a cambiar.
	*/
	void cambiarValor(wxString un_valor, int un_indice);
	/**
	 * @brief Obtener un dato una vez que el dialogo a sido cerrado.
	 * @param un_indice numero entre 0 y tamaño-1 que indica la posición del valor a obtener.
	*/
	wxString obtenerValor(int un_indice);
private:
	/** Lista para guardar las etiquetas */
	wxTextCtrl * prv_wxTCValores[100];
	/** Numero de datos en la lista */
	int prv_tamanio;
};
#endif _nkIODialog_H_